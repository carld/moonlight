/*
 * SDLInterfaceProvider.h (c) 2002 Gregor Mueckl
 */

#ifndef SDLInterfaceProvider_H
#define SDLInterfaceProvider_H

#include "SDL.h"

#include "MUIInterfaceProvider.h"
#include "MUIRoot.h"
#include "MUIDrawable.h"

#include "SDLDrawable.h"
#include "SDLRoot.h"

namespace SDL {

  /**
   * This is the interface provider implementation for the
   * themed SDL GUI backend.
   */
  class InterfaceProvider : public MetaUI::InterfaceProvider {
  private:
    /**
     * The screen surface onto which things are finally drawn.
     */
    SDL_Surface *screen;

    /**
     * The active root window instance. This will only exist
     * once.
     */
    Root *root;
  public:
    InterfaceProvider();
    ~InterfaceProvider();

    // pure virtual implementations because of MetaUI::Interfaceprovider
    bool initDisplay();
    bool cleanupDisplay();

    MetaUI::Root *getRoot() { return root; };
    MetaUI::Drawable *createDrawable() { new Drawable; };
    MetaUI::Label *createLabel() { return 0; };
    MetaUI::Button *createButton() { return 0; };
    MetaUI::Window *createWindow() { return 0; };
    
    void redraw();

    void handleEvents();

    /**
     * Helper function needed internally to get the things actually drawn
     * to the screen. The pointer to screen is fetched by SDL::Drawable
     * during costruction.
     */
    SDL_Surface *getScreen() { return screen; };
  };
}

#endif
