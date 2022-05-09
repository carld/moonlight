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
  MLReference.h

  Stephane Rehel
  January 12 1997
*/

#ifndef __MLReference_h
#define __MLReference_h

/////////////////////////////////////////////////////////////////////////////

class MLReference
{
private:
  int ref_count;

public:
  MLReference()
    {
    ref_count= 0;
    }

  virtual ~MLReference()
    {}

  friend void ref( MLReference* r );
  friend void unref( MLReference*& r );
};

/////////////////////////////////////////////////////////////////////////////

inline void ref( MLReference* r )
{
  if( r == 0 )
    return;

  ++(r->ref_count);
}

template<class REF>
inline void ref( REF* r )
{
  ref( ((MLReference*)r) );
}

/////////////////////////////////////////////////////////////////////////////

inline void unref( MLReference*& r )
{
  if( r == 0 )
    return;

  if( --(r->ref_count) <= 0 )
    {
    delete r;
    r= 0;
    }
}

template<class REF>
inline void unref( REF*& r )
{
  unref( ((MLReference*&)r) );
}

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLReference_h
