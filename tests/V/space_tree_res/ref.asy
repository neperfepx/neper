
// A simple tree drawing module contributed by adarovsky
// See example treetest.asy

real treeNodeStep = 0.5cm;
real treeLevelStep = 1cm;
real treeMinNodeWidth = 2cm;

struct TreeNode {
  TreeNode parent;
  TreeNode[] children;

  frame content;

  pair pos;
  real adjust;
}

void add( TreeNode child, TreeNode parent )
{
  child.parent = parent;
  parent.children.push( child );
}

TreeNode makeNode( TreeNode parent = null, frame f )
{
  TreeNode child = new TreeNode;
  child.content = f;
  if( parent != null ) {
    add( child, parent );
  }
  return child;
}

TreeNode makeNode( TreeNode parent = null, Label label )
{
  frame f;
  roundbox( f, label, 2, 2, linewidth(0.5), FillDraw(orange+white));
  return makeNode( parent, f );
}


real layout( int level, TreeNode node )
{
  if( node.children.length > 0 ) {
    real width[] = new real[node.children.length];
    real curWidth = 0;

    for( int i = 0; i < node.children.length; ++i ) {
      width[i] = layout( level+1, node.children[i] );

      node.children[i].pos = (curWidth + width[i]/2,
                              -level*treeLevelStep);
      curWidth += width[i] + treeNodeStep;
    }

    real midPoint = ( sum( width )+treeNodeStep*(width.length-1)) / 2;
    for( int i = 0; i < node.children.length; ++i ) {
      node.children[i].adjust = - midPoint;
    }

    return max( (max(node.content)-min(node.content)).x,
                sum(width)+treeNodeStep*(width.length-1) );
  }
  else {
    return max( treeMinNodeWidth, (max(node.content)-min(node.content)).x );
  }
}

void drawAll( TreeNode node, frame f )
{
  pair pos;
  if( node.parent != null )
    pos = (node.parent.pos.x+node.adjust, 0);
  else
    pos = (node.adjust, 0);
  node.pos += pos;

  node.content = shift(node.pos)*node.content;
  add( f, node.content );


  if( node.parent != null ) {
    path p = point(node.content, N)--point(node.parent.content,S);
    draw( f, p, currentpen );
  }

  for( int i = 0; i < node.children.length; ++i )
    drawAll( node.children[i], f );
}

void draw( TreeNode root, pair pos )
{
  frame f;

  root.pos = (0,0);
  layout( 1, root );

  drawAll( root, f );

  add(f,pos);
}
treeNodeStep = 0.4cm;
treeLevelStep = 1.8cm;

TreeNode root = makeNode("\tt n2-id1.sim");

TreeNode child1 = makeNode(root, "\tt inputs");
TreeNode child1_content = makeNode(child1, "\tt \begin{tabular}{c}simulation.tess \\\end{tabular}");
TreeNode child2 = makeNode(root, "\tt results");
TreeNode child21 = makeNode(child2, "\tt cell");
TreeNode child211 = makeNode(child21, "\tt diameq");
TreeNode child2111 = makeNode(child211, "\tt \begin{tabular}{c}diameq.step0 \\\end{tabular}");


draw (root, (0, 0));
