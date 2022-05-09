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
  MLGLPixmap.C

  Stephane Rehel
  July 22 1996
*/

#include "GL/opengl.h"

#include "GraphicsSystem.h"
#include "MLGLPixmap.h"

/////////////////////////////////////////////////////////////////////////////

MLGLPixmap::MLGLPixmap()
{
  size= IVector(1,1);
}

/////////////////////////////////////////////////////////////////////////////

MLGLPixmap::~MLGLPixmap()
{}

/////////////////////////////////////////////////////////////////////////////

void MLGLPixmap::pixelMode()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(size.x()), 0., GLfloat(size.y()), -1., 1. );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

#ifdef __WINDOWS__
  glTranslatef( 0.5, 0.375, 0. );
#else
  glTranslatef( 0.375, 0.375, 0. );
#endif

  glShadeModel(GL_FLAT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
}

/////////////////////////////////////////////////////////////////////////////

void MLGLPixmap::currentPixel()
{
  current();
  pixelMode();
}

/////////////////////////////////////////////////////////////////////////////

void MLGLPixmap::print( MLWindow* dest, int dest_x, int dest_y )
{
  print( dest, 0, 0, size.x(), size.y(), dest_x, dest_y );
}

/////////////////////////////////////////////////////////////////////////////

void MLGLPixmap::print( MLGLPixmap* dest, int dest_x, int dest_y )
{
  print( dest, 0, 0, size.x(), size.y(), dest_x, dest_y );
}

/////////////////////////////////////////////////////////////////////////////

// static
MLGLPixmap* MLGLPixmap::newMLGLPixmap()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  assert( graphicsSystem != 0 );
  return graphicsSystem->newMLGLPixmap();
}

/////////////////////////////////////////////////////////////////////////////

