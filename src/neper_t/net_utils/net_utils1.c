/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_utils_.h"

void
net_in_set_zero (struct IN_T *pIn)
{
  (*pIn).mode = NULL;
  (*pIn).nstring = NULL;
  (*pIn).n = NULL;
  (*pIn).idstring = NULL;
  (*pIn).id = NULL;

  (*pIn).morphostring = NULL;
  (*pIn).morpho = NULL;
  (*pIn).morphooptiinistring = NULL;
  (*pIn).morphooptiini = NULL;
  (*pIn).morphooptialgostring = NULL;
  (*pIn).morphooptialgo = NULL;
  (*pIn).morphooptialgoneighstring = NULL;
  (*pIn).morphooptialgoneigh = NULL;
  (*pIn).morphooptialgomaxiterstring = NULL;
  (*pIn).morphooptialgomaxiter = NULL;
  (*pIn).morphooptiobjectivestring = NULL;
  (*pIn).morphooptiobjective = NULL;
  (*pIn).morphooptigridstring = NULL;
  (*pIn).morphooptigrid = NULL;
  (*pIn).morphooptismoothstring = NULL;
  (*pIn).morphooptismooth = NULL;
  (*pIn).morphooptistopstring = NULL;
  (*pIn).morphooptistop = NULL;
  (*pIn).oricrysymstring = NULL;
  (*pIn).oricrysym = NULL;
  (*pIn).orioptistopstring = NULL;
  (*pIn).orioptistop = NULL;
  (*pIn).orioptineighstring = NULL;
  (*pIn).orioptineigh = NULL;
  (*pIn).orioptiinistring = NULL;
  (*pIn).orioptiini = NULL;
  (*pIn).orioptifixstring = NULL;
  (*pIn).orioptifix = NULL;
  (*pIn).orioptilogvarstring = NULL;
  (*pIn).orioptilogvar = NULL;
  (*pIn).morphooptilogtimestring = NULL;
  (*pIn).morphooptilogtime = NULL;
  (*pIn).morphooptilogvarstring = NULL;
  (*pIn).morphooptilogvar = NULL;
  (*pIn).morphooptilogdisstring = NULL;
  (*pIn).morphooptilogdis = NULL;
  (*pIn).morphooptilogtesrstring = NULL;
  (*pIn).morphooptilogtesr = NULL;
  (*pIn).morphooptilogvalstring = NULL;
  (*pIn).morphooptilogval = NULL;
  (*pIn).morphooptidofstring = NULL;
  (*pIn).morphooptidof = NULL;
  (*pIn).morphooptideltamaxstring = NULL;
  (*pIn).morphooptideltamax = NULL;
  (*pIn).morphooptimultiseedstring = NULL;
  (*pIn).morphooptimultiseed = NULL;
  (*pIn).morphooptiinistepstring = NULL;
  (*pIn).morphooptiinistep = NULL;
  (*pIn).periodicstring = NULL;
  (*pIn).periodic = NULL;
  (*pIn).reg = 0;
  (*pIn).fmax = 0;
  (*pIn).selstring = NULL;
  (*pIn).mloop = 0;
  (*pIn).seltype = NULL;

  (*pIn).levelqty = 0;

  (*pIn).input = 0;

  (*pIn).domain = NULL;
  (*pIn).transform = NULL;

  (*pIn).tesrsizestring = NULL;

  (*pIn).checktess = 0;

  (*pIn).format = NULL;
  (*pIn).tesrformat = NULL;
  (*pIn).oristring = NULL;
  (*pIn).ori = NULL;
  (*pIn).orides = NULL;
  (*pIn).oriformat = NULL;
  (*pIn).body = NULL;
  (*pIn).load = NULL;
  (*pIn).loadpoint = NULL;
  (*pIn).tess = NULL;
  (*pIn).mtess = NULL;
  (*pIn).tesr = NULL;
  (*pIn).vtk = NULL;
  (*pIn).raw = NULL;
  (*pIn).geo = NULL;
  (*pIn).mgeo = NULL;
  (*pIn).ply = NULL;
  (*pIn).stl = NULL;
  (*pIn).stc = NULL;
  (*pIn).stv = NULL;
  (*pIn).ste = NULL;
  (*pIn).stf = NULL;
  (*pIn).stp = NULL;
  (*pIn).sts = NULL;
  (*pIn).dec = NULL;
  (*pIn).obj = NULL;
  (*pIn).orif = NULL;
  (*pIn).debug = NULL;

  (*pIn).sortstring = NULL;
  (*pIn).stpt = NULL;
  (*pIn).stvox = NULL;

  return;
}

