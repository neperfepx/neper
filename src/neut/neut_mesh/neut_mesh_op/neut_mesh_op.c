/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_op_.h"

struct MESH *
neut_mesh_alloc_1d (int size)
{
  struct MESH *array = (struct MESH *) calloc (size, sizeof (struct MESH));

  if (!array)
    ut_print_message (2, 0, "memory allocation failed! (%d)\n", size);

  return array;
}

void
neut_mesh_free_1d (struct MESH *array, int size)
{
  int i;

  if (!array)
    return;

  for (i = 0; i < size; i++)
    neut_mesh_free (array + i);

  free (array);
  array = NULL;

  return;
}

struct MESH
neut_mesh_alloc (int Dimension, char *EltType, int EltOrder, int EltQty,
                 int ElsetQty)
{
  struct MESH Mesh;
  int eltnodeqty = neut_elt_nodeqty (EltType, Dimension, EltOrder);

  neut_mesh_set_zero (&Mesh);

  Mesh.Dimension = Dimension;
  Mesh.EltOrder = EltOrder;
  ut_string_string (EltType, &(Mesh.EltType));

  Mesh.EltQty = EltQty;

  if (EltQty > 0)
  {
    Mesh.EltNodes = ut_alloc_2d_int (EltQty + 1, eltnodeqty);
    Mesh.EltElset = ut_alloc_1d_int (EltQty + 1);
  }
  else
  {
    Mesh.EltNodes = NULL;
    Mesh.EltElset = NULL;
  }

  Mesh.ElsetQty = ElsetQty;
  Mesh.ElsetId = NULL;

  if (ElsetQty > 0)
    Mesh.Elsets = ut_alloc_2d_int (Mesh.ElsetQty + 1, 2);
  else
    Mesh.Elsets = NULL;

  Mesh.CustomElsetQty = 0;
  Mesh.NodeQty = 0;
  Mesh.NodeElts = NULL;
  Mesh.EltBody = NULL;

  return Mesh;
}

void
neut_mesh_free (struct MESH *pMesh)
{
  if (!pMesh)
    return;

  ut_free_1d_char (&(*pMesh).Domain);

  ut_free_1d_char (&(*pMesh).EltType);

  if ((*pMesh).EltNodes)
    ut_free_2d_int (&(*pMesh).EltNodes, (*pMesh).EltQty + 1);

  ut_free_1d_int (&(*pMesh).ElsetId);

  ut_free_2d_int (&(*pMesh).Elsets, (*pMesh).ElsetQty + 1);

  ut_free_1d_int (&(*pMesh).EltElset);

  ut_free_1d_int (&(*pMesh).EltPart);

  ut_free_2d (&(*pMesh).ElsetOri, (*pMesh).ElsetQty - (*pMesh).CustomElsetQty + 1);

  ut_free_2d_char (&(*pMesh).ElsetLabels, (*pMesh).ElsetQty + 1);

  ut_free_1d_char (&(*pMesh).ElsetOriDes);

  ut_free_2d (&(*pMesh).EltOri, (*pMesh).EltQty);

  ut_free_2d (&(*pMesh).SimEltOri, (*pMesh).EltQty);

  ut_free_1d_char (&(*pMesh).EltOriDes);

  ut_free_1d_int (&(*pMesh).ElsetGroup);

  ut_free_1d_char (&(*pMesh).ElsetCrySym);

  if ((*pMesh).NodeElts)
   ut_free_2d_int (&(*pMesh).NodeElts, (*pMesh).NodeQty + 1);

 ut_free_1d_int (&(*pMesh).EltBody);

  neut_mesh_set_zero (pMesh);

  return;
}

void
neut_mesh_set_zero (struct MESH *pMesh)
{
  (*pMesh).Domain = NULL;

  (*pMesh).Dimension = 0;
  (*pMesh).EltOrder = 0;
  (*pMesh).EltType = NULL;

  (*pMesh).EltQty = 0;
  (*pMesh).ElsetQty = 0;
  (*pMesh).CustomElsetQty = 0;

  (*pMesh).EltNodes = NULL;

  (*pMesh).ElsetId = NULL;
  (*pMesh).Elsets = NULL;
  (*pMesh).ElsetLabels = NULL;

  (*pMesh).NodeQty = 0;

  (*pMesh).NodeElts = NULL;
  (*pMesh).EltElset = NULL;
  (*pMesh).EltBody = NULL;

  (*pMesh).PartQty = 0;
  (*pMesh).Parts = NULL;

  (*pMesh).EltPart = NULL;

  (*pMesh).ElsetOri = NULL;
  (*pMesh).ElsetOriDes = NULL;

  (*pMesh).EltOri = NULL;
  (*pMesh).EltOriDes = NULL;

  (*pMesh).ElsetGroup = NULL;

  (*pMesh).ElsetCrySym = NULL;

  (*pMesh).pSim = NULL;
  (*pMesh).SimEltOri = NULL;

  return;
}

void
neut_mesh_reset (struct MESH *pMesh)
{
  neut_mesh_free (pMesh);

  neut_mesh_set_zero (pMesh);

  return;
}

void
neut_mesh_memcpy (struct MESH Old, struct MESH *pNew)
{
  int i, eltnodeqty;

  neut_mesh_reset (pNew);

  (*pNew).Dimension = Old.Dimension;
  (*pNew).EltOrder = Old.EltOrder;
  (*pNew).EltQty = Old.EltQty;
  (*pNew).NodeQty = Old.NodeQty;
  (*pNew).ElsetQty = Old.ElsetQty;

  if (Old.EltType)
    ut_string_string (Old.EltType, &(*pNew).EltType);

  if (Old.EltNodes)
  {
    eltnodeqty =
      neut_elt_nodeqty ((*pNew).EltType, (*pNew).Dimension, (*pNew).EltOrder);

    (*pNew).EltNodes = ut_alloc_2d_int (Old.EltQty + 1, eltnodeqty);

    for (i = 1; i <= Old.EltQty; i++)
      ut_array_1d_int_memcpy (Old.EltNodes[i], eltnodeqty,
                              (*pNew).EltNodes[i]);
  }

  if (Old.ElsetId)
  {
    (*pNew).ElsetId = ut_alloc_1d_int (Old.ElsetQty + 1);
    ut_array_1d_int_memcpy (Old.ElsetId + 1, Old.ElsetQty,
                            (*pNew).ElsetId + 1);
  }

  if (Old.Elsets)
  {
    (*pNew).Elsets = ut_alloc_1d_pint (Old.ElsetQty + 1);
    for (i = 1; i <= Old.ElsetQty; i++)
    {
      (*pNew).Elsets[i] = ut_alloc_1d_int (Old.Elsets[i][0] + 1);
      ut_array_1d_int_memcpy (Old.Elsets[i], Old.Elsets[i][0] + 1,
                              (*pNew).Elsets[i]);
    }
  }

  if (Old.EltElset)
  {
    (*pNew).EltElset = ut_alloc_1d_int (Old.EltQty + 1);
    ut_array_1d_int_memcpy (Old.EltElset + 1, Old.EltQty,
                            (*pNew).EltElset + 1);
  }

  if (Old.EltBody)
  {
    (*pNew).EltBody = ut_alloc_1d_int (Old.EltQty + 1);
    ut_array_1d_int_memcpy (Old.EltBody + 1, Old.EltQty,
                            (*pNew).EltBody + 1);
  }

  if (Old.EltPart)
  {
    (*pNew).EltPart = ut_alloc_1d_int (Old.EltQty + 1);
    ut_array_1d_int_memcpy (Old.EltPart + 1, Old.EltQty,
                            (*pNew).EltPart + 1);
  }

  if (Old.ElsetOriDes)
    ut_string_string (Old.ElsetOriDes, &(*pNew).ElsetOriDes);

  if (Old.ElsetOri)
  {
    (*pNew).ElsetOri = ut_alloc_2d (Old.ElsetQty + 1, 4);
    ut_array_2d_memcpy (Old.ElsetOri + 1, Old.ElsetQty, 4,
                        (*pNew).ElsetOri + 1);
  }

  if (Old.EltOri)
  {
    (*pNew).EltOri = ut_alloc_2d (Old.EltQty + 1, 4);
    ut_array_2d_memcpy (Old.EltOri + 1, Old.EltQty, 4,
                        (*pNew).EltOri + 1);
  }

  if (Old.SimEltOri)
  {
    (*pNew).SimEltOri = ut_alloc_2d (Old.EltQty + 1, 4);
    ut_array_2d_memcpy (Old.SimEltOri + 1, Old.EltQty, 4,
                        (*pNew).SimEltOri + 1);
  }

  if (Old.ElsetGroup)
  {
    (*pNew).ElsetGroup = ut_alloc_1d_int (Old.EltQty + 1);
    ut_array_1d_int_memcpy (Old.ElsetGroup + 1, Old.EltQty,
                            (*pNew).ElsetGroup + 1);
  }

  if (Old.ElsetCrySym)
    ut_string_string (Old.ElsetCrySym, &(*pNew).ElsetCrySym);

  if (Old.Parts)
  {
    (*pNew).Parts = ut_alloc_1d_pint (Old.PartQty + 1);
    for (i = 1; i <= Old.PartQty; i++)
    {
      (*pNew).Parts[i] = ut_alloc_1d_int (Old.Parts[i][0] + 1);
      ut_array_1d_int_memcpy (Old.Parts[i], Old.Parts[i][0] + 1,
                              (*pNew).Parts[i]);
    }
  }

  return;
}

