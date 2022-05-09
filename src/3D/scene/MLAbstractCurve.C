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
  MLAbstractCurve.C

  Stephane Rehel

  January 21 1998
*/

#include <stdio.h>
#include <assert.h>

#include "MLAbstractCurve.h"
#include "MLAbstractCurveAllocator.h"

#include "formal/FormalCurveRep.h"

#include "CurveRep.h"
#include "MLCurve.h"
#include "MLScene.h"
#include "MLObjectAllocator.h"

/////////////////////////////////////////////////////////////////////////////

static const int null_id= -1;

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve::MLAbstractCurve( MLScene* _scene,
                                  FormalCurveRep* _formalRep /* = 0 */ ):
  MLAbstractObject(_scene)
{
  abstract_curve_id= scene->abstractCurveAllocator->allocID(this);

  name.name= MLString("curve-") + MLString::valueOf(abstract_curve_id);

  formalRep= _formalRep;
  curveRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve::~MLAbstractCurve()
{
#ifndef NDEBUG
printf("ABSTRACT CURVE '%s' DELETED\n",name.name.get());
#endif

  scene->abstractCurveAllocator->freeID(abstract_curve_id);
  abstract_curve_id= null_id;

  delete formalRep;
  formalRep= 0;
//***BUG???  delete curveRep;  -> from MLAbstractMesh::~()

  curveRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

int MLAbstractCurve::getSizeOf() const
{
  int s= sizeof(*this) + max_instances * sizeof(instances[0]);

  if( formalRep != 0 )
    s+= formalRep->getSizeOf();

  if( curveRep != 0 )
    s+= curveRep->getSizeOf();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurve::addReference( MLCurve* instance )
{
  if( instance == 0 )
    return;

  if( MLAbstractObject::_addReference(instance) )
    instance->abstract= this;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurve::removeReference( MLCurve* instance )
{
  if( instance == 0 )
    return;

  if( MLAbstractObject::_removeReference(instance) )
    {
    if( nInstances > 0 )
      instance->abstract= 0;
      // else: dont delete me please, i'm still referenced
    }
   else
    {
    // not found...
    instance->abstract= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractCurve* MLAbstractCurve::duplicate()
{
  FormalCurveRep* fr= 0;
  if( formalRep != 0 )
    fr= formalRep->duplicate();

  MLAbstractCurve* ac= new MLAbstractCurve(scene,fr);

  if( curveRep != 0 )
    ac->curveRep= curveRep->duplicate();

//  ao->bbox= bbox;

  return ac;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurve::setFormalRep( FormalCurveRep* _formalRep )
{
  if( formalRep != 0 )
    delete formalRep;

  formalRep= _formalRep;
  delete curveRep;
  curveRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurve::generateCurve()
{
  if( curveRep == 0 )
    curveRep= new CurveRep;

  if( formalRep != 0 )
    formalRep->generate(curveRep);
}

/////////////////////////////////////////////////////////////////////////////

/*
Box MLAbstractCurve::getBBox() const
{
  if( curveRep == 0 )
    return Box( Point(0,0,0), Point(1,1,1) * 1e-8 );

  return curveRep->bbox;
}
*/

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurve::polygonize()
{
  formalRep= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLAbstractCurve::unpolygonize( FormalCurveRep* _formalRep )
{
  formalRep= _formalRep;
}

/////////////////////////////////////////////////////////////////////////////
