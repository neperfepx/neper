/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#define _GNU_SOURCE             // pipe2
#include"nem_mesh_gmsh_.h"

/* Gmsh is called as a library, by worker processes.  The Gmsh library is
 * not thread-safe (it has a single global state), so that each meshing
 * thread has its own worker process, which is a separate instance of
 * neper (run with NEM_GMSH_WORKER_ARG as argument), started at its first
 * request (with posix_spawn, as a fork of the multithreaded calling
 * process would not be safe).  A worker reads requests on file
 * descriptor NEM_GMSH_WORKER_IN and writes replies on file descriptor
 * NEM_GMSH_WORKER_OUT, in binary form:
 * - request: dimension, meshing algorithm and optimization (int; in 2D,
 *   recombination into quadrangles: 1, or not: 0), random
 *   factor and maximum mesh size (double, <= 0 for the defaults), mesh
 *   size gradient (double, < 0 for none, see below), and the geometry to
 *   mesh:
 *   - in 2D, the points of the face boundary: quantity (int), and
 *     coordinates and mesh size of each point (4 double each), and the
 *     mesh size factor of the face projection (type (int) and parameters
 *     (NEM_GMSH_SIZEFACT_PARMQTY double), see nem_mesh_gmsh_sizefact);
 *   - in 3D, the mesh of the poly boundary: quantity of nodes (int), node
 *     coordinates and mesh size (4 double each), quantity of triangles
 *     (int), and triangle nodes (3 int each);
 * - reply: quantity of nodes (int), node coordinates (3 double each),
 *   quantity of elements (int), and element nodes (dimension + 1 int
 *   each, or, with recombination, 4 int each, the last one being 0 for a
 *   triangle).
 * In 2D, the mesh size is set by a callback (it is not extended from the
 * boundary by Gmsh, and the boundary is not remeshed), to the boundary point
 * mesh sizes increased by the mesh size gradient times the distance to the
 * points, up to the maximum mesh size (see neut_nodes_point_clgrad), and
 * multiplied by the mesh size factor of the face projection.  In 3D, the
 * mesh size is set by a callback only with a mesh size gradient (in the
 * same way, without factor), and is extended from the boundary by Gmsh
 * otherwise.
 * A worker exits when its input is closed, and when the calling process
 * ends (which is detected by a watchdog thread, as the calling process
 * cannot stop its workers when it aborts or is killed).  A request that
 * exceeds the allowed time is aborted by killing the worker, which is
 * started again at the next request.  Ctrl+C ends the workers (which receive it too), so
 * that the current requests fail, and pressed again within 2 seconds, it
 * ends execution (see ut_sys_ctrlc_begin). */

#include<fcntl.h>
#include<poll.h>
#include<pthread.h>
#include<signal.h>
#include<spawn.h>
#include<sys/wait.h>
#include<errno.h>
#include<limits.h>
#ifdef __APPLE__
#include<mach-o/dyld.h>
#endif
#include<gmshc.h>

extern char **environ;

// Gmsh API version (>= 4.9, see CMakeLists.txt), for its changes:
// gmshOptionRestoreDefaults (4.13)
#define NEM_GMSH_API_GE(major, minor) \
  (GMSH_API_VERSION_MAJOR > (major) \
   || (GMSH_API_VERSION_MAJOR == (major) && GMSH_API_VERSION_MINOR >= (minor)))

#define NEM_GMSH_WORKER_IN 3
#define NEM_GMSH_WORKER_OUT 4
#define NEM_GMSH_WORKER_MAX 1024

struct NEM_GMSH_WORKER
{
  pid_t pid;                    // pid, 0 if not running
  int in;                       // file descriptor of the worker's input
  int out;                      // file descriptor of the worker's output
};

static struct NEM_GMSH_WORKER nem_gmsh_workers[NEM_GMSH_WORKER_MAX];

// message buffer
struct NEM_GMSH_BUF
{
  char *data;
  size_t size;
  size_t cap;
};

static void
nem_gmsh_buf_put (struct NEM_GMSH_BUF *pBuf, const void *data, size_t size)
{
  if ((*pBuf).size + size > (*pBuf).cap)
  {
    (*pBuf).cap = 2 * ((*pBuf).size + size);
    (*pBuf).data = (char *) realloc ((*pBuf).data, (*pBuf).cap);
  }

  memcpy ((*pBuf).data + (*pBuf).size, data, size);
  (*pBuf).size += size;

  return;
}

