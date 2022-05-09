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
  MLRFileBlock.h

  Stephane Rehel
  April 6 1997

  Read Only File Block
*/

#ifndef __MLRFileBlock_h
#define __MLRFileBlock_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLFileBlock_h
#include "MLFileBlock.h"
#endif

#ifndef __MLRFile_h
#include "MLRFile.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLRFileBlock: public MLFileBlock
{
protected:
  MLRFile* file;
  long end_position;

public:
  MLRFileBlock();
  virtual ~MLRFileBlock();

  MLRFile* getFile() const
    {
    return file;
    }

  boolean open( MLRFile* _file );
  void close();

  boolean find( int name )
    {
    return MLFileBlock::findItem(name) != 0;
    }

  boolean go( int name );

public:
  void read( void* data, int length );

  MLRFileBlock& operator >> ( char& c )         { file->getChar(&c); return *this; }
  MLRFileBlock& operator >> ( unsigned char& c ){ file->getUChar(&c); return *this; }
  MLRFileBlock& operator >> ( int& i )          { file->getInteger(&i); return *this; }
  MLRFileBlock& operator >> ( unsigned int& i ) { file->getUInteger(&i); return *this; }
  MLRFileBlock& operator >> ( short& i )        { file->getShort(&i); return *this; }
  MLRFileBlock& operator >> ( unsigned short& i){ file->getUShort(&i); return *this; }
  MLRFileBlock& operator >> ( long& l )         { file->getLong(&l); return *this; }
  MLRFileBlock& operator >> ( unsigned long& l ){ file->getULong(&l); return *this; }
  MLRFileBlock& operator >> ( float& f )        { file->getFloat(&f); return *this; }
  MLRFileBlock& operator >> ( double& d )       { file->getDouble(&d); return *this; }
  MLRFileBlock& operator >> ( MLString& s )      { file->getString(&s); return *this; }

  MLRFileBlock& operator >> ( Point& p )        { file->getPoint(&p); return *this; }
  MLRFileBlock& operator >> ( Point2& p )       { file->getPoint2(&p); return *this; }
  MLRFileBlock& operator >> ( Vector& v )       { file->getVector(&v); return *this; }
  MLRFileBlock& operator >> ( Vector2& v )      { file->getVector2(&v); return *this; }
  MLRFileBlock& operator >> ( Box& b )          { file->getBox(&b); return *this; }
  MLRFileBlock& operator >> ( Matrix4x4& m )    { file->getMatrix4x4(&m); return *this; }
  MLRFileBlock& operator >> ( Transform& t )    { file->getTransform(&t); return *this; }
  MLRFileBlock& operator >> ( Color& c )        { file->getColor(&c); return *this; }
  MLRFileBlock& operator >> ( Spectrum& s )     { file->getSpectrum(&s); return *this; }

  double getDouble()
    {
    double d;
    file->getDouble(&d);
    return d;
    }
  int getInteger()
    {
    int i;
    file->getInteger(&i);
    return i;
    }
};

/////////////////////////////////////////////////////////////////////////////

template<class ARRAY>
inline
void readArray( MLRFileBlock& block, ARRAY& array, int index1, int index2 )
{
  for( register int i= index1; i <= index2; ++i )
    block >> array [ i ];
}

/////////////////////////////////////////////////////////////////////////////

template<class TYPE>
inline
void skipArray( MLRFileBlock& block, TYPE, int index1, int index2 )
{
  TYPE t;
  for( register int i= index1; i <= index2; ++i )
    block >> t;
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLWFileBlock_h
