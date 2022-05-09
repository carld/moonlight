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
  MLPath.h

  Stephane Rehel
  April 15 1997
*/

#ifndef __MLPath_h
#define __MLPath_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __MLString_h
#include "MLString.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLPath
{
protected:
  MLString path; // always finish by a "/"
                // current = "./"

public:
  static const char separator;

public:
  MLPath(); // initialed by "./"

  MLPath( const MLPath& p );

  MLPath( const MLString& filename );

  virtual ~MLPath()
    {}

  const MLString& get() const
    {
    return path;
    }

  MLPath& operator = ( const MLString& fullname );

  boolean isAbsolute() const;
  boolean isCurrent() const;

  // if error, return FALSE
  boolean operator == ( const MLPath& path2 ) const;

  MLPath& operator += ( const MLString& p );

  static MLString checkPathSeparators( const MLString& filename );
  static MLString toUnixPathSeparators( const MLString& filename );

  static MLPath getHomeDirectory();
  static MLPath getCurrentDirectory();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLPath_h
