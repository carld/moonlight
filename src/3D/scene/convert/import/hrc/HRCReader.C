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
  HRCReader.C

  Stephane Rehel

  June 18th, 1996

  ML version: October 8 1997

  Yes, yes, definitely yes, using the Tokenize class is a ugly way...
  flex/bison should be far better.
  /SR October.8.1997
*/

#include <stdio.h>

#include "HRCReader.h"

#include "Token.h"
#include "Tokenize.h"

#include "scene/MeshRep.h"
#include "scene/MLAbstractMesh.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLScene.h"

#include "scene/convert/import/MLImporter.h"

/////////////////////////////////////////////////////////////////////////////

HRCReader::HRCReader( MLImporter* _importer )
{
  importer= _importer;
  tokensList= 0;
  scene= importer->scene;
  verbose= importer->verbose;
  file= 0;
  tk= 0;
}

/////////////////////////////////////////////////////////////////////////////

HRCReader::~HRCReader()
{
  importer->close_file(file);
  file= 0;

  delete tk;
  tk= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::init()
{
  file= importer->open_file();

  if( file == 0 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::load()
{
  return readHRCModel() != 0;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* HRCReader::readHRCModel()
{
  if( file == 0 )
    return 0;

  delete tk;
  tk= new Tokenize;

  tk->openInputFile(file);

  boolean outcome= tk->tokenize(100);

  int nErrors= tk->getNErrors() - 2;
  if( verbose && !outcome > 0 )
    fprintf( stderr, "%d parsing errors\n", nErrors );

  tokensList= &tk->getTokens();

  for(;;)
    {
    Token* t= nextToken();
    if( t == 0 )
      return 0;

    if( t->getType() != Token::NAME )
      continue;
    if( t->getName() == MLString("model") )
      break;
    }

  MLObject* mainModel= readSubModel(scene->getRootObject());

  if( mainModel == 0 )
    {
    int line;
    Token* t= nextToken();
    if( t == 0 )
      line= 0;
     else
      line= t->getLine();

    fprintf( stderr, "error line %d\n", line );
    }

  mainModel->updateTransformations();
  mainModel->updateMaterials();

  return mainModel;
}

/////////////////////////////////////////////////////////////////////////////

Token* HRCReader::nextToken()
{
  assert( tokensList != 0 );

  static Token* prevToken= 0;

  if( tokensList->empty() )
    {
    tk->tokenize(100);
    if( tokensList->empty() )
      return 0;
    }

  Token* t= tokensList->getFirst();
  tokensList->removeFirst();

  delete prevToken;
  prevToken= t;

  return t;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::skipBrace()
{
  for(;;)
    {
    Token* t= nextToken();
    if( t == 0 )
      break;

    if( t->getType() != Token::CHAR )
      continue;
    if( t->getChar() == Token::Char('{') )
      {
      if( ! skipBrace() )
        return false;
      }
     else
      if( t->getChar() == '}' )
        return true; // got it
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void HRCReader::skipToken()
{
  nextToken();
}

/////////////////////////////////////////////////////////////////////////////

void HRCReader::skipTokens( int n )
{
  while( n-- > 0 )
    skipToken();
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::needChar( Token::Char theChar )
{
  Token* t= nextToken();
  if( t == 0 )
    return false;

  if( t->getType() != Token::CHAR )
    return false;

  return t->getChar() == theChar;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::findChar( Token::Char theChar )
{
  for(;;)
    {
    Token* t= nextToken();
    if( t == 0 )
      break;

    if( t->getType() != Token::CHAR )
      continue;

    if( t->getChar() == theChar )
      return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::needName( const MLString& theName )
{
  Token* t= nextToken();
  if( t == 0 )
    return false;

  if( t->getType() != Token::NAME )
    return false;

  return t->getName() == theName;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getScalar( double& scalar )
{
  Token* t= nextToken();
  if( t == 0 )
    return false;

  boolean neg= false;

  if( t->getType() == Token::CHAR )
    {
    if( t->getChar() != Token::Char('-') )
      return false;
    neg= true;
    t= nextToken();
    if( t == 0 )
      return false;
    }

  if( t->getType() != Token::SCALAR )
    return false;

  scalar= t->getScalar();

  if( neg )
    scalar= -scalar;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getNamedScalar( const MLString& name, double& scalar )
{
  if( ! needName(name) ) return false;

  return getScalar(scalar);
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getString( MLString& string )
{
  Token* t= nextToken();
  if( t == 0 )
    return false;

  if( t->getType() != Token::STRING )
    return false;

  string= t->getString();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getVector( Vector& vector )
{
  double x, y, z;
  if( ! getScalar(x) ) return false;
  if( ! getScalar(y) ) return false;
  if( ! getScalar(z) ) return false;

  vector= Vector(x,y,z);
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getVector2( Vector2& vector )
{
  double x, y;
  if( ! getScalar(x) ) return false;
  if( ! getScalar(y) ) return false;

  vector= Vector2(x,y);
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getNamedVector( const MLString& name, Vector& vector )
{
  if( ! needName(name) ) return false;

  return getVector(vector);
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getNamedVector2( const MLString& name, Vector2& vector )
{
  if( ! needName(name) ) return false;

  return getVector2(vector);
}

/////////////////////////////////////////////////////////////////////////////

boolean HRCReader::getNamedPoint( const MLString& name, Point& point )
{
  if( ! needName(name) ) return false;
  Vector v;
  if( ! getVector(v) ) return false;
  point= Point(v.x(),v.y(),v.z());

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* HRCReader::readMesh( MLAbstractMesh* model )
{
  if( ! needChar( Token::Char('{') ) )
    return 0;

  if( ! needName( "flag" ) )
    return 0;
  if( ! findChar( Token::Char(')') ) )
    return 0;

  double discontinuity= 0;
  if( ! getNamedScalar( "discontinuity", discontinuity ) )
    return 0;

  double _nVertices;
  if( ! getNamedScalar( "vertices", _nVertices ) ) return 0;
  int nVertices= int(_nVertices);
  if( nVertices <= 0 ) return 0;

  if( ! needChar( Token::Char('{') ) ) return 0;

  MeshRep* mesh= model->getMeshRep();

  if( nVertices > 1000000 && verbose )
    fprintf( stderr, "warning: nVertices = %d\n", nVertices );

  Point* vertices= new Point [ nVertices ];
  int i;

  for( i= 0; i < nVertices; ++i )
    {
    if( ! findChar( Token::Char('[') ) )
      return 0;

    skipTokens(2);
    if( ! getNamedPoint( "position", vertices[i] ) )
      return 0;
    }

  if( ! findChar( Token::Char('}') ) ) return 0;

  double _nPolygons;
  if( ! getNamedScalar( "polygons", _nPolygons ) ) return 0;
  int nPolygons= int(_nPolygons);
  if( nPolygons <= 0 ) return 0;

  if( ! needChar( Token::Char('{') ) ) return 0;

  if( nPolygons > 100000 && verbose )
    fprintf( stderr, "warning: nPolygons = %d\n", nPolygons );

  mesh->alloc( nVertices, 0, nPolygons );

  for( i= 0; i < nVertices; ++i )
    {
    MLVertex& v= mesh->vertices[i+1];
    v.flags |= MLVertex::ALLOCATED;
    v.point= vertices[i];
    }

  delete vertices;
  vertices= 0;

  for( i= 0; i < nPolygons; ++i )
    {
    if( ! findChar( Token::Char('[') ) )
      return 0;
    skipTokens(2);

    double _nNodes;
    if( ! getNamedScalar( "nodes", _nNodes ) )
      return 0;
    int nNodes= int(_nNodes);
    if( nNodes <= 0 ) return 0;

    if( ! needChar( Token::Char('{') ) ) return 0;

    if( nNodes > 16 && verbose )
      fprintf( stderr, "warning: nNodes = %d for polygon %d\n", nNodes,i );

    MLPolygon& polygon= mesh->polygons[i+1];
    polygon.alloc(nNodes);
    polygon.flags |= MLPolygon::ALLOCATED;

    for( int j= 0; j < nNodes; ++j )
      {
      if( ! findChar( Token::Char('[') ) )
        return 0;
      skipTokens(2);

      double _vertex;
      if( ! getNamedScalar( "vertex", _vertex ) )
        return 0;
      int vertex= int(_vertex);
      if( vertex < 0 || vertex >= nVertices )
        return 0;

      polygon.elements[j].vertex= vertex + 1;

      Vector normal;
      Vector2 uvTexture;
      if( ! getNamedVector( "normal", normal ) ) return 0;
      if( ! getNamedVector2( "uvTexture", uvTexture ) ) return 0;
      }

    if( ! findChar( Token::Char('}') ) ) return 0;

    double _material;
    if( ! getNamedScalar( "material", _material ) )
      return 0;
    }

  if( ! findChar( Token::Char('}') ) ) return 0;

  if( ! needName( "edges" ) ) return 0;
  if( ! findChar( Token::Char('{') ) )
    return 0;
  if( ! skipBrace() )
    return 0;

  // closing mesh { ... }
  if( ! skipBrace() ) return 0;

  return model;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* HRCReader::readSubModel( MLObject* father )
{
  if( ! needChar( Token::Char('{') ) )
    return 0;

  MLMesh* model= new MLMesh(father);
  ::ref(model);

  if( ! needName( "name" ) )
    return 0;
  MLString model_name("");
  if( ! getString(model_name) )
    return 0;

  if( verbose )
    fprintf( stderr, "reading model %s\n", model_name.get() );

  importer->convertName(model_name);
//  model->setName( model_name );

  Vector scale, rotate, translate;
  if( ! getNamedVector( "scaling", scale ) ) return 0;
  if( ! getNamedVector( "rotation", rotate ) ) return 0;
  if( ! getNamedVector( "translation", translate ) ) return 0;

  model->scale(scale);
  model->rotate(rotate);
  model->translate(translate);

  // skip edges, material, etc.
  for(;;)
    {
    Token* t= nextToken();
    if( t == 0 )
      return 0;

    if( t->getType() == Token::CHAR )
      if( t->getChar() == Token::Char('}') )
        break;

    if( t->getType() != Token::NAME )
      return 0;

    if( t->getName() == MLString("model") )
      {
      MLObject* child= readSubModel(model);
      if( child == 0 )
        return 0;
      continue;
      }

    if( t->getName() == MLString("mesh") )
      {
      MLAbstractMesh* ao= new MLAbstractMesh(model->getScene());
      ao->generateMesh();

      if( readMesh(ao) == 0 )
        {
        delete ao;
        return 0;
        }

      model->setAbstract(ao);

      continue;
      }

    if( ! findChar( Token::Char('{') ) )
      return 0;
    if( ! skipBrace() )
      return 0;
    }

  return model;
}

/////////////////////////////////////////////////////////////////////////////
