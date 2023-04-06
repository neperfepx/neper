/* This file is part of the 'hermes' program. */
/* Copyright (C) 2007-2010, Romain Quey. */
/* See the COPYINPFG file in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf_header (struct PF Pf, FILE *file)
{
  int i;

  fprintf (file, "import graph;\n");
  fprintf (file, "import palette;\n");
  fprintf (file, "import contour;\n");
  // fprintf (file, "settings.outformat=\"pdf\";\n");
  if (!strcmp (Pf.font, "TimesRoman"))
    fprintf (file, "usepackage(\"mathptmx\"); defaultpen(TimesRoman());\n");
  else if (!strcmp (Pf.font, "ComputerModern"))
  { }
  else
    ut_print_message (1, 0, "Font symbol unknown (ignoring)\n");

  fprintf (file, "unitsize(1mm);\n");
  fprintf (file, "\n");

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
      fprintf (file, "real scale = 30;\n");
    else if (!strcmp (Pf.shape, "quarter"))
      fprintf (file, "real scale = 60;\n");
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    if (!strcmp (Pf.crysym, "cubic"))
      fprintf (file, "real scale = 60;\n");
    else if (!strcmp (Pf.crysym, "hexagonal"))
      fprintf (file, "real scale = 80;\n");
    else
      abort ();
  }

  else
    abort ();

  fprintf (file, "pair O  = (0,0);\n");
  fprintf (file, "pair X  = (1,0);\n");
  fprintf (file, "pair Y  = (0,1);\n");
  fprintf (file, "\n");

  // circle
  fprintf (file, "void circle (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file,
	   "{\n  if (w > 0) filldraw(shift(p)*circle(O, s), rgba+opacity(1-transa), rgbb+w);\n");
  fprintf (file,
	   "  else filldraw(shift(p)*circle(O, s), rgba, invisible);\n}\n");

  // sphere
  fprintf (file, "void sphere (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file, "{ circle (p, s, theta, rgba, transa, rgbb, w); }\n");

  // square
  fprintf (file, "void square (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file,
	   "{ real a = 1.77245385090551602729 * s;\n"); // sqrt (pi)
  fprintf (file,
           "  if (w > 0) filldraw(shift(p)*rotate(theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), rgbb+w);\n");
  fprintf (file,
	   "  else filldraw(shift(p)*rotate(theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), invisible);\n}\n");

  // cube
  fprintf (file, "void cube (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file, "{ square (p, s, theta, rgba, transa, rgbb, w); }\n");

  // triangle
  fprintf (file, "void triangle (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file,
	   "{ real a = 2.69354737417719672123 * s;\n"); // sqrt (4 * pi / sqrt (3))
  fprintf (file,
	   "  if (w > 0) filldraw(shift(p)*rotate(theta)*scale(a)*((-0.5,-0.289)--(0.5,-0.289)--(0,0.577)--cycle), rgba+opacity(1-transa), rgbb+w);\n");
  fprintf (file,
	   "  else filldraw(shift(p)*rotate(theta)*scale(a)*((-0.5,-0.289)--(0.5,-0.289)--(0,0.577)--cycle), rgba+opacity(1-transa), invisible);\n}\n");

  // diamond
  fprintf (file, "void diamond (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file,
	   "{ real a = 1.77245385090551602729 * s;\n"); // sqrt (pi)
  fprintf (file,
           "  if (w > 0) filldraw(shift(p)*rotate(45+theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), rgbb+w);\n");
  fprintf (file,
	   "  else filldraw(shift(p)*rotate(45+theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), invisible);\n}\n");

  // diamond
  fprintf (file, "void star (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)\n");
  fprintf (file, "{\n");

  double *coo = ut_alloc_1d (2);
  for (i = 0; i < 5; i++)
  {
    coo[0] = cos (2 * M_PI * i / 5. + M_PI / 2);
    coo[1] = sin (2 * M_PI * i / 5. + M_PI / 2);
    fprintf (file, "  pair pair%d = (%lf,%lf);\n", i, coo[0], coo[1]);
  }
  for (i = 0; i < 5; i++)
  {
    coo[0] = (3 - sqrt(5)) / 2 * cos (2 * M_PI * i / 5. + M_PI / 2 + M_PI / 5);
    coo[1] = (3 - sqrt(5)) / 2 * sin (2 * M_PI * i / 5. + M_PI / 2 + M_PI / 5);
    fprintf (file, "  pair pair%d = (%lf,%lf);\n", i + 5, coo[0], coo[1]);
  }

  ut_free_1d (&coo);

  fprintf (file, "if (w > 0) filldraw(shift(p)*rotate(theta)*scale(1.895 * s)*(pair0--pair5--pair1--pair6--pair2--pair7--pair3--pair8--pair4--pair9--cycle), rgba+opacity(1-transa), rgbb+w);\n");
  fprintf (file, "      else filldraw(shift(p)*rotate(theta)*scale(1.895 * s)*(pair0--pair5--pair1--pair6--pair2--pair7--pair3--pair8--pair4--pair9--cycle), rgba+opacity(1-transa), invisible);\n");
  fprintf (file, "}\n");

  return;
}

void
nev_print_pf_background (struct IN_V In, struct PF Pf, FILE *file, struct PRINT Print)
{
  double xmin = 0, xmax = 0, ymin = 0, ymax = 0;
  int density = ut_list_testelt (Pf.mode, NEUT_SEP_NODEP, "density");

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
    {
      xmin = -1 - WMARGINPF;
      xmax =  1 + (!density || !Print.showscale ? EMARGINPF : 1.10);
      ymin = -1 - SMARGINPF;
      ymax =  1 + NMARGINPF;
    }
    else if (!strcmp (Pf.shape, "quarter"))
    {
      xmin =  0 - WMARGINPF / 2;
      xmax =  1 + (!density || !Print.showscale ? EMARGINPF / 2 : 0.55);
      ymin =  0 + NMARGINPF;
      ymax = -1 - SMARGINPF / 2;
    }
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    xmin =  0 - WMARGINPF / 2;
    if (!strcmp (Pf.crysym, "cubic"))
      xmax =  1 + (!density || !Print.showscale ? EMARGINPF / 2 : 0.55);
    else if (!strcmp (Pf.crysym, "hexagonal"))
      xmax =  1 + (!density || !Print.showscale ? EMARGINPF / 2 : 0.41);

    if (!strcmp (Pf.crysym, "cubic"))
      ymin =  0 - NMARGINPF / 2;
    else if (!strcmp (Pf.crysym, "hexagonal"))
      ymin =  0 - NMARGINPF / 2 - 0.02;
    else
      abort ();

    if (!strcmp (Pf.crysym, "cubic"))
      ymax =  1;
    else if (!strcmp (Pf.crysym, "hexagonal"))
      ymax =  0.62;
    else
      abort ();
  }

  else
    abort ();

  fprintf (file, "filldraw (scale(scale)*((%.3f,%.3f)--(%.3f,%.3f)--(%.3f,%.3f)--(%.3f,%.3f)--cycle),%s,%s);\n", xmin, ymin, xmax, ymin, xmax, ymax, xmin, ymax,
      In.scenebackground, In.scenebackground);

  if (!ut_list_testelt (Pf.mode, NEUT_SEP_NODEP, "density"))
  {
    if (!strcmp (Pf.space, "pf"))
    {
      if (!strcmp (Pf.shape, "full"))
      {
        fprintf (file, "draw (shift(O)*scale(scale)*(-X--X), black);\n");
        fprintf (file, "draw (shift(O)*scale(scale)*(-Y--Y), black);\n");
      }
      else if (!strcmp (Pf.shape, "quarter"))
      {
        fprintf (file, "draw (shift(O)*scale(scale)*(O--X), black);\n");
        fprintf (file, "draw (shift(O)*scale(scale)*(O--(-Y)), black);\n");
      }
    }
  }

  return;
}

void
nev_print_pf_border (FILE *file, struct PF *pPf)
{
  if (!strcmp ((*pPf).space, "pf"))
  {
    if (!strcmp ((*pPf).shape, "full"))
      fprintf (file, "draw (shift(O)*scale(scale)*unitcircle, black);\n");
    else if (!strcmp ((*pPf).shape, "quarter"))
      fprintf (file, "draw (shift(O)*scale(scale)*arc(O,-Y,X), black);\n");
    else
      abort ();
  }

  else if (!strcmp ((*pPf).space, "ipf"))
  {
    int i;

    fprintf (file, "path line110111 = ");
    for (i = 1; i < (*pPf).ipfptqty; i++)
      fprintf (file, "(%f,%f)%s", (*pPf).ipfpts[i][0], (*pPf).ipfpts[i][1],
               i < (*pPf).ipfptqty - 1 ? "--" : ";\n");

    fprintf (file, "path border = (0,0)--line110111--cycle;\n");

    fprintf (file, "draw (shift(O)*scale(scale/%f)*border,black);\n",
             (!strcmp ((*pPf).space, "ipf") && !strcmp ((*pPf).crysym, "cubic")) ? (*pPf).ipfpts[1][0] : 1.);
  }

  else
    abort ();

  return;
}

void
nev_print_pf_inputs (struct PRINT Print, struct POINT *Points,
                     int PointQty, struct PF *pPf)
{
  int i;

  if (Print.showtess == 1)
  {
    (*pPf).inputqty++;
    (*pPf).inputs = ut_realloc_1d_pchar_null ((*pPf).inputs, (*pPf).inputqty, 1);
    ut_string_string ("tess", (*pPf).inputs + (*pPf).inputqty - 1);
  }

  if (Print.showtesr == 1)
  {
    (*pPf).inputqty++;
    (*pPf).inputs = ut_realloc_1d_pchar_null ((*pPf).inputs, (*pPf).inputqty, 1);
    ut_string_string ("tesr", (*pPf).inputs + (*pPf).inputqty - 1);
  }

  if (Print.showmesh == 1)
  {
    (*pPf).inputqty++;
    (*pPf).inputs = ut_realloc_1d_pchar_null ((*pPf).inputs, (*pPf).inputqty, 1);
    ut_string_string ("mesh", (*pPf).inputs + (*pPf).inputqty - 1);
  }

  for (i = 0; i < PointQty; i++)
    if (ut_array_1d_int_sum (Print.showpoint[i] + 1, Points[i].Qty) > 0)
    {
      (*pPf).inputqty++;
      (*pPf).inputs = ut_realloc_2d_char_addline ((*pPf).inputs, (*pPf).inputqty, 100);
      sprintf ((*pPf).inputs[(*pPf).inputqty - 1], "point%d", i);
    }

  if (!strcmp ((*pPf).space, "ipf"))
  {
    (*pPf).inputcellqty = ut_alloc_1d_int ((*pPf).inputqty);
    (*pPf).inputcellpole = ut_alloc_1d_pint ((*pPf).inputqty);
    (*pPf).inputcellori = ut_alloc_1d_ppdouble ((*pPf).inputqty);
  }

  return;
}

void
nev_print_pf_pre_tess (struct PRINT Print, struct TESS Tess,
                       struct DATA *TessData, struct DATAINPUT *pDataInput,
                       struct DATA **ppData, int *pcellqty, double **pwgts,
                       double ***pdata, char **ptype, int **pshow)
{
  int i;
  char *value = NULL;
  struct SIMRES SimRes;
  neut_simres_set_zero (&SimRes);

  ut_string_string ("tess", &(*pDataInput).input);
  *ppData = TessData + Tess.Dim;

  (*pcellqty) = Tess.CellQty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  if (!(**ppData).Value || !strcmp ((**ppData).Value, "ori"))
  {
    ut_string_string ("ori", ptype);
    neut_tess_cellori (Tess, *pdata - 1);
  }
  else
  {
    neut_data_datastring_type_value ("cell", "", (**ppData).Value, ptype, &value);
    neut_data_fscanf_general (*pDataInput, "cell", 3, (**ppData).Qty, "", *ptype, value, *ppData);
    ut_array_2d_memcpy ((**ppData).Data + 1, *pcellqty, (**ppData).DataSize, *pdata);
  }

  *pshow = Tess.Dim == 3 ? Print.showpoly + 1 : Print.showface + 1;

  neut_simres_free (&SimRes);

  ut_free_1d_char (&value);

  return;
}

void
nev_print_pf_pre_tesr (struct PRINT Print, struct TESR Tesr,
                       struct DATA *TesrData, struct DATAINPUT *pDataInput,
                       struct DATA **ppData, int *pcellqty, double **pwgts,
                       double ***pdata, char **ptype, int **pshow)
{
  int i;
  char *value = NULL;
  struct SIMRES SimRes;
  neut_simres_set_zero (&SimRes);

  ut_string_string ("tesr", &(*pDataInput).input);
  *ppData = TesrData;

  (*pcellqty) = Tesr.CellQty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  if (!(**ppData).Value || !strcmp ((**ppData).Value, "ori"))
  {
    ut_string_string ("ori", ptype);
    neut_tesr_cellori (Tesr, *pdata - 1);
  }

  else
  {
    neut_data_datastring_type_value ("cell", "", (**ppData).Value, ptype, &value);
    neut_data_fscanf_general (*pDataInput, "cell", 3, (**ppData).Qty, "", *ptype, value, *ppData);
    ut_array_2d_memcpy ((**ppData).Data + 1, *pcellqty, (**ppData).DataSize, *pdata);
  }

  ut_free_1d_char (&value);

  *pshow = Tesr.Dim == 3 ? Print.showpoly + 1 : Print.showface + 1;

  neut_simres_free (&SimRes);

  return;
}

void
nev_print_pf_pre_mesh (struct PRINT Print,
                       struct MESH *Mesh, struct DATA **MeshData,
                       struct DATAINPUT *pDataInput, struct DATA **ppData,
                       int *pcellqty, double **pwgts, double ***pdata,
                       char **ptype, int **pshow)
{
  int i, dim = neut_mesh_array_dim (Mesh);
  char *value = NULL;
  struct SIMRES SimRes;
  neut_simres_set_zero (&SimRes);

  ut_string_string ("mesh", &(*pDataInput).input);
  *ppData = MeshData[dim] + 1;

  (*pcellqty) = Mesh[dim].ElsetQty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  if (!(**ppData).Value || !strcmp ((**ppData).Value, "ori"))
  {
    ut_string_string ("ori", ptype);
    neut_mesh_elsetori (Mesh[dim], *pdata - 1);
  }

  else
  {
    neut_data_datastring_type_value ("elset", "", (**ppData).Value, ptype, &value);
    neut_data_fscanf_general (*pDataInput, "elset", 3, (**ppData).Qty, "", *ptype, value, *ppData);
    ut_array_2d_memcpy ((**ppData).Data + 1, *pcellqty, (**ppData).DataSize, *pdata);
  }

  ut_free_1d_char (&value);

  *pshow = dim == 3 ? Print.showelset3d + 1 : Print.showelset2d + 1;

  neut_simres_free (&SimRes);

  return;
}

void
nev_print_pf_pre_points (struct PRINT Print,
                         struct POINT Points, struct DATA *PointData, int id,
                         struct DATAINPUT *pDataInput, struct DATA **ppData,
                         int *pcellqty, double **pwgts, double ***pdata,
                         char **ptype, int **pshow)
{
  int i;

  ut_string_string ("point", &(*pDataInput).input);
  *ppData = PointData;

  (*pcellqty) = Points.Qty;
  (*pwgts) = ut_alloc_1d (*pcellqty);
  for (i = 1; i <= *pcellqty; i++)
    (*pwgts)[i - 1] = (**ppData).Weight[i];

  (*pdata) = ut_alloc_2d (*pcellqty, 4);

  if (!strcmp (Points.Type, "none"))
  {
    for (i = 0; i < *pcellqty; i++)
      ol_R_q (Points.Coo[i + 1], (*pdata)[i]);

    ut_string_string ("ori", &(**ppData).Value);
  }

  else if (!strcmp (Points.Type, "ori"))
  {
    ut_array_2d_memcpy (Points.Coo + 1, *pcellqty, 4, *pdata);
    ut_string_string (strcmp (Points.Type, "none") ? Points.Type : "ori", &(**ppData).Value);

    ut_string_string ("ori", &(**ppData).Value);
  }

  else
  {
    ut_array_2d_memcpy (Points.Coo + 1, *pcellqty, 3, *pdata);

    ut_string_string (Points.Type, &(**ppData).Value);
  }

  ut_string_string ((**ppData).Value, ptype);

  *pshow = Print.showpoint[id] + 1;

  return;
}

void
nev_print_pf_pre_clustering (struct PF Pf, int cellqty, int *pclustering)
{
  int i, modeqty;
  char **modes = NULL;

  ut_list_break (Pf.mode, NEUT_SEP_NODEP, &modes, &modeqty);

  (*pclustering) = 0;
  if (Pf.clustering && !strcmp (Pf.space, "pf") && !strcmp (Pf.shape, "full") && cellqty > 1000)
    for (i = 0; i < modeqty; i++)
      if (!strcmp (modes[i], "density"))
        (*pclustering) = 1;

  ut_free_2d_char (&modes, modeqty);

  return;
}

void
nev_print_pf_pole_proj (struct PF Pf, FILE *file, char *pole_string)
{
  double xmin = 0, y, xshift;

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
    {
      xmin = -1 - WMARGINPF;
      xshift = 0.01;
      y = 1;
    }
    else if (!strcmp (Pf.shape, "quarter"))
    {
      xmin = - WMARGINPF / 2;
      xshift = 0.005;
      y = 0.1;
    }
    else
      abort ();

    fprintf (file, "label(\"%s\", scale(scale)*(%.3f, %.3f), NE);\n", pole_string, xmin, y);

    fprintf (file, "label(\"%s\", scale(scale)*(%.3f, %.3f), SE, fontsize(8));\n",
             Pf.pfprojlabel, xmin + xshift, y);
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    xmin =  - WMARGINPF / 2;
    xshift = 0.05;
    if (!strcmp (Pf.crysym, "cubic"))
      y = 0.9;
    else
      y = 0.5;

    fprintf (file, "label(\"%s\", scale(scale)*(%.3f, %.3f), NE);\n", pole_string, xmin + xshift, y);

    fprintf (file, "label(\"%s\", scale(scale)*(%.3f, %.3f), SE, fontsize(8));\n",
        Pf.pfprojlabel, xmin + xshift, y);
  }

  else
    abort ();

  return;
}

void
nev_print_pf_csys (struct PF Pf, FILE *file, struct DATA CsysData)
{
  int id;
  double* v = ol_r_alloc ();
  double* p110 = ol_p_alloc ();
  double* p111 = ol_p_alloc ();

  ut_array_1d_set_3 (v, 0, OL_IS2, OL_IS2);
  if (! strcmp (Pf.projection, "stereographic"))
    ol_vect_stprojxy (v, p110);
  else if (! strcmp (Pf.projection, "equal-area"))
    ol_vect_eaprojxy (v, p110);

  ut_array_1d_set_3 (v, OL_IS3, OL_IS3, OL_IS3);
  if (! strcmp (Pf.projection, "stereographic"))
    ol_vect_stprojxy (v, p111);
  else if (! strcmp (Pf.projection, "equal-area"))
    ol_vect_eaprojxy (v, p111);

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
    {
      id = ut_array_1d_absmax_index (Pf.pfdir[0], 3);
      fprintf (file, "label(\"%s\", scale(scale)*(%d,0), %s);\n",
               CsysData.Label[id],
               ut_num_sgn (Pf.pfdir[0][id]), Pf.pfdir[0][id] > 0 ? "E" : "W");
      id = ut_array_1d_absmax_index (Pf.pfdir[1], 3);
      fprintf (file, "label(\"%s\", scale(scale)*(0,%d), %s);\n",
               CsysData.Label[id],
               ut_num_sgn (Pf.pfdir[1][id]), Pf.pfdir[1][id] > 0 ? "N" : "S");
      // fprintf (file, "label(\"$%s$\", scale(scale)*( 0,0), NE);\n", " ");

    }
    else if (!strcmp (Pf.shape, "quarter"))
    {
      id = ut_array_1d_absmax_index (Pf.pfdir[0], 3);
      fprintf (file, "label(\"%s\", scale(scale)*( 1,0), E);\n", CsysData.Label[id]);
      id = ut_array_1d_absmax_index (Pf.pfdir[1], 3);
      fprintf (file, "label(\"%s\", scale(scale)*(0,-1), S);\n", CsysData.Label[id]);
      id = ut_array_1d_absmax_index (Pf.pfdir[2], 3);
      fprintf (file, "label(\"%s\", scale(scale)*( 0,0), W);\n", CsysData.Label[id]);
    }
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    if (!strcmp (Pf.crysym, "cubic"))
    {
      fprintf (file, "label(\"$[1\\,0\\,0]$\", scale(scale)*O, S);\n");
      fprintf (file, "label(\"$[1\\,1\\,0]$\", scale(scale)*X, S);\n");
      fprintf (file, "label(\"$[1\\,1\\,1]$\", scale(scale/%f)*(%f,%f), N);\n",
               p110[1], p111[1], p111[1]);
    }

    else if (!strcmp (Pf.crysym, "hexagonal"))
    {
      fprintf (file, "label(\"$[0\\,0\\,0\\,1]$\", scale(scale)*O, S);\n");
      fprintf (file, "label(\"$[2\\\\\\overline{1}\\\\\\overline{1}\\,0]$\", scale(scale)*X, S);\n");
      fprintf (file, "label(\"$[1\\,0\\,\\\\\\overline{1}\\,0]$\", scale(scale/%f)*(%f,%f), N);\n",
               Pf.ipfpts[1][0],
               Pf.ipfpts[Pf.ipfptqty - 1][0],
               Pf.ipfpts[Pf.ipfptqty - 1][1]);
    }
  }

  else
    abort ();

  ol_r_free (v);
  ol_p_free (p111);

  return;
}

void
nev_print_pf_pts (int input, struct PF *pPf, char *type_in,
                  double **data_in, double *weights, int qty, int *show,
                  int clustering, double ***ppts, double **pptwgts,
                  int **pptcells, int *pptqty, double ***cellpts,
                  int *cellptqty)
{
  int i;
  char *type = NULL;
  double **data = NULL;

  if (!type_in || !strcmp (type_in, "none")) // if none, we assume rodrigues vectors
  {
    data = ut_alloc_2d (qty, 4);
    for (i = 0; i < qty; i++)
      ol_R_q (data_in[i], data[i]);
    ut_string_string ("ori", &type);
  }
  else
  {
    type = type_in;
    data = data_in;
  }

  if (!strcmp (type, "ori"))
    nev_print_pf_file_data_ori (input, pPf, data, weights, qty, show,
                                ppts, pptwgts, pptcells, pptqty);

  else if (!strcmp (type, "vector"))
    nev_print_pf_file_data_vect (input, pPf, data, weights, qty, show,
                                 ppts, pptwgts, pptcells, pptqty);

  else
    ut_print_exprbug (type);

  if (!strcmp ((*pPf).space, "pf") && ut_num_equal (ut_array_1d_min ((*pPf).pfdir[2], 3), -1, 1e-6))
    ut_array_2d_scale (*ppts, *pptqty, 2, -1);

  if (!strcmp ((*pPf).refsym, "orthotropic"))
  {
    if (!strcmp ((*pPf).space, "ipf"))
      ut_print_message (1, 3, "Skipping `-pfsym orthotropic' with `-space ipf'...\n");
    else
      nev_print_pf_file_data_orthotropic (ppts, pptwgts, pptcells, pptqty);
  }

  if (!strcmp ((*pPf).space, "ipf"))
    nev_print_pf_file_data_crysym (*pPf, ppts, pptwgts, pptcells, pptqty);

  if (clustering)
    nev_print_pf_file_data_clustering (*pPf, ppts, pptwgts, pptcells, pptqty);

  // record cellptqty and cellpts

  int ptcell;

  for (i = 0; i < *pptqty; i++)
  {
    ptcell = (*pptcells)[i];

    cellptqty[ptcell]++;
    cellpts[ptcell] = ut_realloc_2d_addline (cellpts[ptcell],
                                             cellptqty[ptcell], 2);
    ut_array_1d_memcpy ((*ppts)[i], 2, cellpts[ptcell][cellptqty[ptcell] - 1]);
  }

  if (type != type_in)
    ut_free_1d_char (&type);
  if (data != data_in)
    ut_free_2d (&data, qty);

  return;
}

void
nev_print_pf_ptsprint (struct IN_V In, int input, struct PF *pPf, FILE *file,
                       char *basename, struct PRINT Print, struct DATA Data,
                       double **pts, double *ptwgts, int *ptcells, int ptqty,
                       int *pdensity_plotted)
{
  int i, modeqty;
  char **modes = NULL;

  ut_list_break ((*pPf).mode, NEUT_SEP_NODEP, &modes, &modeqty);

  for (i = 0; i < modeqty; i++)
  {
    if (!strcmp (modes[i], "symbol"))
    {
      ut_print_message (0, 2, "Computing symbols...\n");

      nev_print_pf_ptsprint_symbols (In, input, pPf, file, Data, pts, ptcells,
                                     ptqty);
    }

    else if (!strcmp (modes[i], "density"))
    {
      ut_print_message (0, 2, "Computing densities...\n");

      if (!(*pdensity_plotted))
      {
        nev_print_pf_ptsprint_density (In, *pPf, file, basename, Print, Data,
                                       pts, ptwgts, ptqty);
        (*pdensity_plotted) = 1;
      }
    }

    else
      ut_print_exprbug (modes[i]);
  }

  ut_free_2d_char (&modes, modeqty);

  return;
}

void
nev_print_pf_compress (struct PF Pf, double ***ppt,
                             double **pwgt, int *pqty)
{
  int i, qty;

  qty = 0;
  for (i = 0; i < (*pqty); i++)
    if ((*pwgt)[i] > 1e-12)
    {
      if (i != qty)
      {
	ut_array_1d_memcpy ((*ppt)[i], 2, (*ppt)[qty]);
	(*pwgt)[qty] = (*pwgt)[i];
      }
      qty++;
    }

  (*pwgt) = ut_realloc_1d (*pwgt, qty);

  for (i = *pqty - 1; i >= qty; i--)
    ut_free_1d ((*ppt) + i);
  (*ppt) = ut_realloc_1d_pdouble ((*ppt), qty);

  (*pqty) = qty;

  ut_print_message (0, 0, "number of data: %d\n",
                    !strcmp (Pf.space, "pf") ? qty : qty / 4);

  return;
}

void
nev_print_pf_polestring (struct PF Pf, struct DATA *pData, char **ppole_string)
{
  if (!strcmp (Pf.space, "pf"))
  {
    if (!pData || !(*pData).Value || !strcmp ((*pData).Value, "ori"))
    {
      char *tmp = ut_alloc_1d_char (1000);
      sprintf (tmp, "%s", Pf.pfpolestring);
      ut_string_fnrs (tmp, "-", "\\bar", INT_MAX);
      sprintf (*ppole_string, "$\\left\\{%s\\right\\}$", tmp);
      ut_string_fnrs (*ppole_string, NEUT_SEP_DEP, "", INT_MAX);
      ut_free_1d_char (&tmp);
    }
    else if (!strncmp ((*pData).Value, "vector", 6))
      ut_string_string ("directions", ppole_string);
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    if (!pData || !(*pData).Value || !strcmp ((*pData).Value, "ori"))
    {
      if (ut_num_equal (Pf.pfpoles[1][0], 1, 1e-3))
        sprintf (*ppole_string, "X direction");
      else if (Pf.pfpoles[1][1] == 1)
        sprintf (*ppole_string, "Y direction");
      else if (Pf.pfpoles[1][2] == 1)
        sprintf (*ppole_string, "Z direction");
      else
        sprintf (*ppole_string, "%s direction", Pf.pfpolestring);
    }
    else
      ut_string_string ("directions", ppole_string);
  }

  if (strcmp (Pf.pfpolelabel, "default"))
    ut_string_string (Pf.pfpolelabel, ppole_string);

  return;
}
