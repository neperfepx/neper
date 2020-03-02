/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_stat_.h"

void
nem_stat (struct NODES Nodes, struct MESH *Mesh, struct PART Part,
          struct IN_M In, struct MESHPARA MeshPara, struct TESS Tess,
          struct POINT Point)
{
  FILE *file = NULL;
  char *name = NULL;
  int dim, realdim;

  if (In.stn)
  {
    name = ut_string_addextension (In.body, "stnode");

    file = ut_file_open (name, "w");
    nem_stat_nodes (file, In.stn, Nodes, Mesh, Part, Tess);
    ut_file_close (file, name, "w");

    ut_free_1d_char (&name);
  }

  for (dim = 0; dim <= 4; dim++)
    if (In.stelt[dim])
    {
      if (dim < 4)
      {
        name = ut_string_addextension (In.body, "stelt%dd", dim);
        realdim = dim;
      }
      else
      {
        name = ut_string_addextension (In.body, "stelt");
        realdim = neut_mesh_array_dim (Mesh);
      }

      if (Mesh[realdim].EltQty == 0)
        ut_print_message (1, 2, "%dD mesh is void.  Skipping...\n", realdim);
      else
      {
        file = ut_file_open (name, "w");
        nem_stat_elts (file, realdim, In.stelt[dim], Nodes, Mesh, Part, Tess);
        ut_file_close (file, name, "w");
      }

      ut_free_1d_char (&name);
    }

  for (dim = 0; dim <= 4; dim++)
    if (In.stelset)
      if (In.stelset[dim])
      {
        if (dim < 4)
        {
          name = ut_string_addextension (In.body, "stelset%dd", dim);
          realdim = dim;
        }
        else
        {
          name = ut_string_addextension (In.body, "stelset");
          realdim = neut_mesh_array_dim (Mesh);
        }

        file = ut_file_open (name, "w");
        nem_stat_elsets (file, realdim, In.stelset[dim], Nodes, Mesh, Part,
                         MeshPara, Tess);
        ut_file_close (file, name, "w");

        ut_free_1d_char (&name);
      }

  if (In.stpt)
  {
    name = ut_string_addextension (In.body, "stpoint");

    file = ut_file_open (name, "w");
    nem_stat_point (file, In.stpt, Nodes, Mesh, Tess, Point);
    ut_file_close (file, name, "w");

    ut_free_1d_char (&name);
  }

  return;
}
