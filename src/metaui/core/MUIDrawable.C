/*
 * MUIDrawable.C (c) 2002 Gregor Mueckl
 */

#include "MUIDrawable.h"

namespace MetaUI {
  Drawable::Drawable()
  {
  }

  Drawable::~Drawable()
  {
  }

  void Drawable::drawLine(int X1, int Y1, int X2, int Y2) 
  {
    float m, deltaX, deltaY;
    int i;

    if(X2<X1) {
      i=X2; X2=X1; X1=i;
      i=Y2; Y2=Y1; Y1=i;
    }

    if(X1==X2) {
      if(Y2<Y1) {
      i=Y2; Y2=Y1; Y1=i;
      }
      for(i=Y1;i<Y2;i++) {
	drawPixel(X1,i);
      }
      return;
    }
    if(Y1==Y2) {
      if(X2<X1) {
	i=X2; X2=X1; X1=i;
      }
      for(i=X1;i<X2;i++) {
      drawPixel(i,Y1);
      }
      return;
    }
    m=((double)Y2-(double)Y1)/((double)X2-(double)X1);
    if(m>1.0) {
      m=1.0/m;
      for(i=Y1;i<Y2;i++) {
	drawPixel((int)(m*(i-Y1)+X1),i);
      }
    } else if(m<-1.0) {
      m=1.0/m;
      for(i=Y2;i<Y1;i++) {
	drawPixel((int)(m*(i-Y1)+X1),i);
      }
    } else if(m<0.0) {
      for(i=X2;i>X1;i--) {
	drawPixel(i,(int)(m*(i-X1)+Y1));
      }
    } else {
      for(i=X1;i<X2;i++) {
	drawPixel(i,(int)(m*(i-X1)+Y1));
      }
    }
  }

  void Drawable::drawRect(int X1, int Y1, int X2, int Y2)
  {
    drawLine(X1,Y1,X2,Y1);
    drawLine(X1,Y1,X1,Y2);
    drawLine(X2,Y1,X2,Y2);
    drawLine(X1,Y2,X2,Y2);
  }

  void Drawable::fillRect(int X1, int Y1, int X2, int Y2)
  {
    int x, y;
    int swap;

    if(X2<X1) {
      swap=X1;
      X1=X2;
      X2=swap;
    }
    if(Y2<Y1) {
      swap=Y1;
      Y1=Y2;
      Y2=swap;
    }

    for(y=Y1;y<=Y2;y++) {
      for(x=X1;x<=X2;x++) {
	drawPixel(x,y);
      }
    }
  }
}
