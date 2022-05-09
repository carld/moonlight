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
   VBlankButton.h

   Creation: SR, July 28th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VBlankButton_h
#define __VBlankButton_h

#ifndef __VButtonBase_h
#include "VButtonBase.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VBlankButton: public VButtonBase
{
public:
  VBlankButton( VWindow* _parent );

  virtual ~VBlankButton();

  void create( int x, int y, int width, int height );
  void create( const IPoint& p1, const IPoint& p2 );

  virtual VWidget::TYPE type() const
    {
    return VWidget::BLANKBUTTON;
    }

  virtual void drawButton();
protected:
  void _drawButton();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VBlankButton_h

