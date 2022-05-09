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
  VScrollingCartouche.h

  Stephane Rehel
  August 24 1996
*/

#ifndef __VScrollingCartouche_h
#define __VScrollingCartouche_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __VWindow_h
#include "VWindow.h"
#endif

#ifndef __VWidgetEvent_h
#include "VWidgetEvent.h"
#endif

#ifndef __VWindowEventHandler_h
#include "VWindowEventHandler.h"
#endif

class KWDialog;
class VSKillButton;
class VDialog;
class SystemWindow;
class MLFont;

/////////////////////////////////////////////////////////////////////////////

class VScrollingCartouche: public VWindow,
                           public VWidgetEventHandler
{
  friend class VScrollingDialogs;
protected:
  SystemWindow* parent;

  boolean frame;
  boolean killIcon;
  MLString title;
  MLFont* titleFont;
  int titleHeight;
  VSKillButton* killButton;
  VDialog* dialog;
  KWDialog* kwDialog;
  VScrollingDialogs* scrollingDialogs; // may be null
                                      // set by VScrollingDialogs in append()
public:
  VScrollingCartouche( SystemWindow* _parent, KWDialog* _kwDialog = 0 );
  virtual ~VScrollingCartouche();

  IRectangle getClientArea() const;
  static IVector probeCartoucheSize( const IVector& clientSize, boolean frame );

  void create( int x, int y, int width, int height,
               boolean _frame, boolean _killIcon,
               const MLString& _title );

  void drawCartouche();

  VDialog* getDialog() const
    {
    return dialog;
    }

  KWDialog* getKWDialog() const
    {
    return kwDialog;
    }

  void drawImage();

private:
  virtual boolean handlePrivateWidgetEvent( const VWidgetEvent& event );
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VScrollingCartouche_h
