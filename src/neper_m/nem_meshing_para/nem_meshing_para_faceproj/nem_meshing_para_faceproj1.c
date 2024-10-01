/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_faceproj_.h"

void
nem_meshing_para_faceproj (struct TESS Tess, struct NODES RNodes,
                           struct MESH *RMesh, struct MESHPARA *pMeshPara)
{
  int i, j, status, facecpy_qty;
  char *message = ut_alloc_1d_char (8);
  int *failed = ut_alloc_1d_int (Tess.FaceQty + 1);
  int failed_sum;
  FILE *file = NULL;

  (*pMeshPara).face_eq = ut_alloc_2d (Tess.FaceQty + 1, 4);

  ut_print_progress (stdout, 0, Tess.FaceQty, "%3.0f%%", message);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    if (RMesh[2].EltQty > 0)
      ut_string_string ("meshproj", (*pMeshPara).face_op + i);
    else if (neut_tess_face_iscurved (Tess, i)
             && !strcmp (Tess.DomFaceType[Tess.FaceDom[i][1]], "cylinder"))
      ut_string_string ("cylinderproj", (*pMeshPara).face_op + i);
    else if (neut_tess_face_iscurved (Tess, i)
             && !strcmp (Tess.DomFaceType[Tess.FaceDom[i][1]], "sphere"))
      ut_string_string ("sphereproj", (*pMeshPara).face_op + i);
    else
      ut_string_string ("planeproj", (*pMeshPara).face_op + i);

    status =
      nem_meshing_para_faceproj_eq (Tess, RNodes, RMesh, i,
                                    (*pMeshPara).face_eq[i]);

    if (status != 0)
    {
      if (!strcmp ((*pMeshPara).face_op[i], "meshproj"))
        ut_string_string ("copy", (*pMeshPara).face_op + i);
      else
        failed[i] = 1;
    }

    ut_print_progress (stdout, i, Tess.FaceQty, "%3.0f%%", message);
    fflush (stdout);
  }

  failed_sum = ut_array_1d_int_sum (failed + 1, Tess.FaceQty);
  if (failed_sum)
  {
    ut_print_lineheader (2);
    ut_print_level (2);
    printf ("Failed to project %d face%s:", failed_sum, (failed_sum == 1) ? "" : "s");
    for (i = 1; i <= Tess.FaceQty; i++)
      if (failed[i])
        printf (" %d", i);
    printf (".\n");

    file = ut_file_open ("failed_faces", "w");
    for (i = 1; i <= Tess.FaceQty; i++)
      if (failed[i])
        fprintf (file, "%d\n", i);
    ut_file_close (file, "failed_faces", "w");

    file = ut_file_open ("failed_polys", "w");
    for (i = 1; i <= Tess.FaceQty; i++)
      if (failed[i])
        fprintf (file, "%d %d\n", Tess.FacePoly[i][0], Tess.FacePoly[i][1]);
    ut_file_close (file, "failed_polys", "w");

    abort ();
  }

  facecpy_qty = 0;
  for (i = 1; i <= Tess.FaceQty; i++)
    if (!strcmp ((*pMeshPara).face_op[i], "copy"))
    {
      facecpy_qty++;
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
        ut_string_string ("copy",
                          (*pMeshPara).edge_op + Tess.FaceEdgeNb[i][j]);
    }

  if (facecpy_qty != 0)
    ut_print_message (0, 3, "%d face%s cannot be meshed (will be copied).\n",
                      facecpy_qty, (facecpy_qty > 1) ? "s" : "");

  ut_free_1d_char (&message);
  ut_free_1d_int (&failed);

  return;
}
