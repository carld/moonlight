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
  MLPNodeStruct.C

  Stephane Rehel
  April 15 1998
*/

#include "tools/xdr/XDRFile.h"

#include "MLPNodeStruct.h"

#include "MLNode.h"

/////////////////////////////////////////////////////////////////////////////

boolean MLPNodeStruct::set( MLNode* _nodestr )
{
  if( nodestr == _nodestr )
    return false;

  if( _nodestr == 0 )
    {
    unref(nodestr);
    nodestr= 0;
    return true;
    }

  MLNode* __nodestr= _nodestr->clone();
  if( __nodestr == 0 )
    return true; // error

  ref(__nodestr);
  unref(nodestr);
  nodestr= __nodestr;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPNodeStruct::_serialWrite( XDRFile* file )
{
  if( file == 0 )
    return false;

  if( nodestr == 0 )
    return file->putInteger(0);

  if( ! file->putInteger(1) )
    return false;

  return nodestr->serialWrite(file);
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
boolean MLPNodeStruct::_serialRead( XDRFile* file )
{
  if( file == 0 )
    return false;

  int null_node;

  if( ! file->getInteger(&null_node) )
    return false;

  set(0);

  if( null_node == 0 )
    return true;

  nodestr= MLNode::nodeSerialRead( 0, /*approot*/
                                   0, /*parent*/
                                   file );
  if( nodestr == 0 )
    return false;

  ref(nodestr);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// return the size (in bytes) that will be read/written by
// serialRead()/serialWrite() on this property
int MLPNodeStruct::_serialSize() const
{
  int s= XDROp::getIntegerSize();

  if( nodestr != 0 )
    s+= nodestr->serialSize();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLPNodeStruct::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPNodeStruct*)p)->set(nodestr);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLProperty* MLPNodeStruct::clone() const
{
  if( nodestr == 0 )
    return new MLPNodeStruct(0);

  MLNode* _nodestr= nodestr->clone();
  if( _nodestr == 0 )
    return 0;

  MLPNodeStruct* p= new MLPNodeStruct(0);
  p->set(_nodestr);
  return p;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLPNodeStruct::toString() const
{
  MLString s= (nodestr==0) ? "" : nodestr->toString();
  return MLString("NodeStruct(") + s + MLString(")");
}

/////////////////////////////////////////////////////////////////////////////

