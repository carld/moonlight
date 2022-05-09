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
   MLRendererOptions.C

   Stephane Rehel

   September 14 1997
*/

#include "MLRendererOptions.h"

#include "image/MLImageFormat.h"

/////////////////////////////////////////////////////////////////////////////

MLRendererOptions::MLRendererOptions( MLScene* _scene )
{
  scene= _scene;

  clear();
}

/////////////////////////////////////////////////////////////////////////////

MLRendererOptions::~MLRendererOptions()
{}

/////////////////////////////////////////////////////////////////////////////

void MLRendererOptions::clear()
{
  xres= 400;
  yres= 300;
  maxRayDepth= 2;

  verticesShader= false;

  backgroundColor= Color(0,0,0);

  use_system_gamma_correction= true;
  current_system_gamma_correction= 1.;
  gamma_correction= 1.;

  trace_shadows= true;
  trace_global_shadows= false ;
  trace_reflections= true;
  trace_transmissions= true;

  writeImage= false;
  imageFilename= "";
  imageFormat= int(MLImageFormat::JPG);
  compress_tiff= true;
  progressive_jpeg= true;
  jpeg_quality= 95;

  has_constrain_ratio= true;
  constrain_ratio= 4. / 3.;

  emitBeep= false;
}

/////////////////////////////////////////////////////////////////////////////

double MLRendererOptions::getGammaCorrection() const
{
  return use_system_gamma_correction
         ? current_system_gamma_correction
         : gamma_correction;
}

/////////////////////////////////////////////////////////////////////////////
