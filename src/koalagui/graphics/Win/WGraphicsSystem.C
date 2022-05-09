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
  WGraphicsSystem.C

  Windows 95/NT Graphics System Class

  Stephane Rehel
  July 6 1997
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "GL/opengl.h"

#include "tools/MLCommandLine.h"
#include "tools/Chrono.h"
#include "tools/MLConfig.h"

#include "WGraphicsSystem.h"
#include "WWindowDriver.h"
#include "WMLGLPixmap.h"
#include "WMLCursor.h"

#include "SystemWindow.h"

#include "WinDebug.h"

/////////////////////////////////////////////////////////////////////////////

//static boolean need_overrideRedirect_windows= false;

/////////////////////////////////////////////////////////////////////////////

static HANDLE hInstance= HANDLE(0);

static ATOM aMainRegister= ATOM(0);
static const char* lpszMainClassName= "ML_MainWindowClass";

static ATOM aChildRegister= ATOM(0);
static const char* lpszChildClassName= "ML_ChildWindowClass";

static ATOM aSaveUnderChildRegister= ATOM(0);
static const char* lpszSaveUnderChildClassName= "ML_SaveUnderChildWindowClass";

static IVector screenSize(0,0);
static IVector screenDimension(0,0); // in millimeters
static int depth= 0; // just default depth of screen

static HPALETTE hPalette= HPALETTE(0);

static HWND hRootWindow= HWND(0); // root window of the application

// for unblocking
//static Display* display2= 0;
//static Atom unblock_protocol= Atom(0);

static MLString applicationName;
static MLString iconName;

/////////////////////////////////////////////////////////////////////////////

WGraphicsSystem::WGraphicsSystem()
{
  applicationName= "";
  iconName= "";
}

/////////////////////////////////////////////////////////////////////////////

WGraphicsSystem::~WGraphicsSystem()
{}

/////////////////////////////////////////////////////////////////////////////

/*
static Cursor makeCursor( unsigned int cursorShape,
                          float r, float g, float b )
{
  if( display == 0 || screen == 0 )
    return Cursor(0);

  Cursor cursor= XCreateFontCursor( display, cursorShape );
  if( cursor == Cursor(0) )
    return cursor;

  typedef unsigned short ColorComponent;

  XColor fore;
  fore.flags= DoRed | DoGreen | DoBlue;
  fore.red=   min( ColorComponent(65535), ColorComponent(r * 65535.) );
  fore.green= min( ColorComponent(65535), ColorComponent(g * 65535.) );
  fore.blue=  min( ColorComponent(65535), ColorComponent(b * 65535.) );

  XColor back;
  back.flags= DoRed | DoGreen | DoBlue;
  back.red=   0xffff;
  back.green= 0xffff;
  back.blue=  0xffff;

  XRecolorCursor( display, cursor, &fore, &back );

  return cursor;
}
*/

/////////////////////////////////////////////////////////////////////////////

// this function is defined in WEventManager
extern long FAR PASCAL _export
WEventManager_WndProc( HWND hWnd, UINT message, DWORD wParam, LONG lParam );

