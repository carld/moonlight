/*
 * SDLWidgetBase.C (c) 2002 Gregor Mueckl
 */

#include "SDLWidgetBase.h"

namespace SDL {
  WidgetBase::WidgetBase()
  {
    width=0;
    height=0;
    posX=0;
    posY=0;
    absX=0;
    absY=0;

    area=new ScreenArea;
  }

  WidgetBase::~WidgetBase()
  {
    delete area;
  }

  void WidgetBase::setPosition(int X, int Y)
  {
    posX=X;
    posY=Y;

    if(getSDLParent()==0) {
      cout << "SDL::WidgetBase::setPosition(): no parent widget" << endl;
      absX=posX;
      absY=posY;
    } else {
      cout << "SDL::WidgetBase::setPosition(): got a parent widget" << endl;
      absX=((WidgetBase*)getSDLParent())->getAbsPositionX()+X;
      absY=((WidgetBase*)getSDLParent())->getAbsPositionY()+Y;
    }
    area->setPosition(absX,absY);
  }

  void WidgetBase::resize(int Width, int Height)
  {
    width=Width;
    height=Height;

    area->setSize(Width,Height);
  }

}