#ifdef HAVE_LIBSCOTCH
void
neut_mesh_scotchmesh (struct MESH Mesh, int vnodnbr, SCOTCH_Mesh * pSCMesh)
{
  int i, id, status;
  int velmbas, vnodbas, velmnbr, vertnbr, edgenbr;
  int *verttab, *edgetab;
  int eltnodeqty =
    neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  velmbas = 1;                  /* element base number */
  vnodbas = 1 + Mesh.EltQty;    /* node base number */
  velmnbr = Mesh.EltQty;

  vertnbr = velmnbr + vnodnbr;
  edgenbr = 2 * velmnbr * eltnodeqty;

  verttab = ut_alloc_1d_int (vertnbr + 1);
  edgetab = ut_alloc_1d_int (edgenbr + 1);

  /* recording elements */
  id = 0;
  for (i = 1; i <= velmnbr; i++)
  {
    verttab[i - 1] = id + 1;
    ut_array_1d_int_memcpy (Mesh.EltNodes[i], eltnodeqty, edgetab + id);
    ut_array_1d_int_addval (edgetab + id, eltnodeqty, vnodbas - 1,
                            edgetab + id);
    id += eltnodeqty;
  }

  /* recording nodes */
  for (i = 1; i <= vnodnbr; i++)
  {
    verttab[i + Mesh.EltQty - 1] = id + 1;
    ut_array_1d_int_memcpy (Mesh.NodeElts[i] + 1, Mesh.NodeElts[i][0],
                            edgetab + id);
    id += Mesh.NodeElts[i][0];
  }
  verttab[vertnbr] = edgenbr + 1;

  SCOTCH_meshBuild (pSCMesh, velmbas, vnodbas, velmnbr, vnodnbr, verttab,
                    NULL, NULL, NULL, NULL, edgenbr, edgetab);
  /*
     file = ut_file_open ("dd", "w");
     SCOTCH_meshSave (pSCMesh, file);
     ut_file_close (file, "dd", "w");
   */

  status = SCOTCH_meshCheck (pSCMesh);
  if (status != 0)
    printf ("error");

  return;
}
#endif /* HAVE_LIBSCOTCH */

/* nodes & elements are considered to be numbered contiguously from 1 */
void
neut_mesh_init_nodeelts (struct MESH *pMesh, int NodeQty)
{
  int i, j, eltnodeqty, node;

  if ((*pMesh).NodeElts)
    ut_free_2d_int (&(*pMesh).NodeElts, (*pMesh).NodeQty + 1);

  if (pMesh == NULL || (*pMesh).EltQty == 0)
    return;

  eltnodeqty =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                      (*pMesh).EltOrder);

  if (NodeQty <= 0)
    for (i = 1; i <= (*pMesh).EltQty; i++)
      for (j = 0; j < eltnodeqty; j++)
        NodeQty = ut_num_max (NodeQty, (*pMesh).EltNodes[i][j]);

  (*pMesh).NodeElts = ut_alloc_2d_int (NodeQty + 1, 1);
  (*pMesh).NodeQty = NodeQty;

  /* first pass: recording, for each node, the quantity of elements */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).NodeElts[(*pMesh).EltNodes[i][j]][0]++;

  /* allocation */
  for (i = 1; i <= NodeQty; i++)
    (*pMesh).NodeElts[i] =
      ut_realloc_1d_int ((*pMesh).NodeElts[i], (*pMesh).NodeElts[i][0] + 1);

  ut_array_2d_int_zero ((*pMesh).NodeElts + 1, NodeQty, 1);

  /* recording node elements */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
    {
      node = (*pMesh).EltNodes[i][j];
      (*pMesh).NodeElts[node][++(*pMesh).NodeElts[node][0]] = i;
    }

  return;
}

void
neut_mesh_array_init_nodeelts (struct MESH *Mesh, int NodeQty)
{
  int i, dim = neut_mesh_array_dim (Mesh);

  for (i = 0; i <= dim; i++)
    if (Mesh[i].EltQty > 0)
      neut_mesh_init_nodeelts (Mesh + i, NodeQty);

  return;
}

void
neut_mesh_init_eltelset (struct MESH *pMesh, int *elset_nbs)
{
  int i, j;

  (*pMesh).EltElset =
    ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty + 1);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).EltElset[(*pMesh).Elsets[i][j]] = elset_nbs ? elset_nbs[i] : i;

  return;
}

void
neut_mesh_init_elsets (struct MESH *pMesh)
{
  int i, j, k, elt, neltqty, node, firstelt;
  int *elts = NULL;
  int *nelts = NULL;

  neut_mesh_init_nodeelts (pMesh, 0);

  (*pMesh).ElsetQty = 0;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    (*pMesh).ElsetQty = ut_num_max ((*pMesh).EltElset[i], (*pMesh).ElsetQty);

  (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
    neut_mesh_elset_addelt (pMesh, (*pMesh).EltElset[i], i);

  // In case of 1D elements, ordering elements in the elsets, as well as
  // the nodes in the elements.
  if ((*pMesh).Dimension == 1)
  {
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      // nothing to do (don't remove this)
      if ((*pMesh).Elsets[i][0] <= 1)
        continue;

      elts = ut_alloc_1d_int ((*pMesh).Elsets[i][0] + 1);
      elts[0] = -1;

      firstelt = 0;
      // Looking for an element that is at the extremity of the elset
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      {
        elt = (*pMesh).Elsets[i][j];
        neut_mesh_elt_elset_neighelts (*pMesh, elt, i, &nelts, &neltqty);

        if (neltqty <= 1)
        {
          firstelt = elt;
          break;
        }
      }

      int loopelset = 0;

      if (firstelt > 0)
        elts[1] = firstelt;

      // an exception is when the 1D mesh is reconstructed from a 2D mesh
      // with a single elset (e.g. a mesh of a face). In this case, the
      // 1D elset is a loop, which must be handled in a slightly different way.
      else
      {
        loopelset = 1;
        elts[1] = (*pMesh).Elsets[i][1];
      }

      if (loopelset == 0)
      {
        // Ordering the nodes of the first element - the first node must
        // be at the extremity of the elset
        node = (*pMesh).EltNodes[elts[1]][0];

        // == 1 means no other elset, so is at the extremity
        // == 2 means has 2 elts, that may or may not belong the the
        // current elset.  If not, the node is at the extremity.
        // Otherwise, need to reverse nodes.
        if ((*pMesh).NodeElts[node][0] == 2)
        {
          if ((*pMesh).EltElset[(*pMesh).NodeElts[node][1]] ==
              (*pMesh).EltElset[(*pMesh).NodeElts[node][2]])
            neut_mesh_elt_reversenodes (pMesh, elts[1]);
        }
      }

      for (j = 2; j <= (*pMesh).Elsets[i][0]; j++)
      {
        neut_mesh_elt_elset_neighelts (*pMesh, elts[j - 1], i, &nelts,
                                       &neltqty);

        // for an open elset (general case), the neighbor must belong to
        // the same elset and be different from the already registered
        // element (j - 2).
        if (loopelset == 0)
        {
          for (k = 0; k < neltqty; k++)
            if (nelts[k] != elts[j - 2])
            {
              elts[j] = nelts[k];
              break;
            }
        }
        // for a loop elset, the neighbor must share the second node of
        // the previous element.
        else
        {
          for (k = 0; k < neltqty; k++)
          {
            if (ut_array_1d_int_eltpos
                ((*pMesh).EltNodes[nelts[k]], 2,
                 (*pMesh).EltNodes[elts[j - 1]][1]) != -1)
            {
              elts[j] = nelts[k];
              break;
            }
          }
        }

        if (elts[j] == 0)
          ut_print_neperbug ();

        // reversing elt nodes if necessary
        if ((*pMesh).EltNodes[elts[j]][0] !=
            (*pMesh).EltNodes[elts[j - 1]][1])
        {
          neut_mesh_elt_reversenodes (pMesh, elts[j]);
          /*
             if ((*pMesh).EltNodes[elts[j]][0]
             != (*pMesh).EltNodes[elts[j - 1]][1])
             ut_print_neperbug ();
           */
        }
      }

      // recording new element sequence
      ut_array_1d_int_memcpy (elts + 1, (*pMesh).Elsets[i][0],
                              (*pMesh).Elsets[i] + 1);

      ut_free_1d_int (&elts);
    }
  }

  ut_free_1d_int (&nelts);

  return;
}

