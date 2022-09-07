/* This id is part of the 'hermes' program. */
/* Copyright (C) 2007-2010, Romain Quey. */
/* See the COPYINPFG id in the top-level directory. */

#include "nev_print_pf_ptsprint_density_.h"

void
nev_print_pf_ptsprint_density_grid (struct PRINT Print, double ****pgrid,
                           double ***pdensity,
			   double ****pPgrid, double ****plgrid,
                           double ***pldensity)
{
  int i, j, gridsize;

  gridsize = Print.pfgridsize;
  (*pgrid) = ut_alloc_3d (gridsize, gridsize, 2);
  (*pdensity) = ut_alloc_2d (gridsize, gridsize);
  (*plgrid) = ut_alloc_3d (gridsize + 2, gridsize + 2, 2);
  (*pPgrid) = ut_alloc_3d (gridsize, gridsize, 3);
  (*pldensity) = ut_alloc_2d (gridsize + 2, gridsize + 2);

  if (!strcmp (Print.pfshape, "full"))
  {
    for (i = 0; i < gridsize; i++)
      for (j = 0; j < gridsize; j++)
      {
	(*pgrid)[i][j][0] = 2 * (double) (i + 0.5) / (double) (gridsize) - 1;
	(*pgrid)[i][j][1] = 2 * (double) (j + 0.5) / (double) (gridsize) - 1;
	if (ut_array_1d_norm ((*pgrid)[i][j], 2) <= 1)
	{
	  if (!strcmp (Print.pfprojection, "stereographic"))
	    ol_stprojxy_vect ((*pgrid)[i][j], (*pPgrid)[i][j]);
	  else if (!strcmp (Print.pfprojection, "equal-area"))
	      ol_eaprojxy_vect ((*pgrid)[i][j], (*pPgrid)[i][j]);
	  else
	    abort();
	}
      }

    for (i = 0; i < gridsize + 2; i++)
      for (j = 0; j < gridsize + 2; j++)
      {
	(*plgrid)[i][j][0] = 2 * (double) (i - 0.5) / (double) (gridsize) - 1;
	(*plgrid)[i][j][1] = 2 * (double) (j - 0.5) / (double) (gridsize) - 1;
      }
  }

  else if (!strcmp (Print.pfshape, "quarter"))
  {
    for (i = 0; i < gridsize; i++)
      for (j = 0; j < gridsize; j++)
      {
	(*pgrid)[i][j][0] = (double) (i + 0.5) / (double) (gridsize);
	(*pgrid)[i][j][1] = (double) (j + 0.5) / (double) (gridsize);
	if (ut_array_1d_norm ((*pgrid)[i][j], 2) <= 1)
	{
	  if (!strcmp (Print.pfprojection, "stereographic"))
	    ol_stprojxy_vect ((*pgrid)[i][j], (*pPgrid)[i][j]);
	  else if (!strcmp (Print.pfprojection, "equal-area"))
	      ol_eaprojxy_vect ((*pgrid)[i][j], (*pPgrid)[i][j]);
	  else
	    abort();
	}
      }

    for (i = 0; i < gridsize + 2; i++)
      for (j = 0; j < gridsize + 2; j++)
      {
	(*plgrid)[i][j][0] = (double) (i - 0.5) / (double) (gridsize);
	(*plgrid)[i][j][1] = (double) (j - 0.5) / (double) (gridsize);
      }
  }

  else if (!strcmp (Print.space, "ipf"))
  {
    double* v110 = ut_alloc_1d (3);
    double* p110 = ut_alloc_1d (3);
    ut_array_1d_set_3 (v110, 0, OL_IS2, OL_IS2);
    if (! strcmp (Print.pfprojection, "stereographic"))
      ol_vect_stprojxy (v110, p110);
    else if (! strcmp (Print.pfprojection, "equal-area"))
      ol_vect_eaprojxy (v110, p110);

    double* v111 = ut_alloc_1d (3);
    double* p111 = ut_alloc_1d (3);
    ut_array_1d_set_3 (v111, OL_IS3, OL_IS3, OL_IS3);
    if (! strcmp (Print.pfprojection, "stereographic"))
      ol_vect_stprojxy (v111, p111);
    else if (! strcmp (Print.pfprojection, "equal-area"))
      ol_vect_eaprojxy (v111, p111);

    for (i = 0; i < gridsize; i++)
      for (j = 0; j < gridsize; j++)
      {
	(*pgrid)[i][j][0] = p110[1] * (double) (i + 0.5) / (double) (gridsize);
	(*pgrid)[i][j][1] = p111[0] * (double) (j + 0.5) / (double) (gridsize);
	if (!strcmp (Print.pfprojection, "stereographic"))
	  ol_stprojxy_vect ((*pgrid)[i][j], (*pPgrid)[i][j]);
	else if (!strcmp (Print.pfprojection, "equal-area"))
	    ol_eaprojxy_vect ((*pgrid)[i][j], (*pPgrid)[i][j]);
	else
	  abort();
      }

    for (i = 0; i < gridsize + 2; i++)
      for (j = 0; j < gridsize + 2; j++)
      {
	(*plgrid)[i][j][0] = p110[1] * (double) (i - 0.5) / (double) (gridsize);
	(*plgrid)[i][j][1] = p111[0] * (double) (j - 0.5) / (double) (gridsize);
      }

    ut_free_1d (&v110);
    ut_free_1d (&p110);
    ut_free_1d (&v111);
    ut_free_1d (&p111);
  }

  return;
}

