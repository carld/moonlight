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
   VChooser.h

   Stephane Rehel
   August 17 1996
*/

#ifndef __VChooser_h
#define __VChooser_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IList_h
#include "tools/IList.h"
#endif

#ifndef __VButtonBase_h
#include "VButtonBase.h"
#endif

#ifndef __MLEventGrabber_h
#include "graphics/MLEventGrabber.h"
#endif

class SystemWindow;

/////////////////////////////////////////////////////////////////////////////

class VChooser: public VButtonBase, public MLEventGrabber
{
private:
  MLFont* buttonFont;

  MLString currentMessage, drawnMessage;
  IList<MLString> messages;

private: // popup info
  MLEventGrabber* previousGrabber;
  boolean grabbing;
  SystemWindow* popupSW;
  VWindow* popup;
  int messageHeight;
  int mouseIndex;
  boolean entered;
  unsigned long pressTime;

public:
  VChooser( VWindow* _parent );

  virtual ~VChooser();

  void create( int x, int y, int _width, int _height );

  const MLString& getMessage() const
    {
    return currentMessage; // may be ""
    }

  virtual VWidget::TYPE type() const
    {
    return VWidget::CHOOSER;
    }

  void changeMessage( const MLString& message );

  int getInteger(); // may return 0

  boolean appendMessage( const MLString& text );
  // 1 <= i <= nElements
  boolean setMessage( int i, const MLString& text );
  // 1 <= i <= nElements+1
  boolean insertMessage( int i, const MLString& text );
  // 1 <= i <= nElements
  boolean deleteMessage( int i );
  void setInteger( int i );

  int getLength() const;
  // 1 <= i <= nElements
  const MLString& getMessage( int i );

  int getMessagePosition( const MLString& message );

protected:
  void drawMessage( const IVector& displ = IVector(0,0) );
  void drawButton();
  void drawChooserPopup();

  virtual boolean handleEvent( MLEvent& event );
  virtual boolean handleKey( Key key );

private:
  void go_grabbing();
  void end_grabbing();
  int getMouseIndex( const IPoint& mousePos );
  void changeMouseIndex( int newIndex );
  void drawPopupMessage( int i, boolean highlight );
  virtual boolean eventGrabber( MLWindow*, MLEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VChooser_h

