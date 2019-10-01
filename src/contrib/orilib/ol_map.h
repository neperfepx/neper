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
  int ***rgb;
};
typedef struct OL_MAP OL_MAP;

extern struct OL_MAP ol_map_alloc (size_t, size_t, double, char*);
extern void ol_map_free (struct OL_MAP);

extern int ol_map_fscanf (FILE *, struct OL_MAP *);
extern int ol_map_fprintf (FILE *, struct OL_MAP, char *);
extern int ol_map_fscanf_ch5 (FILE *, struct OL_MAP *);
extern int ol_map_fscanf_ch5_rad (FILE *, struct OL_MAP *);
extern int ol_map_fprintf_ch5 (FILE *, struct OL_MAP, char *);
extern int ol_map_fscanf_ch5_short (FILE *, struct OL_MAP *);
extern int ol_map_fscanf_ch5_short_rad (FILE *, struct OL_MAP *);

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

#ifdef HAVE_PNG

extern void ol_map_png (struct OL_MAP, char *);
extern void ol_map_rgb_png (unsigned int, unsigned int, int ***, char *);

#endif

extern void ol_map_vect_ipfrgb (struct OL_MAP, double *, int *);
extern void ol_ipfweight_rgb (double *, int *);
extern void ol_ipfrgb_legend (size_t, size_t*, int ****);

extern void ol_map_transformparam (struct OL_MAP, double **, size_t *,
				   size_t *, double *);
extern void ol_map_transform (struct OL_MAP, double **, size_t, size_t,
			      double *, struct OL_MAP *);

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
#ifndef OL_MAPx
#define OL_MAPx

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"ut.h"
#define isNaN(x) ((x) != (x))
#include <png.h>
#include "ol_glo.h"
#include "ol_des.h"
#include "ol_set.h"
#endif /* OL_MAPx */
#ifdef __cplusplus
}
#endif
