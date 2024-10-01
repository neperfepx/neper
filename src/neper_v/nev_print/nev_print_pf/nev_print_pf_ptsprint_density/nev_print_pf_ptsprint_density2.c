/* This id is part of the 'hermes' program. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYINPFG id in the top-level directory. */

#include "nev_print_pf_ptsprint_density_.h"

void
nev_print_pf_ptsprint_density_grid (struct PF Pf,
                           double ***pdensity,
                           double ***pldensity)
{
  (*pdensity) = ut_alloc_2d (Pf.gridsize[0], Pf.gridsize[1]);
  (*pldensity) = ut_alloc_2d (Pf.gridsize[0] + 2, Pf.gridsize[1] + 2);

  return;
}

void
nev_print_pf_ptsprint_density_odf (struct PF Pf,
                          double **pos, double *wgt,
			  int pqty, double **density, double *pmin, double *pmax)
{
  int i, j;
  double mean, sum, weightsum;
  double INPFVSIG2 = 1. / (2 * Pf.pfkernelsig * Pf.pfkernelsig);
  double INPFVSQRTSIG2PI = 1. / (Pf.pfkernelsig * sqrt (2 * OL_PI));

  ut_array_2d_set (density, Pf.gridsize[0], Pf.gridsize[1], 0);

#pragma omp parallel for private(i)
  for (i = 0; i < Pf.gridsize[0]; i++)
  {
    int j, k;
    double theta, *P = ol_vect_alloc ();
    for (j = 0; j < Pf.gridsize[1]; j++)
    {
      if (ut_array_1d_norm (Pf.grid[i][j], 2) <= 1)
	for (k = 0; k < pqty; k++)
	{
	  if (!strcmp (Pf.projection, "stereographic"))
	    ol_stprojxy_vect (pos[k], P);
	  else if (!strcmp (Pf.projection, "equal-area"))
	    ol_eaprojxy_vect (pos[k], P);
	  else
	    abort();

          ol_vect_vect_theta (P, Pf.Pgrid[i][j], &theta);

	  if (theta > 90)
	    theta = 180 - theta;

          density[i][j] += INPFVSQRTSIG2PI * wgt[k]
                         * exp (-theta*theta * INPFVSIG2);
	}
    }

    ol_vect_free (P);
  }

  sum = 0;
  if (!strcmp (Pf.space, "pf"))
  {
    weightsum = 0;
#pragma omp parallel for private(i)
    for (i = 0; i < Pf.gridsize[0]; i++)
    {
      int j;
      double norm, weight;

      for (j = 0; j < Pf.gridsize[1]; j++)
      {
	norm = ut_array_1d_norm (Pf.grid[i][j], 2);
	if (norm <= 1)
	{
          if (!strcmp (Pf.projection, "stereographic"))
            weight = 1 / pow (1 + norm * norm, 2);
          else if (!strcmp (Pf.projection, "equal-area"))
            weight = 1;
          else
            abort ();
#pragma omp critical
	  sum += weight * density[i][j];
#pragma omp critical
	  weightsum += weight;
	}
      }
    }
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    weightsum = 0;
#pragma omp parallel for private(i)
    for (i = 0; i < Pf.gridsize[0]; i++)
    {
      int j;
      double norm, weight;

      for (j = 0; j < Pf.gridsize[1]; j++)
	if (ut_space_point_indomain_2d (Pf.grid[i][j], Pf.domainpts, Pf.domainptqty))
        {
          norm = ut_array_1d_norm (Pf.grid[i][j], 2);
          if (!strcmp (Pf.projection, "stereographic"))
            weight = 1 / pow (1 + norm * norm, 2);
          else if (!strcmp (Pf.projection, "equal-area"))
            weight = 1;
          else
            abort ();
#pragma omp critical
	  sum += weight * density[i][j];
#pragma omp critical
	  weightsum += weight;
	}
    }
  }

  else
    abort ();

  if (weightsum == 0)
    abort ();
  mean = sum / weightsum;

  ut_array_2d_scale (density, Pf.gridsize[0], Pf.gridsize[1], 1. / mean);

  for (i = 0; i < Pf.gridsize[0]; i++)
    for (j = 0; j < Pf.gridsize[1]; j++)
      if (density[i][j] == 0)
        density[i][j] = 1;

  if (!strcmp (Pf.refsym, "uniaxial"))
  {
    int qty = Pf.gridsize[0] * Pf.gridsize[1];
    double **density2 = ut_alloc_2d (Pf.gridsize[0], Pf.gridsize[1]);
    int *ids = ut_alloc_1d_int (qty);
    double *vals = ut_alloc_1d (qty);
    double *norm = ut_alloc_1d (qty);

    ut_array_2d_memcpy (density, Pf.gridsize[0], Pf.gridsize[1], density2);

    int id = 0;
    for (i = 0; i < Pf.gridsize[0]; i++)
      for (j = 0; j < Pf.gridsize[1]; j++)
      {
	norm[id] = ut_array_1d_norm (Pf.grid[i][j], 2);
	vals[id] = density[i][j];
        id++;
      }

    double sig = (2. / Pf.gridsize[0]);

#pragma omp parallel for private(i, j, id) schedule(dynamic)
    for (i = 0; i < Pf.gridsize[0]; i++)
      for (j = 0; j < Pf.gridsize[1]; j++)
      {
        int id0 = i * Pf.gridsize[0] + j;
        double d;
	if (norm[id0] <= 1)
        {
          double *ws = ut_alloc_1d (qty);
          for (id = 0; id < qty; id++)
            if (norm[id] <= 1)
            {
              d = fabs (norm[id0] - norm[id]);
              ws[id] = exp (- d * d / (2 * sig * sig));
            }

          density2[i][j] = ut_array_1d_wmean (vals, ws, qty);
          ut_free_1d (&ws);
        }
      }

    ut_array_2d_memcpy (density2, Pf.gridsize[0], Pf.gridsize[1], density);

    ut_free_1d_int (&ids);
    ut_free_1d (&vals);
    ut_free_2d (&density2, Pf.gridsize[0]);
    ut_free_1d (&norm);
  }

  (*pmin) = ut_array_2d_min (density, Pf.gridsize[0], Pf.gridsize[1]);
  (*pmax) = ut_array_2d_max (density, Pf.gridsize[0], Pf.gridsize[1]);

  return;
}

