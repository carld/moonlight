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
  MLDRendererOptions.h

  Stephane Rehel

  September 20 1997
*/

#include <stdlib.h>
#include <math.h>

#include "tools/MLFilename.h"
#include "tools/MLFileEntry.h"

#include "graphics/KeyCodes.h"

#include "kw/KWDialog.h"
#include "kw/KWFileDialogManager.h"

#include "image/MLImageFormat.h"

#include "scene/MLScene.h"
#include "scene/renderer/MLRendererOptions.h"

#include "interface/MLColorWidget.h"

#include "kmodel/ModelModule.h"

#include "MLDRendererOptions.h"
#include "RendererOptionsDlg.h"

/////////////////////////////////////////////////////////////////////////////

static const int nMaxRatios= 3;
struct Ratio
  {
  const char* text;
  double value;
  };
static Ratio ratios[nMaxRatios+1];
static int nRatios= 0;

/////////////////////////////////////////////////////////////////////////////

MLDRendererOptions::MLDRendererOptions( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  bg_color= 0;
  file_manager= new KWFileDialogManager;
}

/////////////////////////////////////////////////////////////////////////////

MLDRendererOptions::~MLDRendererOptions()
{
  delete bg_color;
  bg_color= 0;

  delete file_manager;
  file_manager= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLDRendererOptions::run()
{
  MLScene* scene= model->getScene();
  MLRendererOptions* options= scene->getRendererOptions();

  if( ! MLGlobalDialog::create(RendererOptionsDlg::data) )
    return;

  kwDialog->setValue( RendererOptionsDlg::xres, double(options->xres) );
  kwDialog->setValue( RendererOptionsDlg::yres, double(options->yres) );

  kwDialog->setChecked( RendererOptionsDlg::trace_global_shadows,
                        options->trace_global_shadows );
  kwDialog->setChecked( RendererOptionsDlg::trace_shadows,
                        options->trace_shadows );
  kwDialog->setChecked( RendererOptionsDlg::trace_reflections,
                        options->trace_reflections );
  kwDialog->setChecked( RendererOptionsDlg::trace_transmissions,
                        options->trace_transmissions );

  kwDialog->setChecked( RendererOptionsDlg::beep, options->emitBeep );

  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "1" );
  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "2" );
  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "3" );
  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "4" );
  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "5" );
  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "8" );
  kwDialog->appendListText( RendererOptionsDlg::ray_depth, "16" );
  kwDialog->setText( RendererOptionsDlg::ray_depth,
                     MLString::valueOf( max( 0, min(16,options->maxRayDepth) ) ) );

  file_manager->init( kwDialog,
                      RendererOptionsDlg::directories,
                      RendererOptionsDlg::files,
                      RendererOptionsDlg::location,
                      RendererOptionsDlg::selection,
                      0, // ok
                      0 // cancel
                    );

  bg_color= new MLColorWidget( kwDialog,
                               RendererOptionsDlg::label_r,
                               RendererOptionsDlg::label_g,
                               RendererOptionsDlg::label_b,
                               RendererOptionsDlg::value_r,
                               RendererOptionsDlg::value_g,
                               RendererOptionsDlg::value_b,
                               RendererOptionsDlg::radio_rgb,
                               RendererOptionsDlg::radio_hsv,
                               RendererOptionsDlg::colorArea );
  bg_color->setRGBColorModel();
  bg_color->set(options->backgroundColor);

  if( options->use_system_gamma_correction )
    kwDialog->setChecked( RendererOptionsDlg::default_gamma, true );
   else
    kwDialog->setChecked( RendererOptionsDlg::custom_gamma, true );
  kwDialog->setValue( RendererOptionsDlg::gamma, options->gamma_correction );

  kwDialog->setChecked( RendererOptionsDlg::constrain_ratio,
                        options->has_constrain_ratio );

  nRatios= 3;
  assert( nRatios <= nMaxRatios );
  ratios[1].text= "4 / 3";
  ratios[1].value= 4. / 3.;
  ratios[2].text= "1 / 1";
  ratios[2].value= 1.;
  ratios[3].text= "16 / 9";
  ratios[3].value= 16. / 9.;

  int i;

  for( i= 1; i <= nRatios; ++i )
    {
    kwDialog->appendListText( RendererOptionsDlg::ratios, ratios[i].text );
    if( fabs(options->constrain_ratio - ratios[i].value) < 1e-5 )
      kwDialog->setText( RendererOptionsDlg::ratios, ratios[i].text );
    }

  for( i= 0; i <= MLImageFormat::getInstance()->getNFormats(); ++i )
    {
    int fmt=i;
    if( MLImageFormat::getInstance()->availableWriter(fmt) )
      {
      MLString name= MLImageFormat::getInstance()->getName(fmt).c_str();
      kwDialog->appendListText( RendererOptionsDlg::formats, name );
      if( options->imageFormat == int(fmt) )
        kwDialog->setText( RendererOptionsDlg::formats, name );
      }
    }

  kwDialog->setChecked( RendererOptionsDlg::write_image,
                        options->writeImage );

  kwDialog->setChecked( RendererOptionsDlg::compress_tiff,
                        options->compress_tiff );
  kwDialog->setChecked( RendererOptionsDlg::progressive_jpeg,
                        options->progressive_jpeg );

  kwDialog->setValue( RendererOptionsDlg::jpeg_quality,
                      double(options->jpeg_quality) );

  if( options->imageFilename.length() == 0 )
    {
    kwDialog->setText( RendererOptionsDlg::selection, "~/" );
    file_manager->validate();
    changeFormat(false);
    }
   else
    {
    MLFilename filename(options->imageFilename);
    MLString path= filename.getPath().get();

    if( MLFileEntry(path).isAccessibleDirectory() )
      kwDialog->setText( RendererOptionsDlg::selection, path );
     else
      kwDialog->setText( RendererOptionsDlg::selection, "~/" );
    file_manager->validate();

    MLString f= filename.getFilename() + filename.getExtension();
//    if( f.length() != 0 )
//      kwDialog->setText( RendererOptionsDlg::selection, f );
    changeFormat(false);
    if( f.length() != 0 )
      kwDialog->setText( RendererOptionsDlg::selection, f );
    }

  checkAvail();

  kwDialog->focus(RendererOptionsDlg::ok);

  int outcome= kwDialog->run();

  if( outcome == int(KWDialog::VALIDATE) )
    {
    options->xres= int(kwDialog->getValue(RendererOptionsDlg::xres));
    options->yres= int(kwDialog->getValue(RendererOptionsDlg::yres));
    options->has_constrain_ratio=
               int(kwDialog->getChecked(RendererOptionsDlg::constrain_ratio));
    options->constrain_ratio= getCurrentRatio();
    if( options->constrain_ratio < 1e-5 )
      options->constrain_ratio= 4./3.;

    boolean bk_changed= (options->backgroundColor-bg_color->get()).cabs()>1e-5;
    options->backgroundColor= bg_color->get();

    options->use_system_gamma_correction=
              ( kwDialog->getRadioChecked(RendererOptionsDlg::default_gamma)
                == RendererOptionsDlg::default_gamma );
    options->gamma_correction= kwDialog->getValue(RendererOptionsDlg::gamma);

    options->maxRayDepth=
             atoi( kwDialog->getText(RendererOptionsDlg::ray_depth).get() );
    if( options->maxRayDepth < 1 )
      options->maxRayDepth= 1;

    options->trace_global_shadows=
             kwDialog->getChecked(RendererOptionsDlg::trace_global_shadows);
    options->trace_shadows=
             kwDialog->getChecked(RendererOptionsDlg::trace_shadows);
    options->trace_reflections=
             kwDialog->getChecked(RendererOptionsDlg::trace_reflections);
    options->trace_transmissions=
             kwDialog->getChecked(RendererOptionsDlg::trace_transmissions);
    options->emitBeep= kwDialog->getChecked(RendererOptionsDlg::beep);

    options->writeImage=
             kwDialog->getChecked(RendererOptionsDlg::write_image);

    if( file_manager->validate() )
      {
      MLString path= kwDialog->getText(RendererOptionsDlg::location).trim();
      MLString name= kwDialog->getText(RendererOptionsDlg::selection).trim();
      options->imageFilename= path + name;
      }
     else
      {
      MLString path= kwDialog->getText(RendererOptionsDlg::location).trim();
      options->imageFilename= path;
      }

    const MLString& format_name=
                             kwDialog->getText(RendererOptionsDlg::formats);
    options->imageFormat= int(MLImageFormat::getInstance()->getFromName(format_name.get()));

    options->compress_tiff=
                    kwDialog->getChecked(RendererOptionsDlg::compress_tiff);
    options->jpeg_quality=
                  int(kwDialog->getValue(RendererOptionsDlg::jpeg_quality));
    options->progressive_jpeg=
            int(kwDialog->getChecked(RendererOptionsDlg::progressive_jpeg));

    if( bk_changed )
      model->refreshScreen(ModelModule::REFRESH_PREVIEW);
    }

  delete bg_color;
  bg_color= 0;

  MLGlobalDialog::close();
}

