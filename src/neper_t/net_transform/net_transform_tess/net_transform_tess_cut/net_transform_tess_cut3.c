
/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_transform_tess_cut_.h"

void
net_transform_tess_cut_pre_prim (char *expr, struct PRIM **pPrim, int *pPrimQty, int verbosity)
{
  int i, j, status, qty, qty2;
  char *fct = NULL, *fct2 = NULL;
  char **tmp = NULL, **tmp2 = NULL;
  char **prim = NULL;

  ut_string_function (expr, &fct, NULL, &tmp, &qty);

  if (strcmp (fct, "cut"))
    abort ();

  (*pPrimQty) = 0;
  for (i = 0; i < qty; i++)
  {
    ut_string_function (tmp[i], &fct2, NULL, &tmp2, &qty2);

    if (!strncmp (tmp[i], "cubei", 5) && qty2 == 6)
    {
      char **tmp0 = ut_alloc_2d_char (6, 1000);
      ut_string_function (tmp[i], &fct2, NULL, &tmp2, &qty2);
      if (qty2 != 6)
        ut_print_exprbug (tmp[i]);

      sprintf (tmp0[0], "hspace(%g,-1,0,0)", -atof (tmp2[0]));
      sprintf (tmp0[1], "hspace(%g,1,0,0)", atof (tmp2[1]));
      sprintf (tmp0[2], "hspace(%g,0,-1,0)", -atof (tmp2[2]));
      sprintf (tmp0[3], "hspace(%g,0,1,0)", atof (tmp2[3]));
      sprintf (tmp0[4], "hspace(%g,0,0,-1)", -atof (tmp2[4]));
      sprintf (tmp0[5], "hspace(%g,0,0,1)", atof (tmp2[5]));

      for (j = 0; j < 6; j++)
      {
        prim = ut_realloc_1d_pchar_null (prim, ++(*pPrimQty), 1);
        ut_string_string (tmp0[j], prim + (*pPrimQty) - 1);
      }
      ut_free_2d_char (&tmp0, 6);
    }

    else
    {
      prim = ut_realloc_1d_pchar_null (prim, ++(*pPrimQty), 1);
      ut_string_string (tmp[i], prim + (*pPrimQty) - 1);
    }
  }

  (*pPrim) = calloc (*pPrimQty, sizeof (struct PRIM));

  for (i = 0; i < *pPrimQty; i++)
  {
    neut_prim_set_zero (*pPrim + i);
    status = neut_prim_sscanf (prim[i], *pPrim + i);
    if (status == 0)
    {
      if (verbosity > 0)
        ut_print_message (0, 3, "%s...\n", prim[i]);
    }
    else
      ut_print_exprbug (prim[i]);
  }

  ut_free_1d_char (&fct);
  ut_free_1d_char (&fct2);
  ut_free_2d_char (&tmp, qty);
  ut_free_2d_char (&tmp2, qty2);
  ut_free_2d_char (&prim, *pPrimQty);

  return;
}

// merge mirror cells and remove the resulting cell
void
net_transform_tess_cut_clean_remove (struct PRIM Prim, int *seeds,
                                     int seedqty, int *cutcells,
                                     int cutcellqty, int *pnewdomface,
                                     struct TESS *pTess)
{
  int i, mercell, *poly_new_old = NULL;

  mercell = neut_tess_cells_merge_nocompress (pTess, seeds, seedqty, 1);

  neut_tess_polys_remove_nocompress (pTess, &mercell, 1);

  ut_string_string ("cut", &(*pTess).DomType);
  *pnewdomface = (*pTess).DomFaceQty;

  if (strcmp (Prim.Type, "hspace"))
    ut_string_string (Prim.Type, (*pTess).DomFaceType + *pnewdomface);
  else
  {
    ut_string_string ("plane", (*pTess).DomFaceType + *pnewdomface);
    ut_array_1d_memcpy (Prim.Eq, 4, (*pTess).DomFaceEq[*pnewdomface]);
    ut_array_1d_scale ((*pTess).DomFaceEq[*pnewdomface], 4, -1);
  }

  (*pTess).DomFaceParmQty[*pnewdomface] = Prim.ParmQty;
  (*pTess).DomFaceParms[*pnewdomface] =
    ut_alloc_1d ((*pTess).DomFaceParmQty[*pnewdomface]);
  ut_array_1d_memcpy (Prim.Parms, (*pTess).DomFaceParmQty[*pnewdomface],
                      (*pTess).DomFaceParms[*pnewdomface]);

  // This merges faces and edges for each cut cell, when possible.
  // This is needed to remove the small edges and faces created by mirroring.
  for (i = 0; i < cutcellqty; i++)
    neut_tess_cells_merge_nocompress (pTess, cutcells + i, 1, -1);

  ut_free_1d_int (&poly_new_old);

  return;
}

