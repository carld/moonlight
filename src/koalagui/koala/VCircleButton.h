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
   VCircleButton.h

   Creation: SR, August 9th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VCircleButton_h
#define __VCircleButton_h

#ifndef __Color8_h
#include "tools/Color8.h"
#endif

#ifndef __VButtonBase_h
#include "VButtonBase.h"
#endif

#ifndef __VWidget_h
#include "VWidget.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VCircleButton: public VButtonBase
{
public:
  VCircleButton( VWindow* _parent );

  virtual ~VCircleButton();

  void create( int upperleftCorner_x, int upperLeftCorner_y );

  void create( const IPoint& upperleftCorner );

  virtual VWidget::TYPE type() const
    {
    return VWidget::CIRCLEBUTTON;
    }

protected:
  void drawButton();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VCircleButton_h

