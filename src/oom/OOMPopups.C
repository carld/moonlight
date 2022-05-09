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
  OOMPopups.C

  Stephane Rehel

  November 4 1997
*/

#include "interface/MLPopup.h"
#include "interface/MLButtonsPanel.h"

#include "OOMModule.h"

#include "OOMPopups.h"

/////////////////////////////////////////////////////////////////////////////

OOMPopups::OOMPopups( OOMModule* _oom )
{
  oom= _oom;
}

/////////////////////////////////////////////////////////////////////////////

OOMPopups::~OOMPopups()
{}

/////////////////////////////////////////////////////////////////////////////

// called by OOMModule::init()
void OOMPopups::init( MLButtonsPanel* buttons )
{
  MLPopup* p;

  // File
  p= new MLPopup(oom);
  popups.file= p;

  choice1= p->add("Choice1");

  // Components
  p= new MLPopup(oom);
  popups.components= p;

  new_class= p->add("New class");

  // Quit
  p= new MLPopup(oom);
  popups.quit= p;
  quit= p->getPopupLabel();

  buttons->setButton( 1,  "File", popups.file );
  buttons->setButton( 2,  "Components", popups.components );
  buttons->setButton( 20, "Quit", popups.quit );
}

/////////////////////////////////////////////////////////////////////////////

void OOMPopups::updateFlags()
{}

/////////////////////////////////////////////////////////////////////////////
