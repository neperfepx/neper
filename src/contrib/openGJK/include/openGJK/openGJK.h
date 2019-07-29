/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
 *                                   #####        # #    #                *
 *       ####  #####  ###### #    # #     #       # #   #                 *
 *      #    # #    # #      ##   # #             # #  #                  *
 *      #    # #    # #####  # #  # #  ####       # ###                   *
 *      #    # #####  #      #  # # #     # #     # #  #                  *
 *      #    # #      #      #   ## #     # #     # #   #                 *
 *       ####  #      ###### #    #  #####   #####  #    #                *
 *                                                                        *
 *  This file is part of openGJK.                                         *
 *                                                                        *
 *  openGJK is free software: you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by *
 *   the Free Software Foundation, either version 3 of the License, or    *
 *   any later version.                                                   *
 *                                                                        *
 *   openGJK is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See The        *
 *   GNU General Public License for more details.                         *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *   along with Foobar.  If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                        *
 *       openGJK: open-source Gilbert-Johnson-Keerthi algorithm           *
 *            Copyright (C) Mattia Montanari 2018 - 2019                  *
 *              http://iel.eng.ox.ac.uk/?page_id=504                      *
 *                                                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
 *                                                                        *
 *  This is the header file for the openGJK.c file. It defines the        *
 *	 openGJK function and its structures.							      *
 *                                                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

#ifndef __OPENGJK_H__
#define __OPENGJK_H__

#include <stdio.h>
#include <stdlib.h>
#include "math.h"


/**
 * @brief Macro that implements the CompareSign function (see paper).
 */
 #define SAMESIGN( a, b ) ( (a>0) == (b>0) )


/**
 * @brief Structure of a body.
 */
struct bd {
  int numpoints;    /**< Number of points defining the body.            */
  double  **coord;  /**< Pointer to pointer to the points' coordinates. */
  double  s [3];    /**< Support mapping computed last.                 */
};




/**
 * @brief Structure for a simplex.
 */
struct simplex {
  int    nvrtx   ;       /**< Number of simplex's vertices. 			*/  
  double vrtx    [4][3]; /**< Coordinates of simplex's vertices. 		*/ 
  int    wids    [4];    /**< Label of the simplex's vertices. 			*/  
  double lambdas [4];    /**< Barycentric coordiantes for each vertex.  */
};




/**
 * @brief The GJK algorithm which returns the minimum distance between 
 * two bodies.
 */
extern double gjk( struct bd, struct bd, struct simplex * ) ;

#endif