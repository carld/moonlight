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
  getSnapshotOfScreen.C

  MLPicture* MLModule::getSnapshotOfScreen();

  Stephane Rehel
  April 22 1997
*/

#include "graphics/mlgl.h"
#include "graphics/MLPicture.h"
#include "graphics/SystemWindow.h"

#include "MLModule.h"

/////////////////////////////////////////////////////////////////////////////

// return a 256x192 picture of the screen
MLPicture* MLModule::getSnapshotOfScreen()
{
  pushWaitCursor();

  IVector screenSize= root->getSize();
  int width= screenSize.x()/4;
  int height= screenSize.y()/4;

  MLPicture* picture= new MLPicture;
  picture->createRGB(width,height);

  GLint swapbytes, lsbfirst, rowlength;
  GLint skiprows, skippixels, alignment;

  root->current();

  // Save the current packing mode for bitmaps.
  glGetIntegerv( GL_UNPACK_SWAP_BYTES, &swapbytes );
  glGetIntegerv( GL_UNPACK_LSB_FIRST, &lsbfirst );
  glGetIntegerv( GL_UNPACK_ROW_LENGTH, &rowlength );
  glGetIntegerv( GL_UNPACK_SKIP_ROWS, &skiprows );
  glGetIntegerv( GL_UNPACK_SKIP_PIXELS, &skippixels );
  glGetIntegerv( GL_UNPACK_ALIGNMENT, &alignment );

  // Enforce a standard packing mode
  glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );
  glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );

  glPushAttrib(GL_ENABLE_BIT);
  glPushAttrib(GL_PIXEL_MODE_BIT);

  glPixelZoom(1.,1.);
  glPixelTransferf( GL_RED_SCALE,   1. );
  glPixelTransferf( GL_GREEN_SCALE, 1. );
  glPixelTransferf( GL_BLUE_SCALE,  1. );
  glPixelTransferf( GL_RED_BIAS,    0. );
  glPixelTransferf( GL_GREEN_BIAS,  0. );
  glPixelTransferf( GL_BLUE_BIAS,   0. );

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_DITHER);

  glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
  glReadBuffer(GL_FRONT);

  unsigned char* four_lines= new unsigned char [ 4 * screenSize.x() * 4 ];
  int i, j;
  for( j= 0; j < height; ++j )
    {
    glReadPixels( 0, j*4, screenSize.x(), 4,
                  GL_RGBA, GL_UNSIGNED_BYTE, (void*) four_lines );

    register unsigned char* l1= four_lines + screenSize.x()*0*4;
    register unsigned char* l2= four_lines + screenSize.x()*1*4;
    register unsigned char* l3= four_lines + screenSize.x()*2*4;
    register unsigned char* l4= four_lines + screenSize.x()*3*4;
    register unsigned char* picture_line= picture->getPixelRGB(0,j);

    for( i= width; i; --i )
      {
      register
      int r= int(l1[0+0*4])+int(l1[0+1*4])+int(l1[0+2*4])+int(l1[0+3*4])
            +int(l2[0+0*4])+int(l2[0+1*4])+int(l2[0+2*4])+int(l2[0+3*4])
            +int(l3[0+0*4])+int(l3[0+1*4])+int(l3[0+2*4])+int(l3[0+3*4])
            +int(l4[0+0*4])+int(l4[0+1*4])+int(l4[0+2*4])+int(l4[0+3*4]);
      register
      int g= int(l1[1+0*4])+int(l1[1+1*4])+int(l1[1+2*4])+int(l1[1+3*4])
            +int(l2[1+0*4])+int(l2[1+1*4])+int(l2[1+2*4])+int(l2[1+3*4])
            +int(l3[1+0*4])+int(l3[1+1*4])+int(l3[1+2*4])+int(l3[1+3*4])
            +int(l4[1+0*4])+int(l4[1+1*4])+int(l4[1+2*4])+int(l4[1+3*4]);
      register
      int b= int(l1[2+0*4])+int(l1[2+1*4])+int(l1[2+2*4])+int(l1[2+3*4])
            +int(l2[2+0*4])+int(l2[2+1*4])+int(l2[2+2*4])+int(l2[2+3*4])
            +int(l3[2+0*4])+int(l3[2+1*4])+int(l3[2+2*4])+int(l3[2+3*4])
            +int(l4[2+0*4])+int(l4[2+1*4])+int(l4[2+2*4])+int(l4[2+3*4]);
      picture_line[0]= r / 16;
      picture_line[1]= g / 16;
      picture_line[2]= b / 16;

      picture_line += 3;
      l1+= 4 * 4;
      l2+= 4 * 4;
      l3+= 4 * 4;
      l4+= 4 * 4;
      }
    }
  delete four_lines;
  four_lines= 0;

  glPopAttrib();
  glPopAttrib();

  // Restore saved packing modes.
  glPixelStorei( GL_UNPACK_SWAP_BYTES, swapbytes );
  glPixelStorei( GL_UNPACK_LSB_FIRST, lsbfirst );
  glPixelStorei( GL_UNPACK_ROW_LENGTH, rowlength );
  glPixelStorei( GL_UNPACK_SKIP_ROWS, skiprows );
  glPixelStorei( GL_UNPACK_SKIP_PIXELS, skippixels );
  glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

  const int req_width= 256;
  const int req_height= 192;

  if( !(width == req_width && height == req_height) )
    {
    MLPicture* picture2= new MLPicture;
    picture2->createRGB(req_width,req_height);

    for( j= 0; j < req_height; ++j )
      {
      int j0= j * height / req_height;
      unsigned char* source_line= picture->getPixelRGB(0,j0);
      register unsigned char* target= picture2->getPixelRGB(0,j);
      for( i= 0; i < req_width; ++i, target+= 3 )
        {
        register int i0= i * width / req_width;
        register unsigned char* source= source_line + i0 * 3;
        target[0]= source[0];
        target[1]= source[1];
        target[2]= source[2];
        }
      }

    delete picture;

    picture= picture2;
    }

  popWaitCursor();

  return picture;
}

/////////////////////////////////////////////////////////////////////////////
