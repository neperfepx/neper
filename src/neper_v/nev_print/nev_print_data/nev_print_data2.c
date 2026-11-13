/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_.h"

void
nev_print_data_domain (struct TESS Tess, struct NODES Nodes, struct MESH *Mesh,
                       char *space, char *attribute, char *value,
                       struct PRINT *pPrint)
{
  int cellqty;
  double size, vals[5];
  char *tmp = NULL;
  char *vars[5] = { (char *) "default", (char *) "edgerad", (char *) "elt1drad",
                    (char *) "elt2dedgerad", (char *) "elt3dedgerad" };

  ut_print_message (0, 1, "Reading data (domain, %s)...\n", attribute);

  if (!strcmp (attribute, "rad"))
  {
    // default radii of the domain, of the tessellation edges and of the
    // mesh entities (computed from the mesh, or else from the tessellation)
    vals[0] = nev_print_domain_defrad (Tess);

    vals[1] = -1;
    if (!neut_tess_isvoid (Tess))
      nev_print_tess_defrad (Tess, 1, vals + 1);

    size = 1;
    cellqty = 1;
    if (Mesh && neut_mesh_array_dim (Mesh) > 0)
      nev_print_mesh_size_cellqty (Nodes, Mesh, &size, &cellqty);
    else if (!neut_tess_isvoid (Tess))
    {
      neut_tess_size (Tess, &size);
      cellqty = Tess.CellQty;
    }

    nev_print_mesh_defrad (size, cellqty, 1, vals + 2);
    nev_print_mesh_defedgerad (size, cellqty, 2, space, vals + 3);
    nev_print_mesh_defedgerad (size, cellqty, 3, space, vals + 4);

    nev_print_rad_vars (value, 5, vars, vals, &tmp);
    ut_string_real (tmp, &(*pPrint).domainrad);
  }

  else if (!strcmp (attribute, "col"))
  {
    if (!(*pPrint).domaincol)
      (*pPrint).domaincol = ut_alloc_1d_int (3);
    ut_array_1d_int_fnscanf_wcard (value, (*pPrint).domaincol, 3, "color", "r");
  }

  else
    ut_print_exprbug (attribute);

  ut_free_1d_char (&tmp);

  return;
}