void
net_in_free (struct IN_T *pIn)
{
  ut_free_1d_char ((*pIn).domain);
  ut_free_1d_char ((*pIn).transform);
  ut_free_1d_char ((*pIn).format);
  ut_free_1d_char ((*pIn).tesrformat);
  ut_free_1d_char ((*pIn).oristring);
  ut_free_2d_char ((*pIn).ori, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).orides);
  ut_free_1d_char ((*pIn).oriformat);
  ut_free_1d_char ((*pIn).body);
  ut_free_1d_char ((*pIn).load);
  ut_free_1d_char ((*pIn).loadpoint);
  ut_free_1d_char ((*pIn).tess);
  ut_free_1d_char ((*pIn).mtess);
  ut_free_1d_char ((*pIn).tesr);
  ut_free_1d_char ((*pIn).vtk);
  ut_free_1d_char ((*pIn).raw);
  ut_free_1d_char ((*pIn).geo);
  ut_free_1d_char ((*pIn).mgeo);
  ut_free_1d_char ((*pIn).ply);
  ut_free_1d_char ((*pIn).stl);
  ut_free_1d_char ((*pIn).stc);
  ut_free_1d_char ((*pIn).stv);
  ut_free_1d_char ((*pIn).ste);
  ut_free_1d_char ((*pIn).stf);
  ut_free_1d_char ((*pIn).stp);
  ut_free_1d_char ((*pIn).sts);
  ut_free_1d_char ((*pIn).dec);
  ut_free_1d_char ((*pIn).obj);
  ut_free_1d_char ((*pIn).orif);
  ut_free_1d_char ((*pIn).debug);
  ut_free_1d_char ((*pIn).sortstring);
  ut_free_1d_char ((*pIn).stpt);
  ut_free_1d_char ((*pIn).tesrsizestring);

  ut_free_1d_char ((*pIn).input);
  ut_free_1d_char ((*pIn).periodicstring);
  ut_free_1d_int ((*pIn).periodic);
  ut_free_1d_char ((*pIn).seltype);
  ut_free_1d_char ((*pIn).selstring);
  ut_free_1d_char ((*pIn).nstring);
  ut_free_2d_char ((*pIn).n, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).idstring);
  ut_free_2d_char ((*pIn).id, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).mode);

  ut_free_1d_char ((*pIn).morphostring);
  ut_free_2d_char ((*pIn).morpho, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptialgostring);
  ut_free_2d_char ((*pIn).morphooptialgo, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptiobjectivestring);
  ut_free_2d_char ((*pIn).morphooptiobjective, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptismoothstring);
  ut_free_2d_char ((*pIn).morphooptismooth, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptistopstring);
  ut_free_2d_char ((*pIn).morphooptistop, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).oricrysymstring);
  ut_free_2d_char ((*pIn).oricrysym, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).orioptistopstring);
  ut_free_2d_char ((*pIn).orioptistop, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptilogtimestring);
  ut_free_2d_char ((*pIn).morphooptilogtime, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptilogvarstring);
  ut_free_2d_char ((*pIn).morphooptilogvar, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptidofstring);
  ut_free_2d_char ((*pIn).morphooptidof, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptideltamaxstring);
  ut_free_2d_char ((*pIn).morphooptideltamax, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptimultiseedstring);
  ut_free_2d_char ((*pIn).morphooptimultiseed, (*pIn).levelqty + 1);
  ut_free_1d_char ((*pIn).morphooptiinistepstring);
  ut_free_2d_char ((*pIn).morphooptiinistep, (*pIn).levelqty + 1);

  net_in_set_zero (pIn);

  return;
}

