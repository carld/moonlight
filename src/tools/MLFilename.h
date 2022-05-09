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
  MLFilename.h

  Stephane Rehel
  April 12 1997
*/

#ifndef __Filename_h
#define __Filename_h

#include "boolean.h"

#ifndef __MLString_h
#include "MLString.h"
#endif

#ifndef __MLPath_h
#include "MLPath.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLFilename
{
protected:
  MLPath path; // e.g. "/usr/local/lib/"
  MLString filename; // e.g. "libjpeg"
  MLString extension; // e.g. ".a"

public:
  MLFilename();

  MLFilename( const MLString& filename );

  MLFilename( const MLFilename& fn );

  MLFilename& operator = ( const MLFilename& fn );

  virtual ~MLFilename()
    {}

  const MLPath& getPath() const
    {
    return path;
    }

  const MLString& getFilename() const
    {
    return filename;
    }

  const MLString& getExtension() const
    {
    return extension;
    }

  MLString get() const
    {
    return path.get() + filename + extension;
    }

  boolean isAbsolute() const;

  static void split( const MLString& fullname,
                     MLPath& path, MLString& filename );

  // if error, return FALSE
  boolean operator == ( const MLFilename& _file2 ) const;

  static MLString expand( const MLString& _filename,
                          boolean expand_current = true );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFilename_h
