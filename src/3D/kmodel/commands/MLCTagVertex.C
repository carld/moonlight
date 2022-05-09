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
  MLCTagVertex.C

  Stephane Rehel

  October 29 1997
*/

#include "tools/3D/MLVertex.h"

#include "scene/nurbs/MLNURBS_ControlPoint.h"

#include "kmodel/ModelModule.h"
#include "kmodel/MLTagVertexArray.h"
#include "kmodel/ModelCommands.h"

#include "MLCTagVertex.h"

/////////////////////////////////////////////////////////////////////////////

MLCTagVertex::MLCTagVertex( MLScene* scene ): set(scene), unset(scene)
{
  type= ModelCommands::TAG_VERTEX_SET; // default value
}

/////////////////////////////////////////////////////////////////////////////

MLCTagVertex::~MLCTagVertex()
{}

/////////////////////////////////////////////////////////////////////////////

boolean MLCTagVertex::doit( ModelModule* model )
{
  MLCTagVertex* undo= new MLCTagVertex(model->getScene());
  ModelCommand::createUndo(undo);

  int i;

  if( ModelCommand::getType() == ModelCommands::TAG_VERTEX_TOGGLE )
    {
    undo->type= ModelCommands::TAG_VERTEX_TOGGLE;
    undo->set= set;

    int nVertices= set.getNVertices();

    for( i= 0; i < nVertices; ++i )
      {
      if( set.isControlPoint(i) )
        {
        MLNURBS_ControlPoint* cp= set.getControlPoint(i);
        if( cp != 0 )
          {
          if( cp->selected() )
            cp->flags &= ~MLNURBS_ControlPoint::SELECTED;
           else
            cp->flags |=  MLNURBS_ControlPoint::SELECTED;
          }
        continue;
        }

      MLVertex* v= set.getVertex(i);
      if( v != 0 )
        {
        if( v->selected() )
          v->flags &= ~MLVertex::SELECTED;
         else
          v->flags |=  MLVertex::SELECTED;
        continue;
        }
      }
    }
   else
    {
    undo->type= ModelCommands::TAG_VERTEX_SET;

    undo->unset.alloc( set.getSize() );
    int nVertices= set.getNVertices();
    int nToUnset= 0;
    for( i= 0; i < nVertices; ++i )
      {
      if( set.isControlPoint(i) )
        {
        MLNURBS_ControlPoint* cp= set.getControlPoint(i);
        if( cp != 0 )
          {
          if( ! cp->selected() )
            {
            cp->flags |= MLNURBS_ControlPoint::SELECTED;
            int k= nToUnset*2;
            undo->unset[ k     ]= set[ i*2     ];
            undo->unset[ k + 1 ]= set[ i*2 + 1 ];
            ++nToUnset;
            }
          }
        continue;
        }

      MLVertex* v= set.getVertex(i);
      if( v != 0 )
        {
        if( ! v->selected() )
          {
          v->flags |= MLVertex::SELECTED;
          int k= nToUnset*2;
          undo->unset[ k     ]= set[ i*2     ];
          undo->unset[ k + 1 ]= set[ i*2 + 1 ];
          ++nToUnset;
          }
        continue;
        }
      }
    undo->unset.truncate(nToUnset*2);

    undo->set.alloc( unset.getSize() );
    int nToSet= 0;
    nVertices= unset.getNVertices();
    for( i= 0; i < nVertices; ++i )
      {
      if( unset.isControlPoint(i) )
        {
        MLNURBS_ControlPoint* cp= unset.getControlPoint(i);
        if( cp != 0 )
          {
          if( cp->selected() )
            {
            cp->flags &= ~MLNURBS_ControlPoint::SELECTED;
            int k= nToSet*2;
            undo->set[ k     ]= unset[ i*2     ];
            undo->set[ k + 1 ]= unset[ i*2 + 1 ];
            ++nToSet;
            }
          }
        continue;
        }

      MLVertex* v= unset.getVertex(i);
      if( v != 0 )
        {
        if( v->selected() )
          {
          v->flags &= ~MLVertex::SELECTED;
          int k= nToSet*2;
          undo->set[ k     ]= unset[ i*2     ];
          undo->set[ k + 1 ]= unset[ i*2 + 1 ];
          ++nToSet;
          }
        continue;
        }
      }
    undo->set.truncate(nToSet*2);
    }

  model->refreshScreen( ModelModule::REFRESH_SELECTION );

  return true;
}

/////////////////////////////////////////////////////////////////////////////
