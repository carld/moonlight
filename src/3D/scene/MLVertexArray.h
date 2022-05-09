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
   MLVertexArray.h

   November 11 1996
   Stephane Rehel
*/

#ifndef __MLVertexArray_h
#define __MLVertexArray_h

#ifndef __MLVertex_h
#include "tools/3D/MLVertex.h"
#endif

#include "tools/MLDArray.h"

/////////////////////////////////////////////////////////////////////////////

class MLVertexArray: public MLDArray<MLVertex>
{
public:
  MLVertexArray() {}
  MLVertexArray( int _n ): MLDArray<MLVertex>(_n) {}
  MLVertexArray( const MLDArray<MLVertex>& a ): MLDArray<MLVertex>(a) {}
  MLDArray<MLVertex>& getArray() { return *this; }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLVertexArray_h