static void
nem_gmsh_buf_put_int (struct NEM_GMSH_BUF *pBuf, int val)
{
  nem_gmsh_buf_put (pBuf, &val, sizeof (int));

  return;
}

static void
nem_gmsh_buf_put_double (struct NEM_GMSH_BUF *pBuf, double val)
{
  nem_gmsh_buf_put (pBuf, &val, sizeof (double));

  return;
}

// reads / writes size bytes from / to a file descriptor
static int
nem_gmsh_read (int fd, void *data, size_t size)
{
  ssize_t qty;

  while (size > 0)
  {
    qty = read (fd, data, size);
    if (qty < 0 && errno == EINTR)
      continue;
    if (qty <= 0)
      return -1;
    data = (char *) data + qty;
    size -= qty;
  }

  return 0;
}

static int
nem_gmsh_write (int fd, const void *data, size_t size)
{
  ssize_t qty;

  while (size > 0)
  {
    qty = write (fd, data, size);
    if (qty < 0 && errno == EINTR)
      continue;
    if (qty <= 0)
      return -1;
    data = (const char *) data + qty;
    size -= qty;
  }

  return 0;
}

// Worker side =========================================================

// mesh size field
struct NEM_GMSH_SIZE
{
  struct NODES Nodes;           // boundary points / nodes, with their mesh
                                // sizes
  double cl;                    // maximum mesh size
  double clgrad;                // mesh size gradient
  int facttype;                 // mesh size factor type and parameters
  double factparms[NEM_GMSH_SIZEFACT_PARMQTY]; // (see nem_mesh_gmsh_sizefact)
};

static struct NEM_GMSH_SIZE nem_gmsh_size;

// initializes Gmsh, without reading the configuration files (~/.gmshrc,
// etc.)
static void
nem_gmsh_initialize (int *pierr)
{
  gmshInitialize (0, NULL, 0, 0, pierr);

  return;
}

static double
nem_gmsh_worker_size (int dim, int tag, double x, double y, double z,
                      double lc, void *data)
{
  double cl, coo[3] = { x, y, z };
  struct NEM_GMSH_SIZE *pSize = (struct NEM_GMSH_SIZE *) data;

  (void) dim;
  (void) tag;
  (void) lc;

  cl = neut_nodes_point_clgrad ((*pSize).Nodes, coo, (*pSize).cl, (*pSize).clgrad);

  return cl * nem_mesh_gmsh_sizefact ((*pSize).facttype, (*pSize).factparms, coo);
}

static void
nem_gmsh_worker_reset (void)
{
  int ierr;

#if NEM_GMSH_API_GE(4, 13)
  gmshClear (&ierr);
  gmshOptionRestoreDefaults (&ierr);
#else
  gmshFinalize (&ierr);
  nem_gmsh_initialize (&ierr);
#endif

  gmshOptionSetNumber ("General.Terminal", 0, &ierr);
  gmshOptionSetNumber ("General.Verbosity", 0, &ierr);

  gmshModelMeshRemoveSizeCallback (&ierr);
  neut_nodes_reset (&nem_gmsh_size.Nodes);

  return;
}

// sets the meshing options
static void
nem_gmsh_worker_options (int dim, int algo, int opti, double rnd,
                         double clmax, double clgrad)
{
  int ierr;

  // in 2D, opti is the recombination (Blossom, into a mesh of quadrangles
  // and triangles)
  if (dim == 2)
  {
    if (opti)
    {
      gmshOptionSetNumber ("Mesh.RecombineAll", 1, &ierr);
      gmshOptionSetNumber ("Mesh.RecombinationAlgorithm", 1, &ierr);
    }
    opti = 0;
  }

  gmshOptionSetNumber (dim == 2 ? "Mesh.Algorithm" : "Mesh.Algorithm3D",
                       algo, &ierr);
  // (both are set, as Mesh.Optimize is 1 by default)
  gmshOptionSetNumber ("Mesh.Optimize", opti == 1 || opti == 3, &ierr);
  gmshOptionSetNumber ("Mesh.OptimizeNetgen", opti == 2 || opti == 3, &ierr);
  if (rnd > 0)
    gmshOptionSetNumber ("Mesh.RandomFactor", rnd, &ierr);
  // (in 2D, the maximum mesh size is applied by the callback, before the
  // mesh size factor)
  if (dim == 3 && clmax > 0)
    gmshOptionSetNumber ("Mesh.MeshSizeMax", clmax, &ierr);
  // the mesh size is set by the callback (see nem_gmsh_worker_size_set)
  if (dim == 2 || clgrad >= 0)
  {
    gmshOptionSetNumber ("Mesh.MeshSizeExtendFromBoundary", 0, &ierr);
    gmshOptionSetNumber ("Mesh.MeshSizeFromPoints", 0, &ierr);
  }

  return;
}

