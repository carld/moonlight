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
  OOMDWelcome.C

  Stephane Rehel

  October 19 1997
*/

#include "kw/KWDialog.h"

#include "oom/OOMModule.h"

#include "OOMDWelcome.h"
#include "OOMWelcomeDlg.h"

/////////////////////////////////////////////////////////////////////////////

OOMDWelcome::OOMDWelcome( OOMModule* _oom ):
  MLScrollingDialog(_oom)
{
  oom= _oom;
  data= OOMWelcomeDlg::data;
  name= "Welcome";

//  MLScrollingDialog::updateFlags |= OOMModule::REFRESH_;
}

/////////////////////////////////////////////////////////////////////////////

OOMDWelcome::~OOMDWelcome()
{}

/////////////////////////////////////////////////////////////////////////////

void OOMDWelcome::update()
{
}

/////////////////////////////////////////////////////////////////////////////

boolean OOMDWelcome::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::SELECT:
      {
      if( event.widget == OOMWelcomeDlg::close )
        {
        MLScrollingDialog::remove();

        return true;
        }
      }
    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
