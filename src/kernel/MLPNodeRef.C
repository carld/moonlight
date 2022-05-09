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
  MLPNodeRef.C

  Stephane Rehel
  April 15 1998
*/

#include "tools/xdr/XDRFile.h"

#include "MLPNodeRef.h"

#include "MLKernel.h"
#include "MLNode.h"

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPNodeRef::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  return file->putInteger( (noderef==0) ? 0 : noderef->getID() );
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPNodeRef::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  int id= 0;
  if( ! file->getInteger(&id) )
    return false;

  assert( MLProperty::getKernel() != 0 );
  MLNode* _noderef= MLProperty::getKernel()->getNode(id);
  set(_noderef);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPNodeRef::_serialSize() const
{
  return XDROp::getIntegerSize();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPNodeRef::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPNodeRef*)p)->set(noderef);
  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPNodeRef::clone() const
{
  MLPNodeRef* p= new MLPNodeRef(0);
  p->set(noderef);
  return p;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPNodeRef::toString() const
{
  MLString name= (noderef==0) ? "nil" : noderef->getName();

  return MLString("NodeRef(") + name + MLString(")");
}

/////////////////////////////////////////////////////////////////////////////