// sets the mesh size callback, from the boundary points / nodes and their
// mesh sizes (4 double each), and the mesh size factor
static void
nem_gmsh_worker_size_set (double *pts, int qty, double clmax, double clgrad,
                          int facttype, double *factparms)
{
  int i, ierr;

  for (i = 0; i < qty; i++)
    neut_nodes_addnode (&nem_gmsh_size.Nodes, pts + 4 * i, pts[4 * i + 3]);
  nem_gmsh_size.cl = clmax;
  nem_gmsh_size.clgrad = clgrad;
  nem_gmsh_size.facttype = facttype;
  ut_array_1d_memcpy (factparms, NEM_GMSH_SIZEFACT_PARMQTY, nem_gmsh_size.factparms);

  gmshModelMeshSetSizeCallback (nem_gmsh_worker_size, &nem_gmsh_size, &ierr);

  return;
}

// builds the 2D model: points, lines (not remeshed), line loop and plane
// surface, and sets the mesh size callback
static int
nem_gmsh_worker_model_2d (int in, double clmax, double clgrad)
{
  int i, qty, ierr, *lines = NULL, facttype;
  double *pts = NULL, factparms[NEM_GMSH_SIZEFACT_PARMQTY];

  if (nem_gmsh_read (in, &qty, sizeof (int)))
    return -1;

  pts = ut_alloc_1d (4 * qty);
  if (nem_gmsh_read (in, pts, 4 * qty * sizeof (double))
      || nem_gmsh_read (in, &facttype, sizeof (int))
      || nem_gmsh_read (in, factparms, NEM_GMSH_SIZEFACT_PARMQTY * sizeof (double)))
    return -1;

  for (i = 0; i < qty; i++)
    gmshModelGeoAddPoint (pts[4 * i], pts[4 * i + 1], pts[4 * i + 2],
                          pts[4 * i + 3], i + 1, &ierr);

  // Gmsh's characteristic length, used for geometrical tolerances, is set
  // from the model at synchronization; it is set here from the points, so
  // that it does not depend on the previous request
  gmshModelGeoSynchronize (&ierr);

  lines = ut_alloc_1d_int (qty);
  for (i = 1; i <= qty; i++)
  {
    gmshModelGeoAddLine (i, ut_array_rotpos (1, qty, i, 1), i, &ierr);
    lines[i - 1] = i;
    // the lines are not meshed from the callback mesh size (which would
    // add nodes to the boundary)
    gmshModelGeoMeshSetTransfiniteCurve (i, 2, "Progression", 1, &ierr);
  }

  i = 1;
  gmshModelGeoAddCurveLoop (lines, qty, 1, 0, &ierr);
  gmshModelGeoAddPlaneSurface (&i, 1, 1, &ierr);
  gmshModelGeoSynchronize (&ierr);

  nem_gmsh_worker_size_set (pts, qty, clmax, clgrad, facttype, factparms);

  ut_free_1d (&pts);
  ut_free_1d_int (&lines);

  return 0;
}

