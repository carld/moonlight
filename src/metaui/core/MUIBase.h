/*
 * MUIBase.h (c) 2002 Gregor Mueckl
 */
#ifndef MUIBase_H
#define MUIBase_H

#include <vector>

namespace MetaUI {
  /**
   * This is the base class for all Widgets. It implements
   * all common, basic widget behaviour.
   */
  struct Widget {
  protected:
    /**
     * This is a pointer to the parent of this widget, if a
     * parent exists. In case this is a top-level widget this
     * pointer is NULL.
     */
    Widget *parent;

    /**
     * This is an array of pointers to children of this widget.
     */
    vector<Widget *> children;

    /**
     * This registers a new child for this widget.
     */
    void registerChild(Widget *w);
    /**
     * This unregisters a child for this widget.
     */
    void unregisterChild(Widget *w);

  public:
    /**
     * Basic constructor.
     */
    Widget();
    /**
     * Basic destructor
     */
    ~Widget();

    /**
     * This function reparents this widget.
     */
    virtual void setParent(Widget *p);
    /**
     * Returns the parent of this widget or 0 if this is the
     * top-level widget.
     */
    virtual Widget *getParent();

    /**
     * This function forces the widget to completely redraw 
     * itself.
     */
    virtual void redraw() = 0;

    /**
     * This helper function recursively forces this
     * widget and all its childs to redraw themselves.
     * Do not call it directly unless you know what you
     * are doing!
     */
    virtual void redrawAll();

    /** 
     * This function returns the width of the widget in pixel.
     * This is important for layouting the dialogs from the
     * relative positions given in the dialog definition files.
     */
    virtual int getWidth() = 0;
    /**
     * This function returns the height of the widget in pixel. This
     * is important for the layouting the dialogs from the relative
     * positions given in the dialog definition files.
     */
    virtual int getHeight() = 0;

    /**
     * This function sets the widget's position relative to the top
     * left corner of the parent widget.
     */
    virtual void setPosition(int X, int Y) = 0;
  };
}

#endif
