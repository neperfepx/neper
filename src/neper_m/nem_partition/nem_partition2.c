/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"

#ifdef HAVE_LIBSCOTCH

void
nem_partition_init (struct IN_M In, struct PART *pPart)
{
  char *format = ut_alloc_1d_char (10);

  ut_string_format (In.partstring, format);

  if (strcmp (format, "%d") || sscanf (In.partstring, "%d", &(*pPart).qty) != 1
      || (*pPart).qty < 1)
    ut_print_message (2, 2, "Option -part: `%s' is not a valid number of partitions.\n",
                      In.partstring);

  ut_free_1d_char (&format);

  return;
}

void
nem_partition_prep (struct NODES *pNodes, struct MESH *Mesh,
                    struct PART *pPart, SCOTCH_Mesh * pSCMesh)
{
  int i, total;
  int dim = neut_mesh_array_dim (Mesh);

  (*pPart).NodeQty = ut_alloc_1d_int ((*pPart).qty);
  (*pPart).EltQty = ut_alloc_1d_int ((*pPart).qty);
  (*pPart).targeteltqty = ut_alloc_1d_int ((*pPart).qty);

  total = 0;
  for (i = 0; i < (*pPart).qty; i++)
  {
    (*pPart).targeteltqty[i] =
      Mesh[dim].EltQty * (double) (i + 1) / (*pPart).qty - total;
    total += (*pPart).targeteltqty[i];
  }
  assert (total == Mesh[dim].EltQty);

  if (Mesh[dim].NodeElts == NULL)
    neut_mesh_init_nodeelts (Mesh + dim, (*pNodes).NodeQty);

  SCOTCH_meshInit (pSCMesh);
  neut_mesh_scotchmesh (*pNodes, Mesh[dim], pSCMesh);

  return;
}

void
nem_partition_nodes (SCOTCH_Mesh * pSCMesh, struct NODES *pNodes,
                     struct MESH *Mesh, struct PART *pPart)
{
  SCOTCH_Num *parttab;
  int status;
  int dim = neut_mesh_array_dim (Mesh);
  int eltnodeqty = neut_elt_nodeqty (Mesh[dim].EltType, Mesh[dim].Dimension,
                                     Mesh[dim].EltOrder);
  int i;
  SCOTCH_Graph Graph, *pGraph = &Graph;
  SCOTCH_Strat Strat;
  int *version = ut_alloc_1d_int (3);

  SCOTCH_stratInit (&Strat);
  SCOTCH_graphInit (&Graph);

  SCOTCH_version (version, version + 1, version + 2);

  if (version[0] == 5 && version[1] == 1)
  {
    status =
      SCOTCH_stratGraphMap (&Strat,
                            "r{job=t,map=t,poli=S,sep=m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}|m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}}");
    if (status != 0)
      SCOTCH_stratGraphMap (&Strat,
                            "b{job=t,map=t,poli=S,sep=m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}|m{type=h,vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{dif=1,rem=1,pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}}");
  }
  else                          // >= 6
  {
    SCOTCH_stratGraphMap (&Strat,
                          "r{job=t,map=t,poli=S,sep=m{vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}|m{vert=80,low=h{pass=10}f{bal=0.0005,move=80}x,asc=b{bnd=d{pass=40}f{bal=0.005,move=80},org=f{bal=0.005,move=80}}x}}");
  }

  // SCOTCH_Num may be 32- or 64-bit, depending on the Scotch build
  parttab = (SCOTCH_Num *) calloc (Mesh[dim].EltQty * eltnodeqty * 2 + 2,
                                   sizeof (SCOTCH_Num));

  SCOTCH_meshGraph (pSCMesh, &Graph);

  // Scotch >= 7 is multithreaded; it uses Neper's number of threads
#if SCOTCH_VERSION >= 7
  SCOTCH_Context Context;
  SCOTCH_Graph ContextGraph;
  int threadqty = 1;
#ifdef HAVE_OPENMP
  threadqty = omp_get_max_threads ();
#endif

  SCOTCH_contextInit (&Context);
  SCOTCH_contextThreadSpawn (&Context, threadqty, NULL);
  SCOTCH_graphInit (&ContextGraph);
  SCOTCH_contextBindGraph (&Context, &Graph, &ContextGraph);
  pGraph = &ContextGraph;
#endif

  SCOTCH_graphPart (pGraph, (*pPart).qty, &Strat, parttab + 1);

#if SCOTCH_VERSION >= 7
  SCOTCH_graphExit (&ContextGraph);
  SCOTCH_contextExit (&Context);
#endif

  (*pPart).node_parts = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pPart).node_parts[0] = (*pNodes).NodeQty;
  for (i = 1; i <= (*pNodes).NodeQty; i++)
    (*pPart).node_parts[i] = (int) parttab[i];
  free (parttab);

  for (i = 0; i < (*pPart).qty; i++)
    (*pPart).NodeQty[i] =
      ut_array_1d_int_valnb ((*pPart).node_parts + 1, (*pNodes).NodeQty, i);

  ut_free_1d_int (&version);

  return;
}

void
nem_partition_elts (struct NODES *pNodes, struct MESH *Mesh, struct PART *pPart)
{
  int dim = neut_mesh_array_dim (Mesh);

  (*pPart).elt_parts = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
  (*pPart).elt_parts[0] = Mesh[dim].EltQty;
  ut_array_1d_int_set ((*pPart).elt_parts + 1, Mesh[dim].EltQty, -1);

  ut_array_1d_int_zero ((*pPart).EltQty, (*pPart).qty);

  nem_partition_elts_direct (Mesh, pPart);

  Mesh[dim].PartQty = (*pPart).qty;

  Mesh[dim].EltPart = ut_alloc_1d_int (Mesh[dim].EltQty + 1);

  ut_array_1d_int_memcpy ((*pPart).elt_parts + 1, Mesh[dim].EltQty,
                          Mesh[dim].EltPart + 1);
  ut_array_1d_int_addval (Mesh[dim].EltPart + 1, Mesh[dim].EltQty, 1, Mesh[dim].EltPart + 1);

  neut_mesh_init_parts (Mesh + dim);

  (*pNodes).NodePart = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  ut_array_1d_int_memcpy ((*pPart).node_parts + 1, (*pNodes).NodeQty,
                          (*pNodes).NodePart + 1);
  ut_array_1d_int_addval ((*pNodes).NodePart + 1, (*pNodes).NodeQty, 1, (*pNodes).NodePart + 1);

  neut_nodes_init_parts (pNodes);

  return;
}

void
nem_partition_stats (int partnbr, struct MESH Mesh, struct PART Part)
{
  int i, cutqty;
  int nmin, nmax, emin, emax;

  emin = ut_array_1d_int_min (Part.EltQty, partnbr);
  emax = ut_array_1d_int_max (Part.EltQty, partnbr);
  nmin = ut_array_1d_int_min (Part.NodeQty, partnbr);
  nmax = ut_array_1d_int_max (Part.NodeQty, partnbr);

  cutqty = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
    cutqty += neut_mesh_elt_comqty (Mesh, i, Part.node_parts, Part.elt_parts);

  ut_print_message (0, 3, "Nb of nodes per partition: min = %d, max = %d\n",
                    nmin, nmax);
  ut_print_message (0, 3, "Nb of elts  per partition: min = %d, max = %d\n",
                    emin, emax);
  ut_print_message (0, 3, "Nb of communications: %d\n", cutqty);

  return;
}

#endif /* HAVE_LIBSCOTCH */
