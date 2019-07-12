/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*                                    #####        # #    #                *
*        ####  #####  ###### #    # #     #       # #   #                 *
*       #    # #    # #      ##   # #             # #  #                  *
*       #    # #    # #####  # #  # #  ####       # ###                   *
*       #    # #####  #      #  # # #     # #     # #  #                  *
*       #    # #      #      #   ## #     # #     # #   #                 *
*        ####  #      ###### #    #  #####   #####  #    #                *
*                                                                         *
*           Mattia Montanari    |   University of Oxford 2019             *
* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
*                                                                         *
* This is the header file for the openGJK.c file. It defines the openGJK  *
*	function and it two important structures: bd and simplex.  	          *
*                                                                         *
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