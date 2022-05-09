// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
  Common.h

  Stephane Rehel

  General C/C++ Definitions
*/

#ifndef __Common_h
#define __Common_h

/////////////////////////////////////////////////////////////////////////////

#include <assert.h>

#ifdef NDEBUG
#define assert_result(ex) (ex)
#else
#define assert_result(ex) assert(ex)
#endif

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifdef DBL_IS_FLOAT
  typedef float DBL;
#else
  typedef double DBL;
#endif


typedef unsigned char       BYTE;

inline int sqr( int k ) { return k * k; }
inline unsigned int sqr( unsigned int k ) { return k * k; }
inline short sqr( short k ) { return k * k; }
inline unsigned short sqr( unsigned short k ) { return k * k; }
inline long sqr( long k ) { return k * k; }
inline unsigned long sqr( unsigned long k ) { return k * k; }
inline float sqr( float k ) { return k * k; }

// cf. /usr/include/g++/builtin.h
#if ! _G_MATH_H_INLINES /* hpux and SCO define this in math.h */
inline double sqr( double k ) { return k * k; }
#endif

inline float cube( float k ) { return k * k * k; }
inline double cube( double k ) { return k * k * k; }


// we do _not_ use template since gcc never expand inline
// templates :-(
#define COMMON_SWAP_DEFINE(T) inline void swap( T& t1, T& t2 ) \
                                          { T t= t1; t1= t2; t2= t; }
COMMON_SWAP_DEFINE(unsigned char)
COMMON_SWAP_DEFINE(char)
COMMON_SWAP_DEFINE(short int)
COMMON_SWAP_DEFINE(short unsigned int)
COMMON_SWAP_DEFINE(int)
COMMON_SWAP_DEFINE(unsigned int)
COMMON_SWAP_DEFINE(long)
COMMON_SWAP_DEFINE(unsigned long)
COMMON_SWAP_DEFINE(float)
COMMON_SWAP_DEFINE(double)
COMMON_SWAP_DEFINE(void*)
#undef COMMON_SWAP_DEFINE

#ifndef NOMINMAX
#define COMMON_MIN_MAX_DEFINE(T) \
        inline T min( T t1, T t2 ) { return (t1<t2) ? t1 : t2; } \
        inline T max( T t1, T t2 ) { return (t1<t2) ? t2 : t1; }
COMMON_MIN_MAX_DEFINE(unsigned char)
COMMON_MIN_MAX_DEFINE(char)
COMMON_MIN_MAX_DEFINE(short int)
COMMON_MIN_MAX_DEFINE(short unsigned int)
COMMON_MIN_MAX_DEFINE(int)
COMMON_MIN_MAX_DEFINE(unsigned int)
COMMON_MIN_MAX_DEFINE(long)
COMMON_MIN_MAX_DEFINE(unsigned long)
COMMON_MIN_MAX_DEFINE(float)
COMMON_MIN_MAX_DEFINE(double)
#undef COMMON_MIN_MAX_DEFINE
#endif // ifndef NOMINMAX

#define COMMON_SAMESIGN_DEFINE(T) \
        inline boolean sameSign( const T d1, const T d2 ) \
          { return (d1>=(T)(0)) ? (d2>=(T)(0)) : (d2<=(T)(0)); }
COMMON_SAMESIGN_DEFINE(signed char)
COMMON_SAMESIGN_DEFINE(short int)
COMMON_SAMESIGN_DEFINE(int)
COMMON_SAMESIGN_DEFINE(long)
COMMON_SAMESIGN_DEFINE(float)
COMMON_SAMESIGN_DEFINE(double)
#undef COMMON_SAMESIGN_DEFINE

/*
#ifdef NWARN

#define wassert(ignore) ((void) 0)

#else

#include <stdio.h>

#define wassert(expression)  \
  ((void) ((expression) ? 0 : __wassert(#expression, __FILE__, __LINE__)))
#define __wassert(expression, file, lineno)  \
  (fprintf( stderr, "%s:%u: failed assertion `%s'\n", \
                    file, lineno, expression ), \
   fflush(stderr), 0)

#endif
*/

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Common_h
