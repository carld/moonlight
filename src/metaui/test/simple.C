/*
 * simple.C (c) 2002 Gregor Mueckl
 */

#include <iostream>

#include "SDL.h"

#include "MUIKernel.h"
#include "MUIInterfaceProvider.h"
#include "MUIDrawable.h"

int main(int argc, char *argv[])
{
  MetaUI::Kernel *kernel;
  MetaUI::Drawable *drawable;

  kernel=new MetaUI::Kernel;
  if(!kernel->loadBackend(argv[1])) {
    cerr << "kernel->loadBackend() failed" << endl;
    return -1;
  }
  kernel->getInterfaceProvider()->initDisplay();
  drawable=kernel->getInterfaceProvider()->createDrawable();
  drawable->setParent(kernel->getInterfaceProvider()->getRoot());
  drawable->resize(10,10);
  drawable->setPosition(0,0);
  drawable->setColor(1.0,1.0,1.0);
  drawable->fillRect(0,0,10,10);
  kernel->getInterfaceProvider()->redraw();
  SDL_Delay(2000);
  delete drawable;
  kernel->getInterfaceProvider()->cleanupDisplay();
  kernel->unloadBackend();
  delete kernel;
}
