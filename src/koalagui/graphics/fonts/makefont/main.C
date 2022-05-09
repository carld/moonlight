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

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glaux.h"
#include "makefont.h"
#include "struct.h"

GLuint base;
GLXFontStruct* glxfonts= 0;
int count= 0;
int first_char= 0;
int last_char= 0;

//        "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1");

void makeRasterFont( const char* font_name )
{
    XFontStruct *fontInfo;
    Font id;
    unsigned int first, last;
    Display *xdisplay;

    xdisplay = auxXDisplay ();
    fontInfo = XLoadQueryFont(xdisplay,font_name);
    if (fontInfo == NULL) {
        fprintf( stderr, "no font '%s' found\n",font_name);
        exit (1);
    }

    id = fontInfo->fid;
    first = fontInfo->min_char_or_byte2;
    last = fontInfo->max_char_or_byte2;

    base = glGenLists((GLuint) last+1);
    if (base == 0) {
        fprintf( stderr, "out of display lists\n");
        exit (1);
    }

    count= last-first+1;
    first_char= first;
    last_char= last;
    glxfonts= new GLXFontStruct [ count ];
    glXDumpXFont(id, first, count, base+first, glxfonts );

/*
    *height = fontInfo->ascent + fontInfo->descent;
    *width = fontInfo->max_bounds.width;
*/
}




int main( int argc, char* argv[] )
{
  if( argc != 2 )
    {
    fprintf( stderr, "command line error\n" );
    return 1;
    }

  char* font_name= argv[1];

  auxInitDisplayMode( AUX_SINGLE | AUX_RGB );
  auxInitPosition( 0, 0, 5, 5 );
  auxInitWindow( argv[0] );

  makeRasterFont( font_name );

  printf( "static const char* name= \"%s\";\n", font_name );
  printf( "static int font_ascent= %d;\n", font_ascent );
  printf( "static int font_descent= %d;\n", font_descent );
  printf( "static int count= %d;\n", count );
  printf( "static int first= %d;\n", first_char );
  printf( "static int last= %d;\n", last_char );
  printf( "static MLFont::MLFontData data[%d] = {\n", count );

  int pos= 0;
  for( int k= 0; k < count; ++k )
    {
    GLXFontStruct& font= glxfonts[k];
    printf( " { %d, %d, %g, %g, %g, %g, %d, %d }%s\n",
            font.width, font.height, font.x0, font.y0+1, font.xi, font.yi,
            font.length,
            (font.length==0) ? 0 : pos,
            ((k+1)==count)?"":"," );
    pos+= font.length;
    }
  printf( "};\n" );

  printf( "static unsigned char bitmap_data[%d] = {\n", pos+1 );
  for( int k= 0; k < count; ++k )
    {
    GLXFontStruct& font= glxfonts[k];
    for( int i= 0; i < font.length; ++i )
      printf( " 0x%02x,", font.data[i] );
    if( font.length > 0 )
      printf("\n");
    }
  printf( " 0 };\n" );

  return 0;
}

