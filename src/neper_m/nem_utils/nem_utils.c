/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_utils_.h"

void
nem_in_set_zero (struct IN_M *pIn)
{
  (*pIn).abq = NULL;
  (*pIn).body = NULL;
  (*pIn).elttype = NULL;
  (*pIn).faset = NULL;
  (*pIn).format = NULL;
  (*pIn).performat = NULL;
  (*pIn).geof = NULL;
  (*pIn).gmsh = NULL;
  (*pIn).tmp = NULL;
  (*pIn).outtess = NULL;
  (*pIn).loadmesh = NULL;
  (*pIn).loadpoint = NULL;
  (*pIn).mesh2dalgo = NULL;
  (*pIn).mesh3dalgo = NULL;
  (*pIn).meshface = NULL;
  (*pIn).meshpoly = NULL;
  (*pIn).interface = NULL;
  (*pIn).msh = NULL;
  (*pIn).msh4 = NULL;
  (*pIn).ori = NULL;
  (*pIn).bcs = NULL;
  (*pIn).vtk = NULL;
  (*pIn).per = NULL;
  (*pIn).nset = NULL;
  (*pIn).elset = NULL;
  (*pIn).partmethod = NULL;
  (*pIn).partstring = NULL;
  (*pIn).transform = NULL;
  (*pIn).stmesh = ut_alloc_1d_pchar (6);
  (*pIn).stelt = ut_alloc_1d_pchar (6);
  (*pIn).stelset = ut_alloc_1d_pchar (6);
  (*pIn).stnode = NULL;
  (*pIn).stpt = NULL;
  (*pIn).tess = NULL;
  (*pIn).tesr = NULL;
  (*pIn).intf = NULL;

  (*pIn).meshqualexpr = NULL;
  (*pIn).meshqualdisexpr = NULL;
  (*pIn).mesh3dclrepsstring = NULL;
  // (*pIn).meshoptisizeexpr = NULL;
  (*pIn).dimstring = NULL;
  (*pIn).dimout = NULL;
  (*pIn).dimout_msh = NULL;

  (*pIn).cltype = NULL;
  (*pIn).clstring = NULL;
  (*pIn).clfacetype = NULL;
  (*pIn).clfacestring = NULL;
  (*pIn).cledgetype = NULL;
  (*pIn).cledgestring = NULL;
  (*pIn).clvertype = NULL;
  (*pIn).clverstring = NULL;
  (*pIn).clratiostring = NULL;

  (*pIn).transportstring = NULL;
  (*pIn).transporteltmethodstring = NULL;
  (*pIn).tesrsmooth = NULL;

  return;
}

void
nem_in_free (struct IN_M In)
{
  ut_free_1d_char (&In.body);
  ut_free_1d_char (&In.gmsh);
  ut_free_1d_char (&In.tmp);
  ut_free_1d_char (&In.format);
  ut_free_1d_char (&In.performat);
  ut_free_1d_char (&In.mesh2dalgo);
  ut_free_1d_char (&In.mesh3dalgo);
  ut_free_1d_char (&In.meshface);
  ut_free_1d_char (&In.meshpoly);
  ut_free_1d_char (&In.nset);
  ut_free_1d_char (&In.elset);
  ut_free_1d_char (&In.faset);
  ut_free_1d_char (&In.partstring);
  ut_free_1d_char (&In.transform);
  ut_free_1d_char (&In.partmethod);
  ut_free_1d_char (&In.tess);
  ut_free_1d_char (&In.tesr);
  ut_free_1d_char (&In.stnode);
  ut_free_2d_char (&In.stmesh, 6);
  ut_free_2d_char (&In.stelt, 5);
  ut_free_2d_char (&In.stelset, 5);
  ut_free_1d_char (&In.stpt);
  ut_free_1d_char (&In.msh);
  ut_free_1d_char (&In.msh4);
  ut_free_1d_char (&In.ori);
  ut_free_1d_char (&In.bcs);
  ut_free_1d_char (&In.vtk);
  ut_free_1d_char (&In.per);
  ut_free_1d_char (&In.abq);
  ut_free_1d_char (&In.geof);
  ut_free_1d_char (&In.outtess);
  ut_free_1d_char (&In.dimstring);
  ut_free_1d_char (&In.dimout);
  ut_free_1d_char (&In.dimout_msh);
  ut_free_1d_char (&In.meshqualexpr);
  ut_free_1d_char (&In.meshqualdisexpr);
  ut_free_1d_char (&In.mesh3dclrepsstring);
  ut_free_1d_char (&In.elttype);
  ut_free_1d_char (&In.cltype);
  ut_free_1d_char (&In.clstring);
  ut_free_1d_char (&In.clfacetype);
  ut_free_1d_char (&In.clfacestring);
  ut_free_1d_char (&In.cledgetype);
  ut_free_1d_char (&In.cledgestring);
  ut_free_1d_char (&In.clvertype);
  ut_free_1d_char (&In.clverstring);
  ut_free_1d_char (&In.clratiostring);
  ut_free_1d_char (&In.transportstring);
  ut_free_1d_char (&In.transporteltmethodstring);
  ut_free_1d_char (&In.interface);
  ut_free_1d_char (&In.intf);
  ut_free_1d_char (&In.tesrsmooth);

  return;
}
