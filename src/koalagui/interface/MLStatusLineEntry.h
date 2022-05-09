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
  MLStatusLineEntry.h

  Stephane Rehel

  May 24 1998
*/

#ifndef __MLStatusLineEntry_h
#define __MLStatusLineEntry_h

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __Key_h
#include "graphics/Key.h"
#endif

#ifndef __MLStatusLineItem_h
#include "MLStatusLineItem.h"
#endif

class Color;
class MLStatusLineEntryCallback;

/////////////////////////////////////////////////////////////////////////////

class MLStatusLineEntry: public MLStatusLineItem
{
protected:
  MLString msg;
  int msg_offset;
  int cursor_position;

  boolean highlighted;
  boolean entering;

  Color active_bg_color, unactive_bg_color;

  int history_position;
  int history_size;
  MLString** history;

  MLString last_log;

  MLStatusLineEntryCallback* callback;

public:
  MLStatusLineEntry( MLModule* module,
                     MLStatusLine* _line, int n_chars,
                     MLStatusLineItem::Side _side,
                     boolean _expandable = false );

  virtual ~MLStatusLineEntry();

  void setCallback( MLStatusLineEntryCallback* _callback );

protected:
  const MLString& getLastLog() const;

public:
  void setLastLog( const MLString& _last_log );

  int getCharsWidth() const;

  virtual void _draw();

  virtual void draw();

  virtual boolean eventHandler( MLWindow*, MLEvent& );

  void go_right();
  void go_left();
  void go_end();
  void go_home();
  void delete_char();
  void history_up();
  void history_down();
  void do_enter();

  boolean handleKey( Key& key );

  // sent by MLStatusLine::handleMLKeyboard()
  // return false if the key event must be handle by the interface
  virtual boolean handleMLKeyboard( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStatusLineEntry_h
