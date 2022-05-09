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
  MLCurve.h

  Stephane Rehel

  January 21 1998
*/

#ifndef __MLCurve_h
#define __MLCurve_h

#ifndef __MLObject_h
#include "MLObject.h"
#endif

// for MLCurve::getCurveRep()
#ifndef __MLAbstractCurve_h
#include "MLAbstractCurve.h"
#endif

class FormalCurveRep;
class FormalNURBSCurve;
class CurveRep;

/////////////////////////////////////////////////////////////////////////////

class MLCurve: public MLObject
{
  friend class MLScene;
  friend class MLAbstractCurve;
  friend class ModelScene;

  friend class MLCurveIO;

protected:
  MLAbstractCurve* abstract;

public:
  MLCurve( MLObject* _father, MLAbstractCurve* _abstract = 0 );

  virtual ~MLCurve();

  virtual int getSizeOf() const;

protected:
  void copy( MLCurve* m ) const;

public:
  virtual MLObject* duplicate() const;

  virtual ObjectType getObjectType() const
    {
    return MLObject::CURVE;
    }

  // return false if not bounded
  virtual boolean getLocalBBox( Box& bbox ) const;

  MLAbstractCurve* getAbstract() const
    {
    return abstract;
    }

  // CurveRep must be access quickly since raytracing needs it
  CurveRep* getCurveRep() const
    {
    if( abstract == 0 )
      return 0;
    return abstract->getCurveRep();
    }

  FormalCurveRep* getFormalRep() const;

  boolean isFormal() const
    {
    return getFormalRep() != 0;
    }

  FormalNURBSCurve* getFormalNURBS() const;

  boolean isNURBS() const
    {
    return getFormalNURBS() != 0;
    }

  void setAbstract( MLAbstractCurve* _abstract );

  void regenerate();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLCurve_h
