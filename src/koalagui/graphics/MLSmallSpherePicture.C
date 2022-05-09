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
  MLSmallspherePicture.C

  Stephane Rehel

  July 5 1997
*/

#include "tools/Color.h"
#include "tools/HSVColor.h"
#include "tools/MLGammaCorrection.h"

#include "MLSmallSpherePicture.h"

// for grays & gamma correction
#include "GraphicsSystem.h"

#include "sphere_data.h"

/////////////////////////////////////////////////////////////////////////////

// static
const double MLSmallSpherePicture::green  = 0.;
const double MLSmallSpherePicture::yellow =-50.;
const double MLSmallSpherePicture::orange =-100.;
const double MLSmallSpherePicture::red    =-120.;
const double MLSmallSpherePicture::magenta=-180.;
const double MLSmallSpherePicture::cyan   = 50.;
const double MLSmallSpherePicture::blue   = 120.;

/////////////////////////////////////////////////////////////////////////////

MLSmallSpherePicture::MLSmallSpherePicture( double hue /* = 0. */ )
{
  set(hue);
}

/////////////////////////////////////////////////////////////////////////////

MLSmallSpherePicture::MLSmallSpherePicture( const Color& color )
{
  set(color);
}

/////////////////////////////////////////////////////////////////////////////

MLSmallSpherePicture::~MLSmallSpherePicture()
{}

/////////////////////////////////////////////////////////////////////////////

void MLSmallSpherePicture::set( double hue /* = 0. */ )
{
  MLPicture::destroy();

  MLPicture::createRGBA( ::width, ::height );

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();
  MLGammaCorrection* gc= graphicsSystem->getGammaCorrection();

  for( int j= 0; j < ::height; ++j )
    {
    unsigned char* data= MLPicture::getPixelRGBA(0,j);
    const double* hsv_line= ::data + j * ::width * 3;
    for( int i= 0; i < ::width; ++i )
      {
      if( hsv_line[0] != 0. )
        {
        Color color= Color( HSVColor( hsv_line[0] + hue,
                                      hsv_line[1],
                                      hsv_line[2] ) );

        data[0]= gc->do_ub( int(color.r() * 255. + .5) );
        data[1]= gc->do_ub( int(color.g() * 255. + .5) );
        data[2]= gc->do_ub( int(color.b() * 255. + .5) );
        data[3]= 255;
        }
       else
        {
        data[0]= data[1]= data[2]= 0;
        data[3]= 0;
        }

      data+= 4;
      hsv_line+= 3;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLSmallSpherePicture::set( const Color& color )
{
  MLPicture::destroy();

  MLPicture::createRGBA( ::width, ::height );

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  MLGammaCorrection* gc= graphicsSystem->getGammaCorrection();

  for( int j= 0; j < ::height; ++j )
    {
    unsigned char* data= MLPicture::getPixelRGBA(0,j);

    const double* hsv_line= ::data + j * ::width * 3;
    for( int i= 0; i < ::width; ++i )
      {
      if( hsv_line[0] != 0. )
        {
        Color c= Color( HSVColor( hsv_line[0],
                                  0, //hsv_line[1],
                                  hsv_line[2] ) );
        c &= color;
        data[0]= (c.r()<=0.) ? 0 : (c.r()>=255.) ? 255 : int(c.r()* 255.+.5);
        data[1]= (c.g()<=0.) ? 0 : (c.g()>=255.) ? 255 : int(c.g()* 255.+.5);
        data[2]= (c.b()<=0.) ? 0 : (c.b()>=255.) ? 255 : int(c.b()* 255.+.5);
        data[0]= gc->do_ub( data[0] );
        data[1]= gc->do_ub( data[1] );
        data[2]= gc->do_ub( data[2] );
        data[3]= 255;
        }
       else
        {
        data[0]= data[1]= data[2]= 0;
        data[3]= 0;
        }

      data+= 4;
      hsv_line+= 3;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

