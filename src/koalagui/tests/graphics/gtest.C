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
  ktest.C

  Stephane Rehel
  October 28 1996
*/

#include <stdio.h>

#include "tools/MLCommandLine.h"

#include "graphics/InitGraphicsSystem.h"
#include "graphics/MLEventManager.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/SystemWindow.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLDialog.h"
#include "graphics/MLGLPixmap.h"

#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"

#include "graphics/mlgl.h"

#include "kernel/MLKernel.h"

/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
  MLCommandLine command(argc,(const char**)argv);

  MLKernel* kernel= new MLKernel;
  if( ! kernel->init(argc,argv) )
    {
    fprintf( stderr, "error in kernel initialization, aborting\n" );
    return 1;
    }

  if( ! InitGraphicsSystem(kernel->getConfig(),command) )
    {
    fprintf( stderr, "Error: unable to open graphics system\n" );
    return 1;
    }

  // we got the main root window of the program
  SystemWindow* root1= GraphicsSystem::getGraphicsSystem()->getRootWindow();
  int width=  root1->getSize().x();
  int height= root1->getSize().y();

  SystemWindow* root= root1->newSystemChild( 0, 0, width, height );
  root->map();

  MLGLPixmap* img= GraphicsSystem::getGraphicsSystem()->newMLGLPixmap();
  img->create(100,50);
  img->currentPixel();
  mlColor3f(1,0,0);
  mlRect(0,0,100-1,50-1);

  root->currentPixel();
  glViewport( 0, 0, width, height );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( 0, width, 0, height, -1., 1. );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef( 0.375, 0.375, 0. );

  mlFront();
  root->clear(0.3,0.3,0.3);

  mlColor3f(0,1,1);
  glBegin(GL_LINES);
  glVertex2i(2,2);
  glVertex2i(600,600);
  glEnd();

  mlColor3f(0.2,0.6,0.2);
  glBegin(GL_TRIANGLES);
  glVertex2i(0,100);
  glVertex2i(100,200);
  glVertex2i(0,200);
  glEnd();

  mlLine( 99, 5, 99, 600 );

  IPoint O(300,300);
  int i;
  mlColor3f(0,0,1);
  int delta= 7;
  int n= 5;
  for( i= 0; i < n; ++i )
    {
    int x= O.x()+i*7;
    int y= O.y()+i*7;
    mlLine( O.x(), y, O.x()+delta*n, y );
    mlLine( x, O.y(), x, O.y()+delta*n );
    }
  mlColor3f(1,0,0);
  mlLine( O.x()-3, O.y()-3, O.x() + delta*n, O.y()+delta*n );

  mlColor3f(1,1,0);
  normalFont->print(0,0,"Coucoujgy tout le monde\n");

  mlColor3f(0,0,0);
  MLPixmaps::check->print( 10,10+MLPixmaps::check->getHeight()-1 );

  SoftWindow* sw= root->newSoftChild( 50, 50, 100, 100 );
  SoftWindow* sw2= new SoftWindow;
  sw2->create(sw, 10,10,50,20 );
  sw->setPosition( IPoint(100,100) );

  sw->currentPixel();
  sw->scissor();
  sw->clear(1,1,1);
  mlColor3f(0,0,1);
  mlLine( 2, 2, 97, 97 );

  mlColor3f(1,1,0);
  normalFont->print(9,10,"FilCoucouXXXXtout le monde");

  mlBlack();
  mlFrame(0,0,99,99);

  sw->unscissor();

  sw2->currentPixel();
  sw2->clear(1,0,0);

  MLDialog* d= new MLDialog;
  d->create( root, 100,500,200,200, "The Title" );
  d->redraw();

  mlColor3f(0,0,0);
  normalFont->print(10,10,"boubou");

  // print the image
  root->currentPixel();
  mlFront();
  img->print(root,500,500);
/*
glFlush();
sleep(2);
root->currentPixel();
mlFront();
mlColor3f(0,1,0);
mlRect( 95, height-1-495, 95+210, height-1-(495+210) );
glFlush();
sleep(2);
*/

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->wait();

  for(;;)
    {
    eventManager->dispatch();
    eventManager->wait();
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////