// project all vertices of the newdomain face onto it,
// taking domain faces into account
void
net_transform_tess_cut_clean_proj (int *newdomfaces, int newdomfaceqty,
                                   int *DomFaceState, struct TESS *pTess)
{
  int i, j, status;
  int ver, verqty, *vers = NULL;
  int verqty2, *vers2 = NULL;
  int *domfaces = NULL, domfaceqty;
  int domface = -1, newdomface = -1;

  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceDom[i][0] == 2)
    {
      domface = (*pTess).FaceDom[i][1];
      if (DomFaceState[domface] != 0)
      {
        newdomface = newdomfaces[- DomFaceState[domface] - 1];
        (*pTess).FaceDom[i][1] = newdomface;
      }
    }

  if (domface == -1 || newdomface == -1)
    abort ();

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if (DomFaceState[i] != 0)
    {
      newdomface = newdomfaces[- DomFaceState[domface] - 1];
      ut_string_string ((*pTess).DomFaceType[newdomface], (*pTess).DomFaceType + i);
      ut_array_1d_memcpy ((*pTess).DomFaceEq[newdomface], 4, (*pTess).DomFaceEq[i]);
      (*pTess).DomFaceParmQty[i] = (*pTess).DomFaceParmQty[newdomface];
      (*pTess).DomFaceParms[i] = ut_realloc_1d ((*pTess).DomFaceParms[i], (*pTess).DomFaceParmQty[newdomface]);
      ut_array_1d_memcpy ((*pTess).DomFaceParms[newdomface], (*pTess).DomFaceParmQty[newdomface], (*pTess).DomFaceParms[i]);
    }

  neut_tess_domfaces_vers (*pTess, newdomfaces, newdomfaceqty, &vers, &verqty);

  // Adding vertices of the domfaces with status != 0, to be projected
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if (DomFaceState[i] != 0)
    {
      neut_tess_domface_vers (*pTess, i, &vers2, &verqty2);
      for (j = 0; j < verqty2; j++)
      {
        ver = vers2[j];
        (*pTess).VerDom[ver][1] = newdomfaces[-DomFaceState[i] - 1];
      }
      ut_array_1d_int_listpair_merge (vers, verqty, vers2, verqty2, &vers, &verqty);
    }

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    neut_tess_ver_domfaces (*pTess, ver, &domfaces, &domfaceqty);

    status =
      neut_tess_domfaces_point_proj (*pTess, domfaces, domfaceqty,
                                     (*pTess).VerCoo[ver],
                                     (*pTess).VerCoo[ver]);

    if (status)
      ut_print_message (2, 3,
                        "Projection failed.  Increase -n or try other -ids.\n");
  }

  ut_free_1d_int (&vers);
  ut_free_1d_int (&vers2);
  ut_free_1d_int (&domfaces);

  return;
}

void
net_transform_tess_cut_clean_faceequpdate (int *newdomfaces,
                                           int newdomfaceqty,
                                           struct TESS *pTess)
{
  int i, domface;
  int verqty, *vers = NULL;
  int face, faceqty, *faces = NULL;

  neut_tess_domfaces_vers (*pTess, newdomfaces, newdomfaceqty, &vers,
                           &verqty);

  // updating equations of the faces close to the new domain face,
  // from the coordinates of their vertices
  neut_tess_vers_allfaces (*pTess, vers, verqty, &faces, &faceqty);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];
    domface = (*pTess).FaceDom[face][1];

    // setting FacePtCoo to barycentre projected on the domain face
    neut_tess_face_centre (*pTess, face, (*pTess).FacePtCoo[face]);

    if (domface > 0)
      neut_tess_faceinter_fromdomface (pTess, face);

    neut_tess_face_fixorifromedges (*pTess, face, (*pTess).FaceEq[face]);
  }

  ut_free_1d_int (&vers);
  ut_free_1d_int (&faces);

  return;
}

