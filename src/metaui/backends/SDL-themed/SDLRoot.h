/*
 * SDLRoot.h (c) 2002 Gregor Mueckl
 */

#ifndef SDLRoot_H
#define SDLRoot_H

#include "MUIRoot.h"
#include "SDLWidgetBase.h"

namespace SDL {
  class Root : public MetaUI::Root, public WidgetBase {
  public:
    Root() {};
    ~Root() {};

    WidgetBase *getSDLParent() { return dynamic_cast<WidgetBase*>(parent); };

    void redraw() { WidgetBase::redraw(); };
    int getWidth() { return WidgetBase::getWidth(); };
    int getHeight() { return WidgetBase::getHeight(); };
    void setPosition(int X, int Y) { WidgetBase::setPosition(X,Y); };
    void redrawAll() { MetaUI::Widget::redrawAll(); };
  };
}

#endif
