/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

extern int ut_color_name_valid (char*);
extern int ut_color_name_rgb (char *, int *);
extern int name2rgb (char *, int *);
extern void rgb_intensity (int *, double, int *);
extern int ut_color_scheme_val_color (char*, double, double, double, int*);
extern void ut_color_palette (int qty, int** palette, double min, double max);
extern void ut_color_palette_0208 (int*** pp, int *pqty);

#ifdef __cplusplus
}
#endif
