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
  mlgl.C

  Stephane Rehel

  July 18 1997
*/

#include <math.h>

#include "tools/Color.h"
#include "tools/MLGammaCorrection.h"

#include "mlgl.h"

#include "GraphicsSystem.h"

/////////////////////////////////////////////////////////////////////////////

// gamma corrected
void mlColor3f( double r, double g, double b )
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( graphicsSystem != 0 )
    {
    MLGammaCorrection* gc= graphicsSystem->getGammaCorrection();
    if( gc->hasCorrection() )
      {
      r= gc->do_f(r);
      g= gc->do_f(g);
      b= gc->do_f(b);
      }
    }

  glColor3f(r,g,b); // gamma corrected
}

/////////////////////////////////////////////////////////////////////////////

// gamma corrected
void mlColor( const Color& c )
{
  mlColor3f( c.r(), c.g(), c.b() );
}

/////////////////////////////////////////////////////////////////////////////
