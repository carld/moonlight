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
  MLControlPointID.C

  Stephane Rehel

  January 11 1998
*/

#include "MLControlPointID.h"
#include "formal/FormalRep.h"
#include "formal/FormalNURBS.h"
#include "formal/FormalNURBSCurve.h"
#include "nurbs/MLNURBS_Surface.h"
#include "nurbs/MLNURBS_Curve.h"
#include "nurbs/MLNURBS_ControlPoint.h"
#include "MeshRep.h"
#include "CurveRep.h"
#include "MLMesh.h"
#include "MLCurve.h"
#include "MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLMesh* MLControlPointID::getMesh( MLScene* scene )
{
  if( scene == 0 )
    return 0;

  return (MLMesh*) scene->getObject(object,MLObject::MESH);
}

/////////////////////////////////////////////////////////////////////////////

MLCurve* MLControlPointID::getCurve( MLScene* scene )
{
  if( scene == 0 )
    return 0;

  return (MLCurve*) scene->getObject(object,MLObject::CURVE);
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS* MLControlPointID::getNURBS( MLScene* scene )
{
  MLMesh* mesh= getMesh(scene);
  if( mesh != 0 )
    {
    FormalNURBS* fnurbs= mesh->getFormalNURBS();
    if( fnurbs == 0 )
      return 0;

    return fnurbs->getSurface();
    }

  MLCurve* curve= getCurve(scene);
  if( curve != 0 )
    {
    FormalNURBSCurve* fnurbs= curve->getFormalNURBS();
    if( fnurbs == 0 )
      return 0;

    return fnurbs->getCurve();
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLNURBS_ControlPoint* MLControlPointID::getControlPoint( MLScene* scene )
{
  if( cp_index <= 0 )
    return 0;

  MLNURBS* nurbs= getNURBS(scene);
  if( nurbs == 0 )
    return 0;

  if( nurbs->isCurve() )
    {
    MLNURBS_Curve* curve= (MLNURBS_Curve*) nurbs;

    int i= cp_index-1;

    if( i < 0 || i > curve->getn() )
      return 0;

    return &(curve->getControlPoint(i));
    }

  MLNURBS_Surface* surface= (MLNURBS_Surface*) nurbs;

  int j= (cp_index-1) / (surface->getn()+1);
  int i= (cp_index-1) - j * (surface->getn()+1);

  if( i < 0 || i > surface->getn() || j < 0 || j > surface->getm() )
    return 0;

  return &(surface->getControlPoint(i,j));
}

/////////////////////////////////////////////////////////////////////////////