// builds the 3D model: discrete surface from the boundary mesh, surface
// loop and volume, and sets the mesh size callback (with a mesh size
// gradient)
static int
nem_gmsh_worker_model_3d (int in, double clmax, double clgrad)
{
  int i, one = 1, nodeqty, eltqty, ierr, *elts = NULL;
  double *pts = NULL, *coos = NULL;
  size_t *nodetags = NULL, *elttags = NULL, *eltnodes = NULL;

  if (nem_gmsh_read (in, &nodeqty, sizeof (int)))
    return -1;
  pts = ut_alloc_1d (4 * nodeqty);
  if (nem_gmsh_read (in, pts, 4 * nodeqty * sizeof (double))
      || nem_gmsh_read (in, &eltqty, sizeof (int)))
    return -1;
  coos = ut_alloc_1d (3 * nodeqty);
  for (i = 0; i < nodeqty; i++)
    ut_array_1d_memcpy (pts + 4 * i, 3, coos + 3 * i);
  elts = ut_alloc_1d_int (3 * eltqty);
  if (nem_gmsh_read (in, elts, 3 * eltqty * sizeof (int)))
    return -1;

  gmshModelAddDiscreteEntity (2, 1, NULL, 0, &ierr);

  nodetags = (size_t *) calloc (nodeqty, sizeof (size_t));
  for (i = 0; i < nodeqty; i++)
    nodetags[i] = i + 1;
  gmshModelMeshAddNodes (2, 1, nodetags, nodeqty, coos, 3 * nodeqty, NULL, 0,
                         &ierr);

  elttags = (size_t *) calloc (eltqty, sizeof (size_t));
  eltnodes = (size_t *) calloc (3 * eltqty, sizeof (size_t));
  for (i = 0; i < eltqty; i++)
    elttags[i] = i + 1;
  for (i = 0; i < 3 * eltqty; i++)
    eltnodes[i] = elts[i];
  gmshModelMeshAddElementsByType (1, 2, elttags, eltqty, eltnodes,
                                  3 * eltqty, &ierr);

  // setting Gmsh's characteristic length from the boundary mesh (see
  // nem_gmsh_worker_model_2d)
  gmshModelGeoSynchronize (&ierr);

  gmshModelGeoAddSurfaceLoop (&one, 1, 1, &ierr);
  gmshModelGeoAddVolume (&one, 1, 1, &ierr);
  gmshModelGeoSynchronize (&ierr);

  if (clgrad >= 0)
  {
    double factparms[NEM_GMSH_SIZEFACT_PARMQTY] = { 0 };
    nem_gmsh_worker_size_set (pts, nodeqty, clmax, clgrad, NEM_GMSH_SIZEFACT_NONE,
                              factparms);
  }

  ut_free_1d (&pts);
  ut_free_1d (&coos);
  ut_free_1d_int (&elts);
  free (nodetags);
  free (elttags);
  free (eltnodes);

  return 0;
}

// writes the mesh of dimension dim to the output
static int
nem_gmsh_worker_mesh (int out, int dim, int recombine)
{
  int i, j, ierr, status, nodeqty, eltqty, eltnodeqty = recombine ? 4 : dim + 1;
  int *index = NULL, *elts = NULL;
  size_t k, maxtag, *nodetags = NULL, *elttags = NULL, *eltnodes = NULL;
  size_t nodetagqty, cooqty, paraqty, elttagqty, eltnodetagqty;
  double *coos = NULL, *paras = NULL;

  // nodes, numbered from 1
  gmshModelMeshGetNodes (&nodetags, &nodetagqty, &coos, &cooqty, &paras,
                         &paraqty, -1, -1, 0, 0, &ierr);
  nodeqty = nodetagqty;

  maxtag = 0;
  for (k = 0; k < nodetagqty; k++)
    maxtag = (nodetags[k] > maxtag) ? nodetags[k] : maxtag;
  index = ut_alloc_1d_int (maxtag + 1);
  for (i = 0; i < nodeqty; i++)
    index[nodetags[i]] = i + 1;

  // elements (triangles or tetrahedra)
  gmshModelMeshGetElementsByType (dim == 2 ? 2 : 4, &elttags, &elttagqty,
                                  &eltnodes, &eltnodetagqty, -1, 0, 1, &ierr);
  eltqty = elttagqty;
  elts = ut_alloc_1d_int (eltnodeqty * eltqty + 1);
  for (k = 0; k < (size_t) eltqty; k++)
    for (j = 0; j < dim + 1; j++)
      elts[eltnodeqty * k + j] = index[eltnodes[(dim + 1) * k + j]];

  // (with recombination) quadrangles, after the triangles
  if (recombine)
  {
    gmshFree (elttags);
    gmshFree (eltnodes);
    gmshModelMeshGetElementsByType (3, &elttags, &elttagqty, &eltnodes,
                                    &eltnodetagqty, -1, 0, 1, &ierr);
    elts = ut_realloc_1d_int (elts, eltnodeqty * (eltqty + elttagqty) + 1);
    for (k = 0; k < eltnodetagqty; k++)
      elts[eltnodeqty * eltqty + k] = index[eltnodes[k]];
    eltqty += elttagqty;
  }

  status = nem_gmsh_write (out, &nodeqty, sizeof (int))
    || nem_gmsh_write (out, coos, 3 * nodeqty * sizeof (double))
    || nem_gmsh_write (out, &eltqty, sizeof (int))
    || nem_gmsh_write (out, elts, eltnodeqty * eltqty * sizeof (int));

  gmshFree (nodetags);
  gmshFree (coos);
  gmshFree (paras);
  gmshFree (elttags);
  gmshFree (eltnodes);
  ut_free_1d_int (&index);
  ut_free_1d_int (&elts);

  return status ? -1 : 0;
}