void
nev_print_pf_ptsprint_density_odf (struct PRINT Print, double ***grid, double ***Pgrid,
                          double **borderpts, int borderptqty, double **pos, double *wgt,
			  int pqty, double **density, double *pmin, double *pmax)
{
  int i;
  double mean, sum;
  double INPFVSIG2 = 1. / (2 * Print.pfkernelsig * Print.pfkernelsig);
  double INPFVSQRTSIG2PI = 1. / (Print.pfkernelsig * sqrt (2 * OL_PI));

#pragma omp parallel for private(i)
  for (i = 0; i < Print.pfgridsize; i++)
  {
    int j, k;
    double theta, *P = ol_vect_alloc ();
    for (j = 0; j < Print.pfgridsize; j++)
    {
      if (ut_array_1d_norm (grid[i][j], 2) <= 1)
	for (k = 0; k < pqty; k++)
	{
	  if (!strcmp (Print.pfprojection, "stereographic"))
	    ol_stprojxy_vect (pos[k], P);
	  else if (!strcmp (Print.pfprojection, "equal-area"))
	    ol_eaprojxy_vect (pos[k], P);
	  else
	    abort();

	  ol_vect_vect_theta (P, Pgrid[i][j], &theta);

	  if (theta > 90)
	    theta = 180 - theta;

          density[i][j] += INPFVSQRTSIG2PI * wgt[k]
                         * exp (-theta*theta * INPFVSIG2);
	}
    }

    ol_vect_free (P);
  }

  int qty = 0;
  sum = 0;
  if (!strcmp (Print.space, "pf"))
  {
#pragma omp parallel for private(i)
    for (i = 0; i < Print.pfgridsize; i++)
    {
      int j;

      for (j = 0; j < Print.pfgridsize; j++)
	if (ut_array_1d_norm (grid[i][j], 2) <= 1)
	{
#pragma omp critical
	  sum += density[i][j];
#pragma omp critical
	  qty++;
	}
    }
  }

  else if (!strcmp (Print.space, "ipf"))
  {
#pragma omp parallel for private(i)
    for (i = 0; i < Print.pfgridsize; i++)
    {
      int j;

      for (j = 0; j < Print.pfgridsize; j++)
      {
        double *tmp = ut_alloc_1d (2);

        // 2 next lines to confirm (signs)
        tmp[0] = grid[i][j][1];
        tmp[1] = -grid[i][j][0];
	if (ut_space_point_indomain_2d (grid[i][j], borderpts, borderptqty))
	{
#pragma omp critical
	  sum += density[i][j];
#pragma omp critical
	  qty++;
	}

        ut_free_1d (&tmp);
      }
    }
  }

  if (qty == 0)
    abort ();
  mean = sum / qty;

  // FIXME
  ut_array_2d_scale (density, Print.pfgridsize, Print.pfgridsize, 1. / mean);

  (*pmin) = ut_array_2d_min (density, Print.pfgridsize, Print.pfgridsize);
  (*pmax) = ut_array_2d_max (density, Print.pfgridsize, Print.pfgridsize);

  return;
}

