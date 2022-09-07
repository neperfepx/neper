/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fprintf_.h"

void
neut_sim_fprintf (char *dir, struct SIM Sim, char *mode)
{
  int i, j, qty;
  char *oldfilename = ut_string_paste (dir, "/report");
  char *filename = ut_string_paste (dir, "/.sim");
  FILE *file = ut_file_open (filename, mode);

  if (ut_file_exist (oldfilename))
    remove (oldfilename);

  fprintf (file, "***sim\n");
  fprintf (file, " **format\n");
  fprintf (file, "   1.0\n");
  fprintf (file, " **input\n");
  if (Sim.tess)
    fprintf (file, "  *tess\n   %s\n", Sim.tess);
  if (Sim.tesr)
    fprintf (file, "  *tesr\n   %s\n", Sim.tesr);
  if (Sim.msh)
    fprintf (file, "  *msh\n   %s\n", Sim.msh);
  if (Sim.bcs)
    fprintf (file, "  *bcs\n   %s\n", Sim.msh);
  if (Sim.ori)
    fprintf (file, "  *ori\n   %s\n", Sim.msh);
  if (Sim.phase)
    fprintf (file, "  *phase\n   %s\n", Sim.msh);
  if (Sim.config)
    fprintf (file, "  *config\n   %s\n", Sim.config);

  fprintf (file, " **general\n");
  fprintf (file, "   %d %d %d %d %d\n",
                 Sim.CellQty, Sim.NodeQty, Sim.EltQty, Sim.ElsetQty,
                 Sim.PartQty);
  if (Sim.OriDes)
  {
    fprintf (file, "  *orides\n");
    fprintf (file, "   %s\n", Sim.OriDes);
  }

  for (i = 0; i < Sim.EntityQty; i++)
  {
    fprintf (file, " **entity %s\n", Sim.Entities[i]);

    if (!neut_sim_entity_internal (Sim.Entities[i]))
    {
      fprintf (file, "  *type\n");
      fprintf (file, "   %s\n", Sim.EntityType[i]);

      if (Sim.EntityMemberQty[i])
      {
        fprintf (file, "  *member\n");
        fprintf (file, "   %d", Sim.EntityMemberQty[i]);
        if (Sim.EntityMemberExpr[i])
        {
          for (j = 0; j < Sim.EntityMemberQty[i]; j++)
            fprintf (file, " %s", Sim.EntityMemberExpr[i][j]);
        }
        fprintf (file, "\n");
      }
    }

    if (Sim.EntityResQty[i])
    {
      fprintf (file, "  *result\n");
      fprintf (file, "   %d\n  ", Sim.EntityResQty[i]);
      if (Sim.EntityResQty[i])
        for (j = 0; j < Sim.EntityResQty[i]; j++)
        {
          fprintf (file, " %s", Sim.EntityRes[i][j]);
          if (Sim.EntityResExpr[i][j] && strlen (Sim.EntityResExpr[i][j]))
            fprintf (file, "%s%s", NEUT_SEP_DEP, Sim.EntityResExpr[i][j]);
        }
      fprintf (file, "\n");
    }
  }

  if (Sim.OriSpace)
  {
    fprintf (file, " **orispace\n");
    fprintf (file, "   %s\n", Sim.OriSpace);
  }

  if (Sim.StepQty > 0)
  {
    fprintf (file, " **step\n");
    fprintf (file, "   %d\n", Sim.StepQty);

    qty = ut_array_1d_int_valnb (Sim.StepState + 1, Sim.StepQty, 0);
    if (qty)
    {
      fprintf (file, "  *printed\n");
      fprintf (file, "   %d\n", Sim.StepQty - qty);
      fprintf (file, "  ");
      for (i = 1; i <= Sim.StepQty; i++)
        if (!Sim.StepState[i])
          fprintf (file, " %d", i);
      fprintf (file, "\n");
    }
  }

  if (Sim.RestartId || Sim.RestartFiles)
  {
    fprintf (file, " **restart\n");
    fprintf (file, "   %d %d\n", Sim.RestartId, Sim.RestartFiles);
  }

  fprintf (file, "***end\n");

  ut_file_close (file, filename, mode);
  ut_free_1d_char (&oldfilename);
  ut_free_1d_char (&filename);

  return;
}