void
neut_mesh_init_elsets_2 (struct MESH *pMesh, int *elt_nbs, int **pelset_nbs)
{
  int i, qty, elset, elset_inv;
  int elset_nb_max;
  int *elsets_old_new = NULL;
  int contiguous = 0;

  if (elt_nbs == NULL)
    contiguous = 1;

  if ((*pMesh).EltQty <= 0)
    return;

  if (contiguous == 0)
  {
    (*pelset_nbs) = ut_alloc_1d_int ((*pMesh).EltQty + 1);

    ut_array_1d_int_memcpy ((*pMesh).EltElset + 1, (*pMesh).EltQty,
                            (*pelset_nbs) + 1);

    ut_array_1d_int_sort_uniq ((*pelset_nbs) + 1, (*pMesh).EltQty,
                               &((*pelset_nbs)[0]));

    (*pelset_nbs) = ut_realloc_1d_int ((*pelset_nbs), (*pelset_nbs)[0] + 1);

    (*pMesh).ElsetQty = (*pelset_nbs)[0];

    elset_nb_max = ut_array_1d_int_max ((*pelset_nbs) + 1, (*pMesh).ElsetQty);

    elsets_old_new = ut_alloc_1d_int (elset_nb_max + 1);
    for (i = 1; i <= (*pelset_nbs)[0]; i++)
      elsets_old_new[(*pelset_nbs)[i]] = i;

    /* memory allocation of Elsets */
    (*pMesh).Elsets = ut_alloc_1d_pint ((*pMesh).ElsetQty + 1);
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      qty =
        ut_array_1d_int_valnb ((*pMesh).EltElset + 1, (*pMesh).EltQty,
                               (*pelset_nbs)[i]);
      (*pMesh).Elsets[i] = ut_alloc_1d_int (qty + 1);
    }

    /* filling of Elsets */
    for (i = 1; i <= (*pMesh).EltQty; i++)
    {
      elset = (*pMesh).EltElset[i];
      elset_inv = elsets_old_new[elset];
      (*pMesh).Elsets[elset_inv][0]++;
      (*pMesh).Elsets[elset_inv][(*pMesh).Elsets[elset_inv][0]] = elt_nbs[i];
    }

    ut_free_1d_int (&elsets_old_new);
  }
  else
  {
    (*pMesh).ElsetQty =
      ut_array_1d_int_max ((*pMesh).EltElset + 1, (*pMesh).EltQty);

    /* memory allocation of Elsets */
    (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1);
    for (i = 1; i <= (*pMesh).EltQty; i++)
      (*pMesh).Elsets[(*pMesh).EltElset[i]][0]++;

    for (i = 1; i <= (*pMesh).ElsetQty; i++)
    {
      (*pMesh).Elsets[i] =
        ut_realloc_1d_int ((*pMesh).Elsets[i], (*pMesh).Elsets[i][0] + 1);
      (*pMesh).Elsets[i][0] = 0;
    }

    /* filling of Elsets */
    for (i = 1; i <= (*pMesh).EltQty; i++)
    {
      elset = (*pMesh).EltElset[i];
      (*pMesh).Elsets[elset][0]++;
      (*pMesh).Elsets[elset][(*pMesh).Elsets[elset][0]] = i;
    }
  }

  return;
}

void
neut_mesh_init_parts (struct MESH *pMesh)
{
  int i, part;

  (*pMesh).PartQty = ut_array_1d_int_max ((*pMesh).EltPart + 1, (*pMesh).EltQty);

  (*pMesh).Parts = ut_alloc_2d_int ((*pMesh).PartQty + 1, 1);

  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    part = (*pMesh).EltPart[i];

    (*pMesh).Parts[part][0]++;

    (*pMesh).Parts[part] = ut_realloc_1d_int ((*pMesh).Parts[part], (*pMesh).Parts[part][0] + 1);

    (*pMesh).Parts[part][(*pMesh).Parts[part][0]] = i;
  }

  return;
}

void
neut_mesh_init_elsetlabels (struct MESH *pMesh)
{
  int i;
  char *tmp = NULL;

  if ((*pMesh).Dimension == 0)
    ut_string_string ("ver", &tmp);
  else if ((*pMesh).Dimension == 1)
    ut_string_string ("edge", &tmp);
  else if ((*pMesh).Dimension == 2)
    ut_string_string ("face", &tmp);
  else if ((*pMesh).Dimension == 3)
    ut_string_string ("poly", &tmp);

  (*pMesh).ElsetLabels = ut_alloc_2d_char ((*pMesh).ElsetQty + 1, 20);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    sprintf ((*pMesh).ElsetLabels[i], "%s%d", tmp, (*pMesh).ElsetId ? (*pMesh).ElsetId[i] : i);

  ut_free_1d_char (&tmp);

  return;
}

void
neut_mesh_mergeelsets (struct MESH *pEMesh)
{
  int i, tot;

  tot = 0;
  for (i = 1; i <= (*pEMesh).ElsetQty; i++)
    tot += (*pEMesh).Elsets[i][0];

  (*pEMesh).Elsets[1] = ut_realloc_1d_int ((*pEMesh).Elsets[1], tot + 1);

  for (i = 2; i <= (*pEMesh).ElsetQty; i++)
  {
    ut_array_1d_int_memcpy ((*pEMesh).Elsets[i] + 1, (*pEMesh).Elsets[i][0],
                            (*pEMesh).Elsets[1] + (*pEMesh).Elsets[1][0] + 1);
    (*pEMesh).Elsets[1][0] += (*pEMesh).Elsets[i][0];
    ut_free_1d_int (&(*pEMesh).Elsets[i]);
  }

  if ((*pEMesh).Elsets[1][0] != tot)
    abort ();

  ut_array_1d_int_sort ((*pEMesh).Elsets[1] + 1, (*pEMesh).Elsets[1][0]);

  // FIXME
  if ((*pEMesh).ElsetId)
    ut_print_neperbug ();

  (*pEMesh).ElsetQty = 1;

  return;
}

/* returns 1 if meshes match, 0 else */
// TODO: this is brute force method; get less computer intensive
int
neut_mesh_cmp (struct NODES N1, struct MESH M1, struct NODES N2,
               struct MESH M2)
{
  int i, j, k, l;
  int res;
  int eltnodeqty = neut_elt_nodeqty (M1.EltType, M1.Dimension, M1.EltOrder);
  double *dist = ut_alloc_1d (eltnodeqty);
  double min;

  res = 0;

  if (M1.EltQty != M2.EltQty)
    res = 0;
  else
    // loop over M1 elts; for each, loop over M2 to search a matching elt
    for (i = 1; i <= M1.EltQty; i++)
    {
      res = 0;
      for (j = 1; j <= M2.EltQty; j++)
      {
        res = 1;
        for (k = 0; k < eltnodeqty; k++)        // loop over M1 elt nodes
        {
          for (l = 0; l < eltnodeqty; l++)      // loop over M2 elt nodes
            dist[l] =
              ut_space_dist (N1.NodeCoo[M1.EltNodes[i][k]],
                             N2.NodeCoo[M2.EltNodes[j][l]]);

          min = ut_array_1d_min (dist, eltnodeqty);
          if (min > 1e-9)
          {
            res = 0;
            break;
          }
        }

        if (res == 1)
          break;
      }

      if (res == 0)
        break;
    }

  ut_free_1d (&dist);

  return res;
}

// Turn a quad mesh into a tri mesh by  element subdivision
void
neut_mesh_quad_tri (struct NODES Nodes, struct MESH Mesh, struct MESH *pTMesh)
{
  int i, j, k, elt;
  int **nodepos = NULL;
  int eltnodeqty = -1;
  int triqty = -1;
  // `triqty' is the number of tet/tri elements for an hex/quad element.

  if (strcmp (Mesh.EltType, "quad") != 0)
    ut_print_neperbug ();

  // Vertices of the tets describing the cube elt
  if (Mesh.Dimension == 3)
  {
    triqty = 6;
    eltnodeqty = 4;
    nodepos = ut_alloc_2d_int (triqty, eltnodeqty);

    nodepos[0][0] = 0;
    nodepos[0][1] = 1;
    nodepos[0][2] = 2;
    nodepos[0][3] = 6;

    nodepos[1][0] = 0;
    nodepos[1][1] = 3;
    nodepos[1][2] = 2;
    nodepos[1][3] = 6;

    nodepos[2][0] = 0;
    nodepos[2][1] = 6;
    nodepos[2][2] = 3;
    nodepos[2][3] = 7;

    nodepos[3][0] = 0;
    nodepos[3][1] = 6;
    nodepos[3][2] = 4;
    nodepos[3][3] = 7;

    nodepos[4][0] = 0;
    nodepos[4][1] = 4;
    nodepos[4][2] = 5;
    nodepos[4][3] = 6;

    nodepos[5][0] = 0;
    nodepos[5][1] = 5;
    nodepos[5][2] = 1;
    nodepos[5][3] = 6;
  }
  else if (Mesh.Dimension == 2)
  {
    triqty = 2;
    eltnodeqty = 3;
    nodepos = ut_alloc_2d_int (triqty, eltnodeqty);

    nodepos[0][0] = 0;
    nodepos[0][1] = 1;
    nodepos[0][2] = 2;

    nodepos[1][0] = 0;
    nodepos[1][1] = 2;
    nodepos[1][2] = 3;
  }
  else
    ut_print_neperbug ();

  // General data
  (*pTMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pTMesh).EltType, "tri");
  (*pTMesh).Dimension = Mesh.Dimension;
  (*pTMesh).EltOrder = 1;

  (*pTMesh).EltQty = 0;
  (*pTMesh).EltNodes = ut_alloc_2d_int (Mesh.EltQty * triqty + 1, eltnodeqty);

  // Elt data
  if (Mesh.EltElset != NULL)
    (*pTMesh).EltElset = ut_alloc_1d_int (Mesh.EltQty * triqty + 1);

  for (i = 1; i <= Mesh.EltQty; i++)
    for (j = 0; j < triqty; j++)
    {
      elt = ++((*pTMesh).EltQty);

      for (k = 0; k < eltnodeqty; k++)
        (*pTMesh).EltNodes[elt][k] = Mesh.EltNodes[i][nodepos[j][k]];

      if (Mesh.EltElset != NULL)
        (*pTMesh).EltElset[elt] = Mesh.EltElset[i];
    }

  neut_mesh_init_elsets (pTMesh);

  neut_mesh_init_nodeelts (pTMesh, Nodes.NodeQty);

  ut_free_2d_int (&nodepos, triqty);

  return;
}

