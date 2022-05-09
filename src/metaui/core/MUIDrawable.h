#ifndef MUIDrawable_H
#define MUIDrawable_H

#include "MUIBase.h"

namespace MetaUI {

  /**
   * This is a basic widget that implements a drawable area.
   */
  struct Drawable : public Widget {
  public:
    /**
     * Simple constructor
     */
    Drawable();
    /** 
     * Simple destructor
     */
    virtual ~Drawable();

    /**
     * This member function sets the Drawable's size. This
     * function is unique to the Drawable widget.
     */
    virtual void resize(int Width, int Height) = 0;

    /**
     * This function sets the color to use for the next drawing
     * operations. It takes in three numbers giving the value
     * of the R, G and B color components where 0 is darkest and
     * 1 is brightest.
     */
    virtual void setColor(float R, float G, float B) = 0;

    /**
     * Draws a line on this widget. Coordinates are relative to
     * the top left corner of the widget. The coordinates are
     * automatically culled against the borders of this wiget.
     */
    virtual void drawPixel(int X, int Y) = 0;

    /**
     * Draw a line on this widget. Coordinates are relative
     * to the top left corner of the widget. The line is
     * automatically culled against the borders of this
     * widget.
     */
    virtual void drawLine(int X1, int Y1, int X2, int Y2);

    /**
     * Draws a rectangle outline on this widget. Coordinates 
     * are relative to the top left corner of the widget. 
     */
    virtual void drawRect(int X1, int Y1, int X2, int Y2);

    /**
     * Draws a filled rectangle on this widget. Coordniates
     * are relative to the top left corner of the widget.
     */
    virtual void fillRect(int X1, int Y1, int X2, int Y2);

    /**
     * Draws the given Text on this widget. Coordinates are
     * relative to the top left corner of the widget.
     */
    virtual void drawText(int X1, int Y1, const char *Text) = 0;
  };
};

#endif
