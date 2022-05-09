/*
 * SDLInterfaceProvider.C (c) 2002 Gregor Mueckl
 */

#include <iostream>

#include "SDL.h"

#include "SDLInterfaceProvider.h"

using namespace std;

namespace SDL {
  InterfaceProvider::InterfaceProvider()
  {
  }

  InterfaceProvider::~InterfaceProvider()
  {
    cleanupDisplay();
  }

  bool InterfaceProvider::initDisplay()
  {
    cout << "SDL::InterfaceProvider::initDisplay()" << endl;

    if(SDL_Init(SDL_INIT_VIDEO)==-1) {
      return false;
    }

    cout << "SDL_Init OK" << endl;

    // TODO: do proper OpenGL initialisation here
    screen=SDL_SetVideoMode(640,480,32,SDL_RESIZABLE);

    // set up the root window/widget:
    root=new Root;
    root->setPosition(0,0);
    root->resize(640,480);

    return true;
  }

  bool InterfaceProvider::cleanupDisplay()
  {
    if(root) {
      delete root;
      root=0;
    }
    SDL_Quit();
  }

  void InterfaceProvider::redraw()
  {
    root->redrawAll();
    SDL_UpdateRect(screen,0,0,0,0);
  }

  void InterfaceProvider::handleEvents()
  {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_KEYDOWN:
	// TODO: handle key press events
	/* 
	 * Send this event to the widget that has the keyboard focus
	 */
	break;
      case SDL_KEYUP:
	// TODO: handle key release event
	/*
	 * Send this event to the widget that has the keyboard focus
	 */
	break;
      case SDL_MOUSEMOTION:
	// TODO: handle mouse motion events (figure out how)
	/*
	 * Send this event to the widget that has registered to recieve
	 * this event. Ususally the widget registers itself for that inside
	 * the MouseDown event handler and unregisters when it recieves a
	 * MouseUp event.
	 */
	break;
      case SDL_MOUSEBUTTONDOWN:
	// TODO: handle mouse button press events
	/*
	 * Go down the widget tree searching for the deepest widget that
	 * covers the location that clicked happend in.
	 */
	break;
      case SDL_MOUSEBUTTONUP:
	// TODO: handle mouse button release events
	/*
	 * Send this event to the widget that has registered to recieve
	 * this event. Usually the widget registers itself for that inside
	 * the MouseDown event handler.
	 */
	break;
      }
    }
  }
}

extern "C" MetaUI::InterfaceProvider *GetInterfaceProvider()
{
  return new SDL::InterfaceProvider;
}
