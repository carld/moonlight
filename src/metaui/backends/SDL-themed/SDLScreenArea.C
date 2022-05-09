/*
 * SDLScreenArea.C (c) 2002 Gregor Mueckl
 */

#include "SDL.h"

#include "MUIKernel.h"

#include "SDLInterfaceProvider.h"
#include "SDLScreenArea.h"

namespace SDL {
  ScreenArea::ScreenArea()
  {
    InterfaceProvider *provider;
    
    provider=(InterfaceProvider *)MetaUI::kernel->getInterfaceProvider();
    screen=provider->getScreen();

    buffer=0;
  }

  ScreenArea::~ScreenArea()
  {
    if(buffer)
      SDL_FreeSurface(buffer);
  }

  void ScreenArea::setSize(int Width, int Height)
  {
    width=Width;
    height=Height;

    if(buffer) {
      cout << "SDL::ScreenArea::resize() freing old buffer surface" << endl;
      SDL_FreeSurface(buffer);
    }

    cout << "SDL::ScreenArea::resize() allocating new buffer surface" << endl;
    // looks big, but actually does only get a surface in the same format screen has
    buffer=SDL_CreateRGBSurface(SDL_SWSURFACE,
				width,
				height,
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask,
				screen->format->Amask);
    cout << "SDL::ScreenArea::resize() allocation successful" << endl;

    // TODO: check whether surface creation failed (buffer==0)
  }

  void ScreenArea::blitToScreen()
  {
    // Simply blit the contents of buffer to the screen for now:
    cout << "SDL::ScreenArea::blitToScreen() going to blit to (" << x << ", " << y << "), area (" << width << ", " << height << ")" << endl;

    SDL_Rect source, destination;
    source.x=0;
    source.y=0;
    source.w=width;
    source.h=height;
    destination.x=x;
    destination.y=y;
    SDL_BlitSurface(buffer,&source,screen,&destination);
  }

  void ScreenArea::setColor(float R, float G, float B)
  {
    color=SDL_MapRGB(screen->format,(unsigned char)(255*R),(unsigned char)(255*G),(unsigned char)(255*B));
    cout << "SDL::ScreenArea::setColor(): color=" << color << endl;
  }

  void ScreenArea::drawPixel(int X, int Y)
  {
    // clip against the buffer edges:
    if(X<0) return;
    if(Y<0) return;
    if(X>=width) return;
    if(Y>=height) return;

    // XXX: much of this code is untested!
    switch(screen->format->BytesPerPixel) {
    case 1:
      ((unsigned char *)buffer->pixels)[Y*buffer->pitch+X]=(unsigned char)color;
      break;
    case 2:
      ((unsigned short *)buffer->pixels)[(Y*(buffer->pitch/sizeof(unsigned short)))+X]=(unsigned short)color;
      break;
    case 3:
      // TODO
      break;
    case 4:
      ((unsigned int *)buffer->pixels)[(Y*(buffer->pitch/sizeof(unsigned int)))+X]=(unsigned int)color;
      break;
    }
  }

  void ScreenArea::drawLine(int X1, int Y1, int X2, int Y2) 
  {
    cout << "SDL::ScreenArea::drawLine() called" << endl;

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

  void ScreenArea::drawRect(int X1, int Y1, int X2, int Y2)
  {
    cout << "SDL::ScreenArea::drawRect() called" << endl;

    drawLine(X1,Y1,X2,Y1);
    drawLine(X1,Y1,X1,Y2);
    drawLine(X2,Y1,X2,Y2);
    drawLine(X1,Y2,X2,Y2);
  }

  void ScreenArea::fillRect(int X1, int Y1, int X2, int Y2)
  {
    cout << "SDL::ScreenArea::fillRect() called" << endl;

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
