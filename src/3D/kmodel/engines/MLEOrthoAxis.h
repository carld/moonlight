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
  MLEOrthoAxis.h

  Stephane Rehel

  December 1 1996
*/

#ifndef __MLEOrthoAxis_h
#define __MLEOrthoAxis_h

#ifndef __MLEOrtho_h
#include "MLEOrtho.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLEOrthoAxis: public MLEOrtho
{
public:
  enum AXIS
    {
    X_AXIS= 1,
    Y_AXIS,
    Z_AXIS
    };
protected:
  AXIS axis;

public:
  MLEOrthoAxis( ModelCanvas* _canvas, AXIS _axis );
  virtual ~MLEOrthoAxis();

protected:
  virtual void write( MLWFileBlock& block );
  virtual void read( MLRFileBlock& block );

  virtual void resetCamera();
  virtual void resetAll();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLEOrthoAxis_h