// watchdog of a worker: ends the worker when the calling process (its
// parent) has ended, i.e. when the worker has been reparented
static void *
nem_gmsh_worker_watchdog (void *data)
{
  pid_t parent = *(pid_t *) data;

  while (getppid () == parent)
    sleep (1);

  _exit (EXIT_FAILURE);

  return NULL;
}

int
nem_gmsh_worker (void)
{
  int ierr, head[3];
  double headd[3];
  static pid_t parent;
  pthread_t watchdog;

  parent = getppid ();
  if (pthread_create (&watchdog, NULL, nem_gmsh_worker_watchdog, &parent))
    return EXIT_FAILURE;
  pthread_detach (watchdog);

  nem_gmsh_initialize (&ierr);
  if (ierr)
    return EXIT_FAILURE;

  while (!nem_gmsh_read (NEM_GMSH_WORKER_IN, head, 3 * sizeof (int))
         && !nem_gmsh_read (NEM_GMSH_WORKER_IN, headd, 3 * sizeof (double)))
  {
    nem_gmsh_worker_reset ();
    nem_gmsh_worker_options (head[0], head[1], head[2], headd[0], headd[1],
                             headd[2]);

    if ((head[0] == 2
         && nem_gmsh_worker_model_2d (NEM_GMSH_WORKER_IN, headd[1], headd[2]))
        || (head[0] == 3
            && nem_gmsh_worker_model_3d (NEM_GMSH_WORKER_IN, headd[1],
                                         headd[2]))
        || (head[0] != 2 && head[0] != 3))
      break;

    // as with the gmsh binary, the mesh is returned even if errors
    // occurred (success is assessed from the mesh by the calling process)
    gmshModelMeshGenerate (head[0], &ierr);

    if (nem_gmsh_worker_mesh (NEM_GMSH_WORKER_OUT, head[0], head[0] == 2 && head[2]))
      break;
  }

  gmshFinalize (&ierr);

  return EXIT_SUCCESS;
}

// Calling side ========================================================

static int
nem_gmsh_worker_exe (char *exe, unsigned int size)
{
#ifdef __APPLE__
  uint32_t qty = size;
  return _NSGetExecutablePath (exe, &qty);
#else
  ssize_t qty = readlink ("/proc/self/exe", exe, size - 1);

  if (qty < 0)
    return -1;

  exe[qty] = '\0';

  return 0;
#endif
}

// creates a pipe whose file descriptors are closed on exec, so that they
// are not inherited by the other workers (a worker would then not get the
// end of its input when stopped), and are above those of the worker's input
// and output: a file descriptor equal to its target is not duplicated by
// posix_spawn_file_actions_adddup2, and on macOS it then keeps its
// close-on-exec flag (the first worker, whose pipe was often 3 and 4, then
// had no input, and every face it meshed failed)
static int
nem_gmsh_worker_pipe (int *fds)
{
  int i, tmp[2];

#if defined(__linux__)
  if (pipe2 (tmp, O_CLOEXEC))
    return -1;
#else
  if (pipe (tmp))
    return -1;
#endif

  for (i = 0; i < 2; i++)
    fds[i] = fcntl (tmp[i], F_DUPFD_CLOEXEC, NEM_GMSH_WORKER_OUT + 1);

  close (tmp[0]);
  close (tmp[1]);

  if (fds[0] < 0 || fds[1] < 0)
  {
    for (i = 0; i < 2; i++)
      if (fds[i] >= 0)
        close (fds[i]);
    return -1;
  }

  return 0;
}

