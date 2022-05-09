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
  InitMLFont.C

  Stephane Rehel
  June 27, 1996
*/

// This file is a template file

// You must define FONTNAME and include it from another .C

#ifndef FONTNAME
#error Define FONTNAME please
#endif

#ifndef FUNCTION_NAME
#error Define FUNCTION_NAME please
#endif

#include "../MLFont.h"

/////////////////////////////////////////////////////////////////////////////

#define functionName(name) init_##name
#define stringName(name) #name

MLFont* FUNCTION_NAME ()
{
  MLFont* font= new MLFont;

  font->ascent= font_ascent;
  font->descent= font_descent;
  font->height= font_ascent + font_descent;
  font->first= first;
  font->last= last;
  font->count= count;
  font->name= MLString(stringName(FONTNAME));
  font->real_name= name;
  font->data= data;
  font->bitmap_data= bitmap_data;

  font->build();

  return font;
}

/////////////////////////////////////////////////////////////////////////////

