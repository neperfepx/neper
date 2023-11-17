.. _file_and_directory_formats_link:

File and Directory Formats
==========================

.. _tess_file:

Tessellation File (:file:`.tess`)
---------------------------------

Here are details on the :file:`.tess` file format version
2.0.  Developers should note that read and write functions are
available as :code:`neut_tess_fscanf` and :code:`neut_tess_fprintf`,
defined in directories :file:`neut/neut_tess/neut_tess_fscanf`
and
:file:`neut/neut_tess/neut_tess_fprintf`.

.. code-block:: plain

  ***tess
   **format
     <format>
   **general
     <dim> <type>
   **cell
     <number_of_cells>
   [*id
     <cell1_id> <cell2_id> ...]
   [*seed
     <seed_id> <seed_x> <seed_y> <seed_z> <seed_weight>
     ...]
   [*ori
     <descriptor>
     <cellid_param1> <cellid_param2> ...
     ...]
   [*orispread
     <cellid_spread>
     ...]
   [*orispreadtheta
     <cellid_orispreadtheta>
     ...]
   [*weight
     <cellid_weight>
     ...]
   [*lam
     <cell1_lam> <cell2_lam> ...]
   [*mode
     <cell1_mode> <cell2_mode> ...]
   [*crysym
     <crysym>]
   **vertex
     <total_number_of_vertices>
     <ver_id> <ver_x> <ver_y> <ver_z> <ver_state>
     ...
   **edge
     <total_number_of_edges>
     <edge_id> <ver_1> <ver_2> <edge_state>
     ...
   **face
     <total_number_of_faces>
     <face_id> <number_of_vertices> <ver_1> <ver_2> ...
               <number_of_edges> <edge_1*> <edge_2*> ...
               <face_eq_d> <face_eq_a> <face_eq_b> <face_eq_c>
               <face_state> <face_point> <face_point_x> <face_point_y> <face_point_z>
     ...
   **polyhedron
     <total_number_of_polyhedra>
     <poly_id> <number_of_faces> <face_1*> <face_2*> ...
     ...
   **domain
    *general
     <dom_type>
    *vertex
     <total_number_of_dom_vertices>
     <dom_ver_id> <dom_ver_x> <dom_ver_y> <dom_ver_z> <dom_ver_label>
                  <number_of_dom_tess_vertices> <ver_1>
     ...
    *edge
     <total_number_of_dom_edges>
     <dom_edge_id> <number_of_dom_vertices> [<dom_ver_1> <dom_ver_2>]
                   <dom_edge_label>
                   <number_of_dom_tess_edges> <edge_1> <edge_2> ...
     ...
    *face
     <total_number_of_dom_faces>
     <dom_face_id> <number_of_dom_vertices> <dom_ver_1> <dom_ver_2> ...
                   <number_of_dom_edges> <dom_edge_1> <dom_edge_2> ...
                   <dom_face_type>
                   <number_of_params> <dom_face_param1> <dom_face_param2> ...
                   <dom_face_label>
                   <number_of_dom_tess_faces>
                   <dom_tess_face_1> <dom_tess_face_2> ...
     ...
   **periodic
    *general
     <per_x> <per_y> <per_z>
     <per_dist_x> <per_dist_y> <per_dist_z>
    *seed
     <secondary_seed_qty>
     <secondary_seed_id> <primary_seed_id> <per_shift_x> <per_shift_y> <per_shift_z>
    ...
    *vertex
     <secondary_ver_qty>
     <secondary_ver_id> <primary_ver_id> <per_shift_x> <per_shift_y> <per_shift_z>
    ...
    *edge
     <secondary_edge_qty>
     <secondary_edge_id> <primary_edge_id> <per_shift_x> <per_shift_y> <per_shift_z> <secondary_edge_ori>
    ...
    *face
     <secondary_face_qty>
     <secondary_face_id> <primary_face_id> <per_shift_x> <per_shift_y> <per_shift_z> <secondary_face_ori>
    ...
   **scale
    *general
     <number_of_scales>
    *cellid
     <cell1_id> <cell1_scale1> <cell1_scale2> ...
    ...
  ***end



where (with identifiers being integer numbers),

- :data:`***tess` denotes the beginning of a tessellation file.

- :data:`**format` denotes the beginning of the format field.

- :data:`<format>` is the file format, currently `2.0` (character string).

- :data:`**general` denotes the beginning of the general information field.

- :data:`<dim>` is the dimension of the tessellation (1, 2 or 3).

- :data:`<type>` is the type of tessellation (always :data:`standard`).

- :data:`**cell` denotes the beginning of the cell field.

- :data:`<number_of_cells>` is the number of cells.

- :data:`*id` denotes the beginning of an optional identifier field.  It the field is not present, the cells are considered to be numbered contiguously from 1.

- :data:`<cell#_id>` are the actual identifiers of the cells.

- :data:`*lam` denotes the beginning of an optional lamella identifier field.

- :data:`<cell#_lam>` are the lamella identifiers of the cells.

- :data:`*mode` denotes the beginning of an optional mode identifier field.

