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
  MLImageReaderJPG.h

  Stephane Rehel

  August 19 1997
*/

#ifndef __MLImageReaderJPG_h
#define __MLImageReaderJPG_h

#ifndef __MLImageReader_h
#include "image/MLImageReader.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLImageReaderJPG: public MLImageReader
{
//protected:
//  unsigned char* colormap;
//  unsigned char background_color;

public:
  MLImageReaderJPG();

  virtual ~MLImageReaderJPG();

public:
  virtual boolean read( MLImage* image );

protected:
//  unsigned int TGADecodeImage( MLImage* image );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLImageReaderJPG_h
