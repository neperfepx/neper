/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_stat_.h"

void
nem_stat (struct NODES Nodes, struct MESH *Mesh,
          struct IN_M In, struct MESHPARA MeshPara, struct TESS *pTess,
          struct POINT Point)
{
  FILE *file = NULL;
  char *name = NULL;
  int dim, realdim;

  if (In.stnode)
  {
    name = ut_string_addextension (In.body, "stnode");

    file = ut_file_open (name, "w");
    nem_stat_nodes (file, In.stnode, Nodes, Mesh, pTess);
    ut_file_close (file, name, "w");

    ut_free_1d_char (&name);
  }

  for (dim = 0; dim <= 5; dim++)
    if (In.stmesh[dim])
    {
      if (dim < 4)
      {
        name = ut_string_addextension (In.body, "stmesh%dd", dim);
        realdim = dim;
      }
      if (dim == 4)
      {
        name = ut_string_addextension (In.body, "stmeshco");
        realdim = dim;
      }
      else if (dim == 5)
      {
        name = ut_string_addextension (In.body, "stmesh");
        realdim = neut_mesh_array_dim (Mesh);
      }
      else
        abort ();

      if (Mesh[realdim].EltQty == 0)
        ut_print_message (1, 2, "Mesh is void (%d).  Skipping...\n", dim);
      else
      {
        file = ut_file_open (name, "w");
        nem_stat_elts (file, "mesh", dim, realdim, In.stmesh[dim], Nodes, Mesh, MeshPara, pTess);
        ut_file_close (file, name, "w");
      }

      ut_free_1d_char (&name);
    }

  for (dim = 0; dim <= 5; dim++)
    if (In.stelt[dim])
    {
      if (dim < 4)
      {
        name = ut_string_addextension (In.body, "stelt%dd", dim);
        realdim = dim;
      }
      else if (dim == 4)
      {
        name = ut_string_addextension (In.body, "steltco", dim);
        realdim = dim;
      }
      else if (dim == 5)
      {
        name = ut_string_addextension (In.body, "stelt");
        realdim = neut_mesh_array_dim (Mesh);
      }
      else
        abort ();

      if (Mesh[realdim].EltQty == 0)
        ut_print_message (1, 2, "Mesh is void (%d).  Skipping...\n", dim);
      else
      {
        file = ut_file_open (name, "w");
        nem_stat_elts (file, "elt", dim, realdim, In.stelt[dim], Nodes, Mesh, MeshPara, pTess);
        ut_file_close (file, name, "w");
      }

      ut_free_1d_char (&name);
    }

  for (dim = 0; dim <= 5; dim++)
    if (In.stelset)
      if (In.stelset[dim])
      {
        if (dim < 4)
        {
          name = ut_string_addextension (In.body, "stelset%dd", dim);
          realdim = dim;
        }
        else if (dim == 4)
        {
          name = ut_string_addextension (In.body, "stelsetco", dim);
          realdim = dim;
        }
        else if (dim == 5)
        {
          name = ut_string_addextension (In.body, "stelset");
          realdim = neut_mesh_array_dim (Mesh);
        }
        else
          abort ();

        if (Mesh[realdim].EltQty == 0)
          ut_print_message (1, 2, "Mesh is void (%d).  Skipping...\n", dim);
        else
        {
          file = ut_file_open (name, "w");
          nem_stat_elts (file, "elset", dim, realdim, In.stelset[dim], Nodes, Mesh,
                           MeshPara, pTess);
          ut_file_close (file, name, "w");
        }

        ut_free_1d_char (&name);
      }

  if (In.stpt)
  {
    name = ut_string_addextension (In.body, "stpoint");

    file = ut_file_open (name, "w");
    nem_stat_point (file, In.stpt, Nodes, Mesh, *pTess, Point);
    ut_file_close (file, name, "w");

    ut_free_1d_char (&name);
  }

  return;
}
