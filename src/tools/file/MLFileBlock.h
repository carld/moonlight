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
  MLFileBlock.h

  Stephane Rehel
  April 6 1997

  Read/Write File Block
*/

#ifndef __MLFileBlock_h
#define __MLFileBlock_h

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLFileBlock
{
protected:
  struct Item
    {
    long position; // relative
    long size;
    int name;
    };

  int position; // global position in file

  SIList<Item> items;

public:
  MLFileBlock();
  virtual ~MLFileBlock();

  int getNItems() const
    {
    return items.getNElements();
    }

protected:
  Item* findItem( int name );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFileBlock_h