- :data:`<cell#_mode>` are the mode identifiers of the cells.

- :data:`<crysym>` is the crystal symmetry (see :ref:`crystal_symmetries`).

- :data:`*seed` denotes the beginning of a seed field.

- :data:`<seed_id>` is the identifier of a seed and ranges from :data:`1` to :data:`<number_of_cells>`.

- :data:`<seed_x>`, :data:`<seed_y>` and :data:`<seed_z>` are the three coordinates of a seed (real numbers).

- :data:`<seed_weight>` is the weight of a seed (real number).

- :data:`*ori` denotes the beginning of an optional crystal orientation field.

- :data:`<descriptor>` is the descriptor used to parameterize the crystal orientations.  See :ref:`rotations_and_orientations` for the list of available descriptors.

- :data:`<cellid_param#>` are the values of the orientation descriptor of cell `id`.

- :data:`*orispread` denotes the beginning of an optional crystal orientation distribution field.

- :data:`<cellid_spread>` is the value of the orientation distribution of cell `id`.

- :data:`*orispreadtheta` denotes the beginning of an optional crystal orientation distribution field (theta values).

- :data:`<cellid_orispreadtheta>` is the theta value of the orientation distribution of cell `id` (in degrees).

- :data:`*weight` denotes the beginning of an optional weight field.

- :data:`<cellid_weight>` is the weight of cell `id`.

- :data:`**vertex` denotes the beginning of the vertex field.

- :data:`<total_number_of_vertices>` is the total number of vertices.

- :data:`<ver_id>` is the identifier of a vertex and ranges from :data:`1` to :data:`<total_number_of_vertices>`.

- :data:`<ver_x>`, :data:`<ver_y>` and :data:`<ver_z>` are the three coordinates of a vertex (real numbers).

- :data:`<ver_state>` is an integer indicating the state of a vertex. For a standard tessellation (no regularization), it equals :data:`0`.  For a regularized tessellation, it equals :data:`0` if the vertex has not been modified by regularization and is higher than :data:`0` otherwise.

- :data:`**edge` denotes the beginning of the edge field.

- :data:`<total_number_of_edges>` is the total number of edges.

- :data:`<edge_id>` is the identifier of an edge and ranges from :data:`1` to :data:`<total_number_of_edges>`.

- :data:`<ver_1>`, :data:`<ver_2>`, ... are identifiers of vertices.

- :data:`<edge_state>` is an integer indicating the state of an edge (always :data:`0`).

- :data:`**face` denotes the beginning of the face field. It is present for a tessellation of dimension 2 or 3.

- :data:`<total_number_of_faces>` is the total number of faces.

- :data:`<face_id>` is the identifier of a face and ranges from :data:`1` to :data:`<total_number_of_faces>`.

- :data:`<number_of_vertices>` is the number of vertices of a face.

- :data:`<number_of_edges>` is the number of edges of a face.

- :data:`<edge_#*>` are identifiers of the edges of a face, signed according to their orientation in the face.

- :data:`<face_eq_a>`, :data:`<face_eq_b>`, :data:`<face_eq_c>` and :data:`<face_eq_d>` are the parameters of the equation of a face: :math:`a \, x + b \, y + c \, z = d` with :math:`a^2+b^2+c^2=1`.

- :data:`<face_state>` is an integer indicating the state of a face. For a standard tessellation (no regularization), it equals :data:`0`.  For a regularized tessellation, it equals :data:`0` if it has not been modified by regularization and :data:`1` otherwise.

- :data:`<face_point>` is an integer indicating the point used for the interpolation of a face. For a standard tessellation (no regularization), it equals :data:`0`.  For a regularized tessellation: if the point is the face barycenter, it equals :data:`0`; if the point is one of the face vertices, it equals to the position of the vertex in the list of vertices of the face. It equals :data:`-1` if the point is undefined.

- :data:`<face_point_x>`, :data:`<face_point_y>` and :data:`<face_point_z>` are the coordinates of the point used for the interpolation of a face (equal :data:`0` if undefined).

- :data:`**polyhedron` denotes the beginning of the polyhedron field.  It is present for a tessellation of dimension 3.

- :data:`<total_number_of_polyhedra>` is the total number of polyhedra.

- :data:`<poly_id>` is the identifier of a polyhedron and ranges from :data:`1` to :data:`<total_number_of_polyhedra>`.

- :data:`<number_of_faces>` is the number of faces of a polyhedron.

- :data:`<face_#*>` are identifiers of the faces of a polyhedron, signed according to their orientations in the polyhedron (positive if the normal of the face is pointing outwards and negative if it is pointing inwards).

- :data:`**domain` denotes the beginning of the domain field.

- :data:`*general` denotes the beginning of the domain general information field.

- :data:`<dom_type>` is the type of the domain (one of :data:`cube`, :data:`cylinder`, :data:`square`, :data:`circle`, :data:`poly` and :data:`planes`).

- :data:`*vertex` denotes the beginning of the domain vertex field.

- :data:`<total_number_of_dom_vertices>` is the total number of domain vertices.

