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
  MLCurve.C

  Stephane Rehel

  January 21 1998
*/

#include "MLScene.h"
#include "MLCurve.h"

#include "formal/FormalCurveRep.h"
#include "formal/FormalNURBSCurve.h"

#include "CurveRep.h"

/////////////////////////////////////////////////////////////////////////////

MLCurve::MLCurve( MLObject* _father, MLAbstractCurve* _abstract /* = 0 */ ):
  MLObject(_father)
{
  abstract= 0;

  setAbstract(_abstract); // set hierBBox info, etc.
}

/////////////////////////////////////////////////////////////////////////////

MLCurve::~MLCurve()
{
  if( abstract != 0 )
    {
    MLAbstractCurve* remember= abstract;
    abstract->removeReference(this);
    abstract= 0; // already done normally
    ::unref(remember); // possibily deletes it
    }
}

/////////////////////////////////////////////////////////////////////////////

// do not add children size
int MLCurve::getSizeOf() const
{
  int s= MLObject::getSizeOf();

  if( abstract != 0 )
    s+= abstract->getSizeOf();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

// same father. Dont duplicate children.
MLObject* MLCurve::duplicate() const
{
  if( isRoot() )
    return 0; // why do you want to duplicate root??!!

  MLAbstractCurve* ac= 0;
  if( abstract != 0 )
    ac= abstract->duplicate();

  MLCurve* c= new MLCurve( getFather(), ac );

  ::ref(c);

  copy(c);

  return c;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLCurve::copy( MLCurve* c ) const
{
  if( c == 0 )
    return;
  if( father == 0 )
    return; // why do you want to duplicate root??!!

  MLObject::copy( (MLObject*) c );
}

/////////////////////////////////////////////////////////////////////////////

// return false if not bounded
boolean MLCurve::getLocalBBox( Box& bbox ) const
{
// mmhh,....
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLCurve::setAbstract( MLAbstractCurve* _abstract )
{
  if( abstract == _abstract )
    return;

  if( abstract != 0 )
    {
    MLAbstractCurve* remember= abstract;
    abstract->removeReference(this);
    unref(remember); // possibly deletes it
    }

  abstract= _abstract;
  if( abstract != 0 )
    {
    abstract->addReference(this);
    ref(abstract);
    }

  updateHierBBox();
}

/////////////////////////////////////////////////////////////////////////////

FormalCurveRep* MLCurve::getFormalRep() const
{
  if( abstract == 0 )
    return 0;

  return abstract->getFormalRep();
}

/////////////////////////////////////////////////////////////////////////////

FormalNURBSCurve* MLCurve::getFormalNURBS() const
{
  FormalCurveRep* formal= getFormalRep();
  if( formal == 0 )
    return 0;
  if( formal->getType() != FormalCurveRep::NURBS )
    return 0;

  FormalNURBSCurve* fnurbs= (FormalNURBSCurve*) formal;

  return fnurbs;
}

/////////////////////////////////////////////////////////////////////////////

void MLCurve::regenerate()
{
  CurveRep* c= getCurveRep();

  if( c == 0 )
    return;

  FormalCurveRep* formalRep= getFormalRep();

  if( formalRep == 0 )
    return;

  formalRep->generate(c);

//***  c->invalidateBoundings();
//***  c->computeBoundings();

//  MLScene* scene= MLObject::getScene();
//  scene->getRootObject()->updateHierBBox();
}

/////////////////////////////////////////////////////////////////////////////
