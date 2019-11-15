/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order (struct IN_M In, struct NODES *pNodes, struct MESH *pMesh1D,
	   struct MESH *pMesh2D, struct MESH *pMesh3D)
{
  int i, j, k, pos1, pos2, pos, eltnb, node, qty,
    nodeqty_1d1, nodeqty_1d, nodeqty_2d1, nodeqty_2d, nodeqty_3d1, nodeqty_3d;
  int **fir3 = NULL, **sec3 = NULL, **fir2 = NULL, **sec2 = NULL,
    **fir1 = NULL, **sec1 = NULL;
  double *coo = ut_alloc_1d (3);
  int *eltlist = NULL;
  int *inter = NULL;
  int qty_bak = (*pNodes).NodeQty;
  int node2min, node2max;
  int node1min, node1max;
  int *nodes = ut_alloc_1d_int (2);
  int do_1d, do_2d, do_3d;

  do_1d = pMesh1D && (*pMesh1D).EltQty > 0;
  do_2d = pMesh2D && (*pMesh2D).EltQty > 0;
  do_3d = pMesh3D && (*pMesh3D).EltQty > 0;

  if (do_1d)
  {
    // ut_string_string (In.elttype, &(*pMesh1D).EltType); // needed by quad9
    nem_order_init_1d ((*pMesh1D).EltType, &fir1, &sec1, &nodeqty_1d1, &nodeqty_1d);
  }

  if (do_2d)
  {
    ut_string_string (In.elttype, &(*pMesh2D).EltType); // needed by quad9
    nem_order_init_2d ((*pMesh2D).EltType, &fir2, &sec2, &nodeqty_2d1, &nodeqty_2d);
  }

  if (do_3d)
  {
    // ut_string_string (In.elttype, &(*pMesh3D).EltType); // needed by quad9
    nem_order_init_3d ((*pMesh3D).EltType, &fir3, &sec3, &nodeqty_3d1, &nodeqty_3d);
  }

  /*
   * algorithm:
   * loop over the elements; creating the second order nodes, and, for
   * each, copying it to the other elements that share the element edge.
   */
  /* switching to order 2 and initializing temporary variables */
  if (do_3d)
    (*pMesh3D).EltOrder = 2;

  if (do_2d)
  {
    neut_mesh_nodemin (*pMesh2D, &node2min);
    neut_mesh_nodemax (*pMesh2D, &node2max);
    (*pMesh2D).EltOrder = 2;
  }

  if (do_1d)
  {
    neut_mesh_nodemin (*pMesh1D, &node1min);
    neut_mesh_nodemax (*pMesh1D, &node1max);
    (*pMesh1D).EltOrder = 2;
  }

  /* (re)allocating EltNodes  */
  if (do_3d)
    ut_realloc_2d_int_size2 ((*pMesh3D).EltNodes + 1, (*pMesh3D).EltQty,
			     nodeqty_3d1, nodeqty_3d);

  if (do_2d)
    ut_realloc_2d_int_size2 ((*pMesh2D).EltNodes + 1, (*pMesh2D).EltQty,
			     nodeqty_2d1, nodeqty_2d);

  if (do_1d)
    ut_realloc_2d_int_size2 ((*pMesh1D).EltNodes + 1, (*pMesh1D).EltQty,
			     nodeqty_1d1, nodeqty_1d);

