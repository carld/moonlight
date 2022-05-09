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
  MLWFileBlock.h

  Stephane Rehel
  April 6 1997

  Write Only File Block
*/

#ifndef __MLWFileBlock_h
#define __MLWFileBlock_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __MLFileBlock_h
#include "MLFileBlock.h"
#endif

#ifndef __MLWFile_h
#include "MLWFile.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLWFileBlock: public MLFileBlock
{
protected:
  MLWFile* file;

  int current_name;
  long name_position;

public:
  MLWFileBlock();
  virtual ~MLWFileBlock();

  MLWFile* getFile() const
    {
    return file;
    }

  boolean open( MLWFile* _file );

  void begin( int name );
  void end();

  // return false if write error (e.g. disk-full)
  boolean close();

private:
  boolean name_is_open() const
    {
    return name_position != -1;
    }

public:
  void write( const void* data, int length );

  MLWFileBlock& operator << ( char c )          { file->putChar(c); return *this; }
  MLWFileBlock& operator << ( unsigned char c ) { file->putUChar(c); return *this; }
  MLWFileBlock& operator << ( int i )           { file->putInteger(i); return *this; }
  MLWFileBlock& operator << ( unsigned int i )  { file->putUInteger(i); return *this; }
  MLWFileBlock& operator << ( short i )         { file->putShort(i); return *this; }
  MLWFileBlock& operator << ( unsigned short i ){ file->putUShort(i); return *this; }
  MLWFileBlock& operator << ( long l )          { file->putLong(l); return *this; }
  MLWFileBlock& operator << ( unsigned long l ) { file->putULong(l); return *this; }
  MLWFileBlock& operator << ( float f )         { file->putFloat(f); return *this; }
  MLWFileBlock& operator << ( double d )        { file->putDouble(d); return *this; }
  MLWFileBlock& operator << ( const MLString& s ){ file->putString(s); return *this; }

  MLWFileBlock& operator << ( const Point& p )  { file->putPoint(p); return *this; }
  MLWFileBlock& operator << ( const Point2& p ) { file->putPoint2(p); return *this; }
  MLWFileBlock& operator << ( const Vector& v ) { file->putVector(v); return *this; }
  MLWFileBlock& operator << ( const Vector2& v ){ file->putVector2(v); return *this; }
  MLWFileBlock& operator << ( const Box& b )    { file->putBox(b); return *this; }
  MLWFileBlock& operator << ( const Matrix4x4& m){ file->putMatrix4x4(m); return *this; }
  MLWFileBlock& operator << ( const Transform& t){ file->putTransform(t); return *this; }
  MLWFileBlock& operator << ( const Color& c)   { file->putColor(c); return *this; }
  MLWFileBlock& operator << ( const Spectrum& s){ file->putSpectrum(s); return *this; }
};

/////////////////////////////////////////////////////////////////////////////

template<class ARRAY>
inline
void writeArray( MLWFileBlock& block, ARRAY& array, int index1, int index2 )
{
  for( register int i= index1; i <= index2; ++i )
    block << array [ i ];
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLWFileBlock_h