void nev_print_pf_ptsprint_density_ldensity (struct PRINT Print, double **density, double ***lgrid,
                                    double **ldensity)
{
  int i, j;

  // copying density to a larger map to avoid edge effects

  for (i = 0; i < Print.pfgridsize; i++)
    ut_array_1d_memcpy (density[i], Print.pfgridsize, ldensity[i + 1] + 1);

  if (!strcmp (Print.pfshape, "full"))
    for (i = 0; i <= Print.pfgridsize + 1; i++)
      for (j = 0; j <= Print.pfgridsize + 1; j++)
      {
	double r = ut_array_1d_norm (lgrid[i][j], 2);
	double theta;

	theta = (180 / OL_PI) * atan2 (lgrid[i][j][0], lgrid[i][j][1]);
	if (r > 1 && r < 1 + 2. / Print.pfgridsize)
	{
	  if (theta >= -45 && theta <= 45)
	    ldensity[i][j] = ldensity[i][j - 1];
	  else if (theta >= 45 && theta <= 135)
	    ldensity[i][j] = ldensity[i - 1][j];
	  else if ((theta >= 135 && theta <= 180)
		|| (theta >= -180 && (theta <= -135)))
	    ldensity[i][j] = ldensity[i][j + 1];
	  else if (theta >= -135 && theta <= -45)
	    ldensity[i][j] = ldensity[i + 1][j];
	}
      }

  else if (!strcmp (Print.pfshape, "quarter"))
  {
    for (i = 0; i <= Print.pfgridsize + 1; i++)
      for (j = 0; j <= Print.pfgridsize + 1; j++)
      {
	double r = ut_array_1d_norm (lgrid[i][j], 2);
	double theta;

	theta = (180 / OL_PI) * atan2 (lgrid[i][j][0], lgrid[i][j][1]);
	if (r > 1 && r < 1 + 2. / Print.pfgridsize)
	{
	  if (theta <= 45)
	    ldensity[i][j] = ldensity[i][j - 1];
	  else
	    ldensity[i][j] = ldensity[i - 1][j];
	}
      }

    for (j = 0; j <= Print.pfgridsize + 1; j++)
      ldensity[0][j] = ldensity[1][j];
    for (i = 0; i <= Print.pfgridsize + 1; i++)
      ldensity[i][0] = ldensity[i][1];
    ldensity[0][0] = ldensity[1][1];
  }

  else if (!strcmp (Print.space, "ipf"))
  {
    for (j = 0; j <= Print.pfgridsize + 1; j++)
    {
      ldensity[0][j] = ldensity[1][j];
      ldensity[Print.pfgridsize + 1][j] = ldensity[Print.pfgridsize][j];
    }
    for (i = 0; i <= Print.pfgridsize + 1; i++)
    {
      ldensity[i][0] = ldensity[i][1];
      ldensity[i][Print.pfgridsize + 1] = ldensity[i][Print.pfgridsize];
    }
  }

  return;
}

