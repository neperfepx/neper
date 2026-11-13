/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_utils_.h"

void
nem_in_set_zero (struct IN_M *pIn)
{
  (*pIn).abq = NULL;
  (*pIn).mesh = NULL;
  (*pIn).sim = NULL;
  (*pIn).body = NULL;
  (*pIn).elttype = NULL;
  (*pIn).eltsplit = 0;
  (*pIn).faset = NULL;
  (*pIn).format = NULL;
  (*pIn).performat = NULL;
  (*pIn).geof = NULL;
  (*pIn).etree = NULL;
  (*pIn).ntree = NULL;
  (*pIn).outtess = NULL;
  (*pIn).loadmesh = NULL;
  (*pIn).loadpoint = NULL;
  (*pIn).mesh2dalgo = NULL;
  (*pIn).mesh3dalgo = NULL;
  (*pIn).meshedge = NULL;
  (*pIn).meshface = NULL;
  (*pIn).meshedgelist = NULL;
  (*pIn).meshfacelist = NULL;
  (*pIn).meshpoly = NULL;
  (*pIn).interface = NULL;
  (*pIn).msh = NULL;
  (*pIn).ori = NULL;
  (*pIn).phase = NULL;
  (*pIn).bcs = NULL;
  (*pIn).vtk = NULL;
  (*pIn).per = NULL;
  (*pIn).nset = NULL;
  (*pIn).elset = NULL;
  (*pIn).partstring = NULL;
  (*pIn).transform = NULL;
  (*pIn).sortelt = NULL;
  (*pIn).sortnode = NULL;
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
  (*pIn).mesh2dclrepsstring = NULL;
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
  (*pIn).clpolystring = NULL;
  (*pIn).plpolystring = NULL;
  (*pIn).plfacestring = NULL;
  (*pIn).pledgestring = NULL;
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
  ut_free_1d_char (&In.mesh);
  ut_free_1d_char (&In.loadmesh);
  ut_free_1d_char (&In.loadpoint);
  ut_free_1d_char (&In.sim);
  ut_free_1d_char (&In.etree);
  ut_free_1d_char (&In.ntree);
  ut_free_1d_char (&In.format);
  ut_free_1d_char (&In.performat);
  ut_free_1d_char (&In.mesh2dalgo);
  ut_free_1d_char (&In.mesh3dalgo);
  ut_free_1d_char (&In.meshedge);
  ut_free_1d_char (&In.meshface);
  ut_free_1d_int (&In.meshedgelist);
  ut_free_1d_int (&In.meshfacelist);
  ut_free_1d_char (&In.meshpoly);
  ut_free_1d_char (&In.nset);
  ut_free_1d_char (&In.elset);
  ut_free_1d_char (&In.faset);
  ut_free_1d_char (&In.partstring);
  ut_free_1d_char (&In.transform);
  ut_free_1d_char (&In.sortelt);
  ut_free_1d_char (&In.sortnode);
  ut_free_1d_char (&In.tess);
  ut_free_1d_char (&In.tesr);
  ut_free_1d_char (&In.stnode);
  ut_free_2d_char (&In.stmesh, 6);
  ut_free_2d_char (&In.stelt, 5);
  ut_free_2d_char (&In.stelset, 5);
  ut_free_1d_char (&In.stpt);
  ut_free_1d_char (&In.msh);
  ut_free_1d_char (&In.ori);
  ut_free_1d_char (&In.phase);
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
  ut_free_1d_char (&In.mesh2dclrepsstring);
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
  ut_free_1d_char (&In.clpolystring);
  ut_free_1d_char (&In.plpolystring);
  ut_free_1d_char (&In.plfacestring);
  ut_free_1d_char (&In.pledgestring);
  ut_free_1d_char (&In.clratiostring);
  ut_free_1d_char (&In.transportstring);
  ut_free_1d_char (&In.transporteltmethodstring);
  ut_free_1d_char (&In.interface);
  ut_free_1d_char (&In.intf);
  ut_free_1d_char (&In.tesrsmooth);

  return;
}
