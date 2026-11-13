/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_nset_.h"

void
neut_nset_expand (struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D,
                  char *in, char **pout)
{
  int i, j, k, iscustom;
  int qty = 0, inqty = 0;
  char **list = NULL, **inlist = NULL, *pos = NULL;

  ut_free_1d_char (pout);
  (*pout) = ut_alloc_1d_char (1);
  (*pout)[0] = '\0';

  if (!in || strlen (in) == 0 || !strcmp (in, "none"))
    return;

  ut_list_break (in, NEUT_SEP_NODEP, &inlist, &inqty);

  if (ut_list_testelt (in, NEUT_SEP_NODEP, "all"))
  {
    if (NSet2D.qty > 0)
    {
      qty = 5;
      list = ut_alloc_2d_char (qty, 11);
      list[0] = strcpy (list[0], "faces");
      list[1] = strcpy (list[1], "facebodies");
      list[2] = strcpy (list[2], "edges");
      list[3] = strcpy (list[3], "edgebodies");
      list[4] = strcpy (list[4], "vertices");
    }
    else if (NSet1D.qty > 0)
    {
      qty = 3;
      list = ut_alloc_2d_char (qty, 11);
      list[0] = strcpy (list[0], "edges");
      list[1] = strcpy (list[1], "edgebodies");
      list[2] = strcpy (list[2], "vertices");
    }
    else if (NSet0D.qty > 0)
    {
      qty = 1;
      list = ut_alloc_2d_char (qty, 11);
      list[0] = strcpy (list[0], "vertices");
    }

    // custom nsets (<label>:<expr>)
    for (i = 0; i < inqty; i++)
      if (strchr (inlist[i], ':'))
      {
        list = ut_realloc_2d_char_addline (list, ++qty, strlen (inlist[i]) + 1);
        strcpy (list[qty - 1], inlist[i]);
      }
  }

  else
    ut_list_break (in, NEUT_SEP_NODEP, &list, &qty);

  for (i = 0; i < qty; i++)
  {
    if (!strcmp (list[i], "faces"))
    {
      for (j = 1; j <= NSet2D.qty; j++)
        if (!strstr (NSet2D.names[j], "body"))
        {
          (*pout) =
            ut_realloc_1d_char ((*pout),
                                strlen ((*pout)) + strlen (NSet2D.names[j]) +
                                2);
          if (strlen ((*pout)) > 0)
            (*pout) = strcat ((*pout), ",");
          (*pout) = strcat ((*pout), NSet2D.names[j]);
        }
    }
    else if (!strcmp (list[i], "facebodies"))
    {
      for (j = 1; j <= NSet2D.qty; j++)
        if (strstr (NSet2D.names[j], "body"))
        {
          (*pout) =
            ut_realloc_1d_char ((*pout),
                                strlen ((*pout)) + strlen (NSet2D.names[j]) +
                                2);
          if (strlen ((*pout)) > 0)
            (*pout) = strcat ((*pout), ",");
          (*pout) = strcat ((*pout), NSet2D.names[j]);
        }
    }
    else if (!strcmp (list[i], "edges"))
    {
      for (j = 1; j <= NSet1D.qty; j++)
        if (!strstr (NSet1D.names[j], "body"))
        {
          (*pout) =
            ut_realloc_1d_char ((*pout),
                                strlen ((*pout)) + strlen (NSet1D.names[j]) +
                                2);
          if (strlen ((*pout)) > 0)
            (*pout) = strcat ((*pout), ",");
          (*pout) = strcat ((*pout), NSet1D.names[j]);
        }
    }
    else if (!strcmp (list[i], "edgebodies"))
    {
      for (j = 1; j <= NSet1D.qty; j++)
        if (strstr (NSet1D.names[j], "body"))
        {
          (*pout) =
            ut_realloc_1d_char ((*pout),
                                strlen ((*pout)) + strlen (NSet1D.names[j]) +
                                2);
          if (strlen ((*pout)) > 0)
            (*pout) = strcat ((*pout), ",");
          (*pout) = strcat ((*pout), NSet1D.names[j]);
        }
    }
    else if (!strcmp (list[i], "vertices"))
    {
      for (j = 1; j <= NSet0D.qty; j++)
      {
        // skipping custom nsets, which are also recorded in NSet0D
        iscustom = 0;
        for (k = 0; k < inqty; k++)
          if (!strncmp (inlist[k], NSet0D.names[j], strlen (NSet0D.names[j]))
              && inlist[k][strlen (NSet0D.names[j])] == ':')
          {
            iscustom = 1;
            break;
          }

        if (iscustom)
          continue;

        (*pout) =
          ut_realloc_1d_char ((*pout),
                              strlen ((*pout)) + strlen (NSet0D.names[j]) +
                              2);
        if (strlen ((*pout)) > 0)
          (*pout) = strcat ((*pout), ",");
        (*pout) = strcat ((*pout), NSet0D.names[j]);
      }
    }
    else
    {
      // custom nsets (<label>:<expr>): keeping the label only
      pos = strchr (list[i], ':');
      if (pos)
        pos[0] = '\0';

      (*pout) =
        ut_realloc_1d_char ((*pout), strlen ((*pout)) + strlen (list[i]) + 2);
      if (strlen ((*pout)) > 0)
        (*pout) = strcat ((*pout), ",");
      (*pout) = strcat ((*pout), list[i]);
    }
  }

  ut_free_2d_char (&list, qty);
  ut_free_2d_char (&inlist, inqty);

  return;
}

