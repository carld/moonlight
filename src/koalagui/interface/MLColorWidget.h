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
  MLColorWidget.h

  Stephane Rehel

  March 15 1997
*/

#ifndef __MLColorWidget_h
#define __MLColorWidget_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

class KWDialog;
class KWEvent;

/////////////////////////////////////////////////////////////////////////////

class MLColorWidget
{
  friend class MLModule;

protected:
  Color color;
  double color_factor;
  KWDialog* dialog;

  enum { R= 0, G= 1, B= 2 };
  enum { H= 0, S= 1, V= 2 };

  unsigned int labels[3]; // rgb
  unsigned int valuesBar[3]; // rgb
  unsigned int radio_rgb, radio_hsv;
  unsigned int area;
  unsigned int factor;

public:
  MLColorWidget( KWDialog* kw,
                 unsigned int label_r,
                 unsigned int label_g,
                 unsigned int label_b,
                 unsigned int valuesBar_r,
                 unsigned int valuesBar_g,
                 unsigned int valuesBar_b,
                 unsigned int _radio_rgb,
                 unsigned int _radio_hsv,
                 unsigned int _area,
                 unsigned int _factor = 0 );

  virtual ~MLColorWidget();

  void set( const Color& _color, double _factor = 1. );

  const Color& get() const
    {
    return color;
    }

  boolean inRGBColorModel();

  void setRGBColorModel();
  void setHSVColorModel();

  boolean colorCallback( KWEvent& );

  void setAvailable( boolean yes );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLColorWidget_h
