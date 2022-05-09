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
  MLLibZ.h

  Stephane Rehel
  March 25 1997
*/

#ifndef __MLLibZ_h
#define __MLLibZ_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLLibZ
{
public:
  static boolean deflate( void* in_uncompr, int in_uncomprLen,
                        void* out_compr,  int out_comprLen,
                        int* compressedSize,
                        int level );

  static boolean inflate( void* in_compr,    int in_comprLen,
                        void* out_uncompr, int out_uncomprLen );
};

/////////////////////////////////////////////////////////////////////////////

class MLZDeflated
{
protected:
  void* p;
  int compressedLength, uncompressedLength;

public:
  MLZDeflated( void* _p );

  int getCompressedLength() const
    {
    return compressedLength;
    }

  int getUncompressedLength() const
    {
    return uncompressedLength;
    }

  boolean inflate( void* dest );

  void* inflate();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLLibZ_h
