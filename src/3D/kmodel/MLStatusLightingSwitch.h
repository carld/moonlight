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
  MLStatusLightingSwitch.h

  Stephane Rehel

  July 5 1997
*/

#ifndef __MLStatusLightingSwitch_h
#define __MLStatusLightingSwitch_h

#ifndef __MLStatusLineButton_h
#include "interface/MLStatusLineButton.h"
#endif

class MLSmallSpherePicture;

/////////////////////////////////////////////////////////////////////////////

class MLStatusLightingSwitch: public MLStatusLineButton
{
protected:
  struct
    {
    MLSmallSpherePicture* disabled;
    MLSmallSpherePicture* asleep;
    MLSmallSpherePicture* working1;
    MLSmallSpherePicture* working2;
    } spheres;

  MLSmallSpherePicture* status_sphere;

  int lightingStatus;

public:
  MLStatusLightingSwitch( MLModule* module,
                          MLStatusLine* _line, int n_chars,
                          MLStatusLineItem::Side _side,
                          boolean _expandable = false );
  virtual ~MLStatusLightingSwitch();

  void setStatus( int _lightingStatus, boolean refresh = true );

private:
  virtual void _draw();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusLightingSwitch_h
