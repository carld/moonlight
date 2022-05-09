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
  MLRendererOptionsIO.C

  Stephane Rehel

  September 20 1997

  Functions in MLSceneIO.C:

  void MLSceneIO::writeRendererOptions( MLScene* scene, MLWFileBlock& block );
  void MLSceneIO::readRendererOptions( MLScene* scene, MLRFileBlock& block );
*/

#include "MLSceneIO.h"
#include "MLSceneIDs.h"

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "scene/MLScene.h"
#include "scene/renderer/MLRendererOptions.h"

/////////////////////////////////////////////////////////////////////////////

static const int renderer_options_version= 1;

/////////////////////////////////////////////////////////////////////////////

void MLSceneIO::writeRendererOptions( MLScene* scene, MLWFileBlock& block )
{
  block.begin(MLSceneID::SCENE_RENDERER_OPTIONS);
    {
    block << int(::renderer_options_version);

    MLRendererOptions* options= scene->getRendererOptions();

    block << int(options->xres);
    block << int(options->yres);

    block << int(options->maxRayDepth);

    block << options->backgroundColor;

    block << int(options->use_system_gamma_correction);
    block << double(options->current_system_gamma_correction);
    block << double(options->gamma_correction);

    block << int(options->trace_global_shadows);
    block << int(options->trace_shadows);
    block << int(options->trace_reflections);
    block << int(options->trace_transmissions);

    block << int(options->writeImage);
    block << options->imageFilename;
    block << int(options->imageFormat);
    block << int(options->compress_tiff);
    block << int(options->progressive_jpeg);
    block << options->jpeg_quality;

    block << int(options->has_constrain_ratio);
    block << double(options->constrain_ratio);

    block << int(options->emitBeep);
    }
  block.end();
}

/////////////////////////////////////////////////////////////////////////////

static void readRendererOptions_1( MLScene* scene, MLRFileBlock& block )
{
  MLRendererOptions* options= scene->getRendererOptions();

  options->xres= block.getInteger();
  options->yres= block.getInteger();

  options->maxRayDepth= block.getInteger();

  block >> options->backgroundColor;

  options->use_system_gamma_correction= (block.getInteger() != 0);
  options->current_system_gamma_correction= block.getDouble();
  options->gamma_correction= block.getDouble();

  options->trace_global_shadows= (block.getInteger() != 0);
  options->trace_shadows= (block.getInteger() != 0);
  options->trace_reflections= (block.getInteger() != 0);
  options->trace_transmissions= (block.getInteger() != 0);

  options->writeImage= (block.getInteger() != 0);
  block >> options->imageFilename;
  options->imageFormat= block.getInteger();
  options->compress_tiff= (block.getInteger() != 0);
  options->progressive_jpeg= (block.getInteger() != 0);
  options->jpeg_quality= block.getInteger();

  options->has_constrain_ratio= (block.getInteger() != 0);
  options->constrain_ratio= block.getDouble();

  options->emitBeep= (block.getInteger() != 0);
}

/////////////////////////////////////////////////////////////////////////////

void MLSceneIO::readRendererOptions( MLScene* scene, MLRFileBlock& block )
{
  // Write the renderer options
  if( ! block.go(MLSceneID::SCENE_RENDERER_OPTIONS) )
    return;

  int version= block.getInteger();

  switch(version)
    {
    case 1: readRendererOptions_1(scene,block); break;
    default: break;
    }
}

/////////////////////////////////////////////////////////////////////////////