static int
nem_gmsh_worker_start (struct NEM_GMSH_WORKER *pWorker)
{
  int in[2], out[2], status;
  char *exe = ut_alloc_1d_char (PATH_MAX + 1);
  char *argv[3] = { (char *) "neper", (char *) NEM_GMSH_WORKER_ARG, NULL };
  posix_spawn_file_actions_t actions;

  // writing to a dead worker must not kill the calling process
  signal (SIGPIPE, SIG_IGN);

  if (nem_gmsh_worker_exe (exe, PATH_MAX + 1) || nem_gmsh_worker_pipe (in)
      || nem_gmsh_worker_pipe (out))
  {
    ut_free_1d_char (&exe);
    return -1;
  }

  posix_spawn_file_actions_init (&actions);
  posix_spawn_file_actions_adddup2 (&actions, in[0], NEM_GMSH_WORKER_IN);
  posix_spawn_file_actions_adddup2 (&actions, out[1], NEM_GMSH_WORKER_OUT);
  posix_spawn_file_actions_addopen (&actions, STDIN_FILENO, "/dev/null",
                                    O_RDONLY, 0);
  posix_spawn_file_actions_addopen (&actions, STDOUT_FILENO, "/dev/null",
                                    O_WRONLY, 0);
  posix_spawn_file_actions_addopen (&actions, STDERR_FILENO, "/dev/null",
                                    O_WRONLY, 0);

  status = posix_spawn (&(*pWorker).pid, exe, &actions, NULL, argv, environ);

  posix_spawn_file_actions_destroy (&actions);
  close (in[0]);
  close (out[1]);
  ut_free_1d_char (&exe);

  if (status)
  {
    (*pWorker).pid = 0;
    close (in[1]);
    close (out[0]);
    return -1;
  }

  (*pWorker).in = in[1];
  (*pWorker).out = out[0];

  return 0;
}

static void
nem_gmsh_worker_stop (struct NEM_GMSH_WORKER *pWorker, int force)
{
  if (!(*pWorker).pid)
    return;

  if (force)
    kill ((*pWorker).pid, SIGKILL);

  close ((*pWorker).in);        // the worker exits when its input is closed
  close ((*pWorker).out);
  waitpid ((*pWorker).pid, NULL, 0);

  (*pWorker).pid = 0;

  return;
}

// reads size bytes from the worker, within the allowed time
static int
nem_gmsh_worker_read (struct NEM_GMSH_WORKER *pWorker, void *data,
                      size_t size, struct timeval *pbeg_time,
                      double allowed_t)
{
  int timeout;
  double remaining;
  ssize_t qty;
  struct pollfd pfd;

  pfd.fd = (*pWorker).out;
  pfd.events = POLLIN;

  while (size > 0)
  {
    remaining = allowed_t - ut_time_subtract (pbeg_time, NULL);
    if (remaining <= 0)
      return -1;
    timeout = (remaining < INT_MAX / 1000) ? (int) (1000 * remaining) + 1 : -1;

    qty = poll (&pfd, 1, timeout);
    if (qty < 0 && errno == EINTR)
      continue;
    if (qty <= 0)
      return -1;

    qty = read ((*pWorker).out, data, size);
    if (qty < 0 && errno == EINTR)
      continue;
    if (qty <= 0)               // worker died
      return -1;

    data = (char *) data + qty;
    size -= qty;
  }

  return 0;
}

// records a mesh (of dimension dim), as a single elset
static void
nem_mesh_gmsh_lib_recmesh (int dim, int eltnodeqty, int nodeqty, double *coos,
                           int eltqty, int *elts, struct NODES *pN, struct MESH *pM)
{
  int i, j, *elset = NULL;

  (*pN).NodeQty = nodeqty;
  (*pN).NodeCoo = ut_alloc_2d (nodeqty + 1, 3);
  for (i = 1; i <= nodeqty; i++)
    ut_array_1d_memcpy (coos + 3 * (i - 1), 3, (*pN).NodeCoo[i]);

  if (eltqty == 0)
    return;

  // (with recombination, tri elts are recorded as quad elts of last node 0)
  (*pM) = neut_mesh_alloc (dim, eltnodeqty == dim + 1 ? "tri" : "quad", 1, eltqty, 0);
  for (i = 1; i <= eltqty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pM).EltNodes[i][j] = elts[(i - 1) * eltnodeqty + j];

  elset = ut_alloc_1d_int (eltqty);
  for (i = 0; i < eltqty; i++)
    elset[i] = i + 1;
  neut_mesh_addelset (pM, elset, eltqty);
  neut_mesh_init_eltelset (pM, NULL);

  ut_free_1d_int (&elset);

  return;
}

