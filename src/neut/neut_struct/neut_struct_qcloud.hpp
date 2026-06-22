#pragma once

#ifndef STRUCTQCLOUD_H
#define STRUCTQCLOUD_H

#include"nanoflann.hpp"

struct QCLOUD
{
  struct pt
  {
    double q[4];
    double theta; // radian
    double weight;
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
    return pts[idx].q[dim];
  }

  // Optional bounding-box computation: return false to default to a standard bbox computation loop.
  //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
  //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
  template < class BBOX > bool kdtree_get_bbox (BBOX & /* bb */ ) const
  {
    return false;
  }
};

typedef nanoflann::KDTreeSingleIndexAdaptor < nanoflann::L2_Simple_Adaptor < double, QCLOUD >, QCLOUD, 4
> my_kd_tree_t;

#endif // STRUCTQCLOUD_H