// Turn a quad mesh into a tri mesh by  element subdivision
void
neut_mesh_quad_tri_insertnode (struct NODES *pNodes, struct MESH Mesh,
                               struct MESH *pTMesh)
{
  int i, j, k, elt, newnode, eltnode;
  double *coo = ut_alloc_1d (3);

  if (strcmp (Mesh.EltType, "quad") != 0)
    ut_print_neperbug ();

  // General data
  (*pTMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pTMesh).EltType, "tri");
  (*pTMesh).Dimension = Mesh.Dimension;
  (*pTMesh).EltOrder = 1;

  (*pTMesh).EltQty = 0;
  (*pTMesh).EltNodes = ut_alloc_2d_int (Mesh.EltQty * 4 + 1, 3);

  // Elt data
  if (Mesh.EltElset)
    (*pTMesh).EltElset = ut_alloc_1d_int (Mesh.EltQty * 4 + 1);

  for (i = 1; i <= Mesh.EltQty; i++)
  {
    newnode = (*pNodes).NodeQty + 1;
    neut_mesh_elt_centre (*pNodes, Mesh, i, coo);
    neut_nodes_addnode (pNodes, coo, 0);

    for (j = 0; j < 4; j++)
    {
      elt = ++((*pTMesh).EltQty);

      (*pTMesh).EltNodes[elt][0] = newnode;
      for (k = 1; k < 3; k++)
      {
        eltnode = Mesh.EltNodes[i][ut_array_rotpos (0, 3, j, k - 1)];
        (*pTMesh).EltNodes[elt][k] = eltnode;
      }

      if (Mesh.EltElset)
        (*pTMesh).EltElset[elt] = Mesh.EltElset[i];
    }
  }

  neut_mesh_init_elsets (pTMesh);

  neut_mesh_init_nodeelts (pTMesh, (*pNodes).NodeQty);

  ut_free_1d (&coo);

  return;
}

void
neut_mesh_tri_prism (struct NODES *pNodes, struct MESH Mesh,
                     struct MESH *pPMesh, double thickness)
{
  int i, j;
  double coo[3];
  int NodeQty = (*pNodes).NodeQty;

  if (Mesh.Dimension != 2)
    ut_print_neperbug ();

  (*pPMesh).EltType = ut_alloc_1d_char (6);
  strcpy ((*pPMesh).EltType, "prism");
  (*pPMesh).Dimension = 3;
  (*pPMesh).EltOrder = 1;

  (*pPMesh).EltQty = Mesh.EltQty;
  (*pPMesh).EltNodes = ut_alloc_2d_int ((*pPMesh).EltQty + 1, 6);

  (*pPMesh).EltElset = ut_alloc_1d_int ((*pPMesh).EltQty + 1);

  for (i = 1; i <= Mesh.EltQty; i++)
  {
    for (j = 0; j < 3; j++)
    {
      (*pPMesh).EltNodes[i][j] = Mesh.EltNodes[i][j];
      (*pPMesh).EltNodes[i][j] = Mesh.EltNodes[i][j] + (*pNodes).NodeQty;
    }
  }

  for (i = 1; i <= NodeQty; i++)
  {
    coo[0] = (*pNodes).NodeCoo[i][0];
    coo[1] = (*pNodes).NodeCoo[i][1];
    coo[2] = (*pNodes).NodeCoo[i][2] + thickness;
    neut_nodes_addnode (pNodes, coo, 0);
  }

  neut_mesh_init_elsets (pPMesh);

  neut_mesh_init_nodeelts (pPMesh, (*pNodes).NodeQty);

  return;
}

void
neut_mesh_tri_tet (struct NODES *pNodes, struct MESH Mesh,
                   struct MESH *pTMesh, double thickness)
{
  int i, j, k;
  int NodeQty_ini = (*pNodes).NodeQty;
  int indir[6], prismnodes[6], newelt;
  double coo[3];

  if (Mesh.Dimension != 2)
    ut_print_neperbug ();

  int *tet[3] = { NULL, NULL, NULL };
  int tet1_1[4] = { 0, 1, 2, 5 };
  int tet2_1[4] = { 0, 1, 5, 4 };
  int tet3_1[4] = { 0, 4, 5, 3 };
  int tet1_2[4] = { 0, 1, 2, 4 };
  int tet2_2[4] = { 0, 4, 2, 5 };
  int tet3_2[4] = { 0, 4, 5, 3 };

  if (strcmp (Mesh.EltType, "tri") != 0 || Mesh.Dimension != 2)
    ut_print_neperbug ();

  // add nodes

  for (i = 1; i <= NodeQty_ini; i++)
  {
    coo[0] = (*pNodes).NodeCoo[i][0];
    coo[1] = (*pNodes).NodeCoo[i][1];
    coo[2] = (*pNodes).NodeCoo[i][2] + thickness;
    neut_nodes_addnode (pNodes, coo, 0);
  }

  // subdivide into tet

  (*pTMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pTMesh).EltType, "tri");
  (*pTMesh).Dimension = 3;
  (*pTMesh).EltOrder = 1;

  (*pTMesh).EltQty = 0;
  (*pTMesh).EltNodes = ut_alloc_2d_int (3 * Mesh.EltQty + 1, 4);

  (*pTMesh).EltElset = ut_alloc_1d_int (3 * Mesh.EltQty + 1);

  for (i = 1; i <= Mesh.EltQty; i++)
  {
    for (k = 0; k < 3; k++)
    {
      prismnodes[k] = Mesh.EltNodes[i][k];
      prismnodes[k + 3] = prismnodes[k] + NodeQty_ini;
    }

    // find minimum vertex and set indirection
    if (prismnodes[0] < prismnodes[1] && prismnodes[0] < prismnodes[2])
    {
      indir[0] = 0;
      indir[1] = 1;
      indir[2] = 2;
      indir[3] = 3;
      indir[4] = 4;
      indir[5] = 5;
    }
    else if (prismnodes[1] < prismnodes[0] && prismnodes[1] < prismnodes[2])
    {
      indir[0] = 1;
      indir[1] = 2;
      indir[2] = 0;
      indir[3] = 4;
      indir[4] = 5;
      indir[5] = 3;
    }
    else if (prismnodes[2] < prismnodes[0] && prismnodes[2] < prismnodes[1])
    {
      indir[0] = 2;
      indir[1] = 0;
      indir[2] = 1;
      indir[3] = 5;
      indir[4] = 3;
      indir[5] = 4;
    }

    // select subdivision manner
    if (prismnodes[indir[1]] < prismnodes[indir[2]])
    {
      tet[0] = tet1_1;
      tet[1] = tet2_1;
      tet[2] = tet3_1;
    }
    else if (prismnodes[indir[1]] > prismnodes[indir[2]])
    {
      tet[0] = tet1_2;
      tet[1] = tet2_2;
      tet[2] = tet3_2;
    }
    else
      ut_print_neperbug ();

    // fill MESH
    for (k = 0; k < 3; k++)
    {
      newelt = ++((*pTMesh).EltQty);
      for (j = 0; j < 4; j++)
      {
        (*pTMesh).EltNodes[newelt][j] = prismnodes[indir[tet[k][j]]];
      }

      (*pTMesh).EltElset[newelt] = Mesh.EltElset[i];
    }
  }

  neut_mesh_init_elsets (pTMesh);
  neut_mesh_init_elsetlabels (pTMesh);

  neut_mesh_init_nodeelts (pTMesh, (*pNodes).NodeQty);

  return;
}

