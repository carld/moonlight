/*
 * MUIEvent.h (c) 2002 Gregor Mueckl
 */
#ifndef MUIEvent_h
#define MUIEvent_H

namespace MetaUI {
  /**
   * This is the class carrying all MetaUI events
   * to be distributed internally in the MetaUI system
   * and to the event handlers.
   */
  struct Event {
  public:
    /**
     * Simple constructor.
     */
    Event() {};
    /**
     * Simple destructor
     */
    ~Event() {};

    /**
     * This is a list of all event types that exist in MetaUI.
     */
    enum Type {
      MouseUp,
      MouseDown,
      MouseMove,
      KeyUp,
      KeyDown,
      // Events which are sometimes emitted instead of a collection of above events:
      KeyPress,
      Click,
      DoubleClick
    };

    /**
     * This function returns the type of the event. The return value is always valid.
     */
    Type getType() = 0;

    /**
     * This function returns the key code of pressed/released key when the event is a 
     * any of the Key* events. Otherwise the return value of this function
     * is undefined.
     */
    int getKeyCode() = 0;
    /**
     * This function returns the X coordinate of the mouse cursor relative to the widget
     * in which the event did happen. It is only valid for Mouse* and *Click events. The
     * return value ís undefined for any other type of events.
     */
    int getMouseX() = 0;
    /**
     * Same as GetMouseX(), but for the Y coordinate of the mouse pointer.
     */
    int getMouseY() = 0;
    /**
     * This returns the button that was pressed or released on a MouseUp, MouseDown, Click
     * or DoubleClick event. With other event types the return value is undefined.
     */
    int getMouseButton() = 0;
  };
};

#endif
