/*
 * SDLWidgetBase.h (c) 2002 Gregor Mueckl
 */

#ifndef SDLWidgetBase_H
#define SDLWidgetBase_H

#include "MUIBase.h"

#include "SDLScreenArea.h"

namespace SDL {
  /**
   * This class ia a common base for all the widgets
   * in the SDL backend and contains all common functionality
   * needed and not provided by the MetaUI core.
   */
  class WidgetBase {
  protected:
    /**
     * Width of this widget
     */
    int width;
    /**
     * Height of this widget
     */
    int height;
    /**
     * X coordinate of this widget relative to the top left
     * corner of the SDL window or screen
     */
    int absX;
    /**
     * Y coordinate of this widget relative to the top left
     * corner of the SDL window or screen
     */
    int absY;
    /**
     * X coordinate of this widget relative to the top left
     * corner of the parent widget
     */
    int posX;
    /**
     * Y coordinate of this widget relative to the top left
     * corner of the parent widget
     */
    int posY;
    /**
     * This ScreenArea is the area on the screen covered by this widget.
     * All drawing operations are passed through to this member.
     */
    ScreenArea *area;
  public:
    WidgetBase();
    ~WidgetBase();

    virtual WidgetBase *getSDLParent() = 0;

    virtual void redraw() { area->blitToScreen(); };

    virtual int getWidth() { return width; };
    virtual int getHeight() { return height; };
    virtual void setPosition(int X, int Y);
    virtual void resize(int Width, int Height);
    
  protected:
    // additional functions needed for this backend only:
    
    /**
     * This function returns the X coordinate of this widget's posiiton
     * relative to the top left corner of the SDL window or screen. It 
     * is used to determine the coordinates at which the widget must be 
     * drawn.
     */
    virtual int getAbsPositionX() { return absX; };
    /**
     * This function returns the Y coordinate of this widget's posiiton
     * relative to the top left corner of the SDL window or screen. It 
     * is used to determine the coordinates at which the widget must be 
     * drawn.
     */
    virtual int getAbsPositionY() { return absY; };
  };
}

#endif
