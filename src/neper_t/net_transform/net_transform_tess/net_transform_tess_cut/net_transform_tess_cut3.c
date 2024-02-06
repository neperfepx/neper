
/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
                                   struct TESS *pTess)
{
  int i, status;
  int ver, verqty, *vers = NULL;
  int *domfaces = NULL, domfaceqty;

  neut_tess_domfaces_vers (*pTess, newdomfaces, newdomfaceqty, &vers,
                           &verqty);

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

    neut_tess_face_fixorifromedges (pTess, face);
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
