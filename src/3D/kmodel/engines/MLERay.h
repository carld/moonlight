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
  MLERay.h

  Stephane Rehel

  January 18 1997
*/

#ifndef __MLERay_h
#define __MLERay_h

#ifndef __MLEPerspective_h
#include "MLEPerspective.h"
#endif

class MLCanvasButton;
class MLCanvasPopup;
class MLPicture;

/////////////////////////////////////////////////////////////////////////////

class MLERay: public MLEPerspective
{
protected:
  int x1, y1, x2, y2;
  int creation_width, creation_height;
  boolean window_valid;
  MLPicture* picture;

  MLCanvasButton* raytrace_button; // MLERay= creator but not destructor

  MLCanvasPopup* qualityPopup;
  struct
    {
    int use_local;
    int trace_shadows;
    int refine_shadows;
    int trace_reflections;
    int trace_transmissions;
    }
    quality_popup;
  int options_popup_select_all;

//  MLCanvasButton* select_all_button;
//  MLCanvasButton* main_camera_button;

public:
  MLERay( ModelCanvas* _canvas );
  virtual ~MLERay();

  virtual boolean handleAction( MLAction* action );

  virtual void refreshScreen( unsigned what );

  // derives MLEngine3D::draw()
  virtual void draw();

  void raytrace();

protected:
  void moveRaytracedWindow( int dx, int dy );
  void resizeRaytracedWindow( int dx, int dy );

  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetAll();

public:
  virtual void updatePopupsFlags();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLERay_h
