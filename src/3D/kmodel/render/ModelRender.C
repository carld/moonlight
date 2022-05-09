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
  ModelRender.C

  Stephane Rehel

  September 21 1997
*/

#include <stdio.h>

#include "ModelRender.h"

#include "tools/Chrono.h"
#include "tools/ListFiles.h"

#include "image/MLImageFormat.h"
#include "image/MLImageWriter.h"
#include "image/tif/MLImageWriterTIF.h"
#include "image/jpg/MLImageWriterJPG.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/MLPicture.h"

#include "kw/KWDialog.h"

#include "interface/Interface.h"

#include "kmodel/ModelModule.h"

#include "scene/MLCamera.h"
#include "scene/MLScene.h"
#include "scene/renderer/MLRendererOptions.h"
#include "scene/renderer/MLRayRenderer.h"

#include "ModelRenderWindow.h"

/////////////////////////////////////////////////////////////////////////////

ModelRender::ModelRender( ModelModule* _model )
{
  model= _model;
  window= new ModelRenderWindow(this);

  scene= 0;
  options= 0;
  renderer= 0;
  picture= 0;

  _isRendering= false;
}

/////////////////////////////////////////////////////////////////////////////

ModelRender::~ModelRender()
{
  delete renderer;
  renderer= 0;

  delete picture;
  picture= 0;

  delete window;
  window= 0;

  options= 0;
  scene= 0;
  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelRender::init()
{
  delete picture;
  picture= 0;

  delete renderer;
  renderer= 0;

  scene= model->getScene();
  options= scene->getRendererOptions();

  const MLCamera& camera= model->getMainCamera();
  renderer= new MLRayRenderer( scene, camera, false );

  int width= options->xres;
  int height= options->yres;

  picture= new MLPicture;
  picture->createRGB(width,height);
  picture->fill( 0, 0, width-1, height-1, 0, 0, 0 );
}

/////////////////////////////////////////////////////////////////////////////

// return false if unable to start
boolean ModelRender::run()
{
  if( renderer == 0 )
    return false;

  if( options->writeImage )
    {
    const MLString& filename= options->imageFilename;
    if( ListFiles::exist(filename) )
      {
      if( ! KWWarningOkCancelf( "File \"%s\" already exists.\n"
                                "Overwrite it?", filename.get() ) )
        return false;
      }

    FILE* file= fopen( filename.get(), "w" );
    if( file == 0 )
      {
      if( ! KWWarningOkCancelf( "I will be unable to save image in file\n"
                                "\"%s\"\n"
                                "Disable saving and continue?",
                                filename.get() ) )
        return false;

      options->writeImage= false;
      }
     else
      {
      fclose(file);
      file= 0;
      }
    }

  if( ! window->open() )
    return false;
  window->eraseWindow();

  _isRendering= true;

  int width= options->xres;
  int height= options->yres;

  renderer->allocBuffer(picture->getData());
  renderer->init();

  renderer->init_trace();

  window->reset_trace();
  window->redraw();

  window->setMessage("Rendering...");

  Chrono chrono;
  chrono.start();

  double display_t= 0.;
  double events_t= 0.;

  // update each 1 second
  double display_delay= 1.;

  // user break test 2 times per second
  double events_delay= 0.5;

  double total_time= 0.;

  for( int j= height-1; j >= 0; --j )
    {
    double t1= chrono.peek();
    renderer->trace_next_line();
    double t2= chrono.peek();
    total_time+= t2 - t1;

    double t= t2;

    if( (t - display_t) > display_delay )
      {
      display_t += display_delay;
      window->set_traced_from_origin_to(j);
      MLString stats= getStatsLine( j, total_time );
      if( options->writeImage )
        stats += MLString(" (") + options->imageFilename + MLString(")");
      window->setMessage(stats);
      }

    if( (t - events_t) > events_delay )
      {
      events_t += events_delay;
      if( ! window->process_events() )
        {
        window->close();
        _isRendering= false;
        return true;
        }
      }
    }

  window->set_traced_from_origin_to(0);

  boolean error_saving= false;
  if( options->writeImage )
    error_saving= !save();

  double us= total_time * 1e6 / (double(height) * double(width));

  MLString time_strg= seconds_to_string(total_time);
  MLString stats= MLString::printf( "DONE %dx%d in %s (%.0f us/pixel)",
                                    width, height, time_strg.get(), us );
  if( options->writeImage )
    {
    if( error_saving )
      stats += MLString(" ** ERROR SAVING in ") + options->imageFilename;
     else
      stats += MLString(" -- SAVED in ") + options->imageFilename;
    }
  window->setMessage(stats);

  if( options->emitBeep )
    GraphicsSystem::getGraphicsSystem()->emitBeep();

  if( error_saving )
    GraphicsSystem::getGraphicsSystem()->emitBeep();

  window->wait_for_user_action();

  window->close();

  _isRendering= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLString ModelRender::seconds_to_string( double s ) const
{
  if( s < 0. )
    s= 0.;

  int hours= int(s / 3600.);
  int minutes= int((s - double(hours)*3600.) / 60.);
  int seconds= int( s - double(hours)*3600. - double(minutes)*60.);

  if( hours == 0 )
    {
    if( minutes == 0 )
      return MLString::printf( "%d s", seconds );
     else
      return MLString::printf( "%d min %d s", minutes, seconds );
    }

  return MLString::printf( "%d h %d min %d s", hours, minutes, seconds );
}

/////////////////////////////////////////////////////////////////////////////

MLString ModelRender::getStatsLine( int j, double dt ) const
{
  int width= options->xres;
  int height= options->yres;

  j= max( 0, min( height, j ) );

  int nLines= height - j;
  int perc= int( double(nLines) / double(height) * 100. + .5 );
  double us= dt * 1e6 / (double(nLines) * double(width));

  double sec_remaining= (double(height-nLines) * double(height))
                        * us * 1e-6;

  return MLString::printf( "%3d%% [%4d/%4d]%6.0f us/pixel (%s remaining)",
                           perc, nLines, height, us,
                           seconds_to_string(sec_remaining).get() );
}

/////////////////////////////////////////////////////////////////////////////

void ModelRender::wakeup()
{
  if( picture == 0 )
    return;

  window->open();
  window->redraw();

  window->wait_for_user_action();

  window->close();
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelRender::save()
{
  if( picture == 0 || window == 0 || options == 0 )
    return false;

  const MLString& filename= options->imageFilename;

  MLImageWriter* writer= MLImageFormat::getInstance()->getWriter(
                              MLImageFormat::FORMAT(options->imageFormat) );

  if( writer == 0 )
    return false;

  if( options->imageFormat == int(MLImageFormat::TIF) )
    {
    MLImageWriterTIF* tif= (MLImageWriterTIF*) writer;

    tif->setCompressed( options->compress_tiff );
    }

  if( options->imageFormat == int(MLImageFormat::JPG) )
    {
    MLImageWriterJPG* jpg= (MLImageWriterJPG*) writer;

    jpg->setProgressive( options->progressive_jpeg );
    jpg->setQuality( options->jpeg_quality );
    }

  if( ! writer->open(filename.get()) )
    {
    delete writer;
    return false;
    }

  picture->upsidedown();
  boolean outcome= writer->write(picture);
  picture->upsidedown();

  delete writer;
  writer= 0;

  return outcome;
}

/////////////////////////////////////////////////////////////////////////////
