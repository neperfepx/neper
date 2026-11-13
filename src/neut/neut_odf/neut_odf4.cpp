/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_.h"
#include "neut/neut_oset/neut_oset.hpp"
#include "neut_struct_qcloud.hpp"
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>

// header of the cache files: magic string and size of the tree data
#define NEUT_ODF_TREE_MAGIC "NEPTREE1"

// Loads a kd-tree from a cache file; returns 0 if it matches the cloud, -1
// otherwise (and the tree is then not set).
int
neut_odf_comp_neigh_tree_load (char *filename, QCLOUD *pqcloud, my_kd_tree_t **pqtree)
{
  char magic[8];
  uint64_t size;
  struct stat st;
  FILE *fp = ut_file_open (filename, "RB");

  // checking the header, as reading an incomplete tree would crash
  if (fread (magic, 1, 8, fp) != 8 || memcmp (magic, NEUT_ODF_TREE_MAGIC, 8)
      || fread (&size, sizeof (uint64_t), 1, fp) != 1
      || fstat (fileno (fp), &st)
      || (uint64_t) st.st_size != 8 + sizeof (uint64_t) + size)
  {
    ut_file_close (fp, filename, "RB");
    return -1;
  }

  (*pqtree) = new my_kd_tree_t (4, *pqcloud);
  (**pqtree).loadIndex (fp);
  ut_file_close (fp, filename, "RB");

  if ((**pqtree).size (**pqtree) != (*pqcloud).pts.size ())
  {
    delete (*pqtree);
    (*pqtree) = NULL;
    return -1;
  }

  return 0;
}

// Name of the cache file of the tree: <cache>/orispace/<crysym>/<fr>-<hash>.<ext>,
// where <hash> is the FNV-1a hash of the orientation space mesh file, so that
// a changed mesh gets a new tree.  Returns -1 if there is no cache (or for a
// custom orientation space mesh).
int
neut_odf_comp_neigh_tree_cachefile (struct ODF *pOdf, char *data, char *ext,
                                    char **pcachefile)
{
  int c;
  uint64_t hash = 14695981039346656037ULL;
  char *msh = NULL, *subdir = NULL, *cache = NULL;
  FILE *fp = NULL;

  if (strncmp ((*pOdf).Sp.fr, "fr", 2) || strchr ((*pOdf).Sp.fr, '/'))
    return -1;

  msh = ut_alloc_1d_char (strlen (data) + strlen ((*pOdf).Sp.crysym)
                          + strlen ((*pOdf).Sp.fr) + 20);
  sprintf (msh, "%s/orispace/%s/%s.msh", data, (*pOdf).Sp.crysym, (*pOdf).Sp.fr);

  fp = fopen (msh, "rb");
  ut_free_1d_char (&msh);
  if (!fp)
    return -1;
  while ((c = fgetc (fp)) != EOF)
  {
    hash ^= (uint64_t) c;
    hash *= 1099511628211ULL;
  }
  fclose (fp);

  subdir = ut_string_paste ((char *) "orispace/", (*pOdf).Sp.crysym);
  if (neut_sys_cache (subdir, &cache))
  {
    ut_free_1d_char (&subdir);
    return -1;
  }

  (*pcachefile) = ut_alloc_1d_char (strlen (cache) + strlen ((*pOdf).Sp.fr)
                                    + strlen (ext) + 30);
  sprintf (*pcachefile, "%s/%s-%016llx.%s", cache, (*pOdf).Sp.fr,
           (unsigned long long) hash, ext);

  ut_free_1d_char (&subdir);
  ut_free_1d_char (&cache);

  return 0;
}

// Saves a tree to the cache, atomically (temporary file renamed), so that
// concurrent runs never read a partial file.  Failures are silent.
void
neut_odf_comp_neigh_tree_save (char *filename, my_kd_tree_t *qtree)
{
  int fd, status;
  FILE *fp = NULL;
  char *tmp = ut_string_paste (filename, (char *) ".XXXXXX");

  fd = mkstemp (tmp);
  if (fd == -1)
  {
    ut_free_1d_char (&tmp);
    return;
  }

  fp = fdopen (fd, "wb");
  if (!fp)
  {
    close (fd);
    unlink (tmp);
    ut_free_1d_char (&tmp);
    return;
  }

  uint64_t size = 0;
  long pos;
  status = (fwrite (NEUT_ODF_TREE_MAGIC, 1, 8, fp) != 8)
         || (fwrite (&size, sizeof (uint64_t), 1, fp) != 1);
  pos = ftell (fp);
  (*qtree).saveIndex (fp);
  size = (uint64_t) (ftell (fp) - pos);
  status |= fseek (fp, 8, SEEK_SET)
         || (fwrite (&size, sizeof (uint64_t), 1, fp) != 1);
  status |= ferror (fp);
  status |= fclose (fp);

  if (status || rename (tmp, filename))
    unlink (tmp);

  ut_free_1d_char (&tmp);

  return;
}
