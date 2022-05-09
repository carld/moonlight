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
   VValueEdit.h

   Stephane Rehel
   August 13 1996
*/

#ifndef __VValueEdit_h
#define __VValueEdit_h

#ifndef __VEdit_h
#include "VEdit.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VValueEdit: public VEdit
{
protected:
  double minValue, maxValue;
  int nDigits; // number of digits after the decimal-point character

  double value;

public:
  VValueEdit( VWindow* _parent );

  virtual ~VValueEdit();

  virtual VWidget::TYPE type() const
    {
    return VWidget::VALUEEDIT;
    }

  void create( int x, int y, int _width,
               double _minValue,
               double _maxValue,
               int _nDigits,
               double _value );

  void setRanges( double _minValue, double _maxValue, int _nDigits );

  double getValue() const
    {
    return value;
    }

  virtual void changeValue( double _value );
  boolean parseValue();

protected:
  virtual boolean handleKey( Key key );

public:
  virtual void setCurrent( boolean yes );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VValueEdit_h