int
neut_mesh_rmelts (struct MESH *pMesh, struct NODES Nodes, int *rmelt,
                  int rmeltqty)
{
  int i, j, eltmin, eltmax, EltQty;
  int *new_old = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int *old_new = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  ut_array_1d_int_sort (rmelt, rmeltqty);
  eltmin = rmelt[0];
  eltmax = rmelt[rmeltqty - 1];

  EltQty = 0;
  for (i = 1; i < eltmin; i++)
    new_old[++EltQty] = i;

  if ((eltmax - eltmin + 1) != rmeltqty)
    for (i = eltmin; i <= eltmax; i++)
      if (ut_array_1d_int_eltpos (rmelt, rmeltqty, i) == -1)
        new_old[++EltQty] = i;

  for (i = eltmax + 1; i <= (*pMesh).EltQty; i++)
    new_old[++EltQty] = i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    old_new[new_old[i]] = i;

  // Updating EltNodes
  for (i = 1; i <= EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[new_old[i]], eltnodeqty,
                            (*pMesh).EltNodes[i]);
  (*pMesh).EltNodes =
    ut_realloc_2d_int_delline ((*pMesh).EltNodes, (*pMesh).EltQty + 1,
                               EltQty + 1);

  // Updating EltElset
  if ((*pMesh).EltElset != NULL)
    for (i = 1; i <= EltQty; i++)
      (*pMesh).EltElset[i] = (*pMesh).EltElset[new_old[i]];
  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, EltQty + 1);

  // Updating EltBody
  if ((*pMesh).EltBody != NULL)
    for (i = 1; i <= EltQty; i++)
      (*pMesh).EltBody[i] = (*pMesh).EltBody[new_old[i]];
  (*pMesh).EltBody = ut_realloc_1d_int ((*pMesh).EltBody, EltQty + 1);

  // Updating NodeElts
  if ((*pMesh).NodeElts != NULL)
  {
    ut_free_2d_int (&(*pMesh).NodeElts, Nodes.NodeQty + 1);
    neut_mesh_init_nodeelts (pMesh, Nodes.NodeQty);
  }

  // Updating Elsets
  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = old_new[(*pMesh).Elsets[i][j]];

  (*pMesh).EltQty = EltQty;

  ut_free_1d_int (&new_old);
  ut_free_1d_int (&old_new);

  return 0;
}

int
neut_mesh_rmelt (struct MESH *pMesh, int elt)
{
  int i, elset, status, node;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  elset = (*pMesh).EltElset[elt];

  // Updating EltNodes
  for (i = elt; i < (*pMesh).EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[i + 1], eltnodeqty,
                            (*pMesh).EltNodes[i]);

  // Updating Elsets
  status =
    ut_array_1d_int_rmelt ((*pMesh).Elsets[elset] + 1,
                           (*pMesh).Elsets[elset][0], elt, 1);
  if (status != 1)
    ut_print_neperbug ();
  else
    (*pMesh).Elsets[elset][0]--;

  // Updating EltElset
  if ((*pMesh).EltElset != NULL)
    for (i = elt; i < (*pMesh).EltQty; i++)
      (*pMesh).EltElset[i] = (*pMesh).EltElset[i + 1];
  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty);

  // Updating EltBody
  if ((*pMesh).EltBody != NULL)
    for (i = elt; i < (*pMesh).EltQty; i++)
      (*pMesh).EltBody[i] = (*pMesh).EltBody[i + 1];
  (*pMesh).EltBody = ut_realloc_1d_int ((*pMesh).EltBody, (*pMesh).EltQty);

  // Updating NodeElts
  if ((*pMesh).NodeElts != NULL)
    for (i = 0; i < eltnodeqty; i++)
    {
      node = (*pMesh).EltNodes[elt][i];
      status =
        ut_array_1d_int_rmelt ((*pMesh).NodeElts[node] + 1,
                               (*pMesh).NodeElts[node][0], elt, 1);
      if (status != 1)
        ut_print_neperbug ();
      (*pMesh).NodeElts[node][0]--;
    }

  (*pMesh).EltQty--;

  return 0;
}

int
neut_mesh_rmelset (struct MESH *pMesh, struct NODES Nodes, int elset)
{
  int i, j, EltQty;
  int *new_old = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int *old_new = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  ut_array_1d_int_sort ((*pMesh).Elsets[elset] + 1,
                        (*pMesh).Elsets[elset][0]);

  EltQty = 0;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    if ((*pMesh).EltElset[i] != elset)
      new_old[++EltQty] = i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    old_new[new_old[i]] = i;

  // Updating EltNodes
  for (i = 1; i <= EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[new_old[i]], eltnodeqty,
                            (*pMesh).EltNodes[i]);
  (*pMesh).EltNodes =
    ut_realloc_2d_int_delline ((*pMesh).EltNodes, (*pMesh).EltQty + 1,
                               EltQty + 1);

  // Updating EltElset
  if ((*pMesh).EltElset != NULL)
    for (i = 1; i <= EltQty; i++)
      (*pMesh).EltElset[i] = (*pMesh).EltElset[new_old[i]];
  (*pMesh).EltElset = ut_realloc_1d_int ((*pMesh).EltElset, EltQty + 1);

  // Updating EltBody
  if ((*pMesh).EltBody != NULL)
    for (i = 1; i <= EltQty; i++)
      (*pMesh).EltBody[i] = (*pMesh).EltBody[new_old[i]];
  (*pMesh).EltBody = ut_realloc_1d_int ((*pMesh).EltBody, EltQty + 1);

  // Updating Elsets
  (*pMesh).Elsets[elset] = ut_realloc_1d_int ((*pMesh).Elsets[elset], 1);
  (*pMesh).Elsets[elset][0] = 0;

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = old_new[(*pMesh).Elsets[i][j]];

  (*pMesh).EltQty = EltQty;

  // Updating NodeElts
  if ((*pMesh).NodeElts != NULL)
  {
    ut_free_2d_int (&(*pMesh).NodeElts, Nodes.NodeQty + 1);
    neut_mesh_init_nodeelts (pMesh, Nodes.NodeQty);
  }

  ut_free_1d_int (&new_old);
  ut_free_1d_int (&old_new);

  return 0;
}

// Ordering elements to all point towards the same side of the face
// (all topological, no buggy normal determination)
void
neut_mesh_orderelsets (struct MESH *pMesh)
{
  int i, j, elt, neltqty;
  int *id = NULL;
  int qty;
  int *elt_ref = NULL;
  int *nelts = NULL;
  int *elts = ut_alloc_1d_int (2);
  int *nodes = NULL;
  int NodeQty;
  int ori1, ori2;
  int *tmp = NULL;

  if ((*pMesh).Dimension != 2)
    ut_print_neperbug ();

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
  {
    id = ut_alloc_1d_int ((*pMesh).Elsets[i][0]);
    elt_ref = ut_alloc_1d_int ((*pMesh).EltQty + 1);

    id[0] = (*pMesh).Elsets[i][1];
    elt = id[0];
    qty = 1;
    neut_mesh_elt_elset_neighelts (*pMesh, elt, i, &tmp, &neltqty);
    ut_array_1d_int_memcpy (tmp, neltqty, id + qty);
    ut_free_1d_int (&tmp);

    qty += neltqty;
    for (j = 0; j < qty; j++)
      elt_ref[id[j]] = elt;

    int pos = 1;
    while (pos < qty)
    {
      elt = id[pos];

      // checking element orientation
      elts[0] = elt_ref[elt];
      elts[1] = elt;
      neut_mesh_elts_comnodes (*pMesh, elts, 2, &nodes, &NodeQty);
      if (NodeQty < 2)
        ut_print_neperbug ();

      neut_mesh_elt_nodes_ori (*pMesh, elts[0], nodes, &ori1);
      neut_mesh_elt_nodes_ori (*pMesh, elts[1], nodes, &ori2);

      if (ori1 == 0 || ori2 == 0)
      {
        neut_debug_mesh (stdout, *pMesh);
        printf ("elts = %d %d\n", elts[0], elts[1]);
        printf ("nodes = %d %d\n", nodes[0], nodes[1]);
        printf ("ori1 = %d ori2 = %d\n", ori1, ori2);
        ut_print_neperbug ();
      }

      if (ori1 == ori2)
        neut_mesh_elt_reversenodes (pMesh, elt);

      // recording element neighbors
      neut_mesh_elt_elset_neighelts (*pMesh, elt, i, &nelts, &neltqty);
      for (j = 0; j < neltqty; j++)
        if (elt_ref[nelts[j]] == 0)
        {
          elt_ref[nelts[j]] = elt;
          id[qty++] = nelts[j];
        }

      pos++;
    }

    ut_free_1d_int (&elt_ref);
  }

  ut_free_1d_int (&nelts);
  ut_free_1d_int (&elts);
  ut_free_1d_int (&nodes);

  return;
}

void
neut_mesh_addelt (struct MESH *pMesh, int *EltNodes)
{
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  (*pMesh).EltQty++;

  (*pMesh).EltNodes =
    ut_realloc_1d_pint ((*pMesh).EltNodes, (*pMesh).EltQty + 1);
  (*pMesh).EltNodes[(*pMesh).EltQty] = ut_alloc_1d_int (eltnodeqty);
  (*pMesh).EltNodes[0] = NULL;  /* mandatory */

  ut_array_1d_int_memcpy (EltNodes, eltnodeqty,
                          (*pMesh).EltNodes[(*pMesh).EltQty]);

  if ((*pMesh).EltElset)
    (*pMesh).EltElset =
      ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty + 1);

  /* update NodeElts */
  /* if ((*pMesh).NodeElts != NULL) */

  return;
}

