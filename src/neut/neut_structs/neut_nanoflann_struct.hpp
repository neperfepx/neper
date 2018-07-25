#pragma once

#include"contrib/nanoflann/nanoflann.hpp"

struct NFCLOUD
{
  struct pt
  {
    double p[3];
  };

    std::vector < pt > pts;

  // Must return the number of data points
  inline size_t kdtree_get_point_count () const
  {
    return pts.size ();
  }

  // Returns the dim'th component of the idx'th point in the class:
  // Since this is inlined and the "dim" argument is typically an immediate value, the
  //  "if/else's" are actually solved at compile time.
  inline double kdtree_get_pt (const size_t idx, int dim) const
  {
    return pts[idx].p[dim];
  }

  // Optional bounding-box computation: return false to default to a standard bbox computation loop.
  //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
  //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
  template < class BBOX > bool kdtree_get_bbox (BBOX & /* bb */ ) const
  {
    return false;
  }
};

typedef
  nanoflann::KDTreeSingleIndexDynamicAdaptor <
  nanoflann::L2_Simple_Adaptor < double,
  NFCLOUD >,
  NFCLOUD,
  3 >
  NFTREE;
