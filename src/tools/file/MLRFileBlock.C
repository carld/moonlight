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
  MLRFileBlock.C

  Stephane Rehel
  April 6 1997

  Read Only File Block
*/

#include "MLRFileBlock.h"

#include "MLRFile.h"


/////////////////////////////////////////////////////////////////////////////

MLRFileBlock::MLRFileBlock()
{
  file= 0;
  end_position= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLRFileBlock::~MLRFileBlock()
{
  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRFileBlock::open( MLRFile* _file )
{
  if( _file == 0 )
    return false;

  while( ! items.empty() )
    {
    delete items.getLast();
    items.removeLast();
    }

  file= _file;
  MLFileBlock::position= file->getPosition();

  long table_position;
  *file >> table_position;

  long pos= file->getPosition();

  file->setPosition(table_position);

  int nItems;
  *file >> nItems;

  for( int i= 1; i <= nItems; ++i )
    {
    Item* item= new Item;

    *file >> item->name;
    *file >> item->position;
    *file >> item->size;

    items.append(item);
    }

  end_position= file->getPosition();

  file->setPosition(pos);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLRFileBlock::close()
{
  if( file == 0 )
    return; // not open!

  file->setPosition(end_position);

  end_position= 0;
  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLRFileBlock::go( int name )
{
  if( file == 0 )
    return false;

  Item* item= MLFileBlock::findItem(name);

  if( item == 0 )
    return false;

  file->setPosition(item->position);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLRFileBlock::read( void* data, int length )
{
  file->read(data,length);
}

/////////////////////////////////////////////////////////////////////////////