/////////////////////////////////////////////////////////////////////////////

void MLDRendererOptions::checkAvail()
{
  if( kwDialog == 0 )
    return;

  boolean ok;

  ok= kwDialog->getChecked(RendererOptionsDlg::constrain_ratio);
  kwDialog->setAvailable( RendererOptionsDlg::ratios, ok );

  ok= (kwDialog->getText(RendererOptionsDlg::ray_depth) != "1");
  kwDialog->setAvailable( RendererOptionsDlg::trace_reflections, ok );
  kwDialog->setAvailable( RendererOptionsDlg::trace_transmissions, ok );

  ok= kwDialog->getChecked(RendererOptionsDlg::trace_shadows);
  kwDialog->setAvailable( RendererOptionsDlg::trace_global_shadows, ok );

  ok= kwDialog->getChecked(RendererOptionsDlg::write_image);
  kwDialog->setAvailable( RendererOptionsDlg::location, ok );
  kwDialog->setAvailable( RendererOptionsDlg::directories, ok );
  kwDialog->setAvailable( RendererOptionsDlg::files, ok );
  kwDialog->setAvailable( RendererOptionsDlg::selection, ok );

  const MLString& format_name= kwDialog->getText(RendererOptionsDlg::formats);
  MLImageFormat::FORMAT fmt= MLImageFormat::getInstance()->getFromName(format_name.get());

  ok= (fmt == MLImageFormat::JPG);
  kwDialog->setAvailable( RendererOptionsDlg::jpeg_quality, ok );
  kwDialog->setAvailable( RendererOptionsDlg::progressive_jpeg, ok );

  ok= (fmt == MLImageFormat::TIF);
  kwDialog->setAvailable( RendererOptionsDlg::compress_tiff, ok );

  ok= ( kwDialog->getRadioChecked(RendererOptionsDlg::default_gamma)
                == RendererOptionsDlg::default_gamma );
  kwDialog->setAvailable( RendererOptionsDlg::gamma, !ok );
}

