/*
 * SDLDrawable.h (c) 2002 Gregor Mueckl
 */

#ifndef SDLDrawable_H
#define SDLDrawable_H

#include "SDL.h"

#include "MUIDrawable.h"

#include "SDLWidgetBase.h"

namespace SDL {
  class Drawable : public MetaUI::Drawable, public WidgetBase {
  protected:
    /**
     * Width of this drawable
     */
    int width;
    /**
     * Height of this drawable
     */
    int height;
    /**
     * X coordinate of this drawable relative to the top left
     * corner of the SDL window or screen
     */
    int absX;
    /**
     * Y coordinate of this drawable relative to the top left
     * corner of the SDL window or screen
     */
    int absY;
    /**
     * X coordinate of this drawable relative to the top left
     * corner of the parent widget
     */
    int posX;
    /**
     * Y coordinate of this drawable relative to the top left
     * corner of the parent widget
     */
    int posY;
  public:
    Drawable();
    ~Drawable();

    // pute virtual implementations needed because of WidgetBase
    WidgetBase *getSDLParent() { return dynamic_cast<WidgetBase*>(parent); };

    // pure virtual implementations needed because of MetaUI::Widget
    void redraw() { WidgetBase::redraw(); };
    int getWidth() { return width; };
    int getHeight() { return height; };
    void setPosition(int X, int Y) { WidgetBase::setPosition(X,Y); };

    // pure virtual implementations needed because of MetaUI::Drawable
    void resize(int Width, int Height) { WidgetBase::resize(Width,Height); };
    void setColor(float R, float G, float B) { area->setColor(R,G,B); };
    void drawPixel(int X, int Y) { area->drawPixel(X,Y); };
    void drawLine(int X1, int Y1, int X2, int Y2) { area->drawLine(X1,Y1,X2,Y2); };
    void fillRect(int X1, int Y1, int X2, int Y2) { area->fillRect(X1,Y1,X2,Y2); };
    void drawRect(int X1, int Y1, int X2, int Y2) { area->drawRect(X1,Y1,X2,Y2); };
    // TODO: replace this dummy implementation with a working one:
    void drawText(int X, int Y, const char * Text) {};
  };
}

#endif