int
net_poly_clip (struct POLY *pPoly, double *eq, int id)
{
  struct POLYMOD Polymod;

  neut_polymod_set_zero (&Polymod);

  neut_poly_polymod (*pPoly, &Polymod);
  NewPolyhedron_plane (id, -1, eq, &Polymod);
  net_polymod_poly (Polymod, pPoly);

  return 0;
}

/*
void
CreateTesl (struct SEEDSET SSet, double **Size, struct TESL *pTesl)
{
  struct POLY Domain;
  struct POLY *Poly = NULL;

  net_domain_cube (Size, &Domain);
  net_polycomp (Domain, SSet, &Poly, NULL, 0);
  net_tesl (SSet, Poly, pTesl);

  return;
}
*/

void
net_tess_poly (struct TESS Tess, int poly, struct POLY *pPoly)
{
  int i, j, ver, face, verqty, faceqty, ori;
  int *vers = NULL;
  int *faces = NULL;
  int verfaceqty;
  int *vers_inv = NULL;
  int vers_invqty;
  int *faces_inv = NULL;
  int faces_invqty;

  neut_poly_free (pPoly);

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);
  (*pPoly).VerQty = verqty;

  (*pPoly).VerFace = ut_alloc_1d_pint ((*pPoly).VerQty + 1);
  (*pPoly).VerCoo = ut_alloc_2d ((*pPoly).VerQty + 1, 3);

  for (i = 1; i <= (*pPoly).VerQty; i++)
  {
    ver = vers[i - 1];

    neut_tess_ver_poly_faces (Tess, ver, poly, &((*pPoly).VerFace[i]),
			      &verfaceqty);

    if (verfaceqty != 3)
    {
      printf ("ver = %d verfaceqty = %d != 3\n", ver, verfaceqty);
      abort ();
    }

    ut_array_1d_memcpy ((*pPoly).VerCoo[i], 3, Tess.VerCoo[ver]);
  }

  (*pPoly).FaceQty = Tess.PolyFaceQty[poly];

  (*pPoly).FacePoly = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
  (*pPoly).FaceEq = ut_alloc_2d ((*pPoly).FaceQty + 1, 4);
  (*pPoly).FaceVerQty = ut_alloc_1d_int ((*pPoly).FaceQty + 1);
  (*pPoly).FaceVerNb = ut_alloc_1d_pint ((*pPoly).FaceQty + 1);

  faceqty = (*pPoly).FaceQty;
  for (i = 1; i <= (*pPoly).FaceQty; i++)
  {
    face = Tess.PolyFaceNb[poly][i];

    (*pPoly).FacePoly[i] = (Tess.FacePoly[face][0] == poly) ?
      Tess.FacePoly[face][1] : Tess.FacePoly[face][0];

    (*pPoly).FaceVerQty[i] = Tess.FaceVerQty[face];
    (*pPoly).FaceVerNb[i] = ut_alloc_1d_int ((*pPoly).FaceVerQty[i] + 1);
    ut_array_1d_int_memcpy ((*pPoly).FaceVerNb[i] + 1,
			    (*pPoly).FaceVerQty[i], Tess.FaceVerNb[face] + 1);

    ut_array_1d_memcpy ((*pPoly).FaceEq[i], 4, Tess.FaceEq[face]);

    // reversing face to get it pointing outwards, if necessary
    neut_tess_poly_face_ori (Tess, poly, face, &ori);

    if (ori == -1)
    {
      ut_array_1d_scale ((*pPoly).FaceEq[i], 4, -1);
      ut_array_1d_int_reverseelts ((*pPoly).FaceVerNb[i] + 1,
				   (*pPoly).FaceVerQty[i]);
    }
  }

  // renumbering vers
  int vermin = ut_array_1d_int_min (vers, verqty);
  ut_array_1d_int_addval (vers, verqty, -vermin, vers);

  ut_array_1d_int_inv (vers, verqty, &vers_inv, &vers_invqty);

  for (i = 1; i <= (*pPoly).FaceQty; i++)
    for (j = 1; j <= (*pPoly).FaceVerQty[i]; j++)
    {
      ver = (*pPoly).FaceVerNb[i][j];
      (*pPoly).FaceVerNb[i][j] = vers_inv[ver - vermin] + 1;
    }

  // renumbering faces
  faces = ut_alloc_1d_int (faceqty);
  ut_array_1d_int_memcpy (faces, faceqty, Tess.PolyFaceNb[poly] + 1);

  int facemin = ut_array_1d_int_min (faces, faceqty);
  ut_array_1d_int_addval (faces, faceqty, -facemin, faces);

  ut_array_1d_int_inv (faces, faceqty, &faces_inv, &faces_invqty);

  for (i = 1; i <= (*pPoly).VerQty; i++)
    for (j = 0; j < 3; j++)
    {
      face = (*pPoly).VerFace[i][j];
      (*pPoly).VerFace[i][j] = faces_inv[face - facemin] + 1;
    }

  /*
  // This is needed by multiscale
  // Actually, this seems only needed for multiscale + call to net_tess3d,
  // which happens only in net_tess_lam or net_tess_tocta.
  // This broke net_tess_clip, for example
  // so, the patch was moved to net_tess3d
  for (i = 1; i <= (*pPoly).FaceQty; i++)
    (*pPoly).FacePoly[i] = -i;
    */

  ut_free_1d_int (faces);
  ut_free_1d_int (vers);
  ut_free_1d_int (faces_inv);
  ut_free_1d_int (vers_inv);

  return;
}