void
nev_print_pf_ptsprint_density_ldensity (struct PF Pf, double **density,
                                    double **ldensity)
{
  int i, j;

  // copying density to a larger map to avoid edge effects

  for (i = 0; i < Pf.gridsize[0]; i++)
    ut_array_1d_memcpy (density[i], Pf.gridsize[1], ldensity[i + 1] + 1);

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
      for (i = 0; i <= Pf.gridsize[0] + 1; i++)
        for (j = 0; j <= Pf.gridsize[1] + 1; j++)
        {
          double r = ut_array_1d_norm (Pf.lgrid[i][j], 2);
          double theta;

          theta = (180 / OL_PI) * atan2 (Pf.lgrid[i][j][0], Pf.lgrid[i][j][1]);
          if (r > 1 && r < 1 + 2. / sqrt (Pf.gridsize[0] * Pf.gridsize[1]))
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

    else if (!strcmp (Pf.shape, "quarter"))
    {
      for (i = 0; i <= Pf.gridsize[0] + 1; i++)
        for (j = 0; j <= Pf.gridsize[1] + 1; j++)
        {
          double r = ut_array_1d_norm (Pf.lgrid[i][j], 2);
          double theta;

          theta = (180 / OL_PI) * atan2 (Pf.lgrid[i][j][0], Pf.lgrid[i][j][1]);
          if (r > 1 && r < 1 + 2. / sqrt (Pf.gridsize[0] * Pf.gridsize[1]))
          {
            if (theta <= 45)
              ldensity[i][j] = ldensity[i][j - 1];
            else
              ldensity[i][j] = ldensity[i - 1][j];
          }
        }

      for (j = 0; j <= Pf.gridsize[1] + 1; j++)
        ldensity[0][j] = ldensity[1][j];
      for (i = 0; i <= Pf.gridsize[0] + 1; i++)
        ldensity[i][0] = ldensity[i][1];
      ldensity[0][0] = ldensity[1][1];
    }
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    for (j = 0; j <= Pf.gridsize[1] + 1; j++)
    {
      ldensity[0][j] = ldensity[1][j];
      ldensity[Pf.gridsize[0] + 1][j] = ldensity[Pf.gridsize[0]][j];
    }
    for (i = 0; i <= Pf.gridsize[0] + 1; i++)
    {
      ldensity[i][0] = ldensity[i][1];
      ldensity[i][Pf.gridsize[1] + 1] = ldensity[i][Pf.gridsize[1]];
    }

    if (!strcmp (Pf.crysym, "hexagonal"))
      for (j = 0; j <= Pf.gridsize[1] + 1; j++)
        for (i = 1; i <= Pf.gridsize[0] + 1; i++)
          if (ut_array_1d_norm (Pf.lgrid[i][j], 2) > 1)
            ldensity[i][j] = ldensity[i - 1][j];
  }

  else
    abort ();

  return;
}

