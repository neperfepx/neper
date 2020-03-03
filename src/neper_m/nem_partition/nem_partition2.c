/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"
#ifdef HAVE_LIBSCOTCH

void
nem_partition_init (struct IN_M In, SCOTCH_Arch * pArch, struct PART *pPart)
{
  char *format = ut_alloc_1d_char (10);
  char **list = NULL;
  FILE *file = NULL;
  int qty = -1;
  char *archfile = NULL;

  ut_string_format (In.partstring, format);
  (*pPart).mode = ut_alloc_1d_char (14);

  // partitioning
  if (format[strlen (format) - 1] == 'd')
  {
    strcpy ((*pPart).mode, "partitioning");
    ut_string_int (In.partstring, &((*pPart).qty));
  }

  // mapping
  else if (format[strlen (format) - 1] == 's')
  {
    strcpy ((*pPart).mode, "mapping");
    (*pPart).mapping = ut_alloc_1d_char (strlen (In.partstring) + 1);
    strcpy ((*pPart).mapping, In.partstring);

    ut_list_break (In.partstring, NEUT_SEP_DEP, &list, &qty);

    // from an architecture file
    if (qty == 1)
    {
      archfile = ut_alloc_1d_char (strlen (In.partstring) + 1);
      strcpy (archfile, In.partstring);
    }

    // from an architecture file
    else if (qty == 2)
    {
      int procqty, proccoreqty;
      ut_string_int (list[0], &procqty);
      ut_string_int (list[1], &proccoreqty);
      archfile = ut_alloc_1d_char (14);
      strcpy (archfile, "neperarch.tmp");

      file = ut_file_open (archfile, "W");
      nem_partition_writearch (file, procqty, proccoreqty, 10, 11, 99);
      ut_file_close (file, archfile, "W");
    }

    else
      ut_print_neperbug ();
  }
  else
    ut_print_neperbug ();

  if (!strcmp ((*pPart).mode, "mapping"))
  {
    file = ut_file_open (archfile, (qty == 1) ? "r" : "R");
    SCOTCH_archLoad (pArch, file);
    ut_file_close (file, archfile, (qty == 1) ? "r" : "R");
    (*pPart).qty = SCOTCH_archSize (pArch);
  }

  if (qty == 2)
    remove ("neperarch.tmp");

  ut_free_1d_char (&archfile);
  ut_free_1d_char (&format);
  ut_free_2d_char (&list, qty);

  return;
}

void
nem_partition_prep (struct NODES *pNodes, struct MESH *Mesh,
                    struct PART *pPart, SCOTCH_Mesh * pSCMesh)
{
  int i, total;
  int dim = neut_mesh_array_dim (Mesh);

  (*pPart).nodeqty = ut_alloc_1d_int ((*pPart).qty);
  (*pPart).eltqty = ut_alloc_1d_int ((*pPart).qty);
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
  neut_mesh_scotchmesh (Mesh[dim], (*pNodes).NodeQty, pSCMesh);

  return;
}

void
nem_partition_nodes (struct IN_M In, SCOTCH_Mesh * pSCMesh,
                     SCOTCH_Arch * pArch, struct NODES *pNodes,
                     struct MESH *Mesh, struct PART *pPart)
{
  SCOTCH_Num *parttab;
  int status;
  int dim = neut_mesh_array_dim (Mesh);
  int eltnodeqty = neut_elt_nodeqty (Mesh[dim].EltType, Mesh[dim].Dimension,
                                     Mesh[dim].EltOrder);
  int i, j, qty;
  SCOTCH_Graph Graph;
  SCOTCH_Strat Strat;
  int *node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
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

  parttab = ut_alloc_1d_int (Mesh[dim].EltQty * eltnodeqty * 2 + 2);
  if (strcmp (In.partmethod, "none") != 0)
  {
    SCOTCH_meshGraph (pSCMesh, &Graph);

    if (!strcmp ((*pPart).mode, "partitioning"))
      SCOTCH_graphPart (&Graph, (*pPart).qty, &Strat, parttab + 1);
    else if (!strcmp ((*pPart).mode, "mapping"))
      SCOTCH_graphMap (&Graph, pArch, &Strat, parttab + 1);
  }
  else if (!strcmp (In.partmethod, "none"))
    for (j = 1; j <= (*pNodes).NodeQty; j++)
      parttab[j] = (*pPart).qty * (j - 1) / (*pNodes).NodeQty;
  else
    ut_print_neperbug ();

