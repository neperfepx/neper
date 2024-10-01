/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_3d_.h"

void
nem_meshing_3d (struct IN_M In, struct MESHPARA MeshPara, struct TESS *pTess,
                struct NODES *pNodes, struct MESH *Mesh)
{
  int i, id, polyqty, *poly = NULL, qty;
  double allowed_t, max_elapsed_t = 0;
  char *message = NULL;
  struct MULTIM Multim;
  struct timeval ctrlc_t = { 0, 0 };
  struct NODES *N =
    (struct NODES *) calloc ((*pTess).PolyQty + 1, sizeof (struct NODES));
  struct MESH *M =
    (struct MESH *) calloc ((*pTess).PolyQty + 1, sizeof (struct MESH));

  for (i = 1; i <= (*pTess).PolyQty; i++)
    neut_nodes_set_zero (N + i);
  for (i = 1; i <= (*pTess).PolyQty; i++)
    neut_mesh_set_zero (M + i);

  ut_print_message (0, 2, "3D meshing... ");

  // this is the default case, input is 3d
  if ((*pTess).Dim == 3)
  {
    message = ut_alloc_1d_char (128);

    neut_multim_set_zero (&Multim);
    neut_multim_init (3, In.mesh3dalgo, (*pTess).PolyQty, &Multim);

    Mesh[3] = neut_mesh_alloc (3, "tri", 1, 0, 0);

    // polys to mesh; default is 'all'
    neut_tess_expr_polys (pTess, In.meshpoly, &poly, &polyqty);

    allowed_t = In.mesh3dmaxtime;
    qty = 0;
#pragma omp parallel for schedule(dynamic) private(i,id)
    for (i = 0; i < polyqty; i++)
      if ((*pTess).PolyFaceQty[poly[i]] > 0)
      {
        id = poly[i];

        nem_meshing_3d_poly (In, MeshPara.poly_cl[id],
                             MeshPara.mesh3dclreps[id], &Multim, &ctrlc_t,
                             &allowed_t, &max_elapsed_t, (*pTess), pNodes, Mesh,
                             N + id, M + id, id);

#pragma omp critical
        nem_meshing_3d_progress (Multim, ++qty, polyqty, message);

        if (In.mesh3dreport)
          nem_meshing_3d_report_poly (Multim, id);
      }

    // Recording poly mesh in global mesh
    for (i = 0; i < polyqty; i++)
    {
      id = poly[i];

      while (Mesh[3].ElsetQty != id - 1)
        neut_mesh_addelset (Mesh + 3, NULL, 0);

      if ((*pTess).PolyFaceQty[id] > 0)
        nem_meshing_3d_poly_record ((*pTess), id, N[id], M[id], pNodes, Mesh);
      else
        neut_mesh_addelset (Mesh + 3, NULL, 0);
    }

    neut_mesh_init_nodeelts (Mesh + 3, (*pNodes).NodeQty);
    neut_mesh_init_eltelset (Mesh + 3, NULL);

    neut_multim_free (&Multim, (*pTess).PolyQty);

#ifdef DEVEL_DEBUGGING_TEST
    sprintf (message, "tmp%d.geo", getpid ());
    remove (message);
    sprintf (message, "tmp%d-surf.msh", getpid ());
    remove (message);
    sprintf (message, "tmp%d.msh", getpid ());
    remove (message);
#endif

    ut_free_1d_char (&message);
    ut_free_1d_int (&poly);
  }

  // if input is 2D, extruding mesh
  else if ((*pTess).Dim == 2)
  {
    neut_mesh_tri_tet (pNodes, Mesh[2], Mesh + 3, MeshPara.ver_cl[1]);
    printf ("100%%\n");
    return;
  }

  for (i = 1; i <= (*pTess).PolyQty; i++)
    neut_nodes_free (N + i);
  free (N);
  for (i = 1; i <= (*pTess).PolyQty; i++)
    neut_mesh_free (M + i);
  free (M);

  return;
}
