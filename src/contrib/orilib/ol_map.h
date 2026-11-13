/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef OL_MAPP
#define OL_MAPP

#include "ol_config.h"
#include "ol_des.h"
#include "ol_cal.h"
#include "ol_set.h"

struct OL_MAP
{
  double stepsize;
  size_t xsize;
  size_t ysize;
  double** gmap;

  unsigned int **id;
  double ***q;

  char* crysym;
};
typedef struct OL_MAP OL_MAP;

extern struct OL_MAP ol_map_alloc (size_t, size_t, double, char*);
extern void ol_map_free (struct OL_MAP *);


extern void ol_map_memcpy (struct OL_MAP, struct OL_MAP *);
extern void ol_map_submap (struct OL_MAP, int, int, size_t, size_t,
			   struct OL_MAP *);
extern void ol_map_submap_center (struct OL_MAP, int, int, size_t, size_t,
				  struct OL_MAP *);
extern void ol_map_set (struct OL_MAP, struct OL_SET *);

extern void ol_map_filter_kuwahara_iter (struct OL_MAP, size_t,
                                         struct OL_MAP *, double *);
extern void ol_map_filter_kuwahara (struct OL_MAP, size_t,
                                    struct OL_MAP *, double *);
extern void ol_map_filter_kuwahara_region (struct OL_MAP,
                                               double *, double *);



#endif /* OL_MAPP */
#ifndef OL_MAPx
#define OL_MAPx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#endif /* OL_MAPx */

#ifdef __cplusplus
}
#endif
