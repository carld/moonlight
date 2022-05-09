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
  MLCmdLineConvert.C

  Stephane Rehel

  August 6 1997

  March 11 1998
    - MD2 file format by Adam Mayer <phooky@gyral.com>
*/

#include "tools/MLCommandLine.h"

#include "scene/MLScene.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLSceneIO.h"

#include "MLCmdLineConvert.h"

#include "MLImporter.h"

#include "dxf/DXFLoader.h"
#include "off/OFFLoader.h"
#include "md2/Md2Loader.h"
#include "asc/ASCLoader.h"
#include "hrc/HRCReader.h"

/////////////////////////////////////////////////////////////////////////////

MLCmdLineConvert::MLCmdLineConvert( MLConfig* _config )
{
  config= _config;
}

/////////////////////////////////////////////////////////////////////////////

MLCmdLineConvert::~MLCmdLineConvert()
{}

/////////////////////////////////////////////////////////////////////////////

int MLCmdLineConvert::run( MLKernel* kernel, MLCommandLine& command )
{
  MLDatabasesList::setConfig(config,"Moonlight 3D");

  MLImporter* importer= new MLImporter;

  int outcome= importer->parseOptions(command);
  if( outcome != 0 )
    {
    delete importer;
    importer= 0;

    return outcome;
    }

  MLString filename= importer->in_filename;
  MLString ext= importer->filename_ext;
  ext= ext.toLowerCase();

  MLScene* scene= new MLScene(kernel);
  if( ! scene->init() )
    {
    return 1;
    }

  importer->scene= scene;

  if( ext == ".dxf" )
    {
    DXFLoader* loader= new DXFLoader(importer);

    if( ! loader->init() )
      {
      fprintf( stderr, "unable to open file '%s'\n", filename.get() );
      return 1;
      }

    if( ! loader->load() )
      {
      fprintf( stderr, "error loading file '%s'\n", filename.get() );
      return 1;
      }

    goto ok;
    }

  if( ext == ".off" )
    {
    OFFLoader* loader= new OFFLoader(importer);

    if( ! loader->init() )
      {
      fprintf( stderr, "unable to open file '%s'\n", filename.get() );
      return 1;
      }

    if( ! loader->load() )
      {
      fprintf( stderr, "error loading file '%s'\n", filename.get() );
      return 1;
      }

    goto ok;
    }

  if( ext == ".md2" )
    {
    Md2Loader* loader= new Md2Loader(importer);

    if( ! loader->init() )
      {
      fprintf( stderr, "unable to open file '%s'\n", filename.get() );
      return 1;
      }

    if( ! loader->load() )
      {
      fprintf( stderr, "error loading file '%s'\n", filename.get() );
      return 1;
      }

    goto ok;
    }

  if( ext == ".asc" )
    {
    ASCLoader* loader= new ASCLoader(importer);

    if( ! loader->init() )
      {
      fprintf( stderr, "unable to open file '%s'\n", filename.get() );
      return 1;
      }

    if( ! loader->load() )
      {
      fprintf( stderr, "error loading file '%s'\n", filename.get() );
      return 1;
      }

    goto ok;
    }

  if( ext == ".hrc" )
    {
    HRCReader* loader= new HRCReader(importer);

    if( ! loader->init() )
      {
      fprintf( stderr, "unable to open file '%s'\n", filename.get() );
      return 1;
      }

    if( ! loader->load() )
      {
      fprintf( stderr, "error loading file '%s'\n", filename.get() );
      return 1;
      }

    goto ok;
    }

  fprintf( stderr, "Error: unknown format extension '%s'\n", ext.get() );
  return 1;

ok:

  importer->finalize();

  MLName name;
  name.name= importer->scene_name;
  scene->setName(importer->scene_name);

  if( ! importer->no_write )
    {
    if( importer->verbose )
      fprintf( stderr, "Writing scene %s in database %s\n",
               importer->scene_name.get(),
               importer->database->getBasePath().get() );

    if( ! MLSceneIO::write( importer->database, scene, name ) )
      {
      fprintf( stderr, "Error writing scene '%s'\n",
               importer->scene_name.get() );
      return 1;
      }
    }

  if( importer->verbose )
    fprintf( stderr, "Success.\n" );

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