// sends a request to the worker of the calling thread and records the
// mesh it returns; returns 0 if a mesh was returned, 1 otherwise
static int
nem_mesh_gmsh_lib_request (int dim, int eltnodeqty, struct NEM_GMSH_BUF Buf,
                           double allowed_t, struct NODES *pN,
                           struct MESH *pM)
{
  int nodeqty, eltqty, *elts = NULL, status = 1;
  double *coos = NULL;
  struct timeval beg_time;
  struct NEM_GMSH_WORKER *pWorker = NULL;

#ifdef HAVE_OPENMP
  if (omp_get_thread_num () >= NEM_GMSH_WORKER_MAX)
    abort ();
  pWorker = nem_gmsh_workers + omp_get_thread_num ();
#else
  pWorker = nem_gmsh_workers;
#endif

  neut_nodes_reset (pN);
  neut_mesh_free (pM);
  neut_mesh_reset (pM);

  // a worker that has ended while idle (Ctrl+C) is started again
  if ((*pWorker).pid
      && waitpid ((*pWorker).pid, NULL, WNOHANG) == (*pWorker).pid)
  {
    close ((*pWorker).in);
    close ((*pWorker).out);
    (*pWorker).pid = 0;
  }

  if (!(*pWorker).pid && nem_gmsh_worker_start (pWorker))
    return 1;

  gettimeofday (&beg_time, NULL);

  if (nem_gmsh_write ((*pWorker).in, Buf.data, Buf.size)
      || nem_gmsh_worker_read (pWorker, &nodeqty, sizeof (int), &beg_time,
                               allowed_t))
    goto end;

  coos = ut_alloc_1d (3 * nodeqty + 1);
  if (nem_gmsh_worker_read (pWorker, coos, 3 * nodeqty * sizeof (double),
                            &beg_time, allowed_t)
      || nem_gmsh_worker_read (pWorker, &eltqty, sizeof (int), &beg_time,
                               allowed_t))
    goto end;

  elts = ut_alloc_1d_int (eltnodeqty * eltqty + 1);
  if (nem_gmsh_worker_read (pWorker, elts, eltnodeqty * eltqty * sizeof (int),
                            &beg_time, allowed_t))
    goto end;

  nem_mesh_gmsh_lib_recmesh (dim, eltnodeqty, nodeqty, coos, eltqty, elts, pN, pM);
  status = 0;

end:
  // timeout, or worker died
  if (status)
    nem_gmsh_worker_stop (pWorker, 1);

  ut_free_1d (&coos);
  ut_free_1d_int (&elts);

  return status;
}

static void
nem_mesh_gmsh_lib_head (struct NEM_GMSH_BUF *pBuf, int dim, int algo,
                        int opti, double rnd, double clmax, double clgrad)
{
  nem_gmsh_buf_put_int (pBuf, dim);
  nem_gmsh_buf_put_int (pBuf, algo);
  nem_gmsh_buf_put_int (pBuf, opti);
  nem_gmsh_buf_put_double (pBuf, rnd);
  nem_gmsh_buf_put_double (pBuf, clmax);
  nem_gmsh_buf_put_double (pBuf, clgrad);

  return;
}

// facttype and factparms: mesh size factor of the face projection (see
// nem_mesh_gmsh_sizefact); with recombination (recombine = 1), the mesh is
// made of quad elts and tri elts, recorded as quad elts of last node 0
int
nem_mesh_gmsh_lib_2d (int algo, int recombine, double rnd, double clmax, double clgrad,
                      double **coos, double *cls, int qty, int facttype,
                      double *factparms, double allowed_t,
                      struct timeval *pctrlc_t, struct NODES *pN,
                      struct MESH *pM)
{
  int i, status;
  struct NEM_GMSH_BUF Buf = { NULL, 0, 0 };

  nem_mesh_gmsh_lib_head (&Buf, 2, algo, recombine, rnd, clmax, clgrad);

  nem_gmsh_buf_put_int (&Buf, qty);
  for (i = 0; i < qty; i++)
  {
    nem_gmsh_buf_put (&Buf, coos[i], 3 * sizeof (double));
    nem_gmsh_buf_put_double (&Buf, cls[i]);
  }
  nem_gmsh_buf_put_int (&Buf, facttype);
  nem_gmsh_buf_put (&Buf, factparms, NEM_GMSH_SIZEFACT_PARMQTY * sizeof (double));

  ut_sys_ctrlc_begin (pctrlc_t);
  status = nem_mesh_gmsh_lib_request (2, recombine ? 4 : 3, Buf, allowed_t, pN, pM);
  ut_sys_ctrlc_end (pctrlc_t);