void
neut_mesh_addelset (struct MESH *pMesh, int *ElsetElts, int EltQty)
{
  int i;

  (*pMesh).ElsetQty++;

  (*pMesh).Elsets =
    ut_realloc_1d_pint ((*pMesh).Elsets, (*pMesh).ElsetQty + 1);
  (*pMesh).Elsets[0] = NULL;    /* mandatory */

  (*pMesh).Elsets[(*pMesh).ElsetQty] = ut_alloc_1d_int (EltQty + 1);
  (*pMesh).Elsets[(*pMesh).ElsetQty][0] = EltQty;
  if (EltQty == (*pMesh).EltQty || ElsetElts == NULL)
    for (i = 1; i <= EltQty; i++)
      (*pMesh).Elsets[(*pMesh).ElsetQty][i] = i;
  else
    ut_array_1d_int_memcpy (ElsetElts, EltQty,
                            (*pMesh).Elsets[(*pMesh).ElsetQty] + 1);

  return;
}

void
neut_mesh_elset_addelt (struct MESH *pMesh, int elset_nb, int elt)
{
  (*pMesh).Elsets[elset_nb][0]++;

  (*pMesh).Elsets[elset_nb] =
    ut_realloc_1d_int ((*pMesh).Elsets[elset_nb],
                       (*pMesh).Elsets[elset_nb][0] + 1);

  (*pMesh).Elsets[elset_nb][(*pMesh).Elsets[elset_nb][0]] = elt;

  return;
}

void
neut_mesh_elt_reversenodes (struct MESH *pMesh, int eltnb)
{
  // implemented for 1D and 2D only.

  // For 2D, the 1st and 2nd order nodes are to be considered separetely
  // for reversing.
  if ((*pMesh).Dimension == 1)
  {
    ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb], 2);
  }
  else if ((*pMesh).Dimension == 2)
  {
    if (!strcmp ((*pMesh).EltType, "tri"))
    {
      if ((*pMesh).EltOrder == 1)
        ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 2);
      else if ((*pMesh).EltOrder == 2)
      {
        ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 2);
        ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 3, 3);
      }
      else
        abort ();
    }
    else if (!strcmp ((*pMesh).EltType, "quad"))
    {
      if ((*pMesh).EltOrder == 1)
        ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 3);
      else if ((*pMesh).EltOrder == 2)
      {
        ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 1, 3);
        ut_array_1d_int_reverseelts ((*pMesh).EltNodes[eltnb] + 4, 4);
      }
      else
        abort ();
    }
    else
      ut_print_neperbug ();
  }
  else
    abort ();

  return;
}

void
neut_mesh_elset_reversenodes (struct MESH *pMesh, int elset)
{
  int i;

  for (i = 1; i <= (*pMesh).Elsets[elset][0]; i++)
    neut_mesh_elt_reversenodes (pMesh, (*pMesh).Elsets[elset][i]);

  return;
}

void
neut_mesh_reversenodes (struct MESH *pMesh)
{
  int i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    neut_mesh_elt_reversenodes (pMesh, i);

  return;
}

void
neut_mesh_elset_reverseelts (struct MESH *pMesh, int elset)
{
  ut_array_1d_int_reverseelts ((*pMesh).Elsets[elset] + 1,
                               (*pMesh).Elsets[elset][0]);

  return;
}

void
neut_mesh_elts_switch_pair (struct MESH *pMesh, int n1, int n2)
{
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  ut_array_2d_int_switchlines ((*pMesh).EltNodes, eltnodeqty, n1, n2);

  return;
}

/* RAM could be improved by more elegant exchange of lines (not through
 * a big copy eltnodes array */
void
neut_mesh_elts_switch (struct MESH *pMesh, int *elt_nbs)
{
  int i, j;
  int **eltnodes = NULL;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                                     (*pMesh).EltOrder);

  eltnodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[i], eltnodeqty, eltnodes[i]);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    ut_array_1d_int_memcpy (eltnodes[i], eltnodeqty,
                            (*pMesh).EltNodes[elt_nbs[i]]);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = elt_nbs[(*pMesh).Elsets[i][j]];

  neut_mesh_init_eltelset (pMesh, NULL);

  if ((*pMesh).NodeElts)
  {
    int NodeQty = (*pMesh).NodeQty;
    ut_free_2d_int (&(*pMesh).NodeElts, NodeQty + 1);
    neut_mesh_init_nodeelts (pMesh, NodeQty);

    // the code below would be a little faster but does not work
    /*
       for (i = 1; i <= (*pMesh).NodeQty; i++)
       for (j = 1; j <= (*pMesh).NodeElts[i][0]; j++)
       (*pMesh).NodeElts[i][j] = elt_nbs[(*pMesh).NodeElts[i][j]];
     */
  }

  ut_free_2d_int (&eltnodes, (*pMesh).EltQty);

  return;
}

/* node_nbs[...] = node qty then new pos */
void
neut_mesh_nodes_switch (struct MESH *pMesh, int *node_nbs)
{
  int i;
  int j, eltnodeqty;

  eltnodeqty =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                      (*pMesh).EltOrder);

  /* neut_mesh_switch (pMesh, node_nbs, NULL); */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).EltNodes[i][j] = node_nbs[(*pMesh).EltNodes[i][j]];

  return;
}

void
neut_mesh_switch (struct MESH *pMesh, int *nodes_old_new, int *elts_old_new,
                  int *elsets_old_new)
{
  int i, j, eltnodeqty;

  /* correcting elt nodes */
  if (nodes_old_new != NULL)
  {
    eltnodeqty =
      neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
                        (*pMesh).EltOrder);

    for (i = 1; i <= (*pMesh).EltQty; i++)
      for (j = 0; j < eltnodeqty; j++)
        (*pMesh).EltNodes[i][j] = nodes_old_new[(*pMesh).EltNodes[i][j]];
  }

  /* correcting elset elts */
  if (elts_old_new != NULL)
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
        (*pMesh).Elsets[i][j] = elts_old_new[(*pMesh).Elsets[i][j]];

  /* correcting elt elsets */
  if (elsets_old_new != NULL)
  {
    for (i = 1; i <= (*pMesh).EltQty; i++)
      (*pMesh).EltElset[i] = elsets_old_new[(*pMesh).EltElset[i]];

    if ((*pMesh).ElsetId)
      ut_print_neperbug ();
  }

  return;
}

void
neut_mesh_renumber_continuous (struct MESH *pMesh, int *node_nbs,
                               int *elt_nbs, int *elset_nbs)
{
  int i, elt_nb_max, elset_nb_max;
  int *nodes_old_new = NULL;
  int *elts_old_new = NULL;
  int *elsets_old_new = NULL;

  int node_nb_max = ut_array_1d_int_max (node_nbs + 1, node_nbs[0]);

  if (node_nbs)
  {
    nodes_old_new = ut_alloc_1d_int (node_nb_max + 1);
    for (i = 1; i <= node_nbs[0]; i++)
      nodes_old_new[node_nbs[i]] = i;
  }

  if (elt_nbs)
  {
    elt_nb_max = ut_array_1d_int_max (elt_nbs + 1, (*pMesh).EltQty);

    elts_old_new = ut_alloc_1d_int (elt_nb_max + 1);
    for (i = 1; i <= (*pMesh).EltQty; i++)
      elts_old_new[elt_nbs[i]] = i;

    elset_nb_max = ut_array_1d_int_max (elset_nbs + 1, (*pMesh).ElsetQty);

    elsets_old_new = ut_alloc_1d_int (elset_nb_max + 1);
    for (i = 1; i <= elset_nbs[0]; i++)
      elsets_old_new[elset_nbs[i]] = i;

    neut_mesh_switch (pMesh, nodes_old_new, elts_old_new, elsets_old_new);
  }

  ut_free_1d_int (&nodes_old_new);
  ut_free_1d_int (&elts_old_new);
  ut_free_1d_int (&elsets_old_new);

  return;
}

