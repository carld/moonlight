// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  MLDExportAs.C

  Stephane Rehel

  August 11 1997

  March 11 1998
    - MD2 format by Adam Mayer <phooky@gyral.com>
*/

#include <string.h>
#include <stdlib.h>

#include "kw/KWFileDialogManager.h"
#include "kw/KWDialog.h"

#include "kmodel/ModelModule.h"

#include "MLDExportAs.h"
#include "ExportAsDlg.h"

#include "scene/convert/export/MLExport.h"
#include "scene/convert/export/MLExportDXF.h"
#include "scene/convert/export/MLExportOFF.h"
#include "scene/convert/export/MLExportVRML1.h"
#include "scene/convert/export/MLExportVRML2.h"
#include "scene/convert/export/MLExportPOV.h"
#include "scene/convert/export/MLExportMd2.h"
#include "scene/convert/export/MLExportRS.h"
#include "scene/convert/export/MLExportMLRaw.h"
#include "scene/convert/export/MLExportRIB.h"

/////////////////////////////////////////////////////////////////////////////

enum FORMAT_ID {
               NULL_ID= 0,
               VRML2_ID= 1,
               VRML1_ID,
               POV_ID,
               RS_ID,
               DXF_ID,
               OFF_ID,
               ML_RAW_ID,
               MD2_ID,
               RIB_ID,
               LAST_FORMAT_ID
               };

struct FORMAT
       {
       FORMAT_ID id;
       char* name;
       char* pattern;
       };

static const int nFormats= LAST_FORMAT_ID - 1;

static const FORMAT formats[nFormats] =
  {
    { VRML2_ID,  "VRML2",        "*.wr*l"  },
    { VRML1_ID,  "VRML1",        "*.wr*l"  },
    { POV_ID,    "POV",          "*.pov"   },
    { RS_ID,     "Rayshade",     "*.ray"   },
    { DXF_ID,    "DXF",          "*.dxf"   },
    { OFF_ID,    "OFF",          "*.off"   },
    { ML_RAW_ID, "ML RAW",       "*.mlraw" },
    { MD2_ID,    "QuakeII .md2", "*.md2"   },
    { RIB_ID,    "RIB",          "*.rib"   }
  };

/////////////////////////////////////////////////////////////////////////////

MLDExportAs::MLDExportAs( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;

  manager= new KWFileDialogManager;
}

/////////////////////////////////////////////////////////////////////////////

MLDExportAs::~MLDExportAs()
{
  model= 0;

  delete manager; manager= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDExportAs::run()
{
  if( ! MLGlobalDialog::create(ExportAsDlg::data) )
    return false;

  manager->init( kwDialog,
                 ExportAsDlg::directories,
                 ExportAsDlg::files,
                 ExportAsDlg::location,
                 ExportAsDlg::selection,
                 ExportAsDlg::ok,
                 ExportAsDlg::cancel );

  static int currentFormat= 1;
  static char* currentPath= 0;

  if( currentPath == 0 )
    currentPath= strdup("~");

  if( currentFormat < 1 || currentFormat > nFormats )
    currentFormat= 1;

  int i;
  for( i= 0; i < nFormats; ++i )
    kwDialog->appendListText( ExportAsDlg::formats, formats[i].name );

  kwDialog->setText( ExportAsDlg::formats,
                     formats[currentFormat-1].name );

  manager->setPattern( formats[currentFormat-1].pattern );

  manager->setPath(currentPath);

  manager->updateDirectories();

  kwDialog->focus(ExportAsDlg::selection);

  int outcome= kwDialog->run();
  int format_index= kwDialog->getInteger(ExportAsDlg::formats);

  currentFormat= format_index;
  free( (void*) currentPath );
  currentPath= strdup( kwDialog->getText(ExportAsDlg::location).get() );

  MLGlobalDialog::close();

  if( outcome == KWDialog::VALIDATE && manager != 0 )
    {
    MLString filename= manager->getFilename();
    if( (format_index >= 1 && format_index <= nFormats) && manager != 0 )
      go_export( filename, format_index );

    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDExportAs::kwCallback( KWEvent& event )
{
  if( manager != 0 )
    if( manager->handleKWEvent(event) )
      return true;

  if( event.type == KWEvent::SELECT &&
      event.widget == ExportAsDlg::formats )
    {
    int i= kwDialog->getInteger(ExportAsDlg::formats);
    if( (i >= 1 && i <= nFormats) && manager != 0 )
      {
      manager->setPattern( formats[i-1].pattern );
      manager->updateDirectories();

      kwDialog->focus(ExportAsDlg::selection);
      }

    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLDExportAs::go_export( const MLString& filename, int format_index )
{
  MLExport* exportObj= 0;

  FORMAT_ID format_id= NULL_ID;
  if( format_index >= 1 && format_index <= nFormats )
    format_id= formats[ format_index-1 ].id;

  MLScene* scene= model->getScene();

  switch( FORMAT_ID(format_id) )
    {
    case VRML2_ID:
      {
      exportObj= new MLExportVRML2(scene);
      break;
      }
    case VRML1_ID:
      {
      exportObj= new MLExportVRML1(scene);
      break;
      }
    case POV_ID:
      {
      exportObj= new MLExportPOV(scene);
      break;
      }
    case RS_ID:
      {
      exportObj= new MLExportRS(scene);
      break;
      }
    case DXF_ID:
      {
      exportObj= new MLExportDXF(scene);
      break;
      }
    case OFF_ID:
      {
      exportObj= new MLExportOFF(scene);
      break;
      }
    case ML_RAW_ID:
      {
      exportObj= new MLExportMLRaw(scene);
      break;
      }
    case MD2_ID:
      {
      exportObj = new MLExportMd2(scene);
      break;
      }
    case RIB_ID:
      {
      exportObj= new MLExportRIB(scene);
      break;
      }
    case LAST_FORMAT_ID:
    case NULL_ID:
      break;
    }

  if( exportObj == 0 )
    {
    MLString fmt_name= "";
    if( format_index >= 1 && format_index <= nFormats )
      fmt_name= formats[format_index-1].name;

    KWError( "Error",
               MLString("No exporter available\nfor format ")
              +fmt_name );
    return;
    }

  if( ! exportObj->open(filename) )
    {
    delete exportObj; exportObj= 0;

    KWError( "Error",
             MLString("Error while opening to file\n") + filename );
    return;
    }

  model->stopLightingProcess();
  model->pushWaitCursor();
  boolean outcome= exportObj->run();
  model->popWaitCursor();

  if( ! outcome )
    {
    delete exportObj; exportObj= 0;

    KWError( "Error",
             MLString("Error while exporting to file\n") + filename );
    return;
    }

  delete exportObj;
  exportObj= 0;
}

/////////////////////////////////////////////////////////////////////////////
