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
  MLAbstractCurve.h

  Stephane Rehel

  January 21 1998
*/

#ifndef __MLAbstractCurve_h
#define __MLAbstractCurve_h

#ifndef __Box_h
#include "tools/Box.h"
#endif

#ifndef __MLAbstractObject_h
#include "MLAbstractObject.h"
#endif

class FormalCurveRep;
class CurveRep;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

class MLAbstractCurve: public MLAbstractObject
{
  friend class MLAbstractCurveAllocator;
  friend class ModelDraw;
  friend class MLCurve;

  friend class MLAbstractCurveIO;
  friend class MLSceneIO; // set name.filename when reading

private:
  int abstract_curve_id;

protected:
  FormalCurveRep* formalRep;
  CurveRep* curveRep;

public:
  MLAbstractCurve( MLScene* _scene, FormalCurveRep* _formalRep = 0 );

  virtual ~MLAbstractCurve();

  int getSizeOf() const;

  void addReference( MLCurve* instance );
  void removeReference( MLCurve* instance );

  MLAbstractCurve* duplicate();

  CurveRep* getCurveRep() const
    {
    return curveRep;
    }

  FormalCurveRep* getFormalRep() const
    {
    return formalRep;
    }

  void setFormalRep( FormalCurveRep* _formalRep );
  void generateCurve();

//  Box getBBox() const;

  int getID() const
    {
    return abstract_curve_id;
    }

  void polygonize();
  void unpolygonize( FormalCurveRep* _formalRep );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAbstractCurve_h
