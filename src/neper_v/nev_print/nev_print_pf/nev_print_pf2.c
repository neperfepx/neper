/* This file is part of the 'hermes' program. */
/* Copyright (C) 2007-2010, Romain Quey. */
/* See the COPYINPFG file in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf_header (FILE *file, struct PRINT Print)
{
  int i;

  fprintf (file, "import graph;\n");
  fprintf (file, "import palette;\n");
  fprintf (file, "import contour;\n");
  // fprintf (file, "settings.outformat=\"pdf\";\n");
  if (!strcmp (Print.pffont, "TimesRoman"))
    fprintf (file, "usepackage(\"mathptmx\"); defaultpen(TimesRoman());\n");
  else if (!strcmp (Print.pffont, "ComputerModern"))
  { }
  else
    ut_print_message (1, 0, "Font symbol unknown (ignoring)\n");

  fprintf (file, "unitsize(1mm);\n");
  fprintf (file, "\n");
  if (!strcmp (Print.pfshape, "full"))
    fprintf (file, "real scale = 30;\n");
  else if (!strcmp (Print.pfshape, "quarter"))
    fprintf (file, "real scale = 60;\n");
  else if (!strcmp (Print.space, "ipf"))
    fprintf (file, "real scale = 60;\n");
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
nev_print_pf_background (FILE *file, struct PRINT Print)
{
  double xmin = 0, xmax = 0, ymin = 0, ymax = 0;
  int density = ut_list_testelt (Print.pfmode, NEUT_SEP_NODEP, "density");

  if (!strcmp (Print.pfshape, "full"))
  {
    xmin = -1 - WMARGINPF;
    xmax =  1 + (!density || !Print.showscale ? EMARGINPF : 1.10);
    ymin = -1 - SMARGINPF;
    ymax =  1 + NMARGINPF;
  }
  else if (!strcmp (Print.pfshape, "quarter"))
  {
    xmin =  0 - WMARGINPF / 2;
    xmax =  1 + (!density || !Print.showscale ? EMARGINPF / 2 : 0.55);
    ymin =  0 + NMARGINPF;
    ymax = -1 - SMARGINPF / 2;
  }
  else if (!strcmp (Print.space, "ipf"))
  {
    xmin =  0 - WMARGINPF / 2;
    xmax =  1 + EMARGINPF / 2;
    ymin =  0 + NMARGINPF / 2;
    ymax =  1;
  }
  else
    abort ();

  fprintf (file, "filldraw (scale(scale)*((%.3f,%.3f)--(%.3f,%.3f)--(%.3f,%.3f)--(%.3f,%.3f)--cycle),%s,%s);\n", xmin, ymin, xmax, ymin, xmax, ymax, xmin, ymax,
      Print.scenebackground ? Print.scenebackground : "white",
      Print.scenebackground ? Print.scenebackground : "white");

  if (!ut_list_testelt (Print.pfmode, NEUT_SEP_NODEP, "density"))
  {
    if (!strcmp (Print.pfshape, "full"))
    {
      fprintf (file, "draw (shift(O)*scale(scale)*(-X--X), black);\n");
      fprintf (file, "draw (shift(O)*scale(scale)*(-Y--Y), black);\n");
    }
    else if (!strcmp (Print.pfshape, "quarter"))
    {
      fprintf (file, "draw (shift(O)*scale(scale)*(O--X), black);\n");
      fprintf (file, "draw (shift(O)*scale(scale)*(O--(-Y)), black);\n");
    }
  }

  return;
}

void
nev_print_pf_border (FILE *file, struct PRINT Print, double ***pipfpts, int *pipfptqty)
{
  if (!strcmp (Print.pfshape, "full"))
  {
    fprintf (file, "draw (shift(O)*scale(scale)*unitcircle, black);\n");
    (*pipfptqty) = 0;
  }
  else if (!strcmp (Print.pfshape, "quarter"))
  {
    fprintf (file, "draw (shift(O)*scale(scale)*arc(O,-Y,X), black);\n");
    (*pipfptqty) = 0;
  }
  else if (!strcmp (Print.space, "ipf"))
  {
    int i;
    double* r = ol_r_alloc ();
    double** g = ol_g_alloc ();
    double* v110 = ut_alloc_1d (3);
    double* v111 = ut_alloc_1d (3);
    double* v = ut_alloc_1d (3);
    double* p = ol_p_alloc ();
    double* p110 = ol_p_alloc ();
    double* p111 = ol_p_alloc ();
    double dtheta;

    ut_array_1d_set_3 (v110, 0, OL_IS2, OL_IS2);
    if (! strcmp (Print.pfprojection, "stereographic"))
      ol_vect_stprojxy (v110, p110);
    else if (! strcmp (Print.pfprojection, "equal-area"))
      ol_vect_eaprojxy (v110, p110);

    ut_array_1d_set_3 (v111, OL_IS3, OL_IS3, OL_IS3);
    if (! strcmp (Print.pfprojection, "stereographic"))
      ol_vect_stprojxy (v111, p111);
    else if (! strcmp (Print.pfprojection, "equal-area"))
      ol_vect_eaprojxy (v111, p111);

    ol_r_set_this (r, 0, OL_IS2, -OL_IS2);
    dtheta = 35.26439 / 35;
    ol_rtheta_g (r, dtheta, g);
    // fprintf (file, "draw (shift(O)*scale(scale/%f)*(O--(%f,%f)", p[1], p[1], -p[0]);
    (*pipfptqty) = 37;
    (*pipfpts) = ut_alloc_2d (*pipfptqty, 2);
    for (i = 0; i <= 35; i++)
    {
      ol_rtheta_g (r, i * dtheta, g);
      ol_g_vect_vect (g, v110, v);
      if (! strcmp (Print.pfprojection, "stereographic"))
	ol_vect_stprojxy (v, (*pipfpts)[i + 1]);
      else if (! strcmp (Print.pfprojection, "equal-area"))
	ol_vect_eaprojxy (v, (*pipfpts)[i + 1]);
    }

    // this is needed because of the linear approximation of the curved
    // 011-111 line
    ut_array_2d_scale (*pipfpts, *pipfptqty, 2, 1 + 1e-3);

    fprintf (file, "path border = ");
    for (i = 0; i < (*pipfptqty); i++)
      fprintf (file, "(%f,%f)--", (*pipfpts)[i][1], -(*pipfpts)[i][0]);
    fprintf (file, "cycle;\n");

    fprintf (file, "path line110111 = ");
    for (i = 1; i < *pipfptqty; i++)
      fprintf (file, "(%f,%f)%s", (*pipfpts)[i][1], -(*pipfpts)[i][0],
	       i < (*pipfptqty) - 1 ? "--" : ";\n");

    fprintf (file, "draw (shift(O)*scale(scale/%f)*border,black);\n",
	     p110[1]);

    ut_free_1d (&v110);
    ut_free_1d (&v111);
    ut_free_1d (&v);
    ol_p_free (p);
    ol_p_free (p110);
    ol_p_free (p111);
    ol_r_free (r);
    ol_g_free (g);
  }
  else
    abort ();

  return;
}

void
nev_print_pf_pole_proj (FILE *file, struct PRINT Print, char *pole_string)
{
  double xmin = 0, y, xshift;
  char *legp = ut_alloc_1d_char (100);

  if (!strcmp (Print.pfshape, "full"))
  {
    xmin = -1 - WMARGINPF;
    xshift = 0.01;
    y = 1;
  }
  else if (!strcmp (Print.pfshape, "quarter"))
  {
    xmin = - WMARGINPF / 2;
    xshift = 0.005;
    y = 0.1;
  }
  else if (!strcmp (Print.space, "ipf"))
  {
    xmin =  - WMARGINPF / 2;
    xshift = 0;
  }
  else
    abort ();

  sprintf (legp, "%s", pole_string);
  fprintf (file, "label(\"%s\", scale(scale)*(%.3f, %.3f), NE);\n", legp, xmin, y);
  ut_free_1d_char (&legp);
  fprintf (file, "label(\"%s\", scale(scale)*(%.3f, %.3f), SE, fontsize(8));\n",
      !strcmp (Print.pfprojection, "stereographic") ? "stereo. proj." : "equal-area proj.",
      xmin + xshift, y);

  ut_free_1d_char (&legp);

  return;
}

void
nev_print_pf_csys (FILE *file, struct PRINT Print, struct DATA CsysData)
{
  int id;
  double* v = ol_r_alloc ();
  double* p110 = ol_p_alloc ();
  double* p111 = ol_p_alloc ();

  ut_array_1d_set_3 (v, 0, OL_IS2, OL_IS2);
  if (! strcmp (Print.pfprojection, "stereographic"))
    ol_vect_stprojxy (v, p110);
  else if (! strcmp (Print.pfprojection, "equal-area"))
    ol_vect_eaprojxy (v, p110);

  ut_array_1d_set_3 (v, OL_IS3, OL_IS3, OL_IS3);
  if (! strcmp (Print.pfprojection, "stereographic"))
    ol_vect_stprojxy (v, p111);
  else if (! strcmp (Print.pfprojection, "equal-area"))
    ol_vect_eaprojxy (v, p111);

  if (!strcmp (Print.pfshape, "full"))
  {
    id = ut_array_1d_absmax_index (Print.pfdir[0], 3);
    fprintf (file, "label(\"%s\", scale(scale)*(%d,0), %s);\n",
             CsysData.Label[id],
             ut_num_sgn (Print.pfdir[0][id]), Print.pfdir[0][id] > 0 ? "E" : "W");
    id = ut_array_1d_absmax_index (Print.pfdir[1], 3);
    fprintf (file, "label(\"%s\", scale(scale)*(0,%d), %s);\n",
             CsysData.Label[id],
             ut_num_sgn (Print.pfdir[1][id]), Print.pfdir[1][id] > 0 ? "N" : "S");
    // fprintf (file, "label(\"$%s$\", scale(scale)*( 0,0), NE);\n", " ");

  }
  else if (!strcmp (Print.pfshape, "quarter"))
  {
    id = ut_array_1d_absmax_index (Print.pfdir[0], 3);
    fprintf (file, "label(\"%s\", scale(scale)*( 1,0), E);\n", CsysData.Label[id]);
    id = ut_array_1d_absmax_index (Print.pfdir[1], 3);
    fprintf (file, "label(\"%s\", scale(scale)*(0,-1), S);\n", CsysData.Label[id]);
    id = ut_array_1d_absmax_index (Print.pfdir[2], 3);
    fprintf (file, "label(\"%s\", scale(scale)*( 0,0), W);\n", CsysData.Label[id]);
  }
  else if (!strcmp (Print.space, "ipf"))
  {
    fprintf (file, "label(\"$[1\\,0\\,0]$\", scale(scale)*O, S);\n");
    fprintf (file, "label(\"$[1\\,1\\,0]$\", scale(scale)*X, S);\n");
    fprintf (file, "label(\"$[1\\,1\\,1]$\", scale(scale/%f)*(%f,%f), N);\n",
	     p110[1], p111[1], p111[1]);
  }

  ol_r_free (v);
  ol_p_free (p111);

  return;
}

void
nev_print_pf_pts (char *type_in, double **data_in, double *weights,
                  int qty, int *show, struct PRINT Print, char **modes,
                  int modeqty, double ***ppts,
                  double **pptwgts, int **pptcells, int *pptqty)
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
    nev_print_pf_file_data_ori (data, weights, qty, show,
                                Print, ppts, pptwgts, pptcells, pptqty);

  else if (!strcmp (type, "vector"))
    nev_print_pf_file_data_vect (data_in, weights, qty, show,
                                 Print, ppts, pptwgts, pptcells, pptqty);

  else
    ut_print_exprbug (type);

  if (ut_num_equal (ut_array_1d_min (Print.pfdir[2], 3), -1, 1e-6))
    ut_array_2d_scale (*ppts, *pptqty, 2, -1);

  if (!strcmp (Print.pfsym, "orthotropic"))
    nev_print_pf_file_data_orthotropic (ppts, pptwgts, pptcells, pptqty);

  if (!strcmp (Print.space, "ipf"))
    nev_print_pf_file_data_crysym (Print, ppts, pptwgts, pptqty);

  if (!strcmp (Print.pfshape, "quarter") && strcmp (Print.pfsym, "orthotropic"))
    nev_print_pf_file_data_orthotropic (ppts, pptwgts, pptcells, pptqty);

  if (Print.pfclustering)
    for (i = 0; i < modeqty; i++)
      if (!strcmp (modes[i], "density"))
      {
        nev_print_pf_file_data_clustering (Print, ppts, pptwgts, pptcells, pptqty);
        break;
      }

  if (type != type_in)
    ut_free_1d_char (&type);
  if (data != data_in)
    ut_free_2d (&data, qty);

  return;
}

void
nev_print_pf_ptsprint (FILE *file, char *basename, struct PRINT Print,
                       struct DATA Data, double **ipfpts, int ipfptqty,
                       double **pts, double *ptwgts, int *ptcells, int ptqty, char **modes,
                       int modeqty, int *pdensity_plotted)
{
  int i;

  for (i = 0; i < modeqty; i++)
  {
    if (!strcmp (modes[i], "symbol"))
    {
      ut_print_message (0, 2, "Computing symbols...\n");

      nev_print_pf_ptsprint_symbols (file, Print, Data, ipfpts, ipfptqty, pts, ptcells, ptqty);
    }

    else if (!strcmp (modes[i], "density"))
    {
      ut_print_message (0, 2, "Computing densities...\n");

      if (!(*pdensity_plotted))
      {
        nev_print_pf_ptsprint_density (file, basename, Print, Data, ipfpts, ipfptqty,
                              pts, ptwgts, ptqty);
        (*pdensity_plotted) = 1;
      }
    }

    else
      ut_print_exprbug (modes[i]);
  }

  return;
}

void
nev_print_pf_compress (struct PRINT Print, double ***ppt,
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
                    !strcmp (Print.space, "pf") ? qty : qty / 4);

  return;
}
