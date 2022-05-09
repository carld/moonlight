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
   VButton.h

   Creation: SR, July 26th, 1995
   Revisions:
     + SR July 5 1996
*/

#ifndef __VButton_h
#define __VButton_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __VButtonBase_h
#include "VButtonBase.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class VButton: public VButtonBase
{
private:
  HotCharMessage hcm;
  MLString message;
  MLFont* buttonFont;
  boolean hasEnterPixmap;

public:
  VButton( VWindow* _parent );

  virtual ~VButton()
    {}

  void create( int x, int y, const MLString& _message, int iwidth = 0 );
  void create( int x, int y, int _width, int _height,
               const MLString& _message );

  const MLString& getMessage() const
    {
    return hcm.message;
    }
  void changeMessage( const MLString& newMessage );
  void setEnterPixmap( boolean yes );

  virtual VWidget::TYPE type() const
    {
    return VWidget::BUTTON;
    }

  virtual char getHotChar() const;

protected:
  void initMessage( const MLString& _message );
  void drawMessage( const IVector& displ = IVector(0,0) );
  void drawButton();

  virtual boolean handleEvent( MLEvent& event );
  virtual boolean handleKey( Key key );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VButton_h