void
neut_nset_set_zero (struct NSET *pNSet)
{
  (*pNSet).qty = 0;
  (*pNSet).names = NULL;
  (*pNSet).nodes = NULL;
  (*pNSet).NodeQty = NULL;

  return;
}

void
neut_nset_free (struct NSET *pNSet)
{
  ut_free_1d_int (&(*pNSet).NodeQty);
  if ((*pNSet).qty > 0)
  {
    ut_free_2d_char (&(*pNSet).names, (*pNSet).qty + 1);
    ut_free_2d_int (&(*pNSet).nodes, (*pNSet).qty + 1);
  }

  return;
}

void
neut_nsets_inter (struct NSET NSet, int id1, int id2, char **pname,
                  int **pnodes, int *pnodeqty)
{
  if (pname != NULL)
  {
    (*pname) =
      ut_alloc_1d_char (strlen (NSet.names[id1]) + strlen (NSet.names[id2]) +
                        1);
    sprintf ((*pname), "%s%s", NSet.names[id1], NSet.names[id2]);
  }

  (*pnodes) = ut_alloc_1d_int (NSet.NodeQty[id1] + NSet.NodeQty[id2]);

  ut_array_1d_int_inter (NSet.nodes[id1], NSet.NodeQty[id1], NSet.nodes[id2],
                         NSet.NodeQty[id2], (*pnodes), pnodeqty);

  (*pnodes) = ut_realloc_1d_int ((*pnodes), (*pnodeqty));

  return;
}

// Renumbering the nodes of an nset array (NSet[0], NSet[1] and NSet[2]), with
// old_new the new numbers of the nodes (0 for a removed node, which is
// removed from the nsets)
void
neut_nset_renumber (struct NSET *NSet, int *old_new)
{
  int i, j, k, qty;

  for (i = 0; i < 3; i++)
    for (j = 1; j <= NSet[i].qty; j++)
    {
      qty = 0;
      for (k = 0; k < NSet[i].NodeQty[j]; k++)
        if (old_new[NSet[i].nodes[j][k]])
          NSet[i].nodes[j][qty++] = old_new[NSet[i].nodes[j][k]];

      NSet[i].NodeQty[j] = qty;
    }

  return;
}

// Adding to each nset of an nset array (NSet[0], NSet[1] and NSet[2]) the
// nodes of the elts (of dimensions 1 to 3) whose vertex nodes all belong to
// the nset, which are the nodes added by switching the mesh to order 2
void
neut_nset_addeltnodes (struct NSET *NSet, struct NODES Nodes, struct MESH *Mesh)
{
  int i, j, k, l, m, vernodeqty, eltnodeqty, isin;
  int *mask = ut_alloc_1d_int (Nodes.NodeQty + 1);

  for (i = 0; i < 3; i++)
    for (j = 1; j <= NSet[i].qty; j++)
    {
      for (k = 0; k < NSet[i].NodeQty[j]; k++)
        mask[NSet[i].nodes[j][k]] = 1;

      for (l = 1; l <= 3; l++)
      {
        if (Mesh[l].EltQty == 0)
          continue;

        vernodeqty = neut_elt_nodeqty (Mesh[l].EltType, l, 1);
        eltnodeqty = neut_elt_nodeqty (Mesh[l].EltType, l, Mesh[l].EltOrder);

        for (k = 1; k <= Mesh[l].EltQty; k++)
        {
          isin = 1;
          for (m = 0; m < vernodeqty; m++)
            if (!mask[Mesh[l].EltNodes[k][m]])
            {
              isin = 0;
              break;
            }

          if (isin)
            for (m = vernodeqty; m < eltnodeqty; m++)
              if (!mask[Mesh[l].EltNodes[k][m]])
              {
                NSet[i].nodes[j] = ut_realloc_1d_int (NSet[i].nodes[j], NSet[i].NodeQty[j] + 1);
                NSet[i].nodes[j][NSet[i].NodeQty[j]++] = Mesh[l].EltNodes[k][m];
                mask[Mesh[l].EltNodes[k][m]] = 1;
              }
        }
      }

      for (k = 0; k < NSet[i].NodeQty[j]; k++)
        mask[NSet[i].nodes[j][k]] = 0;
    }

  ut_free_1d_int (&mask);

  return;
}

// Removing the empty nsets of an nset array (NSet[0], NSet[1] and NSet[2])
void
neut_nset_rmempty (struct NSET *NSet)
{
  int i, j, qty;

  for (i = 0; i < 3; i++)
  {
    qty = 0;
    for (j = 1; j <= NSet[i].qty; j++)
      if (NSet[i].NodeQty[j] > 0)
      {
        qty++;
        if (qty != j)
        {
          ut_free_1d_char (NSet[i].names + qty);
          ut_free_1d_int (NSet[i].nodes + qty);
          NSet[i].names[qty] = NSet[i].names[j];
          NSet[i].nodes[qty] = NSet[i].nodes[j];
          NSet[i].NodeQty[qty] = NSet[i].NodeQty[j];
          NSet[i].names[j] = NULL;
          NSet[i].nodes[j] = NULL;
        }
      }
      else
      {
        ut_free_1d_char (NSet[i].names + j);
        ut_free_1d_int (NSet[i].nodes + j);
      }

    NSet[i].qty = qty;
  }

  return;
}