- :data:`<dom_ver_id>` is the identifier of a domain vertex and ranges between :data:`1` to :data:`<total_number_of_dom_vertices>`.

- :data:`<dom_ver_x>`, :data:`<dom_ver_y>` and :data:`<dom_ver_z>` are the three coordinates of a domain vertex (real numbers).

- :data:`<dom_ver_label>` is the label of a domain vertex, formatted as :data:`x0y0z0`, :data:`x1y0z0`, :data:`x0y1z0`, :data:`x1y1z0`, :data:`x0y0z1`, ... (for a cubic domain).

- :data:`<number_of_dom_tess_vertices>` is the number of tessellation vertices of a domain vertex (must be 1).

- :data:`*edge` denotes the beginning of the domain edge field (for a tessellation of dimension 2 or 3).

- :data:`<total_number_of_dom_edges>` is the total number of domain edges.

- :data:`<dom_edge_id>` is the identifier of a domain edge and ranges between :data:`1` to :data:`<total_number_of_dom_edges>`.

- :data:`<number_of_dom_vertices>` is the number of domain vertices of a domain edge or a domain face.

- :data:`<dom_ver_#>` are identifiers of the domain vertices of a domain edge or face.

- :data:`<dom_edge_label>` is the label of a domain edge, formatted as :data:`x0y0`, :data:`x0y1`, :data:`x1y0`, ... (for a cubic domain).

- :data:`<number_of_dom_tess_edges>` is the number of tessellation edges of a domain edge.

- :data:`*face` denotes the beginning of the domain face field (for a tessellation of dimension 3).

- :data:`<total_number_of_dom_faces>` is the total number of domain faces.

- :data:`<dom_face_id>` is the identifier of a domain face and ranges from :data:`1` to :data:`<total_number_of_dom_faces>`.

- :data:`<number_of_dom_edges>` is the number of domain edges of a domain face.

- :data:`<dom_edge_#>` are identifiers of the domain edges of a domain face.

- :data:`<dom_face_type>` is the type of a face, among `plane`, `cylinder` or `sphere`.

- :data:`<number_of_params>` is the number of parameters of a domain face.

- :data:`<dom_face_param#>` are the parameters of a domain face.  For a planar face, they are the parameters of the equation of the face, listed in the order :data:`<face_eq_d>`, :data:`<face_eq_a>`, :data:`<face_eq_b>` and :data:`<face_eq_c>`.  For a cylindrical face, they are the coordinates of the base point, the axis and the radius.  For a spherical face, they are the coordinates of the center and the radius.

- :data:`<dom_face_label>` is the label of a domain face, formatted as :data:`x0`, :data:`x1`, :data:`y0`, :data:`y1`, :data:`z0` or :data:`z1` (for a cubic domain).  For a cylindrical domain, it is formatted as :data:`z0`, :data:`z1`, :data:`f1`, :data:`f2`, ... Otherwise, it is one of :data:`f1`, :data:`f2`, ...  Edge and vertex labels are derived accordingly.

- :data:`<number_of_dom_tess_faces>` is the number of tessellation faces of a domain face.

- :data:`<dom_tess_face_#` are the identifiers of the tessellation faces of a domain face.

- :data:`***end` denotes the end of a tessellation file.

- :data:`**periodicity` denotes the beginning of the periodicity field.

- :data:`*general` denotes the beginning of the periodicity general information field.

- :data:`<per_x>`, :data:`<per_y>` and :data:`<per_z>` are booleans indicating x, y, and z periodicity.

- :data:`<per_dist_x>`, :data:`<per_dist_y>` and :data:`<per_dist_z>` are the periodicity distances along x, y, and z.

- :data:`*seed` denotes the beginning of the periodicity seed field.

- :data:`<secondary_seed_qty>` is the number of secondary seeds.

- :data:`<secondary_seed_id>` is the identifier of a secondary seed.

- :data:`<primary_seed_id>` is the identifier of the primary of a secondary seed.

- :data:`<per_shift_x>`, :data:`<per_shift_y>` and :data:`<per_shift_z>` are the shifts of a secondary seed (or vertex, etc.) relative to its primary, along x, y and z.  The values can be -1, 0 or 1.

- :data:`*vertex` denotes the beginning of the periodicity vertex field.

- :data:`<secondary_vertex_qty>` is the number of secondary vertices.

- :data:`<secondary_vertex_id>` is the identifier of a secondary vertex.

- :data:`<primary_vertex_id>` is the identifier of the primary of a secondary vertex.

- :data:`*edge` denotes the beginning of the periodicity edge field.

- :data:`<secondary_edge_qty>` is the number of secondary edges.

- :data:`<secondary_edge_id>` is the identifier of a secondary edge.

- :data:`<primary_edge_id>` is the identifier of the primary of a secondary edge.

- :data:`<secondary_edge_ori>` is the orientation of the secondary edge with respect to the primary edge: :data:`1` if identical, :data:`-1` if opposite.

- :data:`*face` denotes the beginning of the periodicity face field (for a tessellation of dimension 3).

- :data:`<secondary_face_qty>` is the number of secondary faces.