void
net_poly_tesl (struct POLY Poly, double *coo, struct TESL *pTesl)
{
  struct SEEDSET SSet;
  struct POLY *PolyArray = (struct POLY *) calloc (2, sizeof (struct POLY));
  PolyArray[1] = Poly;

  neut_tesl_set_zero (pTesl);

  neut_seedset_set_zero (&SSet);

  SSet.N = 1;
  SSet.Nall = 1;
  SSet.Id = 1;

  SSet.SeedCoo = ut_alloc_2d (2, 3);
  ut_array_1d_memcpy (SSet.SeedCoo[1], 3, coo);

  net_tesl (SSet, PolyArray, pTesl);

  free (PolyArray);

  neut_seedset_free (&SSet);

  return;
}

void
net_poly_tess (struct POLY Poly, double *coo, struct TESS *pTess)
{
  struct TESL Tesl;
  struct SEEDSET SSet;
  double *coob = NULL;

  if (Poly.VerQty == 0)
  {
    neut_tess_set_zero (pTess);

    return;
  }

  coob = ut_alloc_1d (3);

  neut_tesl_set_zero (&Tesl);
  neut_seedset_set_zero (&SSet);
  ut_string_string ("standard", &(SSet.Type));

  if (coo)
    ut_array_1d_memcpy (coob, 3, coo);
  else
    neut_poly_centroid (Poly, coob);

  neut_seedset_addseed (&SSet, coob, 0);

  net_poly_tesl (Poly, coob, &Tesl);
  neut_tesl_tess (Tesl, SSet, 0, 0, pTess);
  neut_tess_poly_centroid (*pTess, 1, (*pTess).SeedCoo[1]);
  (*pTess).PseudoDim = Poly.PseudoDim;
  (*pTess).PseudoSize = Poly.PseudoSize;

  neut_tesl_free (&Tesl);
  neut_seedset_free (&SSet);
  ut_free_1d (coob);

  return;
}

