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
  VCartouche.h

  Stephane Rehel
  August 6 1996
*/

#ifndef __VCartouche_h
#define __VCartouche_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __SystemWindow_h
#include "graphics/SystemWindow.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

#ifndef __VWindowEventHandler_h
#include "VWindowEventHandler.h"
#endif

class KWDialog;
class VKillButton;
class VDialog;
class VWindow;

/////////////////////////////////////////////////////////////////////////////

class VCartouche: public SystemWindow,
                  public VWindowEventHandler,
                  public VWidgetEventHandler
{
protected:
  SystemWindow* parent;
  VWindow* window;

  boolean frame;
  boolean killIcon;
  MLString title;
  int titleHeight;
  VKillButton* killButton;
  VDialog* dialog;
  KWDialog* kwDialog;

public:
  VCartouche( SystemWindow* _parent, KWDialog* _kwDialog = 0 );
  virtual ~VCartouche();

  IRectangle getClientArea() const;
  IVector probeCartoucheSize( const IVector& clientSize, boolean frame ) const;

  void create( int x, int y, int width, int height,
               boolean _frame, boolean _killIcon,
               const MLString& _title );

  void drawCartouche();

  VWindow* getWindow() const
    {
    return window;
    }

  VDialog* getDialog() const
    {
    return dialog;
    }

  // newSize = new dialogSize
  void setDialogSize( const IVector& newSize );

  // double buffered
  void drawCartoucheAndDialog();

private:
  virtual boolean VWindowEvent( VWindow*, MLEvent& );
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
  virtual boolean handleEvent( const MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VCartouche_h
