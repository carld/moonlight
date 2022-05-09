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
  VScrollingDialogs.h

  Stephane Rehel
  August 24 1996
*/

#ifndef __VScrollingDialogs_h
#define __VScrollingDialogs_h

#ifndef __IList_h
#include "tools/IList.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __SystemWindow_h
#include "graphics/SystemWindow.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

class VScrollingCartouche;
class VVertBar;
class VWindow;
class KWDialog;

/////////////////////////////////////////////////////////////////////////////

class VScrollingDialogs: public SystemWindow,
                         public VWidgetEventHandler
{
protected:
  static int scrollBarWidth;

private:
  struct SDialog
    {
    VScrollingCartouche* cartouche;
    MLString name;
    };
  IList<SDialog> dialogs;

  VWindow* window;
  VVertBar* scrollBar;

  boolean updatePositionFlag;
  int total_height;
  int y;

public:
  static int getScrollBarWidth();

  VScrollingDialogs();
  virtual ~VScrollingDialogs();

  boolean create( SystemWindow* parent, int x, int y, int width, int height );

  virtual void map( boolean yes = true );

  void append( KWDialog* d );
  void append( VScrollingCartouche* c );
  void remove( const MLString& name );
  KWDialog* find( const MLString& name );

  void set_y( int new_y );
  void delta_y( int delta_y );

  void drawDialogsImages();

  void postDrawScrollBar();

private:
  // handles private widget events from VScrollBar
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );

  virtual boolean handleEvent( const MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef  __VScrollingDialogs_h
