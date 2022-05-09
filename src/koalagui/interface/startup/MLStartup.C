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
  MLStartup.C

  Stephane Rehel
  March 25 1997
*/

#include <string.h>

#include "tools/zlib/MLLibZ.h"
#include "tools/Chrono.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"
#include "graphics/MLWindow.h"
#include "graphics/SystemWindow.h"

#include "graphics/MLPicture.h"

#include "../Interface.h"
#include "../MLInterfaceVersion.h"

#include "MLStartup.h"
#include "Cosmic10Font.h"

/////////////////////////////////////////////////////////////////////////////

static int main_logo_x1= 0, main_logo_y1= 0;
static int main_logo_x2= 0, main_logo_y2= 0;

/////////////////////////////////////////////////////////////////////////////

MLStartup::MLStartup( Interface* _interface )
{
  interface= _interface;

  logo1= 0;
  logo2= 0;
  scrolling_text= 0;

  font= new Cosmic10Font;

  license_length= 0;
  while( license_text[license_length] != 0 )
    ++license_length;

  scrolling_index= 0;
  scrolling_margin= 20;
  scrolling_left_margin= 10;
}

/////////////////////////////////////////////////////////////////////////////

MLStartup::~MLStartup()
{
  delete logo1;
  logo1= 0;

  delete logo2;
  logo2= 0;

  delete scrolling_text;
  scrolling_text= 0;

  delete font;
  font= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLStartup::run()
{
  buildLogos();

  if( logo1 == 0 && logo2 == 0 )
    return; //???

  end= false;

  drawLogo();

  GraphicsSystem* graphicsSystem= interface->getGraphicsSystem();
  SystemWindow* root= graphicsSystem->getRootWindow();

  int text_width= font->getCharWidth('o') * 76 + scrolling_left_margin;
  int text_height= 140;
  IPoint pos= IPoint( (root->getSize().x()-text_width)/2 + 50,
                      main_logo_y1 - 220 );
  pos= root->reverseY(pos);

  SystemWindow* textw= root->newSystemChild( pos.x(), pos.y(),
                                             text_width, text_height );
  textw->map();

  scrolling_text= new MLPicture;
  scrolling_text->createRGB( text_width, text_height + scrolling_margin );
  scrolling_text->fill( 0,0,text_width-1,text_height-1, 0,0,0 );

  scrolling_index= 0;

  MLEventManager* eventManager= MLEventManager::getEventManager();

  MLEventGrabber* prev_grabber= eventManager->setEventGrabber(this);

  Chrono chrono;
  double time_for_text= 60*6.;
  double time_for_line= time_for_text
                        / double(license_length * font->getHeight());

  for(;;)
    {
    eventManager->get();
    eventManager->dispatch();
    if( end )
      break;

    if( ! interface->isIconic() )
      {
      chrono.start();
      textw->currentPixel();
      mlBack();
      scrolling_text->draw( 0, 0,
                            0, scrolling_margin,
                            text_width-1, text_height-1 );
      textw->swapBuffers();
      scroll_next();
      chrono.stop();
      double work= chrono.elapsed();
      if( work < time_for_line )
        Chrono::delay( time_for_line - work );
      }
     else
      eventManager->wait();
    }

  delete scrolling_text;
  scrolling_text= 0;

  delete textw;
  textw= 0;

  // flush any other event
  eventManager->get();
  eventManager->dispatch();

  eventManager->setEventGrabber( prev_grabber );

  SystemWindow* window= graphicsSystem->getRootWindow();
  window->currentPixel();
  mlFront();
  window->clear(0,0,0);
  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLStartup::eventGrabber( MLWindow* window, MLEvent& event )
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( window == graphicsSystem->getRootWindow() &&
      event.type == MLEvent::REDRAW )
    {
    drawLogo();
    return true;
    }

  if( event.type == MLEvent::KEY_RELEASE
#ifndef __WINDOWS__
      || event.type == MLEvent::MOUSE_RELEASE
#endif
    )
    {
    end= true;
    return true;
    }

  return window->handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLStartup::scroll_next()
{
  if( scrolling_text == 0 )
    return;

  int total_height= font->getHeight() * license_length;

  total_height += scrolling_text->getHeight();

  if( scrolling_index >= total_height )
    scrolling_index= 0;

  int height= scrolling_text->getHeight();

  // first lets scroll
  unsigned char* p_up= scrolling_text->getPixel( 0, height - 1 );
  unsigned char* p= scrolling_text->getPixel( 0, height - 2 );
  int line_length= scrolling_text->getLineSize();
  for( int i= scrolling_text->getHeight() - 1; i > 0; --i )
    {
    memcpy( (void*) p_up, (const void*) p, line_length );
    p_up= p;
    p -= line_length;
    }

  memset( (void*) scrolling_text->getPixel(0,0), 0, line_length );
  int current_line= scrolling_index / font->getHeight();

  if( current_line >= license_length )
    {
    ++scrolling_index;
    return;
    }

  if( scrolling_index % font->getHeight() == 0 )
    {
    const char* line= license_text[current_line];

    font->drawString( scrolling_left_margin+3,
                      scrolling_margin - font->getDescent()-5, scrolling_text, line );
    }

/*
  p= scrolling_text->getPixel(0,0);
  // highly optimized!:-)
  for( int j= 0; j < scrolling_left_margin; ++j )
    {
    double phi= double(scrolling_index) / double(font->getHeight())
                * 3.14159 / 3.;

    double theta= double(j) / double(scrolling_left_margin-1) * 3.14*.5;

//    double rr= 1. - (sin(-phi+1.5*theta)+1.) * .5 * .8;
//    double gg= 1. - (sin( phi+1.3*theta)+1.) * .5 * .8;
//    double bb= 1. - (sin( phi-1.8*theta)+1.) * .5 * .8;
//
//    double a= 205. * cos(theta);
//
//    p[0]= (unsigned char)(rr * a);
//    p[1]= (unsigned char)(gg * a);
//    p[2]= (unsigned char)(bb * a);

    double rr= 1. - (sin(-phi+1.5*theta)+1.) * .5 * .8;
    double a= 205. * cos(theta);
    p[0]=p[1]=p[2]= (unsigned char)(rr * a);

    p+= 4;
    }
*/

  ++scrolling_index;
}

/////////////////////////////////////////////////////////////////////////////

void MLStartup::drawLogo()
{
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  SystemWindow* window= graphicsSystem->getRootWindow();

  window->currentPixel();
  mlBack();
  window->clear(0,0,0);

  IVector wsize= window->getSize();

  int x= (wsize.x() - logo1->getWidth()) / 2;
  int y= 2 * (wsize.y() - logo1->getHeight()) / 3 + 50 ;
  logo1->draw(x,y);

  main_logo_x1= x;
  main_logo_y1= y;
  main_logo_x2= x + logo1->getWidth() - 1;
  main_logo_y2= y + logo1->getHeight() - 1;

  if( logo2 != 0 )
    {
    y-= 3 * logo2->getHeight() / 2;
    logo2->draw( (wsize.x() - logo2->getWidth()) / 2, y );
    }

  window->swapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

MLPicture* MLStartup::getLogo( int logo_width, int logo_height,
                               unsigned char logo_data[] )
{
  MLPicture* logo= new MLPicture;
  logo->createRGB( logo_width, logo_height );

  MLZDeflated deflated(logo_data);

  if( ! deflated.inflate( (void*) logo->getData() ) )
    {
    delete logo;
    return 0;
    }

  return logo;
}

/////////////////////////////////////////////////////////////////////////////

void MLStartup::buildLogos()
{
#ifndef __WINDOWS__
  if( logo2 == 0 )
    {
    logo2= getLogo(logo2_width,logo2_height,logo2_data);
    logo2->gammaCorrect();
    }
#endif

  if( logo1 != 0 )
    return;

  logo1= getLogo(logo1_width,logo1_height,logo1_data);
  if( logo1 == 0 )
    return;

  MLString version=  MLString("Version ")
                   + MLString(MLInterfaceVersion::version())
                   + MLString(" (#")
                   + MLString::valueOf(MLInterfaceVersion::build_number())
                   + MLString(")");
  MLString copyright= "Copyright \x7f 1996-1998 Stephane C.F. Rehel";

  int x, y;

  x= logo1->getWidth() - 15 - font->getStringWidth(version.get());
  y= logo1->getHeight() - 107;
  font->drawString( x, y, logo1, version.get() );

  x= logo1->getWidth() - 15 - font->getStringWidth(copyright.get());
  y= y - font->getHeight() - 3;
  font->drawString( x, y, logo1, copyright.get() );

  logo1->gammaCorrect();
}

/////////////////////////////////////////////////////////////////////////////