void
neut_mesh_elt_split (struct NODES *pNodes, struct MESH *pMesh, int elt)
{
  double *coo = ut_alloc_1d (3);
  int *nodes = ut_alloc_1d_int (3);
  int newnode, newelt1, newelt2;

  if ((*pMesh).Dimension != 2)
    ut_print_neperbug ();

  if ((*pMesh).NodeElts == NULL)
    ut_print_neperbug ();

  ut_array_1d_int_memcpy ((*pMesh).EltNodes[elt], 3, nodes);

  neut_mesh_elt_centre (*pNodes, *pMesh, elt, coo);
  neut_nodes_addnode (pNodes, coo, -1);
  newnode = (*pNodes).NodeQty;
  (*pMesh).NodeQty = (*pNodes).NodeQty;

  neut_mesh_addelt (pMesh, nodes);
  neut_mesh_addelt (pMesh, nodes);
  newelt1 = (*pMesh).EltQty - 1;
  newelt2 = (*pMesh).EltQty;
  neut_mesh_elset_addelt (pMesh, (*pMesh).EltElset[elt], newelt1);
  neut_mesh_elset_addelt (pMesh, (*pMesh).EltElset[elt], newelt2);
  (*pMesh).EltElset =
    ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty + 1);
  (*pMesh).EltElset[newelt1] = (*pMesh).EltElset[elt];
  (*pMesh).EltElset[newelt2] = (*pMesh).EltElset[elt];

  ut_array_1d_int_rotate ((*pMesh).EltNodes[newelt1], 3, 1);
  ut_array_1d_int_rotate ((*pMesh).EltNodes[newelt2], 3, 2);

  (*pMesh).EltNodes[elt][2] = newnode;
  (*pMesh).EltNodes[newelt1][2] = newnode;
  (*pMesh).EltNodes[newelt2][2] = newnode;

  (*pMesh).NodeElts[nodes[0]][0]++;
  (*pMesh).NodeElts[nodes[0]] =
    ut_realloc_1d_int ((*pMesh).NodeElts[nodes[0]],
                       (*pMesh).NodeElts[nodes[0]][0] + 1);
  (*pMesh).NodeElts[nodes[0]][(*pMesh).NodeElts[nodes[0]][0]] = newelt2;

  (*pMesh).NodeElts[nodes[1]][0]++;
  (*pMesh).NodeElts[nodes[1]] =
    ut_realloc_1d_int ((*pMesh).NodeElts[nodes[1]],
                       (*pMesh).NodeElts[nodes[1]][0] + 1);
  (*pMesh).NodeElts[nodes[1]][(*pMesh).NodeElts[nodes[1]][0]] = newelt1;

  (*pMesh).NodeElts[nodes[2]][0]++;
  (*pMesh).NodeElts[nodes[2]] =
    ut_realloc_1d_int ((*pMesh).NodeElts[nodes[2]],
                       (*pMesh).NodeElts[nodes[2]][0] + 1);
  (*pMesh).NodeElts[nodes[2]][(*pMesh).NodeElts[nodes[2]][0]] = newelt2;

  ut_array_1d_int_findnreplace ((*pMesh).NodeElts[nodes[2]] + 1,
                                (*pMesh).NodeElts[nodes[2]][0], elt, newelt1);

  (*pMesh).NodeElts =
    ut_realloc_2d_int_addline ((*pMesh).NodeElts, (*pMesh).NodeQty + 1, 4);
  (*pMesh).NodeElts[newnode][0] = 3;
  (*pMesh).NodeElts[newnode][1] = elt;
  (*pMesh).NodeElts[newnode][2] = newelt1;
  (*pMesh).NodeElts[newnode][3] = newelt2;

  /*
     if (neut_mesh_test (*pMesh) != 0)
     ut_print_neperbug ();
   */

  ut_free_1d (&coo);
  ut_free_1d_int (&nodes);

  return;
}

void
neut_mesh_eltpair_flip (struct MESH *pMesh, int elt1, int elt2)
{
  int i;
  int elt1change, elt1mine, elt1keep;
  int elt2mine, elt2keep;
  int elt1change_pos, elt1mine_pos, elt1keep_pos;
  int elt2change_pos, elt2mine_pos, elt2keep_pos;

  if ((*pMesh).Dimension != 2 || (*pMesh).EltOrder != 1)
    ut_print_neperbug ();

  // looking for the nodes belonging to elt1 and elt2 only, respectively
  elt1mine_pos = -1;
  for (i = 0; i < 3; i++)
    if (ut_array_1d_int_eltpos
        ((*pMesh).EltNodes[elt2], 3, (*pMesh).EltNodes[elt1][i]) == -1)
    {
      elt1mine_pos = i;
      break;
    }

  if (elt1mine_pos == -1)
    abort ();

  elt1mine = (*pMesh).EltNodes[elt1][elt1mine_pos];

  elt2mine_pos = -1;
  for (i = 0; i < 3; i++)
    if (ut_array_1d_int_eltpos
        ((*pMesh).EltNodes[elt1], 3, (*pMesh).EltNodes[elt2][i]) == -1)
    {
      elt2mine_pos = i;
      break;
    }

  if (elt2mine_pos == -1)
    abort ();

  elt2mine = (*pMesh).EltNodes[elt2][elt2mine_pos];

  // picking a node to keep in elt1
  elt1keep_pos = (elt1mine_pos != 0) ? 0 : 1;
  elt1keep = (*pMesh).EltNodes[elt1][elt1keep_pos];

  // the node to change is the other one...
  elt1change_pos = 3 - (elt1mine_pos + elt1keep_pos);
  elt1change = (*pMesh).EltNodes[elt1][elt1change_pos];

  // in elt2, keeping the node that elt1 changes
  elt2keep_pos =
    ut_array_1d_int_eltpos ((*pMesh).EltNodes[elt2], 4, elt1change);
  elt2keep = (*pMesh).EltNodes[elt2][elt2keep_pos];

  // the node to change is the other one...
  elt2change_pos = 3 - (elt2mine_pos + elt2keep_pos);

  // changing the nodes for the node that only the other elt had
  (*pMesh).EltNodes[elt1][elt1change_pos] = elt2mine;
  (*pMesh).EltNodes[elt2][elt2change_pos] = elt1mine;

  // updating NodeElts
  (*pMesh).NodeElts[elt1mine][0]++;
  (*pMesh).NodeElts[elt1mine] =
    ut_realloc_1d_int ((*pMesh).NodeElts[elt1mine],
                       (*pMesh).NodeElts[elt1mine][0] + 1);
  (*pMesh).NodeElts[elt1mine][(*pMesh).NodeElts[elt1mine][0]] = elt2;

  (*pMesh).NodeElts[elt2mine][0]++;
  (*pMesh).NodeElts[elt2mine] =
    ut_realloc_1d_int ((*pMesh).NodeElts[elt2mine],
                       (*pMesh).NodeElts[elt2mine][0] + 1);
  (*pMesh).NodeElts[elt2mine][(*pMesh).NodeElts[elt2mine][0]] = elt1;

  if (ut_array_1d_int_rmelt
      ((*pMesh).NodeElts[elt1keep] + 1, (*pMesh).NodeElts[elt1keep][0], elt2,
       1) != 1)
    ut_print_neperbug ();

  (*pMesh).NodeElts[elt1keep][0]--;

  (*pMesh).NodeElts[elt1keep] =
    ut_realloc_1d_int ((*pMesh).NodeElts[elt1keep],
                       (*pMesh).NodeElts[elt1keep][0] + 1);

  if (ut_array_1d_int_rmelt
      ((*pMesh).NodeElts[elt2keep] + 1, (*pMesh).NodeElts[elt2keep][0], elt1,
       1) != 1)
    ut_print_neperbug ();

  (*pMesh).NodeElts[elt2keep][0]--;

  (*pMesh).NodeElts[elt2keep] =
    ut_realloc_1d_int ((*pMesh).NodeElts[elt2keep],
                       (*pMesh).NodeElts[elt2keep][0] + 1);

  /*
     if (neut_mesh_test (*pMesh) != 0)
     ut_print_neperbug ();
   */

  return;
}

void
neut_mesh_eltpair_split (struct NODES *pNodes, struct MESH *pMesh, int elt1,
                         int elt2)
{
  int i, j, elt, node;
  int newnode;
  int *elts = ut_alloc_1d_int (2);
  int *newelts = ut_alloc_1d_int (2);
  int *allelts = ut_alloc_1d_int (4);
  int *allnodes = NULL;
  int *comnodes = NULL;
  int allnodeqty;
  int comnodeqty;
  double *bary = ut_alloc_1d (3);

  if ((*pMesh).Dimension != 2 || (*pMesh).EltOrder != 1)
    ut_print_neperbug ();

  elts[0] = elt1;
  elts[1] = elt2;

  neut_mesh_elts_allnodes (*pMesh, elts, 2, &allnodes, &allnodeqty);
  neut_mesh_elts_comnodes (*pMesh, elts, 2, &comnodes, &comnodeqty);

  ut_array_1d_add ((*pNodes).NodeCoo[comnodes[0]],
                   (*pNodes).NodeCoo[comnodes[1]], 3, bary);
  ut_array_1d_scale (bary, 3, .5);

  // Rotating EltNodes so that the node not shared with the other elt is at [0].
  for (i = 0; i < 2; i++)
    for (j = 0; j < 3; j++)
      if (ut_array_1d_int_eltpos
          (comnodes, comnodeqty, (*pMesh).EltNodes[elts[i]][j]) == -1)
      {
        ut_array_1d_int_rotate ((*pMesh).EltNodes[elts[i]], 3, j);
        break;
      }

  neut_nodes_addnode (pNodes, bary, -1);
  newnode = (*pNodes).NodeQty;
  (*pMesh).NodeQty = (*pNodes).NodeQty;

  for (i = 0; i < 2; i++)
  {
    elt = elts[i];
    neut_mesh_addelt (pMesh, (*pMesh).EltNodes[elt]);
    newelts[i] = (*pMesh).EltQty;
    neut_mesh_elset_addelt (pMesh, (*pMesh).EltElset[elt], newelts[i]);
    (*pMesh).EltElset =
      ut_realloc_1d_int ((*pMesh).EltElset, (*pMesh).EltQty + 1);
    (*pMesh).EltElset[newelts[i]] = (*pMesh).EltElset[elt];

    if (i == 0)
    {
      (*pMesh).EltNodes[elt][2] = newnode;
      (*pMesh).EltNodes[newelts[i]][1] = newnode;
    }
    else
    {
      (*pMesh).EltNodes[elt][1] = newnode;
      (*pMesh).EltNodes[newelts[i]][2] = newnode;
    }
  }
  ut_array_1d_int_memcpy (elts, 2, allelts);
  ut_array_1d_int_memcpy (newelts, 2, allelts + 2);

  // cleaning NodeElts[comnodes[]] from elt1 and elt2, and
  // reinitializing them

  for (i = 0; i < 4; i++)
  {
    node = allnodes[i];

    for (j = 0; j < 2; j++)
      (*pMesh).NodeElts[node][0] -=
        ut_array_1d_int_rmelt ((*pMesh).NodeElts[node] + 1,
                               (*pMesh).NodeElts[node][0], elts[j], 1);

    for (j = 0; j < 4; j++)
    {
      elt = allelts[j];
      if (ut_array_1d_int_eltpos ((*pMesh).EltNodes[elt], 3, node) != -1)
      {
        (*pMesh).NodeElts[node][0]++;
        (*pMesh).NodeElts[node] =
          ut_realloc_1d_int ((*pMesh).NodeElts[node],
                             (*pMesh).NodeElts[node][0] + 1);
        (*pMesh).NodeElts[node][(*pMesh).NodeElts[node][0]] = elt;
      }
    }
  }

  // Setting NodeElts for the new node

  (*pMesh).NodeElts =
    ut_realloc_2d_int_addline ((*pMesh).NodeElts, (*pNodes).NodeQty + 1, 5);

  (*pMesh).NodeElts[newnode][0] = 4;
  (*pMesh).NodeElts[newnode][1] = elt1;
  (*pMesh).NodeElts[newnode][2] = elt2;
  (*pMesh).NodeElts[newnode][3] = (*pMesh).EltQty - 1;
  (*pMesh).NodeElts[newnode][4] = (*pMesh).EltQty;

  /*
     if (neut_mesh_test (*pMesh) != 0)
     ut_print_neperbug ();
   */

  ut_free_1d_int (&elts);
  ut_free_1d_int (&newelts);
  ut_free_1d_int (&allelts);
  ut_free_1d_int (&allnodes);
  ut_free_1d_int (&comnodes);

  return;
}

