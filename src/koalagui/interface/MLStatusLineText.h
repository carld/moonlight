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
  MLStatusLineText.h

  Stephane Rehel

  July 1 1997
*/

#ifndef __MLStatusLineText_h
#define __MLStatusLineText_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __IStack_h
#include "tools/IStack.h"
#endif

#ifndef __MLStatusLineItem_h
#include "MLStatusLineItem.h"
#endif

class Color;
class MLString;
class MLStatusMessage;

/////////////////////////////////////////////////////////////////////////////

class MLStatusLineText: public MLStatusLineItem
{
protected:
  boolean text_offset_if_pressed;

  IStack<MLStatusMessage> messages;

public:
  MLStatusLineText( MLModule* module,
                    MLStatusLine* _line, int n_chars,
                    MLStatusLineItem::Side _side,
                    boolean _expandable = false );
  virtual ~MLStatusLineText();

  void printMessage( const MLString& _message, int color = 1 );

  void pushMessage( const MLString& _message, int color = 1 );

  void popMessage();

protected:
  MLString cutMessage( const MLString& _msg ) const;

  IPoint getTextPosition() const;

  void _draw( int x_offset );

  virtual void _draw();
  virtual void draw();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusLineText_h