void
net_poly_centroid (struct POLY Poly, double *coo)
{
  int i, f, ver;
  double area, vol, totvol;
  double *p0 = ut_alloc_1d (3);
  double *p1 = NULL;
  double *p2 = NULL;
  double *tmp = ut_alloc_1d (3);
  double *tmpcoo = ut_alloc_1d (3);

  ut_array_1d_set (tmpcoo, 3, 0);

  for (i = 1; i <= Poly.VerQty; i++)
    ut_array_1d_add (tmpcoo, Poly.VerCoo[i], 3, tmpcoo);

  ut_array_1d_scale (tmpcoo, 3, 1. / Poly.VerQty);

  totvol = 0;
  ut_array_1d_set (coo, 3, 0);

  for (f = 1; f <= Poly.FaceQty; f++)
  {
    ver = Poly.FaceVerNb[f][1];
    ut_array_1d_memcpy (p0, 3, Poly.VerCoo[ver]);

    for (i = 1; i <= Poly.FaceVerQty[f]; i++)
    {
      p1 = Poly.VerCoo[Poly.FaceVerNb[f][i]];
      p2 = Poly.VerCoo[Poly.FaceVerNb[f]
		       [ut_num_rotpos (1, Poly.FaceVerQty[f], i, 1)]];

      area = ut_space_triangle_area (p0, p1, p2);

      if (area > 1e-20)
      {
	ut_space_tet_centre (tmpcoo, p0, p1, p2, tmp);
	vol = ut_space_tet_volume (tmpcoo, p0, p1, p2);
	ut_array_1d_scale (tmp, 3, vol);
	ut_array_1d_add (coo, tmp, 3, coo);
	totvol += vol;
      }
    }
  }

  ut_array_1d_scale (coo, 3, 1. / totvol);

  ut_free_1d (p0);
  ut_free_1d (tmp);

  return;
}

void
net_tess_tesr (char *tesrsizestring, struct TESS Tess, struct TESR *pTesr)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  (*pTesr).Dim = Tess.Dim;
  neut_tesr_init_tesrsize (pTesr, Tess, Tess.Dim, tesrsizestring);
  neut_tesr_alloc (pTesr, Tess.Dim, (*pTesr).size, (*pTesr).vsize);
  (*pTesr).CellQty = Tess.CellQty;
  (*pTesr).CellBBox = ut_alloc_3d_int ((*pTesr).CellQty + 1, 3, 2);

  neut_tess_bbox (Tess, bbox);
  for (i = 0; i < 3; i++)
    (*pTesr).Origin[i] = bbox[i][0];

  if (Tess.CellId)
  {
    (*pTesr).CellId = ut_alloc_1d_int (Tess.CellQty + 1);
    ut_array_1d_int_memcpy ((*pTesr).CellId + 1, Tess.CellQty,
			    Tess.CellId + 1);
  }
  else if (Tess.CellOri)
  {
    (*pTesr).CellId = ut_alloc_1d_int (Tess.CellQty + 1);
    ut_array_1d_int_set_id ((*pTesr).CellId, Tess.CellQty + 1);
  }

  if (Tess.CellOri)
  {
    (*pTesr).CellOri = ut_alloc_2d (Tess.CellQty + 1, 4);
    ut_array_2d_memcpy ((*pTesr).CellOri + 1, Tess.CellQty, 4,
			Tess.CellOri + 1);
  }

#pragma omp parallel for schedule(dynamic)
  for (i = 1; i <= Tess.CellQty; i++)
    net_tess_tesr_cell (Tess, i, pTesr);

  neut_tesr_init_cellbbox (pTesr);

  neut_tesr_init_hasvoid (pTesr);

  ut_free_2d (bbox, 3);

  return;
}

void
net_clip (char *clip, struct SEEDSET SSet, struct TESS *pTess)
{
  int i, qty, status;
  FILE *fp = NULL;
  char *tmp = NULL;
  double *eq = ut_alloc_1d (4);
  char *input = ut_alloc_1d_char (strlen (clip) + 1);
  char *file = ut_alloc_1d_char (strlen (clip) + 1);

  ut_string_string (clip, &tmp);

  ut_string_fnr (tmp, '(', ' ');
  ut_string_fnr (tmp, ')', ' ');
  ut_string_fnr (tmp, ',', ' ');
  if (sscanf (tmp, "%s%s", input, file) != 2)
    ut_print_message (2, 2, "Failed to read argument `%s'.\n", clip);

  fp = ut_file_open (file, "r");
  if (fscanf (fp, "%d", &qty) != 1)
    abort ();
  for (i = 0; i < qty; i++)
  {
    status = ut_array_1d_fscanf (fp, eq, 4);
    if (status != 1)
      abort ();
    ut_array_1d_scale (eq, 4, 1. / ut_vector_norm (eq + 1));
    net_tess_clip (SSet, pTess, eq);
  }
  ut_file_close (fp, file, "r");

  ut_free_1d_char (tmp);
  ut_free_1d_char (input);
  ut_free_1d_char (file);
  ut_free_1d (eq);

  return;
}

