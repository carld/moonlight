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
  ml.C

  Stephane Rehel
  June 29 1996
*/

#include <stdio.h>

#include "tools/MLCommandLine.h"

#include "graphics/InitGraphicsSystem.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/MLEventManager.h"
#include "graphics/SystemWindow.h"
#include "graphics/SoftWindow.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"
#include "graphics/MLDialog.h"

#include "graphics/MLPixmap.h"
#include "graphics/MLPixmaps.h"

#include "graphics/mlgl.h"

#include "kernel/MLKernel.h"

/////////////////////////////////////////////////////////////////////////////

void buildDemo( SystemWindow* systemParent );
void buildScrolling( SystemWindow* systemParent );

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

//printf( "done\n");
//SystemWindow* W= graphicsSystem->getRootWindow();
  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

SystemWindow* W= graphicsSystem->getRootWindow();
/*
graphicsSystem->newFullScreenWindow();
W->map();
W->createContext();
*/

W->currentPixel();
mlFront();

W->clear(0.5,0.3,0.3);
glFlush();

/*
mlColor3f(0.2,0.6,0.2);
glBegin(GL_TRIANGLES);
glVertex2i(0,100);
glVertex2i(100,200);
glVertex2i(0,200);
glEnd();

mlColor3f(0,1,1);
glBegin(GL_LINES);
glVertex2i(2,2);
glVertex2i(600,600);
glEnd();

mlLine( 99, 5, 99, 600 );

mlColor3f(1,1,0);
normalFont->print(0,0,"Coucoujgy tout le monde\n");

mlColor3f(0,0,0);
MLPixmaps::check->print( 10,10+MLPixmaps::check->getHeight()-1 );


SoftWindow* sw= W->newSoftChild( 50, 50, 100, 100 );
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
d->create( W, 100,500,200,200, "The Title" );
d->redraw();

mlColor3f(0,0,0);
normalFont->print(10,10,"boubou");
*/

buildDemo(W);
buildScrolling(W);

  MLEventManager* eventManager= MLEventManager::getEventManager();
  eventManager->wait();
//  printf("main loop\n");

for(;;)
{
  eventManager->dispatch();
  eventManager->wait();

//  printf( "got_events "); fflush(stdout);
}

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