void
nev_print_pf_ptsprint_density_write (struct PF Pf, struct PRINT Print, FILE *file, char *basename,
                            struct DATA Data, double **ldensity,
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

  nev_print_pf_ptsprint_density_write_data (Pf, filename2, ldensity);

  nev_print_pf_ptsprint_density_write_text (Pf, file, filename2);

  nev_print_scale_ticks (Data.Scale, &min, &max, &ticks, &tickvals, &tickqty, &tickformat);

  if (Data.Scale)
  {
    fprintf (file, "v[0][0] = %f;\n", min);
    fprintf (file, "for (int i = 0; i < n0; ++i)\n");
    fprintf (file, "  for (int j = 0; j < n1; ++j)\n");
    fprintf (file, "    if (v[i][j] < v[0][0])\n");
    fprintf (file, "      v[i][j] = v[0][0];\n");

    fprintf (file, "v[0][1] = %f;\n", max);
    fprintf (file, "for (int i = 0; i < n0; ++i)\n");
    fprintf (file, "  for (int j = 0; j < n1; ++j)\n");
    fprintf (file, "    if (v[i][j] > v[0][1])\n");
    fprintf (file, "      v[i][j] = v[0][1];\n");
  }

  fprintf (file, "real[][] vt=new real[n0][n1];\n");
  fprintf (file, "for (int i = 0; i < n0; ++i)\n");
  fprintf (file, "  for (int j = 0; j < n1; ++j)\n");
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

  double px = 1. / Pf.gridsize[0];

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
      fprintf (file, "bounds range=image(v,scale*%f*(-1,-1),scale*%f*(1,1),Palette);\n",
               1 + 2 * px, 1 + 2 * px);
      // fprintf (file, "bounds range=image(v,scale*1.035*(-1,-1),scale*1.035*(1,1),Palette);\n");
    else if (!strcmp (Pf.shape, "quarter"))
      fprintf (file, "bounds range=image(vt,scale*(%f,%f),scale*(%f,%f),Palette);\n",
          -px, px, 1 + px, -1 - px);
  }

  else if (!strcmp (Pf.space, "ipf"))
    fprintf (file, "bounds range=image(v,scale*(%f,%f),scale*(%f,%f),Palette);\n",
	     -px, -Pf.ipfpts[Pf.ipfptqty - 1][1] / Pf.ipfpts[1][0] * px,
             1. + px, (Pf.ipfpts[Pf.ipfptqty - 1][1] / Pf.ipfpts[1][0]) * (1 + px));

  else
    abort ();

  double y;
  if (strcmp (Pf.space, "ipf") != 0)
    y = 0;
  else
    y = 2;

  fprintf (file, "%spicture bar;\n", Print.showscale ? "" : "// ");

  if (!strcmp (Pf.crysym, "cubic"))
    fprintf (file, "%spalette(bar,\"%s\",range,(0cm,0cm),(0.5cm,6cm),Right,Palette,\n",
             Print.showscale ? "" : "// ", Data.ScaleTitle ? Data.ScaleTitle : "Density");
  else
    fprintf (file, "%spalette(bar,\"%s\",range,(0cm,0cm),(0.392cm,4.7cm),Right,Palette,\n",
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

  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
      fprintf (file, "%sadd(bar.fit(),point(E),W+%fS);\n", Print.showscale ? "" : "// ", y);

    else if (!strcmp (Pf.shape, "quarter"))
      fprintf (file, "%sadd(bar.fit(),point(E),W+%fS);\n", Print.showscale ? "" : "// ", y);

    if (!strcmp (Pf.shape, "full"))
      fprintf (file, "draw (contour (points, values, new real[] {");

    else if (!strcmp (Pf.shape, "quarter"))
      fprintf (file, "draw (contour (scale(1,-1)*points, values, new real[] {");
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    fprintf (file, "%sadd(bar.fit(),point(E),W);\n", Print.showscale ? "" : "// ");
    fprintf (file, "draw (contour (scale(%f)*points, values, new real[] {",
             1. / Pf.ipfpts[1][0]);
  }

  else
    abort ();

  for (i = 1; i < tickqty; i++)
    fprintf (file, "%g,", tickvals[i]); // g avoids failed tests associated to machine precision
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
nev_print_pf_ptsprint_density_write_text0 (struct PF Pf, FILE *file)
{
  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
    {
      fprintf (file,
        "label(\"\\begin{tabular}{r@{}}Gaussian smoothing: $%g^{\\circ}$\\\\", Pf.pfkernelsig);
    }
    else if (!strcmp (Pf.shape, "quarter"))
    {
      fprintf (file,
        "label(\"\\begin{tabular}{r@{}}%s\\\\Gaussian smoothing: $%g^{\\circ}$\\\\",
        !strcmp (Pf.projection, "stereographic") ? "stereo. proj." : "equal-area proj.",
        Pf.pfkernelsig);
      fprintf (file,
               "\\end{tabular}\", scale(scale)*(%.3f,%.3f), NW, fontsize(8));\n",
              1., -1 - SMARGINPF * 0.465);
    }
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    char *legp = ut_alloc_1d_char (1000);
    sprintf (legp, "$\\!\\left\\{%.0f%.0f%.0f\\right\\}$",
             Pf.pfpoles[1][0], Pf.pfpoles[1][1], Pf.pfpoles[1][2]);

    fprintf (file,
      "label(\"\\begin{tabular}{l}%s\\\\%s\\\\Gaussian smoothing: $%g^{\\circ}$\\\\",
      legp, !strcmp (Pf.projection, "stereographic") ? "stereo. proj." : "equal-area proj.",
      Pf.pfkernelsig);

    ut_free_1d_char (&legp);

    fprintf (file,
	     "\\end{tabular}\", scale(scale)*(%.3f,%.3f), SE, fontsize(8));\n",
	     - WMARGINPF / 2, 1.);
  }

  else
    abort ();

  return;
}

