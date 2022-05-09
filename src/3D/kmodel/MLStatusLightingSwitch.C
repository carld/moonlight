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
  MLStatusLightingSwitch.C

  Stephane Rehel

  July 5 1997
*/

#include "MLStatusLightingSwitch.h"

#include "graphics/mlgl.h"
#include "graphics/MLFont.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLSmallSpherePicture.h"

#include "interface/MLStatusLine.h"

#include "ModelModule.h"

/////////////////////////////////////////////////////////////////////////////

MLStatusLightingSwitch::MLStatusLightingSwitch( MLModule* module,
                                                MLStatusLine* _line,
                                                int n_chars,
                                        MLStatusLineItem::Side _side,
                                        boolean _expandable /* = false */ ):
  MLStatusLineButton( module,
                      _line,
                      n_chars + 1,
                      _side,
                      _expandable )
{
  lightingStatus= 0;

  spheres.disabled= new MLSmallSpherePicture( Color(1,1,1)*0.75 );
  spheres.asleep= new MLSmallSpherePicture( Color(0,1,0) );
  spheres.working1= new MLSmallSpherePicture( Color(1,0.25,0) );
  spheres.working2= new MLSmallSpherePicture( Color(1,1,0) );

  status_sphere= 0;

  setStatus( int(ModelModule::LS_DISABLED), false );
}

/////////////////////////////////////////////////////////////////////////////

MLStatusLightingSwitch::~MLStatusLightingSwitch()
{
  delete spheres.disabled; spheres.disabled= 0;
  delete spheres.asleep; spheres.asleep= 0;
  delete spheres.working1; spheres.working1= 0;
  delete spheres.working2; spheres.working2= 0;

  status_sphere= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLightingSwitch::setStatus( int _lightingStatus,
                                        boolean refresh /* = true */ )
{
  if( _lightingStatus == lightingStatus )
    return;

  lightingStatus= _lightingStatus;

  switch(lightingStatus)
    {
    case ModelModule::LS_ASLEEP:
      {
      status_sphere= spheres.asleep;
      break;
      }

    case ModelModule::LS_WORKING1:
      {
      status_sphere= spheres.working1;
      break;
      }

    case ModelModule::LS_WORKING2:
      {
      status_sphere= spheres.working2;
      break;
      }

    case ModelModule::LS_DISABLED:
    default:
      {
      status_sphere= spheres.disabled;
      break;
      }
    }

  if( refresh )
    {
    if( window == 0 )
      return;
    if( ! window->mapped() || module->isIconic() )
      return;

    window->currentPixel();
    mlFront();

    IPoint position= MLStatusLineButton::getTextPosition();

    if( status_sphere != 0 )
      status_sphere->draw( position.x(), position.y()-1 );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLStatusLightingSwitch::_draw()
{
  IPoint position= MLStatusLineButton::getTextPosition();
  int offset= 0;
  if( status_sphere != 0 )
    {
    offset+= status_sphere->getWidth() + 1;
    status_sphere->draw( position.x(), position.y()-1 );
    }

  MLStatusLineText::_draw(offset);
}

/////////////////////////////////////////////////////////////////////////////