  /* for every element */
  if (do_3d)
    for (i = 1; i <= (*pMesh3D).EltQty; i++)
    {
      /* for every second order node */
      for (j = nodeqty_3d1; j < nodeqty_3d; j++)
	/* if not recorded yet */
	if ((*pMesh3D).EltNodes[i][j] == 0)
	{
	  /* recording corresponding first order nodes */
	  nodes[0] = (*pMesh3D).EltNodes[i][fir3[j][0]];
	  nodes[1] = (*pMesh3D).EltNodes[i][fir3[j][1]];

	  /* searching com elements between the two */
	  neut_mesh_nodes_comelts (*pMesh3D, nodes, 2, &eltlist, &qty);

	  /* if the first element is >= i, that means that the node at
	   * this position has not been recorded through another elements
	   * before; recording it in 3D mesh, so as lower dimension meshes */
	  if (eltlist[0] < i)
	    ut_error_reportbug ();

	  for (k = 0; k < 3; k++)
	    coo[k] = .5 * ((*pNodes).NodeCoo[nodes[0]][k]
			   + (*pNodes).NodeCoo[nodes[1]][k]);

	  neut_nodes_addnode (pNodes, coo, -1);

	  node = (*pNodes).NodeQty;

	  /* adding node to 3D mesh */
	  (*pMesh3D).EltNodes[i][j] = node;

	  for (k = 1; k < qty; k++)
	  {
	    eltnb = eltlist[k];
	    pos1 =
	      ut_array_1d_int_eltpos ((*pMesh3D).EltNodes[eltnb], nodeqty_3d1,
				      nodes[0]);
	    pos2 =
	      ut_array_1d_int_eltpos ((*pMesh3D).EltNodes[eltnb], nodeqty_3d1,
				      nodes[1]);
	    pos = sec3[pos1][pos2];
	    (*pMesh3D).EltNodes[eltnb][pos] = node;
	  }

	  /* adding node to 2D mesh if needed */
	  if (do_2d)
	    if (nodes[0] >= node2min && nodes[0] <= node2max &&
		nodes[1] >= node2min && nodes[1] <= node2max)
	    {
	      /* searching com elts between the two */
	      neut_mesh_nodes_comelts (*pMesh2D, nodes, 2, &eltlist, &qty);

	      /* note: qty may be zero */
	      for (k = 0; k < qty; k++)
	      {
		eltnb = eltlist[k];
		if (ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb],
					    nodeqty_2d, node) == -1)
		{
		  pos1 =
		    ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb],
					    nodeqty_2d1, nodes[0]);
		  pos2 =
		    ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb],
					    nodeqty_2d1, nodes[1]);
		  pos = sec2[pos1][pos2];
		  (*pMesh2D).EltNodes[eltnb][pos] = node;
		}
	      }
	    }

	  /* adding node to 1D mesh if needed */
	  if (do_1d)
	    if (nodes[0] >= node1min && nodes[0] <= node1max &&
		nodes[1] >= node1min && nodes[1] <= node1max)
	    {
	      /* searching com elts between the two */
	      neut_mesh_nodes_comelts (*pMesh1D, nodes, 2, &eltlist, &qty);

	      for (k = 0; k < qty; k++)
	      {
		eltnb = eltlist[k];
		if (ut_array_1d_int_eltpos ((*pMesh1D).EltNodes[eltnb],
					    nodeqty_1d, node) == -1)
		  (*pMesh1D).EltNodes[eltnb][2] = node;
	      }
	    }
	}
    }

  /* for every element */
  if (do_2d)
    for (i = 1; i <= (*pMesh2D).EltQty; i++)
    {
      /* for every second order node */
      for (j = nodeqty_2d1; j < nodeqty_2d; j++)
	/* if not recorded yet */
	if ((*pMesh2D).EltNodes[i][j] == 0)
	{
          // if an edge 2nd order node
          if (ut_array_1d_int_min (fir2[j], 2) != -1)
          {
            /* recording corresponding first order nodes */
            nodes[0] = (*pMesh2D).EltNodes[i][fir2[j][0]];
            nodes[1] = (*pMesh2D).EltNodes[i][fir2[j][1]];

            for (k = 0; k < 3; k++)
              coo[k] = .5 * ((*pNodes).NodeCoo[nodes[0]][k]
                             + (*pNodes).NodeCoo[nodes[1]][k]);

            /* searching com elements between the two order-1 nodes */
            neut_mesh_nodes_comelts (*pMesh2D, nodes, 2, &eltlist, &qty);
          }

          // if a central 2nd order node
          else
          {
            ut_array_1d_int_set (nodes, 2, -1);

            neut_mesh_elt_centre (*pNodes, *pMesh2D, i, coo);

            qty = 0;
            ut_array_1d_int_list_addelt (&eltlist, &qty, i);
          }

	  neut_nodes_addnode (pNodes, coo, -1);

	  node = (*pNodes).NodeQty;

	  /* adding node to 2D mesh */
	  (*pMesh2D).EltNodes[i][j] = node;

          /* if the first element is >= i, that means that the node at
           * this position has not been recorded through another elements
           * before; recording it in 2D mesh, so as lower dimension meshes */
          if (eltlist[0] < i)
            ut_error_reportbug ();

	  for (k = 1; k < qty; k++)
	  {
	    eltnb = eltlist[k];

            if (ut_array_1d_int_min (nodes, 2) != -1)
            {
              pos1 =
                ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb],
                                        nodeqty_2d1, nodes[0]);
              pos2 =
                ut_array_1d_int_eltpos ((*pMesh2D).EltNodes[eltnb],
                                        nodeqty_2d1, nodes[1]);
              pos = sec2[pos1][pos2];
              (*pMesh2D).EltNodes[eltnb][pos] = node;
            }

            else // last, central nodes as in quad9
              (*pMesh2D).EltNodes[i][nodeqty_2d - 1] = node;
          }

	  /* adding node to 1D mesh if needed */
	  if (do_1d)
	    if (ut_array_1d_int_min (nodes, 2) >= node1min
	     && ut_array_1d_int_max (nodes, 2) <= node1max)
	    {
	      /* searching com elts between the two */
	      neut_mesh_nodes_comelts (*pMesh1D, nodes, 2, &eltlist, &qty);

	      for (k = 0; k < qty; k++)
	      {
		eltnb = eltlist[k];
		if (ut_array_1d_int_eltpos ((*pMesh1D).EltNodes[eltnb],
					    nodeqty_1d, node) == -1)
		  (*pMesh1D).EltNodes[eltnb][2] = node;
	      }
	    }
	}
    }

  /* for every element */
  if (do_1d)
    for (i = 1; i <= (*pMesh1D).EltQty; i++)
    {
      /* for every second order node */
      for (j = nodeqty_1d1; j < nodeqty_1d; j++)
	/* if not recorded yet */
	if ((*pMesh1D).EltNodes[i][j] == 0)
	{
	  /* recording corresponding first order nodes */
	  nodes[0] = (*pMesh1D).EltNodes[i][fir1[j][0]];
	  nodes[1] = (*pMesh1D).EltNodes[i][fir1[j][1]];

	  /* searching com elements between the two */
	  neut_mesh_nodes_comelts (*pMesh1D, nodes, 2, &eltlist, &qty);

	  /* if the first element is >= i, that means that the node at
	   * this position has not been recorded through another elements
	   * before; recording it in 1D mesh, so as lower dimension meshes */
	  if (eltlist[0] < i)
	    ut_error_reportbug ();

	  for (k = 0; k < 3; k++)
	    coo[k] = .5 * ((*pNodes).NodeCoo[nodes[0]][k]
			   + (*pNodes).NodeCoo[nodes[1]][k]);

	  neut_nodes_addnode (pNodes, coo, -1);

	  node = (*pNodes).NodeQty;

	  /* adding node to 1D mesh */
	  (*pMesh1D).EltNodes[i][j] = node;

	  for (k = 1; k < qty; k++)
	  {
	    eltnb = eltlist[k];
	    pos1 =
	      ut_array_1d_int_eltpos ((*pMesh1D).EltNodes[eltnb], nodeqty_1d1,
				      nodes[0]);
	    pos2 =
	      ut_array_1d_int_eltpos ((*pMesh1D).EltNodes[eltnb], nodeqty_1d1,
				      nodes[1]);
	    pos = sec1[pos1][pos2];
	    (*pMesh1D).EltNodes[eltnb][pos] = node;
	  }

	  /* adding node to 1D mesh if needed */
	  if (pMesh1D)
	    if (nodes[0] >= node1min && nodes[0] <= node1max &&
		nodes[1] >= node1min && nodes[1] <= node1max)
	    {
	      /* searching com elts between the two */
	      neut_mesh_nodes_comelts (*pMesh1D, nodes, 2, &eltlist, &qty);

	      for (k = 0; k < qty; k++)
	      {
		eltnb = eltlist[k];
		if (ut_array_1d_int_eltpos ((*pMesh1D).EltNodes[eltnb],
					    nodeqty_1d, node) == -1)
		  (*pMesh1D).EltNodes[eltnb][2] = node;
	      }
	    }
	}
    }

  if (do_1d)
  {
    if (ut_array_2d_int_eltpos ((*pMesh1D).EltNodes + 1,
			       (*pMesh1D).EltQty, nodeqty_1d, 0, NULL,
			       NULL) != -1)
      ut_error_reportbug ();

    if ((*pMesh1D).NodeElts)
      ut_free_2d_int ((*pMesh1D).NodeElts, node1max + 1);
    neut_mesh_init_nodeelts (pMesh1D, (*pNodes).NodeQty);
  }

  if (do_2d)
  {
    if (ut_array_2d_int_eltpos ((*pMesh2D).EltNodes + 1,
				(*pMesh2D).EltQty, nodeqty_2d, 0, NULL,
				NULL) != -1)
      ut_error_reportbug ();

    if ((*pMesh2D).NodeElts)
      ut_free_2d_int ((*pMesh2D).NodeElts, node2max + 1);
    neut_mesh_init_nodeelts (pMesh2D, (*pNodes).NodeQty);
  }

  if (do_3d)
  {
    if(ut_array_2d_int_eltpos ((*pMesh3D).EltNodes + 1,
			       (*pMesh3D).EltQty, nodeqty_3d, 0, NULL,
			       NULL) != -1)
      ut_error_reportbug ();

    if ((*pMesh3D).NodeElts)
      ut_free_2d_int ((*pMesh3D).NodeElts, qty_bak + 1);
    neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);
  }

  ut_free_2d_int (fir3, nodeqty_3d);
  ut_free_2d_int (sec3, nodeqty_3d1);
  ut_free_2d_int (fir2, nodeqty_2d);
  ut_free_2d_int (sec2, nodeqty_2d1);
  ut_free_2d_int (fir1, nodeqty_1d);
  ut_free_2d_int (sec1, nodeqty_1d1);

  ut_free_1d (coo);
  ut_free_1d_int (inter);
  ut_free_1d_int (eltlist);
  ut_free_1d_int (nodes);

  return;
}
