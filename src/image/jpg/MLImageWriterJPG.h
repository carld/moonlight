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
  MLImageWriterJPG.h

  Stephane Rehel

  August 19 1997
*/

#ifndef __MLImageWriterJPG_h
#define __MLImageWriterJPG_h

#ifndef __MLImageWriter_h
#include "image/MLImageWriter.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLImageWriterJPG: public MLImageWriter
{
protected:
  int quality;
  boolean progressive;

public:
  MLImageWriterJPG();

  virtual ~MLImageWriterJPG();

  void setQuality( int _quality );
  void setProgressive( boolean _progressive );

public:
  virtual bool write( MLImage* image );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLImageWriterJPG_h
