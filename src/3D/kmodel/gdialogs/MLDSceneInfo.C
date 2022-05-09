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
  MLDSceneInfo.C

  Stephane Rehel

  August 7 1997
*/

#include "kw/KWDialog.h"

#include "scene/MLScene.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"
#include "scene/MLChildrenIterator.h"

#include "kmodel/ModelModule.h"

#include "MLDSceneInfo.h"
#include "SceneInfoDlg.h"

/////////////////////////////////////////////////////////////////////////////

MLDSceneInfo::MLDSceneInfo( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
}

/////////////////////////////////////////////////////////////////////////////

MLDSceneInfo::~MLDSceneInfo()
{
}

/////////////////////////////////////////////////////////////////////////////

void MLDSceneInfo::run()
{
  if( ! MLGlobalDialog::create(SceneInfoDlg::data) )
    return;

  MLScene* scene= model->getScene();

  int nVertices= 0;
  int nSVertices= 0;
  int nEdges= 0;
  int nPolygons= 0;
  int nTessTriangles= 0;
  int nTriangles= 0;
  make_list( scene->getRootObject(),
             nVertices, nSVertices, nEdges,
             nPolygons, nTessTriangles, nTriangles, 0 );

  kwDialog->setText( SceneInfoDlg::sceneName, scene->getName().name );

  kwDialog->focus(SceneInfoDlg::close);

  kwDialog->run();

  MLGlobalDialog::close();
}

/////////////////////////////////////////////////////////////////////////////

void MLDSceneInfo::make_list( MLObject* o,
                              int& nVertices, int& nSVertices,
                              int& nEdges,
                              int& nPolygons, int& nTessTriangles,
                              int& nTriangles, int index )
{
  if( o == 0 )
    return;

  int _nVertices= 0;
  int _nSVertices= 0;
  int _nEdges= 0;
  int _nPolygons= 0;
  int _nTessTriangles= 0;
  int _nTriangles= 0;

  if( o->getObjectType() == MLObject::MESH )
    {
    MLMesh* mesh= (MLMesh*) o;

    mesh->getInfo( _nVertices, _nSVertices, _nEdges,
                   _nPolygons, _nTessTriangles, _nTriangles );
    }

  nVertices+= _nVertices;
  nSVertices+= _nSVertices;
  nEdges+= _nEdges;
  nPolygons+= _nPolygons;
  nTessTriangles+= _nTessTriangles;
  nTriangles+= _nTriangles;

  MLChildrenIterator ci(o);
  while( ! ci.eol() )
    make_list( ci++, nVertices, nSVertices, nEdges,
                     nPolygons, nTessTriangles, nTriangles, index+1 );

  MLString name= o->getName().name;
  boolean isRoot= (o == o->getScene()->getRootObject());

  if( isRoot )
    name= "root";
  for( int i= 0; i < index; ++i )
    name= MLString(" ") + name;

  int name_length= 19;
  if( name.length() > name_length )
    name= name.substring(0,name_length-2) + MLString("+");
   else
    {
    while( name.length() < name_length )
      name += ' ';
    }

  if( ! isRoot )
    {
    MLString strg= MLString::printf( "%s %7d %7d %7d %7d %7d %7d",
                                     name.get(),
                                     _nVertices, _nSVertices, _nEdges,
                                     _nPolygons, _nTessTriangles, _nTriangles );
    kwDialog->insertListText( SceneInfoDlg::list, 1, strg );
    }

  if( o->getNChildren() > 1 || isRoot )
    {
    MLString total= name;
    total[total.length() - 1]= '=';
//    for( int i= 0; i < index; ++i )
//      total= MLString(" ") + total;
//    while( total.length() < name_length )
//      total += ' ';

    MLString strg= MLString::printf( "%s %7d %7d %7d %7d %7d %7d",
                                     total.get(),
                                     nVertices, nSVertices, nEdges,
                                     nPolygons, nTessTriangles,
                                     nTriangles );

    kwDialog->insertListText( SceneInfoDlg::list, 1, strg );
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDSceneInfo::kwCallback( KWEvent& event )
{
  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

