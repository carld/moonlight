/*
 * SDLScreenArea.h (c) 2002 Gregor Mueckl
 */

#ifndef SDLScreenArea_H
#define SDLScreenArea_H

#include "SDL.h"

namespace SDL {
  /**
   * This is a helper class containing drawing functions used
   * by all the widgets in this backend. All these routines
   * clip against the rectangle that is covered.
   */
  class ScreenArea {
  private:
    /**
     * Width of this screen area
     */
    int width;
    /**
     * Height of this screen area
     */
    int height;
    /**
     * X coordinate of this area relative to the top left
     * corner of the SDL window or screen
     */
    int x;
    /**
     * Y coordinate of this area relative to the top left
     * corner of the SDL window or screen
     */
    int y;
    /**
     * The currently selected color to draw in in whatever
     * color format the screen uses.
     */
    unsigned int color;
    /**
     * A pointer to the screen surface to draw onto. The screen
     * surface is managed by SDL::InterfaceProvider along with
     * SDL initialisation and finalization.
     */
    SDL_Surface *screen;
    /**
     * A immediate buffer which is blitted to the screen to speed
     * up expensive redraw actions. Also this simlifies the drawing
     * code a bit.
     */
    SDL_Surface *buffer;
  public:
    ScreenArea();
    ~ScreenArea();

    void setPosition(int X, int Y) { x=X; y=Y; };
    void setSize(int Width, int Height);

    void setColor(float R, float G, float B);
    void drawPixel(int X, int Y);
    void drawLine(int X1, int Y1, int X2, int Y2);
    void drawRect(int X1, int Y1, int X2, int Y2);
    void fillRect(int X1, int Y1, int X2, int Y2);

    void blitToScreen();
  };
}

#endif
