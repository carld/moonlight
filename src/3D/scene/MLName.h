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
   MLName.h

   Stephane Rehel
   April 14 1997
*/

#ifndef __MLName_h
#define __MLName_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLWFileBlock;
class MLRFileBlock;

/////////////////////////////////////////////////////////////////////////////

class MLName
{
public:

  static const int default_name_index;

  MLString filename;
  MLString name;
  int name_index;
  int major_version,
      minor_version;

public:
  MLName();

  virtual ~MLName()
    {}

  MLName& operator = ( const MLName& n );

  boolean operator == ( const MLName& n ) const;

  boolean operator != ( const MLName& n ) const
    {
    return ! operator == (n);
    }

  void write( MLWFileBlock& block ) const;
  void read( MLRFileBlock& block );
};

/////////////////////////////////////////////////////////////////////////////

inline
MLWFileBlock& operator << ( MLWFileBlock& block, const MLName& name )
{
  name.write(block);
  return block;
}

/////////////////////////////////////////////////////////////////////////////

inline
MLRFileBlock& operator >> ( MLRFileBlock& block, MLName& name )
{
  name.read(block);
  return block;
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLName_h