void
neut_sim_fprintf_asy (char *dir, struct SIM Sim, char *mode)
{
  int i, j, k;
  char *oldfilename = ut_string_paste (dir, "/report");
  char *filename = NULL, *filename2 = NULL;
  if (strstr (dir, ".sim"))
  {
    ut_string_string (dir, &filename);
    ut_string_fnrs (filename, ".sim", ".asy", 1);
  }
  else
    filename = ut_string_addextension (dir, "asy");

  ut_string_string (filename, &filename2);
  ut_string_fnrs (filename2, ".asy", ".pdf", 1);

  char *command = ut_alloc_1d_char (strlen (filename) + 100);
  FILE *file = ut_file_open (filename, "W");
  ut_file_openmessage (filename2, mode);

  if (ut_file_exist (oldfilename))
    remove (oldfilename);

  fprintf (file, "\n");
  fprintf (file, "// A simple tree drawing module contributed by adarovsky\n");
  fprintf (file, "// See example treetest.asy\n");
  fprintf (file, "\n");
  fprintf (file, "real treeNodeStep = 0.5cm;\n");
  fprintf (file, "real treeLevelStep = 1cm;\n");
  fprintf (file, "real treeMinNodeWidth = 2cm;\n");
  fprintf (file, "\n");
  fprintf (file, "struct TreeNode {\n");
  fprintf (file, "  TreeNode parent;\n");
  fprintf (file, "  TreeNode[] children;\n");
  fprintf (file, "\n");
  fprintf (file, "  frame content;\n");
  fprintf (file, "\n");
  fprintf (file, "  pair pos;\n");
  fprintf (file, "  real adjust;\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");
  fprintf (file, "void add( TreeNode child, TreeNode parent )\n");
  fprintf (file, "{\n");
  fprintf (file, "  child.parent = parent;\n");
  fprintf (file, "  parent.children.push( child );\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");
  fprintf (file, "TreeNode makeNode( TreeNode parent = null, frame f )\n");
  fprintf (file, "{\n");
  fprintf (file, "  TreeNode child = new TreeNode;\n");
  fprintf (file, "  child.content = f;\n");
  fprintf (file, "  if( parent != null ) {\n");
  fprintf (file, "    add( child, parent );\n");
  fprintf (file, "  }\n");
  fprintf (file, "  return child;\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");
  fprintf (file, "TreeNode makeNode( TreeNode parent = null, Label label )\n");
  fprintf (file, "{\n");
  fprintf (file, "  frame f;\n");
  fprintf (file, "  roundbox( f, label, 2, 2, linewidth(0.5), FillDraw(orange+white));\n");
  fprintf (file, "  return makeNode( parent, f );\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");
  fprintf (file, "\n");
  fprintf (file, "real layout( int level, TreeNode node )\n");
  fprintf (file, "{\n");
  fprintf (file, "  if( node.children.length > 0 ) {\n");
  fprintf (file, "    real width[] = new real[node.children.length];\n");
  fprintf (file, "    real curWidth = 0;\n");
  fprintf (file, "\n");
  fprintf (file, "    for( int i = 0; i < node.children.length; ++i ) {\n");
  fprintf (file, "      width[i] = layout( level+1, node.children[i] );\n");
  fprintf (file, "\n");
  fprintf (file, "      node.children[i].pos = (curWidth + width[i]/2,\n");
  fprintf (file, "                              -level*treeLevelStep);\n");
  fprintf (file, "      curWidth += width[i] + treeNodeStep;\n");
  fprintf (file, "    }\n");
  fprintf (file, "\n");
  fprintf (file, "    real midPoint = ( sum( width )+treeNodeStep*(width.length-1)) / 2;\n");
  fprintf (file, "    for( int i = 0; i < node.children.length; ++i ) {\n");
  fprintf (file, "      node.children[i].adjust = - midPoint;\n");
  fprintf (file, "    }\n");
  fprintf (file, "\n");
  fprintf (file, "    return max( (max(node.content)-min(node.content)).x,\n");
  fprintf (file, "                sum(width)+treeNodeStep*(width.length-1) );\n");
  fprintf (file, "  }\n");
  fprintf (file, "  else {\n");
  fprintf (file, "    return max( treeMinNodeWidth, (max(node.content)-min(node.content)).x );\n");
  fprintf (file, "  }\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");
  fprintf (file, "void drawAll( TreeNode node, frame f )\n");
  fprintf (file, "{\n");
  fprintf (file, "  pair pos;\n");
  fprintf (file, "  if( node.parent != null )\n");
  fprintf (file, "    pos = (node.parent.pos.x+node.adjust, 0);\n");
  fprintf (file, "  else\n");
  fprintf (file, "    pos = (node.adjust, 0);\n");
  fprintf (file, "  node.pos += pos;\n");
  fprintf (file, "\n");
  fprintf (file, "  node.content = shift(node.pos)*node.content;\n");
  fprintf (file, "  add( f, node.content );\n");
  fprintf (file, "\n");
  fprintf (file, "\n");
  fprintf (file, "  if( node.parent != null ) {\n");
  fprintf (file, "    path p = point(node.content, N)--point(node.parent.content,S);\n");
  fprintf (file, "    draw( f, p, currentpen );\n");
  fprintf (file, "  }\n");
  fprintf (file, "\n");
  fprintf (file, "  for( int i = 0; i < node.children.length; ++i )\n");
  fprintf (file, "    drawAll( node.children[i], f );\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");
  fprintf (file, "void draw( TreeNode root, pair pos )\n");
  fprintf (file, "{\n");
  fprintf (file, "  frame f;\n");
  fprintf (file, "\n");
  fprintf (file, "  root.pos = (0,0);\n");
  fprintf (file, "  layout( 1, root );\n");
  fprintf (file, "\n");
  fprintf (file, "  drawAll( root, f );\n");
  fprintf (file, "\n");
  fprintf (file, "  add(f,pos);\n");
  fprintf (file, "}\n");

  fprintf (file, "treeNodeStep = 0.4cm;\n");
  fprintf (file, "treeLevelStep = 1.8cm;\n");
  fprintf (file, "\n");
  fprintf (file, "TreeNode root = makeNode(\"\\tt %s/\");\n", Sim.simdir);
  fprintf (file, "\n");
  fprintf (file, "TreeNode child1 = makeNode(root, \"\\tt inputs/\");\n");

  int id = 0;
  if (Sim.tess)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.tess);
  if (Sim.tesr)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.tesr);
  if (Sim.msh)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.msh);
  if (Sim.bcs)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.bcs);
  if (Sim.ori)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.ori);
  if (Sim.phase)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.phase);
  if (Sim.config)
    fprintf (file, "TreeNode child1%d = makeNode(child1, \"\\tt %s\");\n", ++id, Sim.config);

  int print_results = 0;
  char *child1 = ut_alloc_1d_char (100);
  char *child2 = ut_alloc_1d_char (100);
  for (i = 0; i < Sim.EntityQty; i++)
    if (Sim.EntityResQty[i])
    {
      if (!print_results)
      {
        fprintf (file, "TreeNode child2 = makeNode(root, \"\\tt results/\");\n");
        print_results = 1;
      }

      sprintf (child1, "child2%d", i + 1);

      fprintf (file, "TreeNode %s = makeNode(child2, \"\\tt %s/\");\n",
               child1, Sim.Entities[i]);

      for (j = 0; j < Sim.EntityResQty[i]; j++)
      {
        sprintf (child2, "%s%d", child1, j + 1);

        fprintf (file, "TreeNode %s = makeNode(%s, \"\\tt %s/\");\n",
                 child2, child1, Sim.EntityRes[i][j]);

        fprintf (file, "TreeNode %s1 = makeNode(%s, \"\\tt \\begin{tabular}{c}", child2, child2);
        for (k = 0; k <= Sim.StepQty; k++)
          fprintf (file, "%s.step%d \\\\", Sim.EntityRes[i][j], k);
        fprintf (file, "\\end{tabular}\");\n");
      }
      fprintf (file, "\n");
    }
  ut_free_1d_char (&child1);
  ut_free_1d_char (&child2);

  /*
  if (Sim.OriSpace)
  {
    fprintf (file, " **orispace\n");
    fprintf (file, "   %s\n", Sim.OriSpace);
  }
  */

  fprintf (file, "\n");
  fprintf (file, "draw (root, (0, 0));\n");
  ut_file_close (file, filename, "W");
  ut_file_closemessage (filename2, mode);

  ut_free_1d_char (&oldfilename);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&filename2);
  ut_free_1d_char (&command);

  return;
}

