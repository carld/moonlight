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
  MLWFileBlock.C

  Stephane Rehel
  April 6 1997

  Write Only File Block
*/

#include <assert.h>
#include <errno.h>

#include "MLWFileBlock.h"
#include "MLWFile.h"

/////////////////////////////////////////////////////////////////////////////

MLWFileBlock::MLWFileBlock()
{
  file= 0;
  current_name= 0;
  name_position= -1;
}

/////////////////////////////////////////////////////////////////////////////

MLWFileBlock::~MLWFileBlock()
{
  if( name_is_open() )
    end();

  close();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLWFileBlock::open( MLWFile* _file )
{
  assert( file == 0 );
  assert( _file != 0 );

  file= _file;

  current_name= 0;
  name_position= -1;

  MLFileBlock::position= file->getPosition();

  *file << long(0); // items table position

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLWFileBlock::begin( int name )
{
  assert( findItem(name) == 0 );
  assert( ! name_is_open() );

  current_name= name;
  name_position= file->getPosition();
}

/////////////////////////////////////////////////////////////////////////////

void MLWFileBlock::end()
{
  if( ! name_is_open() )
    return;

  long size= file->getPosition() - name_position;

  Item* item= new Item;
  item->name= current_name;
  item->size= size;
  item->position= name_position;

  items.append(item);

  current_name= 0;
  name_position= -1;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLWFileBlock::close()
{
  if( file == 0 )
    return true; // already closed

  errno= 0;

  end();

  long table_position= file->getPosition();

//  file->setPosition(MLFileBlock::position);
  file->setPosition(position);
  *file << table_position;

  file->setPosition(table_position);

  *file << int(items.getNElements());
  SIListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* item= li++;

    *file << item->name;
    *file << item->position;
    *file << item->size;
    }

  file= 0;

  return errno == 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLWFileBlock::write( const void* data, int length )
{
  file->write(data,length);
}

/////////////////////////////////////////////////////////////////////////////
