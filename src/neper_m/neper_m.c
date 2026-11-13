/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neper_m_.h"

int
neper_m (int fargc, char **fargv, int argc, char **argv)
{
  // ###################################################################
  // ### INITIALIZING ##################################################

  // Variable declaration ###
  int i, status, dim, nsetloaded = 0, nsetfaces = 0;
  struct IN_M In;
  struct TESS Tess;
  struct TESR Tesr;
  struct MESHPARA MeshPara;
  struct NODES RNodes;          // input nodes (remeshing)
  struct MESH *RMesh = NULL;    // input mesh (remeshing)
  struct NODES Nodes;           // output nodes
  struct MESH *Mesh = NULL;     // output mesh
  struct PART Part;
  struct NSET *NSet = NULL;
  struct POINT Point;
  struct BOUNDARY Bound;

  RMesh = malloc (5 * sizeof (struct MESH));
  Mesh = malloc (6 * sizeof (struct MESH));
  NSet = malloc (3 * sizeof (struct NSET));

  nem_in_set_zero (&In);
  neut_tess_set_zero (&Tess);
  neut_meshpara_set_zero (&MeshPara);
  neut_tesr_set_zero (&Tesr);
  neut_nodes_set_zero (&RNodes);
  neut_nodes_set_zero (&Nodes);
  neut_boundary_set_zero (&Bound);

  for (i = 0; i < 5; i++)
    neut_mesh_set_zero (RMesh + i);

  for (i = 0; i < 6; i++)
    neut_mesh_set_zero (Mesh + i);

  for (i = 0; i < 3; i++)
    neut_nset_set_zero (NSet + i);

  neut_part_set_zero (&Part);
  neut_point_set_zero (&Point);

  // Printing program header ###
  ut_print_moduleheader ("-M", fargc, fargv, argc, argv);

  // Reading input data ###
  ut_print_message (0, 1, "Reading input data...\n");
  nem_input (&In, fargc, fargv, argc, argv);

  // ###################################################################
  // ### LOADING INPUT DATA ############################################

  ut_print_message (0, 1, "Loading input data...\n");

  // Loading tessellation ###
  if (In.tess)
  {
    ut_print_message (0, 2, "Loading tessellation...\n");
    neut_tess_fnscanf (In.tess, &Tess, "r");
    nem_input_init_dim_tess (&In, Tess);

  }

  // Loading raster tessellation ###
  if (In.tesr)
  {
    ut_print_message (0, 2, "Loading raster tessellation...\n");
    neut_tesr_fnscanf (In.tesr, &Tesr);
    if (Tesr.Dim == 3 && !strcmp (In.elttype, "tri"))
      ut_print_message (2, 3, "Free meshing not available for 3D tesrs (use `-elttype hexgrid').\n");

    nem_input_init_dim_tesr (&In, Tesr);

    if (!strcmp (In.elttype, "tri"))
    {
      ut_print_message (0, 2, "Reconstructing topology...\n");
      nem_reconstruct_tesr (&Tesr, &Tess, &RNodes, RMesh, NSet);

      if (Tess.Dim >= 2 && strcmp (In.tesrsmooth, "none"))
      {
        ut_print_message (0, 2, "Smoothing mesh...\n");
        nem_smoothing (In, &Tess, &RNodes, RMesh);
      }
    }
  }

  // Loading input mesh (remeshing and / or transport) ###
  if (In.mesh)
  {
    int topology;

    ut_print_message (0, 1, "Loading mesh...\n");
    neut_mesh_fnscanf_msh (In.mesh, &RNodes, RMesh, RMesh + 1, RMesh + 2,
                           RMesh + 3, RMesh + 4, NULL, NULL, &topology, "r");
    nem_input_init_dim_mesh (&In, RMesh);

    if (!strcmp (In.elttype, "tri") && topology)
    {
      ut_print_message (0, 2, "Reconstructing topology...\n");
      nem_reconstruct_mesh ("all", &RNodes, RMesh, &Tess);
    }
  }

  // Loading points ###
  if (In.loadpoint)
  {
    ut_print_message (0, 1, "Loading points...\n");
    neut_point_fnscanf (In.loadpoint, &Point);
  }

  // Loading output mesh ###
  if (In.loadmesh)
  {
    int topology;
    ut_print_message (0, 1, "Loading result mesh...\n");
    // without the tessellation, the nsets of the mesh are kept
    nsetloaded = nem_readmesh (In.loadmesh, &Nodes, Mesh, In.tess ? NULL : NSet, &topology);
    if (!In.tess)
      status = nem_input_init_dim_mesh (&In, Mesh);

    if (nsetloaded && !strcmp (In.nset, "default"))
      ut_string_string ("all", &In.nset);

    if (Nodes.NodeQty == 0)
      ut_print_message (1, 2, "Mesh is void (%d node(s), 0 elt).\n",
                        Nodes.NodeQty);

    else
    {
      status = 0;
      for (i = 0; i <= 2; i++)
        if (ut_list_testelt_int (In.dimout, NEUT_SEP_NODEP, i)
            && Mesh[i].EltQty == 0)
        {
          status = 1;
          break;
        }

      int nsetrecon = Tess.PolyQty == 0 && strcmp (In.nset, "none") != 0;

      if (topology && (status || nsetrecon))
      {
        ut_print_message (0, 2, "Reconstructing mesh...\n");
        nem_reconstruct_mesh (nsetrecon ? "all" : In.dimout, &Nodes, Mesh, &Tess);

        // domain face labels from the fasets or else from the face nsets (if
        // loaded with the mesh)
        if (Mesh[5].ElsetQty > 0)
          nem_reconstruct_mesh_fasets (Mesh[2], Mesh[5], &Tess);
        else if (nsetloaded)
          nsetfaces = !nem_reconstruct_mesh_nsets (Nodes, Mesh[2], NSet[2], &Tess);
      }

      // no fasets can be written (e.g. periodic mesh)
      if (nsetloaded && Mesh[5].ElsetQty == 0 && !nsetfaces && !strcmp (In.faset, "faces"))
        ut_string_string ("none", &In.faset);
    }
  }

// ###################################################################
// ### COMPUTING OUTPUT MESH #########################################

  if (nem_input_outputismesh (In)
      && (!In.loadmesh || (In.loadmesh && neut_mesh_isvoid (Mesh[In.dim]))))
  {
    if (Tess.VerQty > 0 || Tesr.CellQty > 0 || RMesh[3].EltQty > 0)
    {
      ut_print_message (0, 1, "Meshing...\n");

      nem_meshing_para (In, &Tess, &Tesr, &RNodes, RMesh, &MeshPara);

      if (!strcmp (In.elttype, "tri"))
      {
        nem_meshing (In, &MeshPara, &Tess, RNodes, RMesh, &Nodes, Mesh);

        if (In.eltsplit)
        {
          ut_print_message (0, 2, "Splitting elements...\n");
          nem_split (In, Tess, &Nodes, Mesh);
        }
      }

      else if (!strncmp (In.elttype, "quad", 4))
      {
        if (In.tess)
          nem_meshing_tess_str (In, MeshPara, &Tess, &Nodes, Mesh, NSet);
        else if (In.tesr)
          nem_meshing_tesr_str (In, MeshPara, &Tesr, &Nodes, Mesh, NSet);
      }

      else
        ut_print_neperbug ();

      if (!neut_mesh_isvoid (Mesh[Tess.Dim]))
        nem_ori (Tess, &Nodes, Mesh);

      // Post-scaling of input / mesh if necessary (use of clratio) ###
      nem_meshing_para_post (MeshPara, &Tess, &Tesr, &RNodes, &Nodes, Mesh);
    }
  }

  if (ut_list_testelt (In.interface, NEUT_SEP_DEP, "discontinuous")
      || ut_list_testelt (In.interface, NEUT_SEP_DEP, "cohesive"))
  {
    ut_print_message (0, 1, "Processing mesh at interfaces...\n");
    nem_interface (In, Tess, &Nodes, Mesh, &Bound);
  }

  dim = neut_mesh_array_dim (Mesh);

  if (In.singnodedup)
  {
    ut_print_message (0, 1, "Duplicating singular nodes... ");
    nem_cleaning_singnodedup (Mesh, &Nodes, NULL);
  }

  if (In.dupnodemerge > 0)
  {
    ut_print_message (0, 1, "Merging duplicated nodes... ");
    neut_mesh_dupnodemerge (&Nodes, Mesh, NULL, In.dupnodemerge, 1, NULL);
  }

  // (unless loaded with the mesh; with nsets loaded with the mesh, only if the
  // domain faces correspond to face nsets)
  if (Mesh[3].EltQty > 0 && strcmp (In.faset, "none")
      && (!strcmp (Mesh[3].EltType, "tri") || In.eltsplit) && Mesh[5].EltQty == 0
      && (!nsetloaded || nsetfaces))
  {
    ut_print_message (0, 1, "Creating fasets...\n");
    neut_mesh_fasetmesh (Tess, Mesh[2], Mesh[3], Mesh + 5);
  }

// managing mesh order ###
  if (In.order == 2 && Mesh[dim].EltOrder == 1)
  {
    ut_print_message (0, 1, "Switching mesh to order 2...\n");

    if (dim >= 0 && Mesh[dim].EltOrder == 1)
    {
      if (!MeshPara.msize)
        nem_order (In, Tess, &Nodes, Mesh);
      else
        nem_order_str (In, MeshPara, &Nodes, Mesh);
    }

    if (nsetloaded)
      neut_nset_addeltnodes (NSet, Nodes, Mesh);
  }

  else if (In.order == 1 && dim >= 0 && Mesh[dim].EltOrder == 2)
  {
    ut_print_message (0, 1, "Switching mesh to order 1...\n");
    nem_order_1 (&Nodes, Mesh, NSet);
  }

// ###################################################################
// ### POST-MESHING OPERATIONS #######################################

  if (strcmp (In.transform, "none"))
  {
    ut_print_message (0, 1, "Transforming mesh...\n");
    nem_transform (In, Tess, &Nodes, Mesh);
  }

  if (In.sortnode || In.sortelt)
  {
    ut_print_message (0, 1, "Sorting mesh...\n");
    nem_sort (In, &Tess, &Nodes, Mesh, NSet);
  }

// Partitioning mesh ###
  if (In.partstring && strcmp (In.partstring, "0"))
  {
    ut_print_message (0, 1, "Partitioning mesh...\n");
#ifdef HAVE_LIBSCOTCH
    nem_partition (In, &Nodes, Mesh, &Part);
#else
    ut_print_message (2, 1, "Compiled without libscotch.\n");
#endif /* HAVE_LIBSCOTCH */
  }

// Searching node sets ###
  if ((strcmp (In.nset, "none")))
  {
    ut_print_message (0, 1, "Creating nsets...\n");

    // loaded nsets: only the custom nsets are created
    if (!nsetloaded)
      nem_nsets (In, Tess, Tesr, Nodes, Mesh, NSet);
    else if (strchr (In.nset, ':'))
      nem_nsets_custom (In, Tess, Nodes, Mesh, NSet);
  }

// Searching element sets ###
  if (strcmp (In.elset, "default"))
  {
    ut_print_message (0, 1, "Searching elsets...\n");
      nem_elsets (In, &Tess, Nodes, Mesh);
  }

// ###################################################################
// WRITING MESH (EVENTUALLY GEOMETRY) ################################

  if (dim >= 0)
  {
    ut_print_message (0, 1, "Writing mesh results...\n");
    nem_writemesh (In, Tess, Nodes, Mesh, NSet, Bound);
  }

  if (ut_list_testelt (In.format, NEUT_SEP_NODEP, "tess"))
  {
    ut_print_message (0, 1, "Writing geometry results...\n");
    neut_tess_name_fprintf (In.outtess, Tess);
  }

// ###################################################################
// ### DATA TRANSPORT (GOES WITH REMESHING) ##########################

  if (In.transportstring)
  {
    ut_print_message (0, 1, "Transporting data...\n");
    nem_transport (In, Tess, RNodes, RMesh, Nodes, Mesh);
  }

// ###################################################################
// ### MESH STATISTICS ###############################################

  if (In.stnode || In.stelt[0] || In.stelt[1] || In.stelt[2] || In.stelt[3] ||
      In.stelt[4] || In.stelt[5] || In.stelset[0] || In.stelset[1] ||
      In.stelset[2] || In.stelset[3] || In.stelset[4] || In.stelset[5] ||
      In.stpt || In.stmesh[0] || In.stmesh[1] || In.stmesh[2] || In.stmesh[3]
      || In.stmesh[4] || In.stmesh[5])
  {
    ut_print_message (0, 1, "Writing mesh statistics...\n");

    nem_stat (Nodes, Mesh, In, MeshPara, &Tess, Point);
  }

// ###################################################################
// ### CLOSING #######################################################

  nem_in_free (In);
  neut_meshpara_free (&MeshPara);
  neut_tess_free (&Tess);
  neut_tesr_free (&Tesr);
  neut_nodes_free (&Nodes);
  for (i = 0; i < 6; i++)
    neut_mesh_free (Mesh + i);
  free (Mesh);
  for (i = 0; i < 5; i++)
    neut_mesh_free (RMesh + i);
  free (RMesh);
  for (i = 0; i < 3; i++)
    neut_nset_free (NSet + i);
  free (NSet);

  neut_part_free (Part);
  neut_nodes_free (&RNodes);
  neut_point_free (&Point);

  return EXIT_SUCCESS;
}
