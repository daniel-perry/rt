/* Daniel Perry
 * for cs6620 - spring 2005
 * started 27jan05
 */

/* util.h is to declare several constants and enum types 
 *  that I would like to use throughout the system.
 * These diffent things are to provide consistency 
 *   accross platforms (like maxfloat).
 */

#ifndef _UTIL_H
#define _UTIL_H

//#define USE_FUNCTION_

//#include <values.h> //for MINFLOAT or #include <float.h>
//#include <iostream>

#define MYMAX 1e9//MAXFLOAT //FLT_MAX
#define MYMIN 1e-8
//linux: FLT_MAX
// Sun: MAXFLOAT 


// define for two sided lighting (default is one-sided lighting).
//#define _TWO_SIDED_LIGHTING_OP

/* use fabs,fmax,fmin
template <class T>
inline T myMax( T a , T b )
{
  return a>b?a:b;
}

template <class T>
inline T myMin( T a , T b )
{
  return a<b?a:b;
}

template <class T>
inline T myAbs(T a)
{
  return a>=0?a:-a;
}
*/

#endif