- :data:`<secondary_face_id>` is the identifier of a secondary face.

- :data:`<primary_face_id>` is the identifier of the primary of a secondary face.

- :data:`<secondary_face_ori>` is the orientation of the secondary face with respect to the primary face: :data:`1` if identical, :data:`-1` if opposite.

- :data:`<number_of_scales>` is the number of scales.

- :data:`<cell1_scale#>` are the identifiers of the cells of the scale-1, scale-2, ... tessellations which the cell belongs to.

.. _tesr_file:

Raster Tessellation File (:file:`.tesr`)
----------------------------------------

Here are details on the :file:`.tesr` file format version :data:`2.1` (format :data:`2.0` was used up to version 4.1.2 and can still be read).  Developers should note that read and write functions are available as :code:`neut_tesr_fscanf` and :code:`neut_tesr_fprintf`, defined in directories :file:`neut/neut_tesr/neut_tesr_fscanf` and :file:`neut/neut_tesr/neut_tesr_fprintf`.  Compared to a tessellation file (:file:`.tess`), a raster tessellation file in enriched by cell morphological properties (centroids, etc.) as they are computed by option :option:`-statcell`, which makes it possible to later retrieve them rapidly.

.. code-block:: plain

  ***tesr
   **format
     <format>
   **general
     <dimension>
     <size_x> <size_y> [<size_z>]
     <voxsize_x> <voxsize_y> [<voxsize_z>]
   [*origin
      <origin_x> <origin_y> [<origin_z>]]
  [**cell
   [<number_of_cells>]
   [*id
      <cell1_id> <cell2_id> ...]
   [*seed
     <seed_id> <seed_x> <seed_y> [<seed_z>] <seed_weight>
      ...]
   [*ori
     <descriptor>
     <cell1_param1> <cell1_param2> ...
     <cell2_param1> <cell2_param2> ...
     ...]
   [*orispread
     <cellid_spread>
     ...]
   [*crysym
     <crysym>]
  ]
   **data
     <data_format>
     <vox1_cell> <vox2_cell> ...
     or
    *file <data_file_name>
  [**oridata
     <descriptor>
     <oridata_format>
     <vox1_param1> <vox1_param2> ...
     <vox2_param1> <vox2_param2> ...
     or
     <descriptor>
     <oridata_format>
    *file <oridata_file_name>
  ]
  [**oridef
     <oridef_format>
     <vox1_oridef> <vox2_oridef> ...
     or
     <oridef_format>
    *file <oridef_file_name>
  ]
  ***end

where

- :data:`***tesr` denotes the beginning of a raster tessellation file.

- :data:`**format` denotes the beginning of the format field.

- :data:`<format>` is the file format, currently `2.0` (character string).

- :data:`**general` denotes the beginning of the general information field.

- :data:`<dimension>` is the dimension of the raster tessellation.

- :data:`<size_x>`, :data:`<size_y>` and :data:`<size_z>` are the raster sizes along the coordinate axes. The number of sizes must match `<dimension>`.

- :data:`<voxsize_x>`, :data:`<voxsize_y>` and :data:`<voxsize_z>` are the voxel (pixel, in 2D) sizes along the coordinate axes.  The number of sizes must match `<dimension>`.

- :data:`*origin` denotes the beginning of an optional origin field.

- :data:`<origin_x>`, :data:`<origin_y>` and :data:`<origin_z>` are the (absolute) coordinates of the origin of the raster tessellation along the coordinate axes.  The number of coordinates must match `dimension`.

- :data:`**cell` denotes the beginning of an optional cell field.

- :data:`<number_of_cells>` is the number of cells.

- :data:`*id` denotes the beginning of an optional identifier field.  If the field is present, the cell identifiers listed under :data:`**data` are supposed to be numbered contiguously from 1 (or 0 in case of void), and their actual identifiers are considered to be the ones provided in the list.  The actual identifiers are used in output files.

- :data:`<cell#_id>` are the actual identifiers of the cells.

- :data:`*seed` denotes the beginning of a seed field.

- :data:`<seed_id>` is the identifier of a seed and ranges from :data:`1` to :data:`<number_of_cells>`.

- :data:`<seed_x>`, :data:`<seed_y>` and :data:`<seed_z>` are the three coordinates of a seed (real numbers).

- :data:`<seed_weight>` is the weight of a seed (real number).

- :data:`*ori` denotes the beginning of an optional crystal orientation field.

- :data:`<descriptor>` is the descriptor used to parametrize the crystal orientations.  See :ref:`rotations_and_orientations` for the list of available descriptors.

- :data:`<cellid_param#>` are the values of the orientation descriptor of cell `id`.

- :data:`*orispread` denotes the beginning of an optional crystal orientation distribution field.

- :data:`<cellid_spread>` is the value of the orientation distribution of cell `id`.

- :data:`*crysym` denotes the beginning of an optional crystal symmetry field.

- :data:`<crysym>` is the crystal symmetry (:data:`triclinic`, :data:`cubic` or :data:`hexagonal`).

- :data:`**data` denotes the beginning of the data field.  Data can be provided in the :file:`.tesr` file or in a separate file, using :data:`*file`, see below.

