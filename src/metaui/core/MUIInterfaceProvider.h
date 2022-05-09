/*
 * MUIKernel.h (c) 2002 Gregor Mueckl
 */

#ifndef MUIInterfaceProvider_H
#define MUIInterfaceProvider_H

#include "MUIRoot.h"
#include "MUIDrawable.h"

namespace MetaUI {

  // A couple of forward references:
  struct Label;
  struct Button;
  struct Window;

  /**
   * This class provides all basic interface services
   * for the GUI and serves as a widget creator.
   */
  struct InterfaceProvider {
  public:
    InterfaceProvider() {};
    ~InterfaceProvider() {};

    /** 
     * Initialize the display. This includes any actions required to prepare
     * the display for drawing. Returns true on success, false on failure.
     */
    virtual bool initDisplay() = 0;
    /**
     * Clean up the display. Does all steps neccessary to get the display in
     * an acceptable shapes (remove all open windows, reset mouse cursor to
     * normal, resetting screen mode, etc.). Returns true on success, false
     * on failure.
     */
    virtual bool cleanupDisplay() = 0;
    
    /**
     * Returns the root window.
     */
    virtual Root *getRoot() = 0;

    /**
     * Creates a new backend-specific Drawable and returns
     * it or returns NULL on failure.
     */
    virtual Drawable *createDrawable() = 0;
    /**
     * Creates a new backend-specific Label and returns it
     * or returns NULL on failure.
     */
    virtual Label *createLabel() = 0;
    /**
     * Creates a new backend-specific Button and returns it
     * or returns NULL on failure.
     */
    virtual Button *createButton() = 0;
    /**
     * Creates a new backend-specific Window and returns it
     * or returns NULL on failure.
     */
    virtual Window *createWindow() = 0;
    
    /**
     * Forces a redraw of the whole GUI.
     */
    virtual void redraw() = 0;

    /**
     * Handles all pending GUI events and immediately returns
     * after that.
     */
    virtual void handleEvents() = 0;
  };
}

#endif