/////////////////////////////////////////////////////////////////////////////

void MLDRendererOptions::changeFormat( boolean change_sel_ext /* = true */ )
{
  if( kwDialog == 0 || file_manager == 0 )
    return;

  const MLString& format_name= kwDialog->getText(RendererOptionsDlg::formats);
  MLImageFormat::FORMAT fmt= MLImageFormat::getInstance()->getFromName(format_name.get());

  MLString fmt_ext= MLImageFormat::getInstance()->getExtension(fmt).c_str();
  MLString pattern= MLString("*") + fmt_ext;

  MLString prev_selection= kwDialog->getText(RendererOptionsDlg::selection);
  prev_selection= prev_selection.trim();

  file_manager->setPattern(pattern);
  file_manager->validate();

  if( change_sel_ext )
    {
    if( prev_selection.length() > 0 &&
        prev_selection.indexOf('?') == -1 &&
        prev_selection.indexOf('*') == -1 )
      {
      if( prev_selection[ prev_selection.length() - 1 ] != '/' )
        {
        MLFilename f(prev_selection);
        kwDialog->setText( RendererOptionsDlg::selection,
                           f.getFilename() + fmt_ext );
        }
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

// return 0 if error
double MLDRendererOptions::getCurrentRatio()
{
  if( kwDialog == 0 )
    return 0;

  const MLString& r= kwDialog->getText(RendererOptionsDlg::ratios);
  for( int i= 1; i <= nRatios; ++i )
    {
    if( MLString(ratios[i].text) == r )
      return ratios[i].value;
    }

  return 0.;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDRendererOptions::kwCallback( KWEvent& event )
{
  if( bg_color != 0 )
    if( bg_color->colorCallback(event) )
      return true;

  if( file_manager != 0 )
    if( file_manager->handleKWEvent(event) )
      return true;

  if( event.type == KWEvent::SELECT &&
      event.widget == RendererOptionsDlg::write_image )
    {
    checkAvail();
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == RendererOptionsDlg::formats )
    {
    changeFormat();
    checkAvail();
    return true;
    }

  if( event.type == KWEvent::SELECT )
    {
    if( event.widget == RendererOptionsDlg::ray_depth ||
        event.widget == RendererOptionsDlg::trace_global_shadows ||
        event.widget == RendererOptionsDlg::trace_shadows ||
        event.widget == RendererOptionsDlg::trace_reflections ||
        event.widget == RendererOptionsDlg::trace_transmissions )
      {
      checkAvail();
      return true;
      }
    }

  if( event.type == KWEvent::SELECT &&
      ( event.widget == RendererOptionsDlg::default_gamma ||
        event.widget == RendererOptionsDlg::custom_gamma ) )
    {
    checkAvail();
    if( kwDialog->getRadioChecked(RendererOptionsDlg::custom_gamma)
                                     == RendererOptionsDlg::custom_gamma )
      kwDialog->focus(RendererOptionsDlg::gamma);

    return true;
    }

  if( event.type == KWEvent::SELECT &&
      ( event.widget == RendererOptionsDlg::constrain_ratio ||
        event.widget == RendererOptionsDlg::ratios ) )
    {
    checkAvail();

    if( ! kwDialog->getChecked(RendererOptionsDlg::constrain_ratio) )
      return true;

    double ratio= getCurrentRatio();
    if( ratio < 1e-5 )
      return true;

    double xres= kwDialog->getValue(RendererOptionsDlg::xres);
    int yres= int(xres / ratio + 0.5);
    kwDialog->setValue(RendererOptionsDlg::yres,double(yres));

    return true;
    }

  if( event.type == KWEvent::KEY_PROCESSED &&
      (event.widget == RendererOptionsDlg::xres ||
       event.widget == RendererOptionsDlg::yres ) )
    {
    if( ! kwDialog->getChecked(RendererOptionsDlg::constrain_ratio) )
      return true;

    double ratio= getCurrentRatio();
    if( ratio < 1e-5 )
      return true;

    if( event.widget == RendererOptionsDlg::xres )
      {
      const MLString& xres_strg= kwDialog->getText(RendererOptionsDlg::xres);
      int xres= atoi(xres_strg.get());
      int yres= int(double(xres) / ratio + 0.5);
      kwDialog->setValue(RendererOptionsDlg::yres,double(yres));
      }
     else
      {
      const MLString& yres_strg= kwDialog->getText(RendererOptionsDlg::yres);
      int yres= atoi(yres_strg.get());
      int xres= int(double(yres) * ratio + 0.5);
      kwDialog->setValue(RendererOptionsDlg::xres,double(xres));
      }

    return true;
    }

  if( event.type == KWEvent::KEY &&
      event.key == KeyCodes::ENTER &&
      event.widget == RendererOptionsDlg::selection )
    {
    if( file_manager != 0 )
      file_manager->validate();
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////
