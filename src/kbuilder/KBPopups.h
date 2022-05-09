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
  KBPopups.h

  Stephane Rehel

  October 19 1997
*/

#ifndef __KBPopups_h
#define __KBPopups_h

class MLPopup;
class MLButtonsPanel;
class KBModule;

/////////////////////////////////////////////////////////////////////////////

class KBPopups
{
protected:
  KBModule* kb;

public:
  // File
  int new_dialog;
  int open_dialog;
  int save_dialog;
  int save_dialog_as;

  // Dialog
  int edit_dialog;

  // Widget
  int edit_widget;

  // Prefs
  int edit_prefs;

  // Test
  int test;

  // Quit
  int quit;

  struct
    {
    MLPopup* file;
    MLPopup* edit_dialog;
    MLPopup* edit_widget;
    MLPopup* edit_prefs;
    MLPopup* test;
    MLPopup* quit;
    } popups;

public:
  KBPopups( KBModule* _kb );

  virtual ~KBPopups();

  void init( MLButtonsPanel* _buttons );

  void updateFlags();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KBPopups_h