int
net_tess_clip (struct SEEDSET SSet, struct TESS *pTess, double *eq)
{
  int i, side, level, tessid, domface;
  struct POLY *Poly = calloc ((*pTess).CellQty + 1, sizeof (struct POLY));
  struct POLYMOD Polymod;
  int *BadVer = ut_alloc_1d_int (1000);
  struct TESL Tesl;
  struct TESS TessCpy;
  int plane_id;

  level = (*pTess).Level;
  tessid = (*pTess).TessId;

  plane_id = ut_array_2d_int_min ((*pTess).FacePoly + 1, (*pTess).FaceQty, 2) - 1;

  neut_tesl_set_zero (&Tesl);
  neut_tess_set_zero (&TessCpy);
  neut_tess_tess (*pTess, &TessCpy);

  neut_polymod_set_zero (&Polymod);

  for (i = 1; i <= (*pTess).CellQty; i++)
  {
    neut_tess_cell_planeside (*pTess, i, eq, &side);

    if (side == -1) // downward
      net_tess_poly (*pTess, i, Poly + i);
    else if (side == 0) // cut
    {
      net_tess_poly (*pTess, i, Poly + i);
      net_poly_clip (Poly + i, eq, plane_id);
    }
    else // upward, i.e. removed
      neut_poly_set_zero (Poly + i);
  }

  net_tesl (SSet, Poly, &Tesl);
  neut_tesl_tess (Tesl, SSet, level, tessid, pTess);

  ut_free_1d_int (BadVer);
  neut_tesl_free (&Tesl);

  int j;
  int domfaceqty = 0;
  int *domfaces = ut_alloc_1d_int ((*pTess).FaceQty);
  int *domfaceinv = NULL;
  int domfacemax;
  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      if ((*pTess).FacePoly[i][j] < 0)
	domfaces[domfaceqty++] = -(*pTess).FacePoly[i][j];

  ut_array_1d_int_sort_uniq (domfaces, domfaceqty, &domfaceqty);

  ut_array_1d_int_inv (domfaces, domfaceqty, &domfaceinv, &domfacemax);

  for (i = 1; i <= (*pTess).FaceQty; i++)
    for (j = 0; j < 2; j++)
      if ((*pTess).FacePoly[i][j] < 0)
	(*pTess).FacePoly[i][j] = -domfaceinv[-(*pTess).FacePoly[i][j]] - 1;

  ut_free_1d_int (domfaces);
  ut_free_1d_int (domfaceinv);

  ut_string_string ("clipped", &((*pTess)).DomType);
  neut_tess_init_domain (pTess);

  domface = -1;
  for (i = 1; i <= (*pTess).DomFaceQty; i++)
    if (ut_array_1d_equal ((*pTess).DomFaceEq[i], eq, 4, 1e-12))
    {
      domface = i;
      break;
    }

  if (domface == -1)
    abort ();

  neut_tess_tess_gen (TessCpy, pTess);
  neut_tess_tess_cell (TessCpy, pTess);
  neut_tess_tess_seed (TessCpy, pTess);
  neut_tess_tess_scale (TessCpy, pTess);

  neut_tess_free (&TessCpy);

  return domface;
}

int
net_multiscale_mtess_arg_0d_char_fscanf (int level, struct MTESS MTess, struct TESS *Tess,
                                         int domtess, int dompoly,
					 char *string, char **pval)
{
  char *mid = NULL;

  if (level == 1 && strncmp (string, "msfile(", 7))
    ut_string_string (string, pval);

  else if (ut_string_filename (string))
  {
    (*pval) = ut_alloc_1d_char (1000);
    neut_mtess_tess_poly_mid (MTess, Tess[domtess], dompoly, &mid);
    net_multiscale_arg_0d_char_fscanf (string, mid, *pval);
    (*pval) = ut_realloc_1d_char (*pval, strlen (*pval) + 1);
  }

  else
    ut_string_string (string, pval);

  ut_free_1d_char (mid);

  return 0;
}