void
nev_print_pf_ptsprint_density_write_mask (struct IN_V In, FILE *file, struct PF Pf)
{
  if (!strcmp (Pf.space, "pf"))
  {
    if (!strcmp (Pf.shape, "full"))
    {
      fprintf (file, "draw (scale(scale)*(-X--X), black);\n");
      fprintf (file, "draw (scale(scale)*(-Y--Y), black);\n");

      fprintf (file, "path g = shift(O)*scale(scale*1.15)*((-X-Y)--(-X+Y)--(X+Y)--(X-Y)--cycle);\n");
      fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*unitcircle^^g, evenodd+%s, %s);\n", In.scenebackground, In.scenebackground);
      fprintf (file, "draw (shift(O)*scale(scale)*unitcircle, black);\n");
    }

    else if (!strcmp (Pf.shape, "quarter"))
    {
      fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*((-.05*X)--(-.05*X+.05*Y)--(1.05*X+0.05*Y)--(1.05*X)--cycle), evenodd+%s, %s);\n", In.scenebackground, In.scenebackground);
      fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*((.05*Y)--(.05*Y-.05*X)--(-1.05*Y-0.05*X)--(-1.05*Y)--cycle), evenodd+%s, %s);\n", In.scenebackground, In.scenebackground);
      fprintf (file, "filldraw(shift(O)*scale(scale*1.00)*(arc(O,-Y,X)--(1.05*X)--(1.05*X-1.05*Y)--(-1.05*Y)--cycle), evenodd+%s, %s);\n", In.scenebackground, In.scenebackground);
      fprintf (file, "draw (shift(O)*scale(scale)*arc(O,-Y,X), black);\n");
      fprintf (file, "draw (shift(O)*scale(scale)*(-Y--O--X), black);\n");
    }
  }

  else if (!strcmp (Pf.space, "ipf"))
  {
    {
      fprintf (file,
          "filldraw(shift(O)*scale(scale/%f)*(scale(1+1e-3)*line110111--(%f,%f)--(%f,%f)--(%f,%f)--(%f,%f)--(%f,%f)--(%f,%f)--(%f,%f)--cycle), evenodd+%s+linewidth(3pt), %s);\n",
          Pf.ipfpts[1][0],
          0., 0.,
          Pf.ipfpts[1][0] + 0.03, 0.,
          Pf.ipfpts[1][0] + 0.03, -0.03,
          -0.03, -0.03,
          -0.03, Pf.ipfpts[Pf.ipfptqty - 1][1] + 0.03,
          Pf.ipfpts[1][0] + 0.03, Pf.ipfpts[Pf.ipfptqty - 1][1] + 0.03,
          Pf.ipfpts[1][0] + 0.03, 0.,
          In.scenebackground, In.scenebackground);
    }

    fprintf (file, "draw (scale(scale/%f)*border, black);\n", Pf.ipfpts[1][0]);
  }

  else
    abort ();

  return;
}