void
neut_sim_verbose (struct SIM Sim)
{
  int i;

  ut_print_message (0, 3, "Input files:");
  if (Sim.tess)
    printf (" tess");
  if (Sim.tesr)
    printf (" tesr");
  if (Sim.msh)
    printf (" msh");
  if (Sim.bcs)
    printf (" bcs");
  if (Sim.ori)
    printf (" ori");
  if (Sim.phase)
    printf (" phase");
  if (Sim.config)
    printf (" config");
  printf ("\n");

  if (Sim.RestartId || Sim.RestartFiles)
  {
    ut_print_message (0, 3, "Restart id       : %d\n", Sim.RestartId);
    ut_print_message (0, 3, "Restart files    : %s\n", Sim.RestartFiles ? "yes" : "no");
  }

  if (Sim.CellQty)
    ut_print_message (0, 3, "Cell number      : %d\n", Sim.CellQty);

  if (Sim.NodeQty)
    ut_print_message (0, 3, "Node number      : %d\n", Sim.NodeQty);

  if (Sim.EltQty)
    ut_print_message (0, 3, "Element number   : %d\n", Sim.EltQty);

  if (Sim.ElsetQty)
    ut_print_message (0, 3, "Elset number     : %d\n", Sim.ElsetQty);

  if (Sim.PartQty)
    ut_print_message (0, 3, "Partition number : %d\n", Sim.PartQty);

  ut_print_message (0, 3, "Step number      : %d\n", Sim.StepQty);

  if (Sim.OriSpace)
    ut_print_message (0, 3, "Orientation space: %s\n", Sim.OriSpace);

  for (i = 0; i < Sim.EntityQty; i++)
    neut_sim_verbose_results (Sim.Entities[i], Sim.EntityRes[i], Sim.EntityResQty[i]);

  return;
}
