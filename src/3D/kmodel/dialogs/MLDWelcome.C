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
  MLDWelcome.C

  Stephane Rehel

  August 9 1997
*/

#include "kw/KWDialog.h"

#include "kmodel/ModelModule.h"
#include "kmodel/MLModelVersion.h"
#include "kmodel/gdialogs/MLDReadText.h"

#include "MLDWelcome.h"
#include "WelcomeDlg.h"

/////////////////////////////////////////////////////////////////////////////

MLDWelcome::MLDWelcome( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= WelcomeDlg::data;
  name= "Welcome";

  MLScrollingDialog::updateFlags= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLDWelcome::~MLDWelcome()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDWelcome::update()
{
  kwDialog->setText( WelcomeDlg::version, MLModelVersion::version() );
  kwDialog->setText( WelcomeDlg::cpu,    MLModelVersion::build_cpu() );
  kwDialog->setText( WelcomeDlg::vendor, MLModelVersion::build_vendor() );
  kwDialog->setText( WelcomeDlg::os,     MLModelVersion::build_os() );

// hack
kwDialog->drawDialog();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDWelcome::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    case KWEvent::SELECT:
      {
//      if( event.widget == WelcomeDlg::close )
//        {
//        MLScrollingDialog::remove();
//        break;
//        }

      if( event.widget == WelcomeDlg::introduction )
        {
        MLDReadText intro(model,intro_text);
        intro.run();
        break;
        }
      if( event.widget == WelcomeDlg::license )
        {
        MLDReadText license(model,license_text);
        license.run();
        break;
        }

      if( event.widget == WelcomeDlg::modes_list )
        {
        model->createDialog("ModesList");
        break;
        }
      }
    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