  (*pPart).node_parts = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  (*pPart).node_parts[0] = (*pNodes).NodeQty;
  ut_array_1d_int_memcpy (parttab + 1, (*pNodes).NodeQty,
                          (*pPart).node_parts + 1);
  ut_free_1d_int (&parttab);

  /* renumbering nodes and elt nodes */
  qty = 0;
  for (i = 0; i < (*pPart).qty; i++)
    for (j = 1; j <= (*pNodes).NodeQty; j++)
      if ((*pPart).node_parts[j] == i)
        node_nbs[j] = ++qty;
  node_nbs[0] = (*pNodes).NodeQty;
  ut_array_1d_int_sort ((*pPart).node_parts + 1, (*pNodes).NodeQty);

  if (qty != (*pNodes).NodeQty)
    ut_print_neperbug ();

  neut_nodes_renumber_switch (pNodes, node_nbs);
  for (i = 0; i <= dim; i++)
    neut_mesh_nodes_switch (Mesh + i, node_nbs);
  ut_array_1d_int_sort ((*pPart).node_parts + 1, (*pNodes).NodeQty);
  /* end */

  for (i = 0; i < (*pPart).qty; i++)
    (*pPart).nodeqty[i] =
      ut_array_1d_int_valnb ((*pPart).node_parts + 1, (*pNodes).NodeQty, i);

  ut_free_1d_int (&node_nbs);
  ut_free_1d_int (&version);

  return;
}

void
nem_partition_elts (struct IN_M In, struct MESH *Mesh, struct PART *pPart)
{
  int i, j, qty, partqty;
  int **coms = ut_alloc_2d_int ((*pPart).qty, 2);
  int *elt_nbs = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  (*pPart).elt_parts = ut_alloc_1d_int (Mesh[dim].EltQty + 1);
  (*pPart).elt_parts[0] = Mesh[dim].EltQty;
  ut_array_1d_int_set ((*pPart).elt_parts + 1, Mesh[dim].EltQty, -1);

  elt_nbs = ut_alloc_1d_int (Mesh[dim].EltQty + 1);

  ut_array_1d_int_zero ((*pPart).eltqty, (*pPart).qty);

  if (strcmp (In.partmethod, "none") != 0)
  {
    ut_print_message (0, 3, "Minimizing communications...\n");
    nem_partition_elts_match (Mesh, pPart);
    if (In.partbalancing > 0)
    {
      ut_print_message (0, 3, "Balancing partitions... ");
      nem_partition_elts_balancing (Mesh, pPart, In.partbalancing);
    }
  }
  else if (strcmp (In.partmethod, "none") == 0)
  {
    ut_print_message (0, 3, "Direct partitioning by numbers...\n");
    nem_partition_elts_bynumber (Mesh, pPart);
  }

  qty = 0;
  for (i = 0; i < (*pPart).qty; i++)
    for (j = 1; j <= Mesh[dim].EltQty; j++)
      if ((*pPart).elt_parts[j] == i)
        elt_nbs[j] = ++qty;
  elt_nbs[0] = Mesh[dim].EltQty;

  /* renumbering elts */
  neut_mesh_elts_switch (Mesh + dim, elt_nbs);
  ut_array_1d_int_sort ((*pPart).elt_parts + 1, Mesh[dim].EltQty);
  /* end */

  for (i = 1; i <= Mesh[dim].EltQty; i++)
    if (neut_mesh_elt_isbound (Mesh[dim], i, (*pPart).node_parts))
      neut_mesh_elt_coms (Mesh[dim], i, (*pPart).qty, (*pPart).node_parts,
                          (*pPart).elt_parts, coms, &partqty);

  // reinitializing eltelset (brute force method)
  if (Mesh[dim].EltElset != NULL)
    neut_mesh_init_eltelset (Mesh + dim, NULL);

  ut_free_2d_int (&coms, (*pPart).qty);
  ut_free_1d_int (&elt_nbs);

  return;
}

void
nem_partition_stats (int partnbr, struct MESH Mesh, struct PART Part)
{
  int i, cutqty;
  int nmin, nmax, emin, emax;

  emin = ut_array_1d_int_min (Part.eltqty, partnbr);
  emax = ut_array_1d_int_max (Part.eltqty, partnbr);
  nmin = ut_array_1d_int_min (Part.nodeqty, partnbr);
  nmax = ut_array_1d_int_max (Part.nodeqty, partnbr);

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

#endif
