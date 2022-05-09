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
  MLControlPointID.h

  Stephane Rehel

  January 11 1998
*/

#ifndef __MLControlPointID_h
#define __MLControlPointID_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLScene;
class MLMesh;
class MLCurve;
class MLNURBS;
class MLNURBS_ControlPoint;

/////////////////////////////////////////////////////////////////////////////

class MLControlPointID
{
public:
  int object;
  int cp_index;

public:
  MLControlPointID()
    {
    object= cp_index= -1;
    }

  MLControlPointID( int _object, int _cp_index )
    {
    object= _object;
    cp_index= _cp_index;
    }

  MLControlPointID( const MLControlPointID& cpid )
    {
    operator = ( cpid );
    }

  MLControlPointID& operator = ( const MLControlPointID& cpid )
    {
    object= cpid.object;
    cp_index= cpid.cp_index;

    return *this;
    }

  boolean operator == ( const MLControlPointID& cpid ) const
    {
    return cp_index == cpid.cp_index && object == cpid.object;
    }
  boolean operator != ( const MLControlPointID& cpid ) const
    {
    return ! operator == ( cpid );
    }

  MLMesh* getMesh( MLScene* scene );
  MLCurve* getCurve( MLScene* scene );
  MLNURBS* getNURBS( MLScene* scene );
  MLNURBS_ControlPoint* getControlPoint( MLScene* scene );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLControlPointID_h
