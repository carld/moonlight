// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*
   VWindow.h

   Creation: SR, July 24th, 1995
   Revisions:
     + SR, November 21st 1995 (add `void VWindow::closeWindow()')
     + SR, July 5 1996
*/

#ifndef __VWindow_h
#define __VWindow_h

#ifndef __Common_h
#include "tools/Common.h"
#endif

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

#ifndef __IVector_h
#include "tools/IVector.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

#ifndef __Color8_h
#include "tools/Color8.h"
#endif

#ifndef __MLEventHandler_h
#include "graphics/MLEventHandler.h"
#endif

class MLString;

class SoftWindow;
class MLEvent;
class MLWindow;
class MLFont;
class MLPixmap;
class MLGLPixmap;

class VWindowEventHandler;

class VBlankButton;
class VButton;
class VLabel;
class VArrow;
class VBar;
class VHorizBar;
class VVertBar;
class VScrollBar;
class VHorizScrollBar;
class VVertScrollBar;
class VList;
class VScrollList;
class VCheckButton;
class VCheckBox;
class VEdit;
class VValueEdit;
class VCircleButton;
class VRadioButton;

class VRangeBar;
class VValueBar;

class VScrollingDialogs;
class VWidgetsSet;
class VGroup;
class VLine;
class VHorizLine;
class VVertLine;
class VTextBox;
class VPicture;
class VChooser;
class VCard;

/////////////////////////////////////////////////////////////////////////////

class VWindow: public MLEventHandler
{
  friend class VChooser;

protected:
  MLWindow* systemParent;
  SoftWindow* systemWindow;
  boolean isMapped;

  boolean ownImage;
  MLGLPixmap* image;
  IPoint windowImagePosition;
  boolean frontMode;

  MLFont* font;
  Color8 backgroundColor;

  VWindowEventHandler* veventHandler;

  VWindow* parent;
  SIList<VWindow> children;

public:
  VWindow( MLWindow* _systemParent );
  VWindow( VWindow* _parent );
  virtual ~VWindow();

protected:
  void init();
  void addChild( VWindow* child );
  void removeChild( VWindow* child );

public:
  void deleteChild( VWindow* child );

public:
  virtual void create( int x, int y, int width, int height );

  // aliases
  virtual void create( const IPoint& pos, const IVector& size );
  virtual void create( const IPoint& p1, const IPoint& p2 );

  void createWindowImage();

  MLWindow* getSystemParent() const
    {
    return systemParent;
    }
  VWindow* getParent() const
    {
    return parent;
    }

protected:
  void setWindowImage();

  virtual boolean eventHandler( MLWindow*, MLEvent& event );

public:
  void setEventHandler( VWindowEventHandler* _handler );
  virtual boolean handleEvent( MLEvent& event );

  void removeEvent( int eventType );
  boolean findEvent( int eventType );
  void postPrivateEvent( int eventType );
  void postRedraw();
  void removeRedraw();

public:
  const IPoint& getPosition() const;
  IPoint getAbsolutePosition() const;
  const IVector& getSize() const;
  IRectangle getRectangle() const;
  void setPosition( const IPoint& newPos );
  virtual void setSize( const IVector& newSize );

  boolean mapped() const
    {
    return isMapped;
    }
  virtual void map( boolean yes = true );

  // GraphicsOp
  void front( boolean yes = true );
  void back();
  void swapBuffers();
  void putImage();
private:
  void _front( boolean yes );
public:
  boolean beginDraw();
  void endDraw();
  void flush();

  void scissor();
  void unscissor();

  Color8 getGray0(); // black
  Color8 getGray1();
  Color8 getGray2();
  Color8 getGray3(); // white
  void gray0(); // black
  void gray1();
  void gray2();
  void gray3(); // white

  void setForeground( Color8 color );
  void setBackground( Color8 color );

  void drawLine( int x1, int y1, int x2, int y2 );
  void drawLine( const IPoint& p1, const IPoint& p2 );
  void drawHorizLine( int x1, int x2, int y );
  void drawHorizLine( const IPoint& origin, int length );
  void drawVertLine( int x, int y1, int y2 );
  void drawVertLine( const IPoint& origin, int length );

  void drawRectangle( const IPoint& p1, const IPoint& p2 );
  void drawRectangle( int x, int y, int width, int height );
  void drawDotRectangle( const IPoint& p1, const IPoint& p2 );

  void fillRectangle( const IPoint& p1, const IPoint& p2 );
  void fillRectangle( int x, int y, int width, int height );
  void fillRectangle( const IPoint& pos, const IVector& size );
  void fillGrayRectangle( const IPoint& p1, const IPoint& p2 );
  void fillGrayRectangle2( const IPoint& p1, const IPoint& p2 );

  void fillQuad( const IPoint& p1, const IPoint& p2,
                 const IPoint& p3, const IPoint& p4 );

  void drawTriangle( const IPoint& p1, const IPoint& p2, const IPoint& p3 );
  void fillTriangle( const IPoint& p1, const IPoint& p2, const IPoint& p3 );

  void drawPressedRectangle( int x, int y, int width, int height );
  void drawUnpressedRectangle( int w, int y, int width, int height );

  void drawPressedRectangle( int width, int height );
  void drawUnpressedRectangle( int width, int height );

  void fillWindow( Color8 color );

  void setFont( MLFont* _font );

  MLFont* getFont() const
    {
    return font;
    }

  void drawString( int x, int y, const char* string, int length );
  void drawString( int x, int y, const char* string );
  void drawString( const IPoint& pos, const char* string );
  void drawString( const IPoint& pos, const MLString& string );

  void drawImageString( int x, int y, const char* string, int length );
  void drawImageString( int x, int y, const char* string );
  void drawImageString( const IPoint& pos, const MLString& string );

  int textWidth( const char* string );
  int textHeight() const;

  void drawPixmap( MLPixmap* pixmap, int x, int y );
  void drawImage( MLGLPixmap* image, int dest_x, int dest_y );
  void drawImage( MLGLPixmap* image,
                  int src_x, int src_y,
                  int width, int height,
                  int dest_x, int dest_y );

  boolean cursorInto( const MLEvent& event ) const;

  static MLString cutString( MLFont* font,
                            const MLString& _string, int max_width );

  MLString cutString( const MLString& _string, int max_width ) const;

  static SIList<MLString>* splitString( MLFont* font,
                                       const MLString& string,
                                       int max_width );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VWindow_h

