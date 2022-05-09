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
  XDRFile.h

  Stephane Rehel
  Creation: SR, March 1st, 1996
  Revisions:
   SR August 4 1996
*/

#ifndef __XDRFile_h
#define __XDRFile_h

#include <stdio.h>

#ifndef __XDROp_h
#include "XDROp.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class XDRFile: public XDROp
{
protected:
  MLString filename;
  FILE* file;
  long headerSize;

public:
  XDRFile();
  virtual ~XDRFile();

  boolean open_for_reading( const MLString& _filename );
  boolean open_for_writing( const MLString& _filename );

  boolean getError() const
    {
    return error;
    }

  void close();

public:
  const MLString& getFilename() const
    {
    return filename;
    }

  void setHeaderSize( long hs );
  long getSize();
  long getPosition() const;
  void setPosition( long newPosition );

  void rawWrite( const void* data, int length );
  void rawRead (       void* data, int length );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __XDRFile_h

