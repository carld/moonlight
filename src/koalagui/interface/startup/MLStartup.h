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
  MLStartup.h

  Stephane Rehel
  March 25 1997
*/

#ifndef __MLStartup_h
#define __MLStartup_h

#ifndef __MLEventGrabber_h
#include "graphics/MLEventGrabber.h"
#endif

class MLPicture;
class Cosmic10Font;
class Interface;

/////////////////////////////////////////////////////////////////////////////

class MLStartup: public MLEventGrabber
{
private:
  static int logo1_width, logo1_height;
  static int logo2_width, logo2_height;
  static unsigned char logo1_data[];
  static unsigned char logo2_data[];

  static const char* license_text[];
  int license_length;

protected:
  MLPicture* logo1;
  MLPicture* logo2;
  boolean end;

  MLPicture* scrolling_text;
  int scrolling_index;
  int scrolling_margin;
  int scrolling_left_margin;

  Cosmic10Font* font;

  Interface* interface;

public:
  MLStartup( Interface* _interface );

  virtual ~MLStartup();

  void run();

  virtual boolean eventGrabber( MLWindow*, MLEvent& );

protected:
  void scroll_next();

  void drawLogo();

  MLPicture* getLogo( int logo_width, int logo_height,
                      unsigned char logo_data[] );

  void buildLogos();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLStartup_h
