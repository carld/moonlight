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
  MLDRadiosity.h

  Stephane Rehel

  August 6 1997
*/

#include <math.h>

#include "kmodel/ModelModule.h"

#include "MLDRadiosity.h"
#include "RadiosityDlg.h"

#include "kw/KWDialog.h"

#include "scene/lighting/MLGlobalLighting.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLDRadiosity::MLDRadiosity( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= RadiosityDlg::data;
  name= "Radiosity";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_ILLUM;
}

/////////////////////////////////////////////////////////////////////////////

MLDRadiosity::~MLDRadiosity()
{}

/////////////////////////////////////////////////////////////////////////////

void MLDRadiosity::update()
{
  MLScene* scene= model->getScene();

  MLGlobalLighting* gl= scene->getGlobalLighting();

  Spectrum initial_energy= gl->getInitialEnergy();
  Spectrum direct_shot;
  Spectrum indirect_shot;
  Spectrum unshot;
  gl->getShotUnshot(direct_shot,indirect_shot,unshot);

  MLString strg;
  strg= MLString::printf( "%g", initial_energy.power() );
  kwDialog->setText( RadiosityDlg::initial, strg );

  double power_shot= (direct_shot+indirect_shot).power();
  strg= MLString::printf( "%g", power_shot );
  kwDialog->setText( RadiosityDlg::shot, strg );

  double power_unshot= unshot.power();
  strg= MLString::printf( "%g", power_unshot );
  kwDialog->setText( RadiosityDlg::unshot, strg );

  double perc= 0.;

  if( fabs(power_shot) > 1e-10 )
    {
    perc= 1. - power_unshot / power_shot;
    if( perc < 0. )
      perc= 0.;
    if( perc > 1. )
      perc= 1.;
    }

  kwDialog->setValue( RadiosityDlg::progress, perc );
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDRadiosity::kwCallback( KWEvent& event )
{
  switch( event.type )
    {
    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////
