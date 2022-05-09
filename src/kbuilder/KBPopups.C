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
  KBPopups.C

  void KBModule::initPopups();

  Stephane Rehel

  October 19 1997
*/

#include "interface/MLPopup.h"
#include "interface/MLButtonsPanel.h"

#include "KBModule.h"
#include "KBPopups.h"

/////////////////////////////////////////////////////////////////////////////

KBPopups::KBPopups( KBModule* _kb )
{
  kb= _kb;
}

/////////////////////////////////////////////////////////////////////////////

KBPopups::~KBPopups()
{}

/////////////////////////////////////////////////////////////////////////////

void KBPopups::init( MLButtonsPanel* buttons )
{
  MLPopup* p;

  // File
  p= new MLPopup(kb);
  popups.file= p;

  new_dialog= p->add("New");
  open_dialog= p->add("Open...");
  save_dialog= p->add("Save");
  save_dialog_as= p->add("Save as...");

  // Dialog
  p= new MLPopup(kb);
  popups.edit_dialog= p;
  edit_dialog= p->getPopupLabel();

  // Widget
  p= new MLPopup(kb);
  popups.edit_widget= p;
  edit_widget= p->getPopupLabel();

  // Prefs
  p= new MLPopup(kb);
  popups.edit_prefs= p;
  edit_prefs= p->getPopupLabel();

  // Test
  p= new MLPopup(kb);
  popups.test= p;
  test= p->getPopupLabel();

  // Quit
  p= new MLPopup(kb);
  popups.quit= p;
  quit= p->getPopupLabel();

  buttons->setButton( 1, "File",   popups.file );
  buttons->setButton( 2, "Dialog", popups.edit_dialog );
  buttons->setButton( 3, "Widget", popups.edit_widget );
  buttons->setButton( 4, "Prefs",  popups.edit_prefs );
  buttons->setButton( 5, "Test",   popups.test );
  buttons->setButton( 20,"Quit",   popups.quit );
}

/////////////////////////////////////////////////////////////////////////////

void KBPopups::updateFlags()
{}

/////////////////////////////////////////////////////////////////////////////

