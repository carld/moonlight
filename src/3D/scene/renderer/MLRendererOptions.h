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
   MLRendererOptions.h

   Stephane Rehel

   September 14 1997
*/

#ifndef __MLRendererOptions_h
#define __MLRendererOptions_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Color_h
#include "tools/Color.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLScene;

/////////////////////////////////////////////////////////////////////////////

class MLRendererOptions
{
protected:
  MLScene* scene;

public:
  int xres, yres;

  int maxRayDepth; // >= 1

  boolean verticesShader;

  Color backgroundColor;

  boolean use_system_gamma_correction;
  double current_system_gamma_correction; // set or not by the graphics
                                          // system
  double gamma_correction;

  // trace shadows when necessary
  boolean trace_shadows;

  // trace radiosity shadows
  boolean trace_global_shadows;

  boolean trace_reflections;
  boolean trace_transmissions;

  boolean writeImage;
  MLString imageFilename;
  int imageFormat; // from MLImageFormat
  boolean compress_tiff;
  boolean progressive_jpeg;
  int jpeg_quality;

  boolean has_constrain_ratio;
  double constrain_ratio;

  boolean emitBeep;

public:
  MLRendererOptions( MLScene* _scene );

  virtual ~MLRendererOptions();

  MLScene* getScene() const
    {
    return scene;
    }

  void clear();

  double getGammaCorrection() const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLRendererOptions_h

