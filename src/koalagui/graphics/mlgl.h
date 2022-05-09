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
  mlgl.h

  Stephane Rehel

  July 3 1996
*/

#ifndef __mlgl_h
#define __mlgl_h

#ifndef __ml_gl_wrapper_h
#include "GL/opengl.h"
#endif

/////////////////////////////////////////////////////////////////////////////

inline void mlFront()
{
   glDrawBuffer(GL_FRONT);
}

/////////////////////////////////////////////////////////////////////////////

inline void mlBack()
{
   glDrawBuffer(GL_BACK);
}

/////////////////////////////////////////////////////////////////////////////

inline void mlBlack()
{
  glColor3i(0,0,0);
}

/////////////////////////////////////////////////////////////////////////////

inline void mlWhite()
{
  glColor3f(1.,1.,1.);
}

/////////////////////////////////////////////////////////////////////////////

#define mlMiddleGray() \
{                                                     \
  extern unsigned char middle_gray;                   \
  glColor3ub(middle_gray,middle_gray,middle_gray);    \
}

/////////////////////////////////////////////////////////////////////////////

#define mlDarkGray() \
{                                                     \
  extern unsigned char gray_level_1;                  \
  glColor3ub(gray_level_1,gray_level_1,gray_level_1); \
}

/////////////////////////////////////////////////////////////////////////////

#define mlLightGray() \
{                                                     \
  extern unsigned char gray_level_2;                  \
  glColor3ub(gray_level_2,gray_level_2,gray_level_2); \
}

/////////////////////////////////////////////////////////////////////////////

// gamma corrected
void mlColor3f( double r, double g, double b );

class Color;

// gamma corrected
void mlColor( const Color& c );

/////////////////////////////////////////////////////////////////////////////

inline void mlLine( int x1, int y1, int x2, int y2 )
{
  glBegin(GL_LINES);
  glVertex2i(x1,y1);
  glVertex2i(x2,y2);
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

inline void mlFrame( int x1, int y1, int x2, int y2 )
{
  glBegin(GL_LINE_LOOP);
  glVertex2i(x1,y1);
  glVertex2i(x2,y1);
  glVertex2i(x2,y2);
  glVertex2i(x1,y2);
  glEnd();
}

/////////////////////////////////////////////////////////////////////////////

inline void mlRect( int x1, int y1, int x2, int y2 )
{
  glRecti( x1, y1, x2, y2 );

  // MesaGL bug or is it really OpenGL way?
  mlFrame( x1, y1, x2, y2 );
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __mlgl_h

