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
  MLModes.C

  Stephane Rehel

  December 9 1996
*/

#include "MLModes.h"

#include "MLMode.h"
#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

static const int max_modes= 100;

/////////////////////////////////////////////////////////////////////////////

MLModes::MLModes( MLModule* _module )
{
  module= _module;

  modes= new MLMode* [ max_modes + 1 ];
  for( int i= 0; i <= max_modes; ++i )
    modes[i]= 0;

  nModes= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLModes::~MLModes()
{
  for( int i= 0; i <= max_modes; ++i )
    {
    delete modes[i];
    modes[i]= 0;
    }

  delete modes;
  modes= 0;

  nModes= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLMode* MLModes::get( int id ) const
{
  for( int i= 1; i <= nModes; ++i )
    {
    MLMode* mode= modes[i];
    if( mode == 0 )
      continue;

    if( mode->getID() == id )
      return mode;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLModes::add( int id, MLMode* mode )
{
  if( mode == 0 )
    return;

  if( get(id) != 0 )
    return;

  if( nModes >= max_modes )
    return;

  for( int i= 1; i <= nModes; ++i )
    if( modes[i] == mode )
      return;

  ++nModes;
  modes[nModes]= mode;

  mode->mode_id= id;
}

/////////////////////////////////////////////////////////////////////////////

/*
boolean MLModes::handleKey( Key key, boolean pressed )
{
  if( nModes == 0 )
    return false; // no mode available

  MLMode* m= findMode(key);
  if( m == 0 )
    return false;

  MLMode* previousMode= currentMode;
  if( ! pressed )
    {
    // released
    if( m != currentMode )
      return true;

    if( currentMode != 0 && module != 0 )
      module->leaveMode(currentMode);
    currentMode= 0;
    }
   else
    {
    // pressed
    currentMode= m;
    }

  if( module != 0 )
    {
    if( currentMode == 0 )
      {
      module->setModeMessage("");
      }
     else
      {
      module->setModeMessage(currentMode->message, pressed ? 1 : 0 );
      if( currentMode != previousMode )
        module->enterMode(currentMode);
      }
    }

  return true;
}
*/

/////////////////////////////////////////////////////////////////////////////