  free (Buf.data);

  return status;
}

int
nem_mesh_gmsh_lib_3d (int algo, int opti, double rnd, double clmax,
                      double clgrad, struct NODES SkinNodes, struct MESH Skin,
                      double allowed_t, struct timeval *pctrlc_t,
                      struct NODES *pN, struct MESH *pM)
{
  int i, status;
  struct NEM_GMSH_BUF Buf = { NULL, 0, 0 };

  nem_mesh_gmsh_lib_head (&Buf, 3, algo, opti, rnd, clmax, clgrad);

  nem_gmsh_buf_put_int (&Buf, SkinNodes.NodeQty);
  for (i = 1; i <= SkinNodes.NodeQty; i++)
  {
    nem_gmsh_buf_put (&Buf, SkinNodes.NodeCoo[i], 3 * sizeof (double));
    nem_gmsh_buf_put_double (&Buf, SkinNodes.NodeCl[i]);
  }
  nem_gmsh_buf_put_int (&Buf, Skin.EltQty);
  for (i = 1; i <= Skin.EltQty; i++)
    nem_gmsh_buf_put (&Buf, Skin.EltNodes[i], 3 * sizeof (int));

  ut_sys_ctrlc_begin (pctrlc_t);
  status = nem_mesh_gmsh_lib_request (3, 4, Buf, allowed_t, pN, pM);
  ut_sys_ctrlc_end (pctrlc_t);

  free (Buf.data);

  return status;
}

void
nem_mesh_gmsh_lib_stop (void)
{
  int i;

  for (i = 0; i < NEM_GMSH_WORKER_MAX; i++)
    nem_gmsh_worker_stop (nem_gmsh_workers + i, 0);

  return;
}

// Mesh size factor of a face projection (by which the lengths are
// multiplied), at a point of the projection plane, of type (and parameters):
// - NEM_GMSH_SIZEFACT_NONE: 1 (none);
// - NEM_GMSH_SIZEFACT_STEREO: stereographic projection of a sphere, 1 + d^2
//   / q (centre of the projection, of which d is the distance, and q =
//   4 r^2);
// - NEM_GMSH_SIZEFACT_TORUS: conformal unrolling of a torus, rho0 / (R - r
//   cos v) (origin and unit axis of the plane coordinates, along which the
//   coordinate is rho0 w, rho0, R and r, see nem_mesh_gmsh_tor_w).
double
nem_mesh_gmsh_sizefact (int type, double *parms, double *coo)
{
  int i;
  double w;

  if (type == NEM_GMSH_SIZEFACT_NONE)
    return 1;

  else if (type == NEM_GMSH_SIZEFACT_STEREO)
    return 1 + ut_space_dist2 (coo, parms) / parms[3];

  else if (type == NEM_GMSH_SIZEFACT_TORUS)
  {
    w = 0;
    for (i = 0; i < 3; i++)
      w += (coo[i] - parms[i]) * parms[3 + i];
    w /= parms[6];

    return parms[6] / (parms[7] - parms[8] * cos (nem_mesh_gmsh_tor_v (parms[7], parms[8], w)));
  }

  else
    abort ();
}

// Conformal coordinate w of a torus (of major and minor radii R and r) at
// angle v about the tube (from the point closest to the axis), w = integral
// of r dv / (R - r cos v), i.e. 2 r / sqrt (R^2 - r^2) atan (sqrt ((R + r) /
// (R - r)) tan (v / 2)) for v in [-pi, pi), extended to any v by adding the
// value of the integral over a full turn, 2 pi r / sqrt (R^2 - r^2), per turn
double
nem_mesh_gmsh_tor_w (double R, double r, double v)
{
  double s = sqrt (R * R - r * r);
  double turn = floor ((v + M_PI) / (2 * M_PI));

  v -= 2 * M_PI * turn;

  return 2 * r / s * atan (sqrt ((R + r) / (R - r)) * tan (v / 2)) + 2 * M_PI * r / s * turn;
}

// Inverse of nem_mesh_gmsh_tor_w
double
nem_mesh_gmsh_tor_v (double R, double r, double w)
{
  double s = sqrt (R * R - r * r), period = 2 * M_PI * r / s;
  double turn = floor ((w + 0.5 * period) / period);

  w -= period * turn;

  return 2 * atan (tan (w * s / (2 * r)) / sqrt ((R + r) / (R - r))) + 2 * M_PI * turn;
}
