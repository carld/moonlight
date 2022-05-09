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
  ModelRenderWindow.C

  Stephane Rehel

  September 21 1997
*/

#include "ModelRenderWindow.h"
#include "ModelRender.h"

#include "graphics/SystemWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/MLEventManager.h"
#include "graphics/MLPicture.h"
#include "graphics/MLFonts.h"
#include "graphics/MLFont.h"
#include "graphics/KeyCodes.h"
#include "graphics/mlgl.h"

#include "interface/Interface.h"

#include "kmodel/ModelModule.h"


/////////////////////////////////////////////////////////////////////////////

ModelRenderWindow::ModelRenderWindow( ModelRender* _modelRender )
{
  modelRender= _modelRender;
  interface= modelRender->getModel()->getInterface();

  window= 0;
  x= y= 0;
  image_shrink= 1;
  traced_to_line= 0;
  message= "";
  message_length= 0;
  font= ::smallFont;

  need_to_redraw= false;
  user_action= false;
  user_break= false;

  prev_grabber= 0;
}

/////////////////////////////////////////////////////////////////////////////

ModelRenderWindow::~ModelRenderWindow()
{
  close();

  interface= 0;
  modelRender= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelRenderWindow::open()
{
  if( window != 0 )
    return true;

  if( interface == 0 )
    return false;

  window= interface->enterExclusiveMode();

  if( window == 0 )
    return false;

  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager != 0 )
    prev_grabber= eventManager->setEventGrabber(this);
   else
    prev_grabber= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::close()
{
  if( interface == 0 || window == 0 )
    return;

  eraseWindow();

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->setEventGrabber(prev_grabber);
  prev_grabber= 0;

  interface->leaveExclusiveMode();

  window= 0;
}

/////////////////////////////////////////////////////////////////////////////

int ModelRenderWindow::getWidth() const
{
  return (window==0) ? 0 : window->getSize().x();
}

/////////////////////////////////////////////////////////////////////////////

int ModelRenderWindow::getHeight() const
{
  return (window==0) ? 0 : window->getSize().x();
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelRenderWindow::eventGrabber( MLWindow* w, MLEvent& e )
{
  if( w != window && e.type != MLEvent::KEY_PRESS )
    return false;

  if( e.type == MLEvent::REDRAW )
    {
    need_to_redraw= true;
    return true;
    }

  if( e.type == MLEvent::KEY_PRESS )
    {
    if( interface->testIconifyingKey(e.key) )
      return true;

    if( e.key == KeyCodes::ESC ||
        e.key == KeyCodes::CTRL_G ||
        e.key == KeyCodes::CTRL_C )
      user_break= true;

    if( e.key == KeyCodes::ENTER ||
        e.key == KeyCodes::SPACE )
      user_action= true;

    return true;
    }

  if( e.type == MLEvent::MOUSE_PRESS )
    {
    user_action= true;
    return true;
    }

  return true; //w->handleEvent(e);
}

/////////////////////////////////////////////////////////////////////////////

// return false is user break
boolean ModelRenderWindow::process_events()
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager == 0 )
    return false;

  need_to_redraw= false;
  user_break= false;

  eventManager->get();
  if( ! eventManager->empty() )
    eventManager->dispatch();

  if( need_to_redraw )
    redraw();

  return ! user_break;
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::wait_for_user_action()
{
  MLEventManager* eventManager= MLEventManager::getEventManager();

  if( eventManager == 0 )
    return;

  need_to_redraw= false;
  user_break= false;
  user_action= false;

  eventManager->get();
  eventManager->dispatch();

  for(;;)
    {
    if( user_break || user_action )
      break;

    if( need_to_redraw )
      redraw();

    eventManager->wait();
    eventManager->dispatch();
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean ModelRenderWindow::current()
{
  if( window == 0 || modelRender == 0 )
    return false;

  if( ! window->mapped() || modelRender->getModel()->isIconic() )
    return false;

/*
#ifdef __WINDOWS__
  window->current();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0., GLfloat(window->getSize().x()),
           0., GLfloat(window->getSize().y()), -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#else
  window->currentPixel();
#endif
*/

  window->currentPixel();

  mlFront();
  glEnable(GL_DITHER);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::eraseWindow()
{
  if( ! current() )
    return;

  mlBlack();
  IVector size= window->getSize();

  mlRect( 0, 0, size.x()-1, size.y()-1 );

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::redraw()
{
  need_to_redraw= false;

  if( modelRender == 0 || window == 0 )
    return;

  if( ! current() )
    return;

  eraseWindow();

  MLPicture* picture= modelRender->getPicture();

  if( picture != 0 )
    {
    mlDarkGray();

    glLineStipple( 3, 0x7777 );
    glEnable( GL_LINE_STIPPLE );

    mlFrame( x, y,
             x + picture->getWidth()/image_shrink - 1,
             y + picture->getHeight()/image_shrink - 1 );

    glDisable( GL_LINE_STIPPLE );

    draw_lines( traced_to_line, picture->getHeight()-1 );
    }

  draw_message();
}

/////////////////////////////////////////////////////////////////////////////

// origin is at the top of the raytraced window
void ModelRenderWindow::reset_trace()
{
  if( modelRender == 0 )
    return;

  MLPicture* picture= modelRender->getPicture();

  if( picture == 0 || window == 0 )
    {
    x= y= 0;
    traced_to_line= 0;
    return;
    }

  int width=  max( 1, window->getSize().x() );
  int height= max( 1, window->getSize().y() - font->getHeight() );
  image_shrink= 1;
  int picture_width=  picture->getWidth();
  int picture_height= picture->getHeight();

  while( picture_width /image_shrink > width ||
         picture_height/image_shrink > height )
    ++image_shrink;

  x= (width  - picture_width /image_shrink) / 2;
  y= (height - picture_height/image_shrink) / 2;

  traced_to_line= picture->getHeight();
}

/////////////////////////////////////////////////////////////////////////////

// j begining at height-1 to 0
void ModelRenderWindow::set_traced_from_origin_to( int j )
{
  if( j < 0 )
    j= 0;

  if( j >= traced_to_line )
    return;

  if( current() )
    draw_lines( j, traced_to_line-1 );

  traced_to_line= j;
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::setMessage( const MLString& msg )
{
  if( message == msg )
    return;

  int prev_msg_length= message_length;
  message= msg;
  message_length= msg.length();

  if( current() )
    {
    if( prev_msg_length > 0 )
      {
      int y1= 0;
      int y2= font->getHeight() - 1;

      mlBlack();
      mlRect( 0, y1,
              prev_msg_length*font->getCharWidth(' ')-1, y2 );
      }

    draw_message();
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::draw_lines( int j1, int j2 )
{
  if( modelRender == 0 || window == 0 )
    return;

  if( j1 > j2 )
    return;

  MLPicture* picture= modelRender->getPicture();
  if( picture == 0 )
    return;

  j1= max( 0, j1 );
  j2= min( picture->getHeight()-1, j2 );

#ifdef __WINDOWS__
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#endif

  if( image_shrink == 1 )
    picture->draw( x, y, 0, j1, picture->getWidth()-1, j2 );
   else
    {
    // we need to shrink this image
    int shrinked_width= picture->getWidth() / image_shrink;
    MLPicture image;
    image.createRGB( shrinked_width, 1 );
    for( int j= j1; j <= j2; ++j )
      {
      if( (j % image_shrink) != 0 )
        continue;

      unsigned char* line= picture->getPixel(0,j);
      unsigned char* sline= image.getPixel(0,0);

      for( int i= 0; i < shrinked_width; ++i )
        {
        sline[0]= line[0];
        sline[1]= line[1];
        sline[2]= line[2];
        sline += 3;
        line += 3 * image_shrink;
        }

      image.draw( x, y + j / image_shrink );
      }
    }

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////

void ModelRenderWindow::draw_message()
{
  if( window == 0 )
    return;

  int y1= 0;
//  int y2= font->getHeight() - 1;

  mlDarkGray();

  y1+= font->getDescent();
  font->print( 2, y1, message );

  glFlush();
}

/////////////////////////////////////////////////////////////////////////////