boolean WGraphicsSystem::init( MLConfig* config, MLCommandLine& options )
{
  if( hInstance != 0 )
    return true; // already initialized

/*
  if( options.findOption("-override") )
    {
    // Careful: in this case, our private colormap may not be
    // correctly set by the window manager...
    ::need_overrideRedirect_windows= true;
    }
*/

  hInstance= GetModuleHandle(NULL);
  if( hInstance == 0 )
    return false;

  int rc_fullscreen;
  if( config->getIntegerVariable(rc_group,"Fullscreen",rc_fullscreen) )
    reallyFullScreen= rc_fullscreen;

#ifdef NDEBUG
  reallyFullScreen= true;
#else
  reallyFullScreen= false;
#endif

  if( options.findOption("-fs") )
    reallyFullScreen= true;

  if( options.findOption("-debug") )
    reallyFullScreen= false;

  if( ! GetSystemMetrics(SM_MOUSEPRESENT) )
    {
    // un test pour les malins
    fprintf( stderr, "What about installing a mouse? :-))\n" );
    exit(1);
    }

  // OpenGL-95 v1.0 seems to accept CS_SAVEBITS but not v1.1
  GraphicsSystem::_supportSaveUnder= false;

  screenSize= IVector( GetSystemMetrics(SM_CXSCREEN),
                       GetSystemMetrics(SM_CYSCREEN) );

  HWND hSystemRoot= GetTopWindow(0);
  HDC hRootDC= GetDC(hSystemRoot);
  screenDimension= IVector( GetDeviceCaps(hRootDC,HORZSIZE),
                            GetDeviceCaps(hRootDC,VERTSIZE) ); // millimeters
  depth= GetDeviceCaps( hRootDC, BITSPIXEL ); // depth
  ReleaseDC(hSystemRoot,hRootDC);

// GetSystemMetrics(SM_CMOUSEBUTTONS); // number of buttons

  hRootDC= HDC(0);

  if( ! reallyFullScreen )
    {
    int height= screenSize.y();
    screenSize[1]= height - 50;
    screenDimension[1]= screenDimension.y() * height / screenSize.y();
    }

  return GraphicsSystem::init(config,options);
}

/////////////////////////////////////////////////////////////////////////////