void
net_transform_tess_cut_post_update_domain (struct TESS *pTess)
{
  int i, j, k, newdomface;
  char *body = NULL;

  for (i = 1; i <= (*pTess).DomFaceQty; i++)
  {
    int face, **faces = NULL, *faceqty = NULL, qty;

    neut_tess_faces_contiguousfaces (*pTess, -1,
                                     (*pTess).DomTessFaceNb[i] + 1,
                                     (*pTess).DomTessFaceQty[i], &qty, &faces,
                                     &faceqty);

    if (qty > 1)
    {
      ut_string_string ((*pTess).DomFaceLabel[i], &body);

      (*pTess).DomFaceLabel[i] =
        ut_realloc_1d_char ((*pTess).DomFaceLabel[i], strlen (body) + 10);
      sprintf ((*pTess).DomFaceLabel[i], "%s-1", body);

      for (j = 1; j < qty; j++)
      {
        newdomface = neut_tess_adddomface_alloc (pTess);
        (*pTess).DomFaceLabel[newdomface] =
          ut_realloc_1d_char ((*pTess).DomFaceLabel[newdomface],
                              strlen (body) + 10);
        sprintf ((*pTess).DomFaceLabel[newdomface], "%s-%d", body, j + 1);
        ut_string_string ((*pTess).DomFaceType[i],
                          &(*pTess).DomFaceType[newdomface]);
        ut_array_1d_memcpy ((*pTess).DomFaceEq[i], 4,
                            (*pTess).DomFaceEq[newdomface]);
        (*pTess).DomFaceParmQty[newdomface] = (*pTess).DomFaceParmQty[i];
        (*pTess).DomFaceParms[newdomface] =
          ut_alloc_1d ((*pTess).DomFaceParmQty[newdomface]);
        ut_array_1d_memcpy ((*pTess).DomFaceParms[i],
                            (*pTess).DomFaceParmQty[newdomface],
                            (*pTess).DomFaceParms[newdomface]);

        for (k = 0; k < faceqty[j]; k++)
        {
          face = faces[j][k];

          neut_tess_domface_rmface (pTess, i, face);
          (*pTess).FacePoly[face][1] = -newdomface;
          (*pTess).FaceDom[face][1] = newdomface;
          neut_tess_domface_addface (pTess, newdomface, face);
        }
      }
    }

    ut_free_2d_int (&faces, qty);
    ut_free_1d_int (&faceqty);
  }

  neut_tess_init_domain_3d_fromdomfaces (pTess);

  ut_free_1d_char (&body);

  return;
}

// net_transform_tess_cut_direct_proj projects the vertices of the domain
// edges and domain vertices that belong to a curved domain face onto all
// their domain faces.  This is needed for a vertex of a domain edge between
// two curved domain faces (e.g., a sphere cut by a cylinder), which is
// computed as the intersection between a (flat) face and a primitive, and
// so does not lie on the other primitive.  The equations of the faces of
// the moved vertices and of the faces on curved domain faces are then
// updated (see neut_tess_face_eq).
void
net_transform_tess_cut_direct_proj (struct TESS *pTess)
{
  int i, j, ver, curved, *domfaces = NULL, domfaceqty;
  int *vers = NULL, verqty = 0, *faces = NULL, faceqty;
  double *proj = ut_alloc_1d (3);

  for (ver = 1; ver <= (*pTess).VerQty; ver++)
  {
    if ((*pTess).VerState[ver] == -1 || (*pTess).VerDom[ver][0] > 1)
      continue;

    neut_tess_ver_domfaces (*pTess, ver, &domfaces, &domfaceqty);

    curved = 0;
    for (j = 0; j < domfaceqty; j++)
      if (strcmp ((*pTess).DomFaceType[domfaces[j]], "plane"))
        curved = 1;

    if (!curved || domfaceqty < 2)
      continue;

    if (neut_tess_domfaces_point_proj (*pTess, domfaces, domfaceqty,
                                       (*pTess).VerCoo[ver], proj))
      ut_print_message (1, 3, "Failed to project vertex %d onto the domain.\n",
                        ver);

    // the vertices already on their domain faces are left untouched
    if (ut_space_dist (proj, (*pTess).VerCoo[ver]) > 1e-12)
    {
      ut_array_1d_memcpy (proj, 3, (*pTess).VerCoo[ver]);
      ut_array_1d_int_list_addval (&vers, &verqty, ver);
    }
  }

  neut_tess_vers_allfaces (*pTess, vers, verqty, &faces, &faceqty);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceState[i] != -1 && (*pTess).FaceDom[i][0] == 2
        && strncmp ((*pTess).DomFaceType[(*pTess).FaceDom[i][1]], "plane", 5))
      ut_array_1d_int_list_addval (&faces, &faceqty, i);

  for (i = 0; i < faceqty; i++)
    neut_tess_face_eq (*pTess, faces[i], (*pTess).FaceEq[faces[i]]);

  ut_free_1d (&proj);
  ut_free_1d_int (&domfaces);
  ut_free_1d_int (&vers);
  ut_free_1d_int (&faces);

  return;
}

void
net_transform_tess_cut_direct_poly (struct PRIM Prim, int index, struct POLY *pPoly)
{
  int i, min, max;
  int *sides = ut_alloc_1d_int ((*pPoly).VerQty + 1);

  // this may be overkill
  min = INT_MAX;
  max = -INT_MAX;
  for (i = 1; i <= (*pPoly).VerQty; i++)
  {
    // -1 means inside / cut
    neut_prim_point_side (Prim, (*pPoly).VerCoo[i], sides + i);
    min = ut_num_min (min, sides[i]);
    max = ut_num_max (max, sides[i]);
  }

  if (min == -1 && max == -1)
    neut_poly_set_zero (pPoly);
  if (min == -1 && max == 1)
    net_poly_clip_prim (pPoly, Prim, index);
  else
  {
  }

  ut_free_1d_int (&sides);

  return;
}