- :data:`<data_format>` is the format of the data. It can be either :data:`ascii`, :data:`binary8` (8-bit binary), :data:`binary16` (16-bit binary, Little-Endian), :data:`binary16_big` (16-bit binary, Big-Endian), :data:`binary32` (32-bit binary, Little-Endian) or :data:`binary32_big` (32-bit binary, Big-Endian).

- :data:`<voxid_cell>` is the cell voxel :data:`<id>` belongs to.  Voxels must be listed in column-major order (x varying first, y varying second and z varying last).  The cell identifiers should start from 1. Use 0 for voids.

- :data:`*file` denotes the beginning of a file field.

- :data:`<data_file_name>` is the name of a file that contains the data.  It must be located in the same directory as the parent tesr file, or its path relative to the parent tesr file must be provided. Typically, it is a :file:`.raw` file.

- :data:`**oridata` denotes the beginning of the orientation data field.  Data can be provided in the :file:`.tesr` file or in a separate file, using :data:`*file`, see below.

- :data:`<oridata_format>` is the format of the data. It can be either :data:`ascii` or :data:`binary` (double-precision number).

- :data:`<voxid_param#>` are the values of the orientation descriptor of voxel :data:`<id>`. Orientations must be listed in column-major order (x varying first, y varying second and z varying last). Arbitrary orientations can be used for void voxels (:data:`<voxid_cell>` = 0`). These data must be written under format :data:`<data_format>`, in terms of ASCII or binary. In the case of binary format, double-precision numbers are considered.

- :data:`<oridata_file_name>` is the name of a file that contains the orientation data.   It must be located in the same directory as the parent tesr file, or its path relative to the parent tesr file must be provided.  Typically, it is a :file:`.raw` file.

- :data:`**oridef` denotes the beginning of the orientation definition field.  Data can be provided in the :file:`.tesr` file or in a separate file, using :data:`*file`, see below.

- :data:`<oridef_format>` is the format of the orientation definition. It can be either :data:`ascii` or :data:`binary` (8-bit).

- :data:`<voxid_oridef>` indicates whether the orientation of voxel :data:`<id>` is defined or not (:data:`1` or :data:`0`, respectively). Data must be listed in column-major order (x varying first, y varying second and z varying last). The data must be written under format :data:`<oridef_format>`, in terms of ASCII or binary. In the case of binary format, 8-bit binary is considered.

- :data:`<oridef_file_name>` is the name of a file that contains the orientation definitions.   It must be located in the same directory as the parent tesr file, or its path relative to the parent tesr file must be provided.

.. _tesr_data_oridata_oridef:

.. note:: The :data:`**data`, :data:`**oridata` and :data:`**oridef` sections are independent from each other, so that a cell may contain voxels that do not have any orientation, or a voxel that has an orientation may not belong to any cell.

.. _mesh_file:

Mesh File (:file:`.msh`)
------------------------

Here are details on the native :file:`.msh` (adapted from Gmsh's msh format version :data:`2.2`).  Developers should note that read and write functions are available as `neut_msh_fscanf` and `neut_msh_fprintf`, defined in directories :file:`neut/neut_msh/neut_msh_fscanf` and :file:`neut/neut_msh/neut_msh_fprintf`.

.. code-block:: plain

  $MeshFormat
  2.2 <file_type> <data_size>
  $EndMeshFormat
  $MeshVersion
  <mesh_version>
  $EndMeshVersion
  $Domain
  <domain>
  $EndDomain
  $Topology
  <reconstruct_topology>
  $EndTopology
  $Nodes
  <number_of_nodes>
  <node_id> <node_x> <node_y> <node_z>
  ...
  $EndNodes
  $Elements
  <number_of_elements>
  <elt_id> <elt_type> <number_of_tags> <tag1> ... <elt_id_node1> ...
  ...
  $EndElements
  $Periodicity
  <number_of_periodicities>
  <secondary_node_id> <primary_node_id> <per_vect_x> <per_vect_y> <per_vect_z>
  ...
  $EndPeriodicity
  $NSets
  <number_of_nsets>
  <nset1_label>
  <nset_node_nb>
  <nset_node1>
  <nset_node2>
  ...
  <nset2_label>
  ...
  $EndNSets
  $Fasets
  <number_of_fasets>
  <faset1_label>
  <faset_elt_nb>
  <faset_elt_id> <faset_elt_id_node1> ...
  ...
  <faset2_label>
  ...
  $EndFasets
  $NodePartitions
  <number_of_nodes>
  <node_id> <node_partition>
  ...
  $EndNodePartitions
  $PhysicalNames
  <number_of_physical_names>
  <physical_dimension> <physical_id> <physical_name>
  ...
  $EndPhysicalNames
  $ElsetOrientations
  <number_of_elsets> <orientation_descriptor>
  <elset_id> <ori_des1> ...
  ...
  $EndOrientations
  $ElsetCrySym
  <crysym>
  $EndElsetCrySym
  $ElementOrientations
  <number_of_elements> <orientation_descriptor>
  <element_id> ori_des1> ...
  ...
  $EndElementOrientations
  $Groups
  <group_entity>
  <number_of_group_entities>
  <entity_id group>
  ...
  $EndGroups

where

- :data:`$MeshFormat` denotes the beginning of a mesh format field.

- :data:`<file_type>` is equal to :data:`0` for an ASCII file and :data:`1` for a binary file.

- :data:`<data_size>` is an integer equal to the size of the floating point numbers used in the file (= :data:`sizeof (double)`).

- :data:`$EndMeshFormat` denotes the end of a mesh format field.

- :data:`$MeshVersion` denotes the beginning of a mesh version field.

- :data:`<mesh_version>` is the mesh file version (currently :data:`2.2.3`).

- :data:`$EndMeshVersion` denotes the end of a mesh version field.

- :data:`$Domain` denotes the beginning of an optional domain field.

- :data:`<domain>` is the domain.

- :data:`$EndDomain` denotes the end of an optional domain field.

- :data:`$Topology` denotes the beginning of an optional topology field.

- :data:`<reconstruct_topology>` is a boolean indicating whether the topology is to be reconstructed upon parsing or not (use :data:`0` to solve parsing issues).

- :data:`$EndTopology` denotes the end of an optional topology field.

- :data:`$Nodes` denotes the beginning of a node field.

- :data:`<number_of_nodes>` is the number of nodes.

- :data:`<node_id>` is the identifier of a node and ranges from :data:`1` to :data:`<number_of_nodes>`.

- :data:`<node_x>`, :data:`<node_y>` and :data:`<node_z>` are the three coordinates of a node (real numbers).

- :data:`$EndNodes` denotes the end of a node field.

- :data:`$Elements` denotes the beginning of an element field.

- :data:`<number_of_elements>` is the number of elements.

- :data:`<elt_type>` is an integer specifying the type of elements: :data:`15` for a 0D element, :data:`1` for a 1st-order 1D element (2 nodes), :data:`8` for a 2nd-order 1D element (3 nodes), :data:`2` for a 1st-order triangular element (3 nodes), :data:`3` for a 1st-order quadrangular element (4 nodes), :data:`9` for a 2nd-order triangular element (6 nodes), :data:`16` for a 2nd-order quadrangular element (8 nodes), :data:`10` for a 2nd-order quadrangular element (9 nodes), :data:`4` for a 1st-order tetrahedral element (4 nodes), :data:`5` for a 1st-order hexahedral element (8 nodes), :data:`11` for a 2nd-order tetrahedral element (10 nodes), :data:`17` for a 2nd-order hexahedral element (20 nodes), :data:`6` for a 1st-order prismatic element (6 nodes), :data:`18` for a 2nd-order prismatic element (15 nodes).

- :data:`<number_of_tags>` is the number of tags, and :data:`<tag#>` are the tags.  In the general case, the number of tags is equal to 3, the first and second tags are the elset and the third tag is the element partition.  The mesh partition is non-zero only for the higher-dimension elements of a mesh which was previously partitioned.

- :data:`<elt_id_node#>` are the nodes associated to an element.  The number of nodes depends on the element type (`<elt_type>`).

- :data:`$EndElements` denotes the end of an element field.

- :data:`$Periodicity` denotes the beginning of a periodicity field.

- :data:`<number_of_periodicities>` is the number of periodicities.

- :data:`<primary_node_id>` is the identifier of the primary node.

- :data:`<secondary_node_id>` is the identifier of the secondary node.

- :data:`<per_vect_x>` :data:`<per_vect_y>` :data:`<per_vect_z>` are the scaled components of the vector going from the primary node to the secondary node (-1, 0 or 1).

- :data:`$EndPeriodicity` denotes the end of a periodicity field.

- :data:`$NSets` denotes the beginning of an nset field.

- :data:`<number_of_nsets>` is the number of nsets.

- :data:`<nset#_label>` are the labels of the nsets.

- :data:`<nset_node_nb>` is the number of nodes of an nset.

- :data:`<nset_node_id#>` are the identifiers of the nodes of an nset.

- :data:`$EndNSets` denotes the end of an nset field.

- :data:`$Fasets` denotes the beginning of a faset field.

- :data:`<number_of_fasets>` is the number of fasets.

- :data:`<faset#_label>` are the labels of the fasets.

- :data:`<faset_elt_nb>` is the number of elements of a faset.

- :data:`<faset_elt_id>` are the identifiers of the elements of a faset (3D elements adjacent to the boundary).

- :data:`<faset_elt_id_node#>` are the nodes of an element of a faset.

- :data:`$EndFasets` denotes the end of a faset field.

- :data:`$NodePartitions` denotes the beginning of a node partition field.

- :data:`<nodeid_partition>` is the partition of node :data:`<id>` (ranging from 1 to the total number of partitions).

- :data:`$EndNodePartitions` denotes the end of a node partition field.

- :data:`$PhysicalNames` denotes the beginning of a physical name field.

- :data:`<number_of_physical_names>` is the number of physical names.  There are as many names as physical entities, and the physical entities correspond to all tessellation vertices, edges, faces and polyhedra (i.e., mesh 0D, 1D, 2D and 3D elsets).

- :data:`<physical_dimension>` is the dimension of a physical entity and can be equal to 0, 1, 2 or 3.

- :data:`<physical_id>` is the id of a physical entity.  It ranges from 1 to the number of 0D elsets (tessellation vertices) for the 0D entities, 1 to the number of 1D elsets (tessellation edges) for the 1D entities, 1 to the number of 2D elsets (tessellation faces) for the 2D entities and 1 to the number of 3D elsets (tessellation polyhedra) for the 3D entities.

- :data:`<physical_name>` is the name of a physical entity, under the form :data:`<verid>` for 0D elsets (tessellation vertices), :data:`<edgeid>` for 1D elsets (tessellation edges), :data:`<faceid>` for 2D elsets (tessellation faces) and :data:`<polyid>` for 3D elsets (tessellation polyhedra), where :data:`<id>` ranges from 1 to the number of elsets.

- :data:`$EndPhysicalNames` denotes the end of a physical name field.

- :data:`$ElsetOrientations` denotes the beginning of an elset orientation field.

- :data:`$EndElsetOrientations` denotes the end of an elset orientation field.

- :data:`<number_of_elsets>` is the number of elsets.

- :data:`<orientation_descriptor>` is the orientation descriptor.

- :data:`<elset_id>` is the elset id.

- :data:`<ori_des1>`, ... is the orientation, following :data:`<orientation_descriptor>`.

- :data:`$EndElsetOrientations` denotes the end of an elset orientation field.

- :data:`$ElsetCrySym` denotes the beginning of an elset crystal symmetry field.

- :data:`<crysym>` is the crystal symmetry (:data:`triclinic`, :data:`cubic` or :data:`hexagonal`).

- :data:`$EndElsetCrySym` denotes the end of an elset crystal symmetry field.

- :data:`$ElementOrientations` denotes the beginning of an element orientation field.

- :data:`<number_of_elements>` is the number of elements.

- :data:`<element_id>` is the element id.

- :data:`$EndElementOrientations` denotes the end of an element orientation field.

- :data:`$Groups` denotes the beginning of a group field.

- :data:`<group_entity>` is the entity for which groups are defined, which must be :data:`elset`.

- :data:`<number_of_group_entities>` is the number of group entities (number of elsets).

- :data:`<entity_id>` is the id of an entity.

- :data:`<group>` is the group of the entity.

- :data:`$EndGroups` denotes the end of a group field.

.. _simulation_directory:

Simulation Directory (:file:`.sim`)
-----------------------------------

Here are details on the :file:`.sim` simulation directory (the :file:`.sim` extension is entirely optional).  The directory contains *inputs* and *results* on *entities*, over a certain number of *simulation steps*. It is structured as follows:

.. code-block:: console

  simulation.sim
  |-- inputs
  |   |-- job.sh
  |   |-- simulation.config
  |   |-- simulation.msh
  |   `-- simulation.tess
  `-- results
      |-- elts
      |   |-- ori
      |   |   |-- ori.step0
      |   |   |-- ori.step1
      |   |   `-- ...
      |   `-- ...
      |-- nodes
      |   |-- coo
      |   |   |-- coo.step0
      |   |   |-- coo.step1
      |   |   `-- ...
      |   `-- ...
      |-- mesh
      |   |-- coo
      |   |   |-- coo.step0
      |   |   |-- coo.step1
      |   |   `-- ...
      |   `-- ...
      |-- <entity>
      |   |-- ori
  .   |   |   |-- ori.step0
  .   |   |   |-- ori.step1
  .   |   |   `-- ...
  |   |   `-- ...
  |   `-- ...
  `-- [restart]

where

- :file:`inputs` is an input file directory containing the tessellation file (:file:`.tess`, if found in the input directory), the mesh file (:file:`.msh`), the FEPX configuration file (:file:`.config`), and all script files (:file:`*.sh`, likely including a job submission file).

- :file:`results` is the result directory.

- :file:`results/nodes` is the node result directory.

- :file:`results/elts` is the element result directory.

- :file:`results/mesh` is the mesh result directory.

- :file:`results/<entity>` is the :data:`<entity>` result directory.

- :file:`results/*/<res>` is a directory for result :data:`<res>` of an entity. The directory contains one file per simulation step, named :file:`<res>.step<nb>`, where :data:`nb` is the step number, ranging from 0 (for the initial state) to the total number of steps.

- :file:`restart` is the restart directory.  It is present only if :data:`restart` was switched on in FEPX and contains raw FEPX restart files (requires FEPX version 1.2 or higher).

Results can have integer values, real values, vectorial values or tensorial values. In the result files, values for the different entities (nodes, elements, etc.) are written on successive lines, with components written on successive columns (space delimited). The components of a vector, :data:`v`, are written as :data:`v1` :data:`v2` :data:`v3`. The components of a symmetrical tensor, :data:`t`, are written using Voigt notation, as :data:`t11` :data:`t22` :data:`t33` :data:`t23` :data:`t31` :data:`t12`. The components of a skwe-symmetrical tensor, :data:`t`, are written using :data:`t12` :data:`t13` :data:`t23`. The components of a non-symmetrical tensor, :data:`t`, are written as :data:`t11` :data:`t12` :data:`t13` :data:`t21` :data:`t22` :data:`t23` :data:`t31` :data:`t32` :data:`t33`.

The directory also contains a hidden file, :file:`.sim`, containing information on the simulation and the content of the simulation directory.  This file is only for internal use and is formatted as follows:

.. code-block:: plain

  ***sim
   **format
     <format>
   **input
    *tess
     <tess_file>
    *tesr
     <tesr_file>
    *msh
     <msh_file>
    *ori
     <ori_file>
    *bcs
     <bcs_file>
    *phase
     <phase_file>
    *config
     <config_file>
   **general
     <cell_nb> <node_nb> <elt_nb> <elset_nb> <part_nb>
    *orides
     <orientation_descriptor>
   **entity <entity>                                     \
    *member                                              |
     <member_nb>                                         |
     <member1> <member2> ...                             | section repeated for each entity
    *result                                              |
     <result_nb>                                         |
     <result1> <result2> ...                             /
   **orispace
    *rodrigues <space_file>
   **step
     <step_nb>
  ***end

.. _multiscale_cell_file:

Multiscale Cell File
--------------------

A multiscale cell file provides cell-by-cell values for a multiscale tessellation and can be loaded using :data:`msfile(<file_name>)` [#msfile]_. The file contains, for each cell, its *multiscale identifier*, :data:`mid`, and the value(s).  A *cell multiscale identifier* (`mid`) is a character string identifying a cell at a specific scale.  For a given cell, C, :data:`mid` combines the identifiers of the cells that C belongs to, at successive scales, to its own :data:`id`, separated by :data:`::`.  For a 1-scale tessellation, :data:`mid` equals :data:`id`.  For a 2-scale tessellation composed of :math:`2 \times 3` cells, the :data:`mid` values are equal to :data:`1::1`, :data:`1::2`, :data:`1::3`, :data:`2::1`, :data:`2::2` and :data:`2::3`.  The domain (which can be considered as a cell at scale 0), :data:`mid` is nil.  An example of a multiscale cell file that could be used to define the numbers of cells of a 3-scale tessellation is::

  2
  1 2
  2 4
  1::1 3
  1::2 4
  2::1 5
  2::2 6
  2::3 7
  2::4 8

The file could be used in :data:`-T` as :data:`-n msfile(<file_name>)::msfile(<file_name>)::msfile(<file_name>)`.  The first instance of :data:`msfile(<file_name>)` reads the number of scale-1 cells in line 1, the second instance of :data:`msfile(<file_name>)` reads the number of scale-2 cells in lines 2--3, and the third  instance of :data:`msfile(<file_name>)` reads the number of scale-3 cells in lines 4--7.

.. _data_file:

Data File
---------

A data file contains a number of data to be assigned to entities (seeds, cells, points, etc.).  It must contain the expected number of data.  It is a general, simply-formatted ASCII file that contains a fixed number of data per entity, and the data can be integers, real numbers or character strings.  A real number can have an arbitrary number of digits, but the decimal mark must be :option:`.`.  The data can be separated from each other by spaces, tabulators or newlines (any number as well as arbitrary combinations of them are supported).  However, a good practice is to format the file with the data of one entity per line.  An example of a data file containing 5 values is::

  2.1235
  5.9564
  2.2547
  8.2515
  0.5874

An example of a data file containing 5 crystal orientations is::

  -0.308690123821   -0.220269180809    0.059535375094
  -0.135706415585   -0.060152174145    0.393001001388
  -0.109314835214    0.385212719832    0.395088636982
   0.303226023477    0.088490873532   -0.075079986629
   0.120106944081   -0.211545552905   -0.286940925534

An example of a data file containing 5 colors is::

  red
  green
  blue
  yellow
  magenta

.. _position_file:

Position File
-------------

A position file contains the coordinates of a number of points.  The file must contain 1 coordinate per point in 1D, 2 coordinates per point in 2D and 3 coordinates per point in 3D.  While the dimension can be known from the context in which the file is read, it can also be specified by appending :data:`:dim` to the name of the position file, where :data:`dim` is the dimension.  A coordinate can be an integer or real number. A real number can have an arbitrary number of digits, but the decimal mark must be :option:`.`.  The coordinates can be separated from each other by spaces, tabulators or newlines (any number as well as arbitrary combinations of them are supported).  However, a good practice is to format the file with one point per line.  An example of a position file containing 5 points in 3D is::

  2.1235 9.4544 5.2145
  5.9564 3.6884 9.2145
  2.2547 3.2658 8.2514
  8.2515 9.4157 2.9454
  0.5874 4.2848 2.4874

.. note:: A :ref:`position_file` is a :ref:`data_file` with as many data per entity as dimensions.

.. [#msfile] As of version 3.5.0, :data:`msfile(<file_name>)` should be preferred over :data:`file(<file_name>)`.