void
nev_print_pf_ptsprint_density_write (FILE *file, char *basename, struct PRINT Print,
                            struct DATA Data, double ***lgrid, double **ldensity,
                            double min, double max)
{
  int i, partqty = 0;
  char **parts = NULL;
  int qty = 0;
  char **tmp = NULL;
  int tickqty;
  double *tickvals = NULL;
  char **ticks = NULL;
  char *tickformat = NULL;
  char *filename2 = ut_string_addextension (basename, ".level");

  nev_print_pf_ptsprint_density_write_data (filename2, Print, lgrid, ldensity);

  nev_print_pf_ptsprint_density_write_text (file, filename2, Print);

  nev_print_scale_ticks (Data.Scale, &min, &max, &ticks, &tickvals, &tickqty, &tickformat);

  if (Data.Scale)
  {
    fprintf (file, "v[0][0] = %f;\n", min);
    fprintf (file, "for (int i = 0; i < n0; ++i)\n");
    fprintf (file, "  for (int j = 0; j < n0; ++j)\n");
    fprintf (file, "    if (v[i][j] < v[0][0])\n");
    fprintf (file, "      v[i][j] = v[0][0];\n");

    fprintf (file, "v[0][1] = %f;\n", max);
    fprintf (file, "for (int i = 0; i < n0; ++i)\n");
    fprintf (file, "  for (int j = 0; j < n0; ++j)\n");
    fprintf (file, "    if (v[i][j] > v[0][1])\n");
    fprintf (file, "      v[i][j] = v[0][1];\n");
  }

  fprintf (file, "real[][] vt=new real[n0][n0];\n");
  fprintf (file, "for (int i = 0; i < n0; ++i)\n");
  fprintf (file, "  for (int j = 0; j < n0; ++j)\n");
  fprintf (file, "  {\n");
  fprintf (file, "    vt[j][i] = v[i][j];\n");
  fprintf (file, "  }\n");
  fprintf (file, "\n");

  char *asygradient = NULL;

  // do not use much higher values (e.g., 1000), as it will cause errors in the
  // PDF (at least when opened with mupdf - "too large ...")
  neut_data_colscheme_asygradient (Data.ColScheme, 100, &asygradient);

  fprintf (file, "pen[] Palette=%s;\n", asygradient);

  ut_free_1d_char (&asygradient);

  double px = 1. / Print.pfgridsize;

  if (!strcmp (Print.pfshape, "full"))
    fprintf (file, "bounds range=image(v,scale*%f*(-1,-1),scale*%f*(1,1),Palette);\n",
	     1 + 2 * px, 1 + 2 * px);
    // fprintf (file, "bounds range=image(v,scale*1.035*(-1,-1),scale*1.035*(1,1),Palette);\n");
  else if (!strcmp (Print.pfshape, "quarter"))
    fprintf (file, "bounds range=image(vt,scale*(%f,%f),scale*(%f,%f),Palette);\n",
	-px, px, 1 + px, -1 - px);
  else if (!strcmp (Print.space, "ipf"))
  {
    double* v110 = ut_alloc_1d (3);
    double* p110 = ut_alloc_1d (3);
    double* v111 = ut_alloc_1d (3);
    double* p111 = ut_alloc_1d (3);

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
    fprintf (file, "bounds range=image(v,scale*(%f,%f),scale*(%f,%f),Palette);\n",
	     1. + px, -p111[0] / p110[1] * px, -px, p111[0] / p110[1] + p111[0] / p110[1] * px);

    ut_free_1d (&v110);
    ut_free_1d (&p110);
    ut_free_1d (&v111);
    ut_free_1d (&p111);
  }

  double y;
  if (strcmp (Print.space, "ipf") != 0)
    y = 0;
  else
    y = 2;

  fprintf (file, "%spicture bar;\n", Print.showscale ? "" : "// ");

  fprintf (file, "%spalette(bar,\"%s\",range,(0cm,0cm),(0.5cm,6cm),Right,Palette,\n",
           Print.showscale ? "" : "// ", Data.ScaleTitle ? Data.ScaleTitle : "Density");

  ut_list_break (Data.Scale, ":", &parts, &partqty);

  if (Print.showscale)
  {
    if (Data.Scale)
      fprintf (file, "        PaletteTicks(N=%d,n=1,trailingzero));\n", tickqty - 1);
    else
      fprintf (file, "        PaletteTicks(N=%d,n=1,\"$%s$\"));\n", tickqty - 1,
               tickformat);
  }

  if (!strcmp (Print.pfshape, "full"))
    fprintf (file, "%sadd(bar.fit(),point(E),W+%fS);\n", Print.showscale ? "" : "// ", y);

  else if (!strcmp (Print.pfshape, "quarter"))
    fprintf (file, "%sadd(bar.fit(),point(E),W+%fS);\n", Print.showscale ? "" : "// ", y);

  if (!strcmp (Print.pfshape, "full"))
    fprintf (file, "draw (contour (points, values, new real[] {");

  else if (!strcmp (Print.pfshape, "quarter"))
    fprintf (file, "draw (contour (scale(1,-1)*points, values, new real[] {");

  else
  {
    double* v = ut_alloc_1d (3);
    double* p110 = ol_p_alloc ();
    ut_array_1d_set_3 (v, 0, OL_IS2, OL_IS2);

    if (! strcmp (Print.pfprojection, "stereographic"))
      ol_vect_stprojxy (v, p110);
    else if (! strcmp (Print.pfprojection, "equal-area"))
      ol_vect_eaprojxy (v, p110);

    fprintf (file, "draw (contour (scale(1/%f)*points, values, new real[] {",
	     p110[1]);

    ut_free_1d (&p110);
    ut_free_1d (&v);
  }

  for (i = 1; i < tickqty; i++)
    fprintf (file, "%.12f,", tickvals[i]);
  fprintf (file, "}, operator --), new pen[] {");
  for (i = 1; i < tickqty - 1; i++)
    fprintf (file, "%s+linewidth(%f),", "black", 0.5);
  fprintf (file, "%s+linewidth(%f)});\n", "black", 0.5);

  ut_free_2d_char (&parts, partqty);
  ut_free_2d_char (&tmp, qty);
  ut_free_2d_char (&ticks, tickqty);
  ut_free_1d (&tickvals);
  ut_free_1d_char (&tickformat);
  ut_free_1d_char (&filename2);

  return;
}

