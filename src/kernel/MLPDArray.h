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
  MLPDArray.h

  Stephane Rehel
  April 20 1998
*/

#ifndef __MLPDArray_h
#define __MLPDArray_h

#include "MLProperty.h"

#include "tools/MLDArray.h"

/////////////////////////////////////////////////////////////////////////////

template<class T>
class MLPDArray: public MLProperty
{
protected:
  MLDArray<T> array;

public:
  MLPDArray( MLNode* _node, const char* _name ):
    MLProperty(_node,_name),
    array(16)
    {}

  MLPDArray( MLNode* _node ):
    MLProperty(_node),
    array(16)
    {}

  virtual ~MLPDArray()
    {}

  boolean isArray() const
    {
    return true;
    }
  int getArrayLength() const
    {
    return length();
    }

  MLDArray<T>& getArray()
    {
    return array;
    }

  boolean clear()
    {
    return realloc(0);
    }

  // return true if changed
  boolean realloc( int new_length )
    {
    int prev_length= array.length();
    if( prev_length == new_length )
      return false;
    array.realloc(new_length);
    return true;
    }

  int length() const
    {
    return array.length();
    }

  // 1 <= i <= n
  const T& get( int i ) const
    {
    return array.get(i);
    }

  // !! DONT FORGET TO TOUCH THE PROPERTY IF MODIFIED !!
  // 1 <= i <= n
  T& operator [] ( int i )
    {
    return array[i];
    }

  void fill( int beginIndex, int endIndex, const T& t )
    {
    array.fill(beginIndex,endIndex,t);
    touch();
    }

  void fill( const T& t )
    {
    fill( 1, length(), t );
    }

  void append( const T& t )
    {
    array.append(t);
    touch();
    }

  T& append()
    {
    touch();
    return array.append();
    }

  // 1 <= i <= n
  void remove( int i )
    {
    array.remove(i);
    touch();
    }

  MLPDArray<T>& operator = ( const MLDArray<T>& a )
    {
    array= a;
    touch();
    return *this;
    }

  void set( const T* t, int _length )
    {
    array.set(t,_length);
    touch();
    }

  virtual boolean copy( MLProperty* p ) const;

  virtual MLString toString() const;

protected:
  virtual MLString elementToString( const T& ) const = 0;
};

/////////////////////////////////////////////////////////////////////////////

template<class T>
boolean MLPDArray<T>::copy( MLProperty* p ) const
{
  if( p->getType() != getType() )
    return false;

  ((MLPDArray<T>*)p)->array= array;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
MLString MLPDArray<T>::toString() const
{
  MLString s= "[ ";

  for( int i= 1; i <= length(); ++i )
    {
    s+= elementToString(array.get(i));
    if( i < length() )
      s+= MLString(", ");
    }

  return s + MLString(" ]");
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPDArray_h