// private
boolean WGraphicsSystem::initOpenGL( HDC hDC )
{
  if( hDC == HDC(0) )
    {
    windebugf( "warning: initOpenGL(hDC=0)\n" );
    return false;
    }

  static int pixelFormat= 0;
  static PIXELFORMATDESCRIPTOR pfd;

  if( pixelFormat == 0 )
    {
    const int prefered_color_depth= 24;
    const int prefered_buffer_depth= 16;
/*
    PIXELFORMATDESCRIPTOR pfd =
      {
      sizeof(PIXELFORMATDESCRIPTOR),  // size
      1,                              // version
      PFD_SUPPORT_OPENGL |
        PFD_DRAW_TO_WINDOW |
        PFD_DOUBLEBUFFER,             // support double-buffering
      PFD_TYPE_RGBA,                  // color type
      prefered_color_depth,           // prefered color depth
      0, 0, 0, 0, 0, 0,               // color bits (ignored)
      0,                              // no alpha buffer
      0,                              // alpha bits (ignored)
      0,                              // no accumulation buffer
      0, 0, 0, 0,                     // accum bits (ignored)
      prefered_buffer_depth,          // depth buffer
      0,                              // no stencil buffer
      0,                              // no auxiliary buffers
      PFD_MAIN_PLANE,                 // main layer
      0,                              // reserved
      0, 0, 0,                        // no layer, visible, damage masks
      };
*/
    memset( (void*) &pfd, 0, sizeof(PIXELFORMATDESCRIPTOR) );

    pfd.nSize= sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion= 1;
    pfd.dwFlags= PFD_SUPPORT_OPENGL |
                 PFD_DOUBLEBUFFER |
                 PFD_DRAW_TO_WINDOW;
    pfd.iPixelType= PFD_TYPE_RGBA;
    pfd.cColorBits= prefered_color_depth;
    pfd.cDepthBits= prefered_buffer_depth;
    pfd.iLayerType= PFD_MAIN_PLANE;

    pixelFormat= ChoosePixelFormat( hDC, &pfd );
    if( pixelFormat == 0 )
      {
      windebugf( "ChoosePixelFormat() failed (hDC=%p), err=%d\n",
                 hDC, GetLastError() );
      return false;
      }
    }

  if( ! SetPixelFormat( hDC, pixelFormat, &pfd ) )
    {
    windebugf( "SetPixelFormat() failed (hDC=%p), err=%d",
               hDC, GetLastError() );
    return false;
    }

  setupPalette(hDC);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void WGraphicsSystem::setupPalette( HDC hDC )
{
  if( hDC == HDC(0) )
    {
    windebugf( "warning: setupPalette(hDC=0)\n" );
    return;
    }

  static int pixelFormat= 0;
  static PIXELFORMATDESCRIPTOR pfd;

  if( pixelFormat == 0 )
    {
    pixelFormat= GetPixelFormat(hDC);
    if( pixelFormat == 0 )
      {
      windebugf( "GetPixelFormat(hDC=%p)=0 (err=%d)\n",
                 hDC, GetLastError() );
      return;
      }

    memset( (void*) &pfd, 0, sizeof(PIXELFORMATDESCRIPTOR) );
    if( DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0 )
      {
      windebugf( "setupPalette(): DescribePixelFormat()==0 (err=%d)\n",
                 GetLastError() );
      }
    }

  if( (pfd.dwFlags & PFD_NEED_PALETTE) == 0 )
    return;

  int paletteSize = 1 << pfd.cColorBits;

  static LOGPALETTE* pPal= 0;

  if( pPal == 0 )
    {
    pPal= (LOGPALETTE*)
          malloc( sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY) );
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    // build a simple RGB color palette
    int redMask = (1 << pfd.cRedBits) - 1;
    int greenMask = (1 << pfd.cGreenBits) - 1;
    int blueMask = (1 << pfd.cBlueBits) - 1;

    for( int i= 0; i < paletteSize; ++i )
      {
      pPal->palPalEntry[i].peRed =
                  (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
      pPal->palPalEntry[i].peGreen =
                  (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
      pPal->palPalEntry[i].peBlue =
                  (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
      pPal->palPalEntry[i].peFlags = 0;
      }
    }

  hPalette= CreatePalette(pPal);

  if( hPalette != HPALETTE(0) )
    {
    SelectPalette( hDC, hPalette, FALSE );
    RealizePalette( hDC );
    }
}

/////////////////////////////////////////////////////////////////////////////

// private
boolean WGraphicsSystem::initDIBOpenGL( HDC hDC )
{
  if( hDC == HDC(0) )
    {
    windebugf( "warning: initDIBOpenGL(hDC=0)\n" );
    return false;
    }

  static int pixelFormat= 0;
  static PIXELFORMATDESCRIPTOR pfd;

  if( pixelFormat == 0 )
    {
    const int prefered_color_depth= GetDeviceCaps( hDC, BITSPIXEL );
    const int prefered_buffer_depth= 16;

    memset( (void*) &pfd, 0, sizeof(PIXELFORMATDESCRIPTOR) );

    pfd.nSize= sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion= 1;
    pfd.dwFlags= PFD_SUPPORT_OPENGL |
                 PFD_SUPPORT_GDI |
                 PFD_DRAW_TO_BITMAP;
    pfd.iPixelType= PFD_TYPE_RGBA;
    pfd.cColorBits= prefered_color_depth;
    pfd.cDepthBits= prefered_buffer_depth;
    pfd.iLayerType= PFD_MAIN_PLANE;

    pixelFormat= ChoosePixelFormat( hDC, &pfd );
    if( pixelFormat == 0 )
      {
      windebugf( "ChoosePixelFormat() for DIB failed (err=%d).\n",
                 GetLastError() );
      return false;
      }
    }

  if( ! SetPixelFormat( hDC, pixelFormat, &pfd ) )
    {
    windebugf( "SetPixelFormat() for DIB failed. (err=%d)\n",
               GetLastError() );
    return false;
    }

  setupDIBPalette(hDC);

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void WGraphicsSystem::setupDIBPalette( HDC hDC )
{
  if( hDC == HDC(0) )
    {
    windebugf( "warning: setupPalette(hDC=0)\n" );
    return;
    }

  static int pixelFormat= 0;
  static PIXELFORMATDESCRIPTOR pfd;

  if( pixelFormat == 0 )
    {
    pixelFormat= GetPixelFormat(hDC);
    if( pixelFormat == 0 )
      {
      windebugf( "GetPixelFormat(hDC=%p)=0 (err=%d)\n",
                 hDC, GetLastError() );
      return;
      }

    memset( (void*) &pfd, 0, sizeof(PIXELFORMATDESCRIPTOR) );
    if( DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0 )
      {
      windebugf( "setupPalette(): DescribePixelFormat()==0 (err=%d)\n",
                 GetLastError() );
      }
    }

  if( (pfd.dwFlags & PFD_NEED_PALETTE) == 0 )
    return;

  int paletteSize = 1 << pfd.cColorBits;

  static RGBQUAD* rgb= 0;

  if( rgb == 0 )
    {
    rgb= new RGBQUAD [ paletteSize ];

    // build a simple RGB color palette
    int redMask = (1 << pfd.cRedBits) - 1;
    int greenMask = (1 << pfd.cGreenBits) - 1;
    int blueMask = (1 << pfd.cBlueBits) - 1;

    for( int i= 0; i < paletteSize; ++i )
      {
      rgb[i].rgbRed =
                  (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
      rgb[i].rgbGreen =
                  (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
      rgb[i].rgbBlue =
                  (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
      rgb[i].rgbReserved= 0;
      }
    }
/*
  hPalette= CreatePalette(pPal);

  if( hPalette != HPALETTE(0) )
    {
    SelectPalette( hDC, hPalette, FALSE );
    RealizePalette( hDC );
    }
*/
  SetDIBColorTable( hDC, 0, paletteSize, rgb );
}

/////////////////////////////////////////////////////////////////////////////

void WGraphicsSystem::_iconify( boolean yes )
{
  SystemWindow* root= getRootWindow();
  if( root == 0 )
    return;

  WWindowDriver* wd= (WWindowDriver*) (root->getDriver());
  if( wd == 0 )
    return;

  HWND hWnd= wd->getWindowID();
  boolean current_iconfy_state= IsIconic(hWnd);

  if( ! yes )
    {
    if( (yes?1:0) != (current_iconfy_state?1:0) )
      ShowWindow( hWnd, SW_RESTORE );
    root->currentPixel();
    glDrawBuffer(GL_FRONT);
    root->clear(0,0,0);
    glFlush();
    GraphicsSystem::refreshScreen();
    }
   else
    {
    if( (yes?1:0) != (current_iconfy_state?1:0) )
      ShowWindow( hWnd, SW_MINIMIZE );
    }
}

/////////////////////////////////////////////////////////////////////////////

const MLString& WGraphicsSystem::getName()
{
  static MLString name("Windows");
  return name;
}

/////////////////////////////////////////////////////////////////////////////

IVector WGraphicsSystem::getScreenSize() const
{
  return screenSize;
}

/////////////////////////////////////////////////////////////////////////////

HINSTANCE WGraphicsSystem::getInstance() const
{
  return hInstance;
}

/////////////////////////////////////////////////////////////////////////////

int WGraphicsSystem::getDepth() const
{
  return depth;
}

/////////////////////////////////////////////////////////////////////////////

void WGraphicsSystem::autoRepeat( boolean /*yes*/ /* = true */ )
{
/*
  if( yes )
    XAutoRepeatOn(display);
   else
    XAutoRepeatOff(display);
  XSync(display,False);
*/
}

/////////////////////////////////////////////////////////////////////////////

void WGraphicsSystem::emitBeep()
{
  MessageBeep( MB_ICONEXCLAMATION );
// MB_ICONWARNING
// MB_ICONQUESTION
// MB_OK
}

/////////////////////////////////////////////////////////////////////////////

IVector WGraphicsSystem::getScreenDimemsion() const
{
  return screenDimension;
}

/////////////////////////////////////////////////////////////////////////////

/*
Display* WGraphicsSystem::getDisplay() const
{
  assert( display != 0 );
  return display;
}
*/

/////////////////////////////////////////////////////////////////////////////

/*
XVisualInfo* XGraphicsSystem::getVisualInfo() const
{
  assert( visualInfo != 0 );
  return visualInfo;
}
*/

/////////////////////////////////////////////////////////////////////////////

// parent can be NULL for root window
HWND WGraphicsSystem::createWindow( HWND parent,
                                    int x, int y, int width, int height,
                                    boolean saveUnder )
{
  if( hInstance == HANDLE(0) )
    return HWND(0);

  boolean root= false;
  if( parent == HWND(0) )
    {
    parent= 0;
    root= true;
    }

  if( aMainRegister == ATOM(0) )
    {
    // it's for main application window

    WNDCLASS wndclass;
    wndclass.style         = 0; //CS_HREDRAW | CS_VREDRAW | // redraw if size changed
                             //CS_NOCLOSE | // Disable the close cmd in sys menu
                             //0/*CS_PARENTDC*/;
    wndclass.lpfnWndProc   = (WNDPROC)WEventManager_WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = 0;//LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = 0;//GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = (LPCSTR)lpszMainClassName;

    aMainRegister= RegisterClass(&wndclass);

    if( aMainRegister == ATOM(0) )
      {
      windebugf( "Unable to register main window class\n" );
      return HWND(0);
      }
    }

  if( aChildRegister == ATOM(0) )
    {
    // it's for child windows

    WNDCLASS wndclass;
    wndclass.style         = 0/*CS_PARENTDC*/;
    wndclass.lpfnWndProc   = (WNDPROC)WEventManager_WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = NULL;
    wndclass.hCursor       = 0;//LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = NULL; //GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = (LPCSTR)lpszChildClassName;

    aChildRegister= RegisterClass(&wndclass);

    if( aChildRegister == ATOM(0) )
      {
      windebugf( "Unable to register child window class\n" );
      return HWND(0);
      }
    }

  if( aSaveUnderChildRegister == ATOM(0) )
    {
    // it's for save-under child windows

    WNDCLASS wndclass;
    wndclass.style         = /*CS_PARENTDC |*/ CS_SAVEBITS;
    wndclass.lpfnWndProc   = (WNDPROC)WEventManager_WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = NULL;
    wndclass.hCursor       = 0;//LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = NULL; //GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = (LPCSTR)lpszSaveUnderChildClassName;

    aSaveUnderChildRegister= RegisterClass(&wndclass);

    if( aSaveUnderChildRegister == ATOM(0) )
      {
      windebugf( "Unable to register save-under child window class\n" );
      return HWND(0);
      }
    }

  DWORD dwStyle= DWORD(0);
  if( root )
    {
    dwStyle= WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }
   else
    {
    dwStyle= WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }

  const char* className= root ? lpszMainClassName : lpszChildClassName;

  if( saveUnder )
    {
    className= lpszSaveUnderChildClassName;
    dwStyle |= WS_POPUP;
    }
   else if( !root)
    dwStyle |= WS_CHILD;

  HWND windowID= CreateWindow( className,
                               root ? "Moonlight" : "",
                               dwStyle,
                               x, y, width, height,
                               parent, // HWND hWndParent
                               NULL, // HMENU hMenu
                               hInstance, // HINSTANCE hInstance
                               NULL // LPVOID lpParam
                             );

  if( windowID == HWND(0) )
    return HWND(0); // failed

  if( root )
    {
/*
    HMENU hMenu= GetSystemMenu(windowID,FALSE);
    if( hMenu != 0 )
      DestroyMenu(hMenu);
*/

    // Tell mwm to remove decoration
    // No matter if we're not running mwm
/*
    extern void set_mwm_border( Display *dpy, Window w );
    set_mwm_border(display,windowID);

    // force size and position of root window
    XSizeHints hints;
    hints.x= 0;               hints.y= 0;
    hints.width= width;       hints.height= height;
    hints.max_width= width;   hints.max_height = height;
    hints.min_width= width;   hints.min_height = height;
    hints.flags = USPosition | USSize | PPosition | PMinSize | PMaxSize;

    XSetStandardProperties( display, windowID,
                            0,//applicationName.get(), //window_name
                            "Moonlight", //iconName.get(), // icon_name
                            None, // icon_pixmap
                            NULL, 0, // argv, argc
                            &hints );
*/

    ShowWindow( windowID, SW_SHOWNORMAL );

/*
    // SetWindowPos
    XWindowChanges xwc;
    xwc.x= 0; xwc.y= 0;
    xwc.width= width; xwc.height= height;

    XConfigureWindow( display, windowID,
                      CWX | CWY | CWWidth | CWHeight, &xwc );
*/
    }

  GdiFlush();
  glFlush();

  if( root )
    hRootWindow= windowID;

  return windowID;
}

/////////////////////////////////////////////////////////////////////////////

HWND WGraphicsSystem::getRootWindowID() const
{
  return hRootWindow;
}

/////////////////////////////////////////////////////////////////////////////

HPALETTE WGraphicsSystem::getColormap() const
{
  return hPalette;
}

/////////////////////////////////////////////////////////////////////////////

WindowDriver* WGraphicsSystem::newWindowDriver( SystemWindow* sw ) const
{
  return new WWindowDriver(sw);
}

/////////////////////////////////////////////////////////////////////////////

MLGLPixmap* WGraphicsSystem::newMLGLPixmap() const
{
  return new WMLGLPixmap;
}

/////////////////////////////////////////////////////////////////////////////

MLCursor* WGraphicsSystem::newMLCursor() const
{
  return new WMLCursor;
}

/////////////////////////////////////////////////////////////////////////////

// where is the cursor now?
// returned in MLEvent::window, fills: MLEvent::position, MLEvent::state
MLWindow* WGraphicsSystem::getCursorWindow( MLEvent* event )
{
  POINT point;
  if( ! GetCursorPos(&point) )
    return 0;

  if( event != 0 )
    {
    event->window= 0;
    event->state= 0;
    if( GetKeyState(VK_LBUTTON) < 0 )
      event->state |= int(MLEvent::LEFT_BUTTON);
    if( GetKeyState(VK_MBUTTON) < 0 )
      event->state |= int(MLEvent::MIDDLE_BUTTON);
    if( GetKeyState(VK_RBUTTON) < 0 )
      event->state |= int(MLEvent::RIGHT_BUTTON);

    if( GetKeyState(VK_SHIFT) < 0 )
      event->state |= int(MLEvent::SHIFT);
    if( GetKeyState(VK_CONTROL) < 0 )
      event->state |= int(MLEvent::CONTROL);
//      if( (event->xkey.state & Mod1Mask) != 0 )
//        e->state |= int(MLEvent::ALT);
    }

  MLWindow* w= GraphicsSystem::findCursorWindow( IPoint(point.x,point.y) );

  if( event != 0 )
    {
    event->window= w;
    if( w != 0 )
      {
      IPoint p= w->getAbsPosition();
      event->position= IPoint( int(point.x) - p.x(),
                               int(point.y) - p.y() );
      }
    }

  return w;
}

/////////////////////////////////////////////////////////////////////////////

void WGraphicsSystem::setApplicationName( const char* /*name*/ )
{
/*
  if( name == 0 )
    return;

  ::applicationName= name;
  ::iconName= name;

  XSetStandardProperties( display,
                          xRootWindow,
                          name, // window name
                          name, // icon name
                          None,  // icon pixmap
                          0, 0,  // argv, argc
                          0 );   // &hints

  static MLString __name;
  __name= MLString(name);
  static char* _name;
  _name= __name._get();

  XClassHint* classhints= XAllocClassHint();
  classhints->res_name= _name;
  classhints->res_class= _name;

  if( titleProperty.value != 0 )
    {
    XFree( titleProperty.value );
    titleProperty.value= 0;
    }
  if( XStringListToTextProperty( &_name, 1, &titleProperty ) == 0 )
    {
    // error
    titleProperty.value= 0;
    return;
    }

  if( iconnameProperty.value != 0 )
    {
    XFree( iconnameProperty.value );
    iconnameProperty.value= 0;
    }
  if( XStringListToTextProperty( &_name, 1, &iconnameProperty ) == 0 )
    {
    // error
    iconnameProperty.value= 0;
    return;
    }

  XSetWMProperties( display, xRootWindow,
                    &titleProperty, &iconnameProperty,
                    0, 0, // argv, argc,
                    0, // sizehints
                    0, // wmhints
                    classhints  // classhints
                  );
  XSetWMIconName( display, xRootWindow, &iconnameProperty );
*/
}

/////////////////////////////////////////////////////////////////////////////