int
net_multiscale_arg_0d_char_fscanf (char *string, char *flag, char *val)
{
  int status;
  FILE *file = NULL;

  if (!ut_string_filename (string))
    strcpy (val, string);
  else
  {
    file = ut_file_open (string, "R");
    status = ut_array_0d_char_fscanf_filter_prefix (file, val, flag);
    if (status != 1)
      ut_print_message (2, 0, "Failed to read file.\n");

    ut_file_close (file, string, "R");
  }

  return 0;
}

int
net_multiscale_arg_0d_fscanf (char *string, char *flag,
			      int varqty, char **vars, double *vals,
			      double *pval)
{
  int status;
  FILE *file = NULL;
  char *tmp = ut_alloc_1d_char (1000);

  if (!ut_string_filename (string))
    strcpy (tmp, string);
  else
  {
    file = ut_file_open (string, "R");
    status = ut_array_0d_char_fscanf_filter_prefix (file, tmp, flag);
    if (status != 1)
      ut_print_message (2, 0, "Failed to read file `%s'.\n", string);

    ut_file_close (file, string, "R");
  }

  ut_math_eval (tmp, varqty, vars, vals, pval);

  ut_free_1d_char (tmp);

  return 0;
}

int
net_multiscale_arg_0d_int_fscanf (char *string, char *flag,
				  int varqty, char **vars, double *vals,
				  int *pval)
{
  double val;

  net_multiscale_arg_0d_fscanf (string, flag, varqty, vars, vals,
				&val);
  (*pval) = ut_num_d2ri (val);

  return 0;
}

int
net_multiscale_arg_1d_fscanf (char *string, char *flag, double *val, int valqty)
{
  int status;
  FILE *file = NULL;

  file = ut_file_open (string, "R");
  status = ut_array_1d_fscanf_filter_prefix (file, val, valqty, flag);
  if (status != 1)
    ut_print_message (2, 0, "Failed to read file `%s'.\n", string);
  ut_file_close (file, string, "R");

  return 0;
}

int
net_multiscale_arg_1d_int_fscanf (char *string, char *flag, int *val, int valqty)
{
  int status;
  FILE *file = NULL;

  file = ut_file_open (string, "R");
  status = ut_array_1d_int_fscanf_filter_prefix (file, val, valqty, flag);
  if (status != 1)
    ut_print_message (2, 0, "Failed to read file `%s'.\n", string);
  ut_file_close (file, string, "R");

  return 0;
}

int
net_multiscale_arg_2d_fscanf (char *string, char *flag, double **val, int size1, int size2)
{
  int status;
  FILE *file = NULL;

  file = ut_file_open (string, "R");
  status = ut_array_2d_fscanf_filter_prefix (file, val, size1, size2, flag);
  if (status != 1)
    ut_print_message (2, 0, "Failed to read file `%s'.\n", string);
  ut_file_close (file, string, "R");

  return 0;
}

void
net_cubetess (double **size, struct TESS *pTess)
{
  struct POLY Poly;
  double *coo = ut_alloc_1d (3);

  neut_poly_set_zero (&Poly);
  neut_tess_free (pTess);

  net_domain_cube (size, &Poly);
  neut_poly_centroid (Poly, coo);
  net_poly_tess (Poly, coo, pTess);

  ut_string_string ("cube", &((*pTess).DomType));
  neut_tess_init_domain (pTess);

  neut_poly_free (&Poly);
  ut_free_1d (coo);

  return;
}