void
neut_mesh_2d_laplaciansmooth (struct NODES *pN, struct MESH M, int *bnodes,
                              int bnodeqty)
{
  int i, iter;
  int *nodes = NULL, NodeQty;
  int **neighnodes = NULL, *neighnodeqty = NULL;
  double **neighbarys = NULL;
  double *nodeshifts = NULL;
  double eps;

  NodeQty = 0;
  for (i = 1; i <= (*pN).NodeQty; i++)
    if (ut_array_1d_int_eltpos (bnodes, bnodeqty, i) == -1)
      ut_array_1d_int_list_addval_nocheck (&nodes, &NodeQty, i);

  if (NodeQty == 0)
    return;

  neut_mesh_area (*pN, M, &eps);
  eps *= 1e-9;

  neighnodes = ut_alloc_1d_pint (NodeQty);
  neighnodeqty = ut_alloc_1d_int (NodeQty);
  neighbarys = ut_alloc_2d (NodeQty, 3);
  nodeshifts = ut_alloc_1d (NodeQty);

  for (i = 0; i < NodeQty; i++)
    neut_mesh_node_neighnodes (M, nodes[i], neighnodes + i, neighnodeqty + i);

  iter = 0;
  do
  {
    for (i = 0; i < NodeQty; i++)
      neut_nodes_bary (*pN, neighnodes[i], neighnodeqty[i], neighbarys[i],
                       NULL);

    for (i = 0; i < NodeQty; i++)
      nodeshifts[i] = ut_space_dist ((*pN).NodeCoo[nodes[i]], neighbarys[i]);

    for (i = 0; i < NodeQty; i++)
      ut_array_1d_memcpy (neighbarys[i], 3, (*pN).NodeCoo[nodes[i]]);
  }
  while (++iter < 1000 && ut_array_1d_mean (nodeshifts, NodeQty) > eps);

  ut_free_1d_int (&nodes);
  ut_free_2d_int (&neighnodes, NodeQty);
  ut_free_1d_int (&neighnodeqty);
  ut_free_2d (&neighbarys, NodeQty);
  ut_free_1d (&nodeshifts);

  return;
}

void
neut_mesh_init_eltori (struct MESH *pMesh)
{
  int i;

  (*pMesh).EltOri = ut_alloc_2d ((*pMesh).EltQty + 1, 4);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    ut_array_1d_memcpy ((*pMesh).ElsetOri[(*pMesh).EltElset[i]], 4, (*pMesh).EltOri[i]);

  ut_string_string ((*pMesh).ElsetOriDes, &(*pMesh).EltOriDes);

  return;
}

void
neut_mesh_init_elsetori (struct NODES Nodes, struct MESH *pMesh)
{
  int i;

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    neut_mesh_elts_orimean (Nodes, (*pMesh),
                            (*pMesh).Elsets[i] + 1, (*pMesh).Elsets[i][0],
                            (*pMesh).ElsetOri[i]);

  return;
}

int
neut_mesh_init_simeltori (struct MESH *pMesh)
{
  int status;
  struct SIMRES SimRes;
  struct SIM *pSim = (*pMesh).pSim;

#ifdef HAVE_OPENMP
  if (omp_get_thread_num() != 0)
  {
    printf ("\nneut_mesh_init_simeltori is not thread-safe!\n");
    ut_print_neperbug ();
  }
#endif

  neut_simres_set_zero (&SimRes);
  neut_sim_simres (*pSim, "elts", "ori", &SimRes);

  if ((*pMesh).SimEltOri)
    ut_free_2d (&((*pMesh).SimEltOri), (*pMesh).EltQty + 1);
  (*pMesh).SimEltOri = ut_alloc_2d ((*pMesh).EltQty + 1, 4);

  if (ut_file_exist (SimRes.file))
  {
    status = 0;
    neut_ori_fnscanf (SimRes.file, (*pSim).OriDes, "ascii",
                      (*pMesh).SimEltOri + 1, NULL,
                      (*pMesh).EltQty, NULL, "R");
  }
  else
    status = -1;

  neut_simres_free (&SimRes);

  return status;
}

void
neut_mesh_free_simeltori (struct MESH *pMesh)
{
  ut_free_2d (&((*pMesh).SimEltOri), (*pMesh).EltQty + 1);

  return;
}

int
neut_mesh_elt_body (struct MESH Mesh2D, struct MESH *pMesh3D, int id)
{
  if (!(*pMesh3D).EltBody)
    neut_mesh_init_eltbody (Mesh2D, pMesh3D);

  return (*pMesh3D).EltBody[id];
}

// works also for 1D/2D.
void
neut_mesh_init_eltbody (struct MESH Mesh2D, struct MESH *pMesh3D)
{
  int i, eltnodeqty = neut_elt_nodeqty ((*pMesh3D).EltType, (*pMesh3D).Dimension, (*pMesh3D).EltOrder);

  (*pMesh3D).EltBody = ut_realloc_1d_int ((*pMesh3D).EltBody, (*pMesh3D).EltQty + 1);

  if (!Mesh2D.NodeElts)
    abort ();

  int **neighelts = ut_alloc_1d_pint ((*pMesh3D).EltQty + 1);
  int *neigheltqty = ut_alloc_1d_int ((*pMesh3D).EltQty + 1);

#pragma omp parallel for private(i)
  for (i = 1; i <= (*pMesh3D).ElsetQty; i++)
  {
    int j, k, eltqty = (*pMesh3D).Elsets[i][0], nodeqty, *nodes = NULL, elt, nelt, node;

    for (j = 1; j <= (*pMesh3D).Elsets[i][0]; j++)
      (*pMesh3D).EltBody[(*pMesh3D).Elsets[i][j]] = 1;

    neut_mesh_elset_nodes ((*pMesh3D), i, &nodes, &nodeqty);

    for (j = 1; j <= (*pMesh3D).Elsets[i][0]; j++)
      for (k = 0; k < eltnodeqty; k++)
      {
        node = (*pMesh3D).EltNodes[(*pMesh3D).Elsets[i][j]][k];
        if (node <= Mesh2D.NodeQty && Mesh2D.NodeElts[node][0] > 0)
        {
          (*pMesh3D).EltBody[(*pMesh3D).Elsets[i][j]] = 0;
          break;
        }
      }

    int bumped = 0;
    int bodymax = 1;
    do
    {
      bumped = 0;

      for (j = 1; j <= eltqty; j++)
      {
        elt = (*pMesh3D).Elsets[i][j];

        if ((*pMesh3D).EltBody[elt] == bodymax)
        {
          if (!neighelts[elt])
            neut_mesh_elt_neighelts_all ((*pMesh3D), elt, neighelts + elt, neigheltqty + elt);

          int bump = 1;
          for (k = 0; k < neigheltqty[elt]; k++)
          {
            nelt = neighelts[elt][k];

            if ((*pMesh3D).EltBody[nelt] < bodymax)
            {
              bump = 0;
              break;
            }
          }

          if (bump)
          {
            (*pMesh3D).EltBody[elt]++;
            bumped++;
          }
        }
      }

      bodymax++;
    }
    while (bumped);

    ut_free_1d_int (&nodes);
  }

  ut_free_2d_int (&neighelts, (*pMesh3D).EltQty + 1);
  ut_free_1d_int (&neigheltqty);

  return;
}