void
nev_print_pf_ptsprint_density_write_text0 (FILE *file, struct PRINT Print)
{
  if (Print.space[0] == 'f')
  {
    fprintf (file,
      "label(\"\\begin{tabular}{r@{}}Gaussian smoothing: $%g^{\\circ}$\\\\", Print.pfkernelsig);
  }
  else if (Print.space[0] == 'q')
  {
    fprintf (file,
      "label(\"\\begin{tabular}{r@{}}%s\\\\Gaussian smoothing: $%g^{\\circ}$\\\\",
      !strcmp (Print.pfprojection, "stereographic") ? "stereo. proj." : "equal-area proj.",
      Print.pfkernelsig);
    fprintf (file,
	     "\\end{tabular}\", scale(scale)*(%.3f,%.3f), NW, fontsize(8));\n",
	    1., -1 - SMARGINPF * 0.465);
  }

  else if (Print.space[0] == 'i')
  {
    char *legp = ut_alloc_1d_char (1000);
    sprintf (legp, "$\\!\\left\\{%d%d%d\\right\\}$",
             Print.pfpoles[0][0], Print.pfpoles[0][1], Print.pfpoles[0][2]);

    fprintf (file,
      "label(\"\\begin{tabular}{l}%s\\\\%s\\\\Gaussian smoothing: $%g^{\\circ}$\\\\",
      legp, !strcmp (Print.pfprojection, "stereographic") ? "stereo. proj." : "equal-area proj.",
      Print.pfkernelsig);

    ut_free_1d_char (&legp);

    fprintf (file,
	     "\\end{tabular}\", scale(scale)*(%.3f,%.3f), SE, fontsize(8));\n",
	     - WMARGINPF / 2, 1.);
  }

  return;
}

void
nev_print_pf_ptsprint_density_write_mask (FILE *file, struct PRINT Print, double **borderpt, int borderptqty)
{
  char *color = NULL;

  ut_string_string (Print.scenebackground ? Print.scenebackground : "white", &color);

  if (!strcmp (Print.pfshape, "full"))
  {
    fprintf (file, "draw (scale(scale)*(-X--X), black);\n");
    fprintf (file, "draw (scale(scale)*(-Y--Y), black);\n");

    fprintf (file, "path g = shift(O)*scale(scale*1.15)*((-X-Y)--(-X+Y)--(X+Y)--(X-Y)--cycle);\n");
    fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*unitcircle^^g, evenodd+%s, %s);\n", color, color);
    fprintf (file, "draw (shift(O)*scale(scale)*unitcircle, black);\n");
  }

  else if (!strcmp (Print.pfshape, "quarter"))
  {
    fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*((-.05*X)--(-.05*X+.05*Y)--(1.05*X+0.05*Y)--(1.05*X)--cycle), evenodd+%s, %s);\n", color, color);
    fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*((.05*Y)--(.05*Y-.05*X)--(-1.05*Y-0.05*X)--(-1.05*Y)--cycle), evenodd+%s, %s);\n", color, color);
    fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*(arc(O,-Y,X)--(1.05*X)--(1.05*X-1.05*Y)--(-1.05*Y)--cycle), evenodd+%s, %s);\n", color, color);
    fprintf (file, "draw (shift(O)*scale(scale)*arc(O,-Y,X), black);\n");
    fprintf (file, "draw (shift(O)*scale(scale)*(-Y--O--X), black);\n");
  }

  else if (!strcmp (Print.space, "ipf"))
  {
    fprintf (file,
	"filldraw(shift(O)*scale(scale/%f)*(scale(1+1e-3)*line110111--(%f,%f)--(%f,%f)--(%f,%f)--cycle), evenodd+%s, %s);\n", borderpt[1][1],
	-borderpt[borderptqty - 1][0], borderpt[borderptqty - 1][1] + 0.03,
	-borderpt[borderptqty - 1][0] + 0.1, borderpt[borderptqty - 1][1] + 0.03,
	-borderpt[borderptqty - 1][0] + 0.1, 0., color, color);
    fprintf (file, "filldraw(scale(scale)*((-0.05,-0.05)--(-0.05,1.05)--(1.05,1.05)--(0,0)--(1.05,0)--(1.05,-0.05)--cycle), %s, %s);\n", color, color);
    fprintf (file, "draw (scale(scale/%f)*border, black);\n", borderpt[1][1]);
  }

  ut_free_1d_char (&color);

  return;
}
