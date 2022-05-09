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
  DXFLoader.C

  Creation: SR, August 28th, 1995

  Revision:
    Stephane Rehel
    April 25 1997
*/

#include <stdio.h>

#include "tools/MLString.h"

#include "DXFLoader.h"
#include "DXFImport.h"

#include "scene/MLScene.h"
#include "scene/MLMesh.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MeshRep.h"

#include "scene/convert/import/MLImporter.h"

/////////////////////////////////////////////////////////////////////////////

// static
int DXFLoader::circles_subdivision= 36;

/////////////////////////////////////////////////////////////////////////////

DXFLoader::DXFLoader( MLImporter* _importer )
{
  importer= _importer;
  scene= 0;

  layers2Objects= importer->layers2objects;

  dxfImport= 0;
  file= 0;
  main_object= 0;
  object= 0;
  object_name= "";

  base_point= Point(0,0,0);
  insertion_point= Point(0,0,0);
  scale= Vector(1,1,1);
  zrotate= 0.;
}

/////////////////////////////////////////////////////////////////////////////

DXFLoader::~DXFLoader()
{
  delete dxfImport;
  dxfImport= 0;

  importer->close_file(file);
  file= 0;

  scene= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFLoader::init()
{
  assert( dxfImport == 0 );
  assert( file == 0 );

  file= importer->open_file();

  if( file == 0 )
    return false;

  dxfImport= new DXFImport;
  dxfImport->init( file, (DXFExport*) this, 0 );

  dxfImport->read();
//  dxfImport->computeBoundings();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFLoader::load()
{
  assert( dxfImport != 0 );
  assert( scene == 0 );

  scene= importer->scene;

/*
  int i;
  nLayers= dxfImport->getNLayers();
  layers= new Layer* [ nLayers + 1 ];

  for( i= 0; i <= nLayers; ++i )
    layers[i]= 0;

  for( i= 0; i <= nLayers; ++i )
    {
    DXFReader::LAYER* L= dxfImport->getLayer(i);
    assert( L->index >= 0 );
    assert( L->index <= nLayers );

    assert( L->name != 0 );
    Layer* l= scene->findLayer( MLString(L->name) );
    if( l == 0 )
      l= scene->newLayer( MLString(L->name) );

    layers[L->index]= l;
    }
*/
//  scene->setDefaultLayer();
//  scene->setRootObject();

  dxfImport->exportAll();

  if( gzip_pipe )
    pclose(file);
   else
    fclose(file);

  file= 0;

  delete dxfImport;
  dxfImport= 0;

//  scene->setDefaultLayer();
//  scene->setRootObject();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::FACE( const Point& p1, const Point& p2, const Point& p3 )
{
/*
  DXFReader::LAYER* L= dxfImport->getLayer();
  assert( L->index >= 0 );
  assert( L->index <= nLayers );
  assert( layers != 0 );
  assert( layers[L->index] != 0 );

  scene->setCurrentLayer( layers[L->index] );
  scene->addFace( p1, p2, p3, scene->getCurrentMaterial() );
*/
  Point points[3];
  points[0]= p1;
  points[1]= p2;
  points[2]= p3;
  addPolygon(3,points);
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::FACE( const Point& p1, const Point& p2,
                      const Point& p3, const Point& p4 )
{
/*
  DXFReader::LAYER* L= dxfImport->getLayer();
  assert( L->index >= 0 );
  assert( L->index <= nLayers );
  assert( layers != 0 );
  assert( layers[L->index] != 0 );

  scene->setCurrentLayer( layers[L->index] );

  scene->addFace( p1, p2, p3, p4, scene->getCurrentMaterial() );
*/
  Point points[4];
  points[0]= p1;
  points[1]= p2;
  points[2]= p3;
  points[3]= p4;
  addPolygon(4,points);
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::POLYLINE( int nVertices, const Point* points,
                          boolean /*closed*/ )
{
  addPolygon(nVertices,points);
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::addPolygon( int nVertices, const Point* points )
{
  if( nVertices < 2 || points == 0 )
    return;

  MLAbstractMesh* ao= 0;

  if( layers2Objects )
    {
    char* layer_name= dxfImport->getLayer()->name;
    static char* prev_layer_name= 0;
    static MLAbstractMesh* prev_ao= 0;

    if( prev_layer_name != 0 && prev_ao != 0 )
      {
      if( strcmp( layer_name, prev_layer_name ) == 0 )
        ao= prev_ao;
      }

    if( ao == 0 )
      {
      MLString layerName= MLString(layer_name);
      importer->convertName(layerName);

      MLName _name;
      _name.name= layerName;
      ao= scene->getAbstractObject(_name);

      if( ao == 0 )
        {
        ao= new MLAbstractMesh(scene,0);
        ao->setName(layerName);
        ao->generateMesh();

        MLObject* o= new MLMesh( scene->getRootObject(), ao );
        ::ref(o);
        o->setName(layerName);
        }

      prev_layer_name= layer_name;
      prev_ao= ao;
      }
    }
   else
    {
    if( object == 0 )
      ao= main_object;
     else
      ao= object;
    }

  MeshRep* mesh= ao->getMeshRep();

  importer->addPolygon(mesh,nVertices,points);
}

/////////////////////////////////////////////////////////////////////////////

/*
void DXFLoader::start_exploding_BLOCK( const char* name, int id )
{
  assert( name != 0 );

  char* s= new char [ strlen(name)+20+1 ];

  int inst= 1;
  for(;;)
    {
    sprintf( s, "%s_%d", name, inst );
    if( scene->findObject(MLString(s)) == 0 )
      break;
    ++inst;
    }
#ifdef DXF_DEBUG
  fprintf( stderr, "DXFLoader: start exploding block %s\n", s );
#endif

  Object* curObject= scene->getCurrentObject();
  scene->setCurrentObject( scene->newObject( curObject, s ) );
}
*/

/////////////////////////////////////////////////////////////////////////////

/*
void DXFLoader::end_exploding_BLOCK( const char* name, int id )
{
  assert( ! scene->getCurrentObject()->isRootObject() );

#ifdef DXF_DEBUG
  fprintf( stderr, "DXFLoader: end exploding block %s\n", name );
#endif

  Object* curObject= scene->getCurrentObject();
  scene->setCurrentObject( curObject->getParent() );
}
*/

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::start_BLOCK( const char* name, const Point& basePoint )
{
  base_point= basePoint;

  if( layers2Objects )
    return;

  MLString nn(name);
  importer->convertName(nn);

  if( object != 0 )
    {
    if( object_name == nn )
      return;
    fprintf( stderr, "Warning: recursive included block!! (%s)\n",nn.get());
    addObject(object);
    }

  MLName _name;
  _name.name= nn;
  MLAbstractMesh* already= scene->getAbstractObject(_name);
  if( already != 0 )
    {
    // this object is already defined!
    fprintf( stderr, "Warning: redefining block %s\n",nn.get());
    object= already;
    return;
    }

  insertion_point= Point(0,0,0);
  scale= Vector(1,1,1);
  zrotate= 0.;

  object_name= MLString(name);
  importer->convertName(object_name);

  object= new MLAbstractMesh(scene,0);
  object->setName(object_name);
  object->generateMesh();
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::end_BLOCK()
{
  if( ! layers2Objects )
    addObject(object);

  base_point= Point(0,0,0);
  insertion_point= Point(0,0,0);
  scale= Vector(1,1,1);
  zrotate= 0.;
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::insert_BLOCK( const char* name,
                              const Point& insertionPoint,
                              const Vector& _scale,
                              double rotationAngle )
{
  if( name == 0 )
    return;

  if( layers2Objects )
    {
    fprintf( stderr,
             "Error: unable to insert object '%s' in `layers to objects' mode\n",
             name );
    return;
    }

  MLString nn(name);
  importer->convertName(nn);

  MLObject* father= 0; //object;

  MLName _name;
  _name.name= nn;
  MLAbstractMesh* already= scene->getAbstractObject(_name);
  if( already == 0 )
    {
    fprintf( stderr, "Error: inserting unknown object '%s'\n", nn.get() );
    return;
    }

  insertion_point= insertionPoint;
  scale= _scale;
  zrotate= rotationAngle;

  addObject( already, father );
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::start_MAIN()
{
  main_object= new MLAbstractMesh(scene,0);
  main_object->generateMesh();

  base_point= Point(0,0,0);
  insertion_point= Point(0,0,0);
  scale= Vector(1,1,1);
  zrotate= 0.;
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::end_MAIN()
{
  base_point= Point(0,0,0);
  insertion_point= Point(0,0,0);
  scale= Vector(1,1,1);
  zrotate= 0.;

  addObject(main_object);
}

/////////////////////////////////////////////////////////////////////////////

void DXFLoader::addObject( MLAbstractMesh* ao, MLObject* father /*=0*/ )
{
  if( ao == 0 )
    return;
  MeshRep* mesh= ao->getMeshRep();

  if( mesh->nVertices == 0 || mesh->nPolygons == 0 )
    return;

  MLName _name;
  _name.name= object_name;
  MLAbstractMesh* already= scene->getAbstractObject(_name);
  if( already != 0 )
    {
    fprintf( stderr, "Warning: trying to redefine %s\n", object_name.get());
    // this object is already defined!
    return;
    }

  MLObject* o= new MLMesh( (father==0) ? scene->getRootObject() : 0, ao );
  ::ref(o);

  fprintf( stderr, "DXFLoader: adding new object from geometry '%s'\n",
           ao->getName().name.get() );

//  ao->setName(object_name);
}

/////////////////////////////////////////////////////////////////////////////

