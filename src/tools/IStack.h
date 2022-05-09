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
  IStack.h

  Stack based on IList Class
  Stephane Rehel, SCFR::Kepler

  January 22nd, 1995
*/

#ifndef __IStack_h
#define __IStack_h

#ifndef __IList_h
#include "IList.h"
#endif

/////////////////////////////////////////////////////////////////////////////

template<class T> class IStack: private IList<T>
{
public:
  IStack()
    {}
  virtual ~IStack()
    {}

  inline boolean isEmpty() const
    {
    return IList<T>::getNElements() == 0;
    }
  inline int getNElements() const
    {
    return IList<T>::getNElements();
    }

  inline void push( T* t )
    {
    IList<T>::cons(t);
    }
  inline T* pop()
    {
    register T* t= IList<T>::getFirst();
    IList<T>::remove( IList<T>::getFirstElement() );
    return t;
    }

  inline T* peek() const
    {
    return isEmpty() ? 0 : IList<T>::getFirst();
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __IStack_h