void
net_pts_convexhull (double **coos, int qty, int dim, struct NODES *pN, struct MESH *pM)
{
  int i, **FacePoly = NULL, FaceQty;
  int **EdgeFaceNb = NULL, *EdgeFaceQty = NULL, EdgeQty;
  double **cubesize = ut_alloc_2d (3, 2);
  double* coo = ut_alloc_1d (3);
  struct SEEDSET SSet;
  struct POLY DomPoly;
  struct TESS Dom, Tess, Tess2;
  struct MESH Mesh;

  neut_seedset_set_zero (&SSet);
  neut_poly_set_zero (&DomPoly);
  neut_tess_set_zero (&Tess);
  neut_tess_set_zero (&Tess2);
  neut_tess_set_zero (&Dom);
  neut_mesh_set_zero (&Mesh);

  SSet.N = qty;
  SSet.Nall = qty;
  ut_string_string ("standard", &(SSet.Type));
  SSet.SeedCoo0 = ut_alloc_2d (qty + 1, 3);
  ut_array_2d_memcpy (SSet.SeedCoo0 + 1, qty, dim, coos);
  SSet.SeedCoo = ut_alloc_2d (qty + 1, 3);
  ut_array_2d_memcpy (SSet.SeedCoo + 1, SSet.N, dim, coos);
  SSet.SeedWeight = ut_alloc_1d (qty + 1);

  for (i = 0; i < 3; i++)
    ut_array_1d_set_2 (cubesize[i], -1e6, 1e6);

  net_domain_cube (cubesize, &DomPoly);
  net_poly_tess (DomPoly, coo, &Dom);
  ut_string_string ("cube", &Dom.DomType);

  net_tess3d (Dom, 1, SSet, "nanoflann", 1, NULL, &Tess);
  neut_tess_init_domain_poly (&Tess, Dom, 1, NULL, NULL, NULL);
  if (dim == 2)
  {
    neut_tess_domface_tess (Tess, 6, &Tess2);
    neut_tess_tess (Tess2, &Tess);
  }

  neut_tess_delaunay (Tess, pN, &Mesh);

  if (dim == 3)
    neut_mesh3d_mesh2d (*pN, Mesh, pM, &FacePoly, &FaceQty, 0);
  else if (dim == 2)
    neut_mesh2d_mesh1d (*pN, Mesh, pM, &EdgeFaceNb, &EdgeFaceQty, &EdgeQty, 0);

  ut_free_2d_int (FacePoly, FaceQty);
  ut_free_2d_int (EdgeFaceNb, EdgeQty);
  ut_free_1d_int (EdgeFaceQty);
  ut_free_2d (cubesize, 3);
  ut_free_1d (coo);
  neut_seedset_free (&SSet);
  neut_poly_free (&DomPoly);
  neut_tess_free (&Dom);
  neut_tess_free (&Tess);
  neut_tess_free (&Tess2);
  neut_mesh_free (&Mesh);

  return;
}

int
net_tess_seedset (struct TESS Tess, struct SEEDSET *pSSet)
{
  neut_seedset_free (pSSet);
  neut_seedset_set_zero (pSSet);

  (*pSSet).N = Tess.SeedQty;
  (*pSSet).Nall = Tess.SeedQty;
  ut_string_string (Tess.Type, &(*pSSet).Type);

  if (!strcmp ((*pSSet).Type, "periodic"))
    abort ();

  (*pSSet).SeedCoo = ut_alloc_2d ((*pSSet).Nall + 1, 3);
  ut_array_2d_memcpy ((*pSSet).SeedCoo + 1, (*pSSet).Nall, 3, Tess.SeedCoo + 1);

  (*pSSet).SeedCoo0 = ut_alloc_2d ((*pSSet).N + 1, 3);
  ut_array_2d_memcpy ((*pSSet).SeedCoo0 + 1, (*pSSet).N, 3, (*pSSet).SeedCoo + 1);

  (*pSSet).SeedWeight = ut_alloc_1d ((*pSSet).Nall + 1);
  ut_array_1d_memcpy ((*pSSet).SeedWeight + 1, (*pSSet).Nall, Tess.SeedWeight + 1);

  return 0;
}

void
net_seedset_tess (struct SEEDSET SSet, struct TESS *pTess)
{
  neut_tess_set_zero (pTess);

  (*pTess).Dim = 0;

  (*pTess).CellQty = SSet.N;

  if (SSet.q)
  {
    (*pTess).CellOri = ut_alloc_2d ((*pTess).CellQty + 1, 4);

    ut_array_2d_memcpy ((*pTess).CellOri + 1, (*pTess).CellQty, 4, SSet.q + 1);
  }

  return;
}
