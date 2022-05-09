// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  XGraphicsSystem.C

  X Graphics System Class

  Stephane Rehel
  June 28, 1996
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/opengl.h"
#include "GL/glx.h"

#include "tools/MLCommandLine.h"
#include "tools/Chrono.h"
#include "tools/MLConfig.h"

#include "XGraphicsSystem.h"
#include "XWindowDriver.h"
#include "XMLGLPixmap.h"
#include "XMLCursor.h"

#include "SystemWindow.h"
#include "MLEvent.h"

#include <X11/cursorfont.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>

/////////////////////////////////////////////////////////////////////////////

//static boolean need_overrideRedirect_windows= false;

/////////////////////////////////////////////////////////////////////////////

static Display* display= 0;
static MLString displayName("");
static IVector screenSize(0,0);
static IVector screenDimension(0,0); // in millimeters
static int depth= 0; // just default depth of screen

static Screen* screen= 0;
static int screen_number= 0;
static Colormap colormap= Colormap(0);
static XVisualInfo* visualInfo= 0;
static Window xRootWindow= Window(0);

// for unblocking
//static Display* display2= 0;
//static Atom unblock_protocol= Atom(0);

static MLString applicationName;
static MLString iconName;
static XTextProperty titleProperty;
static XTextProperty iconnameProperty;

// for handling HP CR (HP color recovery)
static Atom     hpCRAtom = Atom(0xffffffff);
static Colormap hpCRMap  = Colormap(0);

/////////////////////////////////////////////////////////////////////////////

static int ErrorHandler( Display* display, XErrorEvent* event )
{
/*
  char buf[256];

  fprintf( stderr, "\nReceived X error!\n" );
  fprintf( stderr, "\tError code   : %d\n", event->error_code );
  fprintf( stderr, "\tRequest code : %d\n", event->request_code );
  fprintf( stderr, "\tMinor code   : %d\n", event->minor_code );
  XGetErrorText( xDisplay, event->error_code, buf, 255 );
  fprintf( stderr, "\tError text : '%s'\n\n", buf );
  return 0;
*/
  // in case the error occurred during the Grab command...
  XUngrabServer(display);
  XUngrabButton(display, (u_int) AnyButton, 0, xRootWindow );

  int xerrcode = event->error_code;

  // non-fatal errors:   (sets xerrcode and returns)
  //    BadAlloc
  //    BadAccess errors on XFreeColors call
  //    Any error on the 'XKillClient()' call
  //    BadWindow errors (on a GetProperty call) (workaround SGI problem)
  //    BadLength errors on XChangeProperty
  //    BadMatch  errors on XGetImage
  //

  int req= event->request_code;

  if( xerrcode==BadMatch && req==X_SetInputFocus ) return 0;

/*
  if( (xerrcode == BadAlloc)              ||
      (xerrcode == BadAccess && req==88 ) || // X_FreeColors
      (                         req==113) || // X_KillClient
      (xerrcode == BadLength && req==18 ) || // X_ChangeProp
      (xerrcode == BadMatch  && req==73 ) || // X_GetImage
      (xerrcode == BadWindow && req==20 ) )  // X_GetProperty
    return 0;
*/

  // all other errors are 'fatal'
  fprintf( stderr, "\nReceived X error!\n" );
  fprintf( stderr, "\t  Error code : %d\n", xerrcode );
  fprintf( stderr, "\tRequest code : %d\n", req );
  fprintf( stderr, "\t  Minor code : %d\n", event->minor_code );
  char buf[256];
  XGetErrorText( display, xerrcode, buf, 255 );
  fprintf( stderr, "\t  Error text : '%s'\n\n", buf );

/*
#ifndef NDEBUG
  // crash 'n' burn for debugging purposes
  XAutoRepeatOn(display);
  char *str;
  str  = NULL;
  *str = '0';
  exit(-1);
#endif
*/

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

XGraphicsSystem::XGraphicsSystem()
{
  applicationName= "";
  iconName= "";
}

/////////////////////////////////////////////////////////////////////////////

XGraphicsSystem::~XGraphicsSystem()
{
  if( display != 0 )
    {
    XCloseDisplay(display);
    display= 0;
    }
}

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

boolean XGraphicsSystem::init( MLConfig* config, MLCommandLine& options )
{
  if( display != 0 )
    return true; // already initialized

/*
  if( options.findOption("-override") )
    {
    // Careful: in this case, our private colormap may not be
    // correctly set by the window manager...
    ::need_overrideRedirect_windows= true;
    }
*/

  displayName= options.getStringAfter("-display").get();

  display= XOpenDisplay( (displayName.length() == 0) ? 0 : displayName.get() );

  if( display == 0 )
    {
    if( displayName.length() == 0 )
      fprintf( stderr, "Error: unable to open default display\n" );
     else
      fprintf( stderr, "Error: unable to open display `%s'\n",
               displayName.get() );
    return false;
    }

#ifndef NDEBUG
  XSynchronize(display, (NULL==getenv("ML_XSYNCHRONIZE"))?0:1);
#endif

  int erb, evb;
  if( !glXQueryExtension( display, &erb, &evb ) )
    {
    fprintf( stderr, "Fatal: no GLX extension in display %s\n",
             displayName.length() ? displayName.get() : ":0" );
    return false;
    }

  XSetErrorHandler(ErrorHandler);

//  display2= XOpenDisplay( (displayName.length() == 0) ? 0 : displayName.get() );
//  if( display2 != 0 )
//    unblock_protocol= XInternAtom( display2, "unblock_ml", False );

  // set screen
  if( ! initOpenGL() )
    return false;

  GraphicsSystem::_supportSaveUnder= XDoesSaveUnders(screen);

  int rc_fullscreen;
  if( config->getIntegerVariable(rc_group.get(),"Fullscreen",rc_fullscreen) )
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

  screenSize= IVector( WidthOfScreen(screen),
                       HeightOfScreen(screen) );
  screenDimension= IVector( WidthMMOfScreen(screen),
                            HeightMMOfScreen(screen) );

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
boolean XGraphicsSystem::initOpenGL()
{
  if( display == 0 )
    return false;

  int gl_attrib[] =
  {
   GLX_LEVEL, 0,
   GLX_DOUBLEBUFFER,
   GLX_RGBA,
   GLX_RED_SIZE, 1,
   GLX_GREEN_SIZE, 1,
   GLX_BLUE_SIZE, 1,
   GLX_DEPTH_SIZE, 1,
   None
  };

  visualInfo= glXChooseVisual( display,
                               DefaultScreen(display),
                               gl_attrib );
  if(!visualInfo)
    {
    fprintf( stderr, "Couldn't find visual for OpenGL RGB mode!\n" );
    int generic_attributes [] = {GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 1, None};
    if (!(visualInfo=glXChooseVisual(display,DefaultScreen(display),generic_attributes)))
       {
         fprintf( stderr, "Display doesn't support double buffer and depth buffer");
	 int doublebuffer_attributes[] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
	 if (!(visualInfo=glXChooseVisual(display,DefaultScreen(display),doublebuffer_attributes)))
	    {
	    fprintf( stderr, "Display doesn't support double buffer");
	    int depthbuffer_attributes [] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
	    if (!(visualInfo=glXChooseVisual(display,DefaultScreen(display),depthbuffer_attributes)))
	       {
	       fprintf( stderr, "Display doesn't support depth buffer");
	       }
	    }
       }
    return false;
    }

  screen_number= visualInfo->screen;
  screen= ScreenOfDisplay( display, screen_number );
  depth= visualInfo->depth;

  // RGB colormap is AllocNone, share the root colormap if possible
  int scrnum= DefaultScreen(display);
  xRootWindow= RootWindowOfScreen(screen);

  boolean private_colormap= true;

  if( MaxCmapsOfScreen(screen) == 1 &&
      visualInfo->visual == DefaultVisual( display, scrnum ) )
    {
    // the window and root are of the same visual type
    if( private_colormap )
      {
      // user doesn't want to share colormaps
      colormap= XCreateColormap( display, xRootWindow,
                                 visualInfo->visual,
                                 AllocNone );
      }
     else
      {
      // share the root colormap
      colormap= DefaultColormap( display, scrnum );
      }
    }
   else
    {
    // Contributed by <jle@star.be>
    if( hpCRAtom == Atom(0xffffffff) )
      {
      const char* vendor= ServerVendor(display);
      if( strcmp( vendor, "Hewlett-Packard" ) == 0 )
        hpCRAtom= XInternAtom(display, "_HP_RGB_SMOOTH_MAP_LIST", True);
       else
        hpCRAtom= None;

      if( hpCRAtom != None )
        {
        Status status;
        XStandardColormap *stdCmaps;
        int numCmaps;

        status= XGetRGBColormaps( display, DefaultRootWindow(display),
                                  &stdCmaps, &numCmaps, hpCRAtom );
        if( status == 1 )
          {
          int i;
          for( i=0; i<numCmaps; i++ )
            {
            if( stdCmaps[i].visualid == visualInfo->visual->visualid )
              {
              hpCRMap= stdCmaps[i].colormap;
              break;
              }
            }
          }
         else
          hpCRAtom= None;
        }
      }

    if( hpCRAtom != None  )
      colormap= hpCRMap;
     else
      {
      // window and root are different visual types, allocate new cmap
      if( visualInfo->c_class == DirectColor )
        {
        colormap= XCreateColormap( display, xRootWindow,
                                   visualInfo->visual,
                                   AllocAll );
        }
       else
        {
        colormap= XCreateColormap( display, xRootWindow,
                                   visualInfo->visual,
                                   AllocNone );
        }
      }
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void XGraphicsSystem::_iconify( boolean yes )
{
  SystemWindow* root= getRootWindow();
  if( root == 0 )
    return;

  XWindowDriver* wd= (XWindowDriver*) (root->getDriver());
  if( wd == 0 )
    return;

  Window rootID= wd->getWindowID();
  if( ! yes )
    {
    XMapWindow( display, rootID );

    root->currentPixel();
    glDrawBuffer(GL_FRONT);
    root->clear(0,0,0);
    glFlush();
    GraphicsSystem::doRedrawEvents();
    GraphicsSystem::refreshScreen();
    }
   else
    {
    XIconifyWindow( display, rootID, screen_number );
    }
}

/////////////////////////////////////////////////////////////////////////////

const MLString& XGraphicsSystem::getName()
{
  static MLString name("X11");
  return name;
}

/////////////////////////////////////////////////////////////////////////////

IVector XGraphicsSystem::getScreenSize() const
{
  return screenSize;
}

/////////////////////////////////////////////////////////////////////////////

int XGraphicsSystem::getDepth() const
{
  return depth;
}

/////////////////////////////////////////////////////////////////////////////

void XGraphicsSystem::autoRepeat( boolean yes /* = true */ )
{
  if( display == 0 )
    return;

  if( yes )
    XAutoRepeatOn(display);
   else
    XAutoRepeatOff(display);
  XSync(display,False);
}

/////////////////////////////////////////////////////////////////////////////

void XGraphicsSystem::emitBeep()
{
  if( display == 0 )
    return;

  XBell(display,50);
}

/////////////////////////////////////////////////////////////////////////////

IVector XGraphicsSystem::getScreenDimemsion() const
{
  return screenDimension;
}

/////////////////////////////////////////////////////////////////////////////

Display* XGraphicsSystem::getDisplay() const
{
  assert( display != 0 );
  return display;
}

/////////////////////////////////////////////////////////////////////////////

XVisualInfo* XGraphicsSystem::getVisualInfo() const
{
  assert( visualInfo != 0 );
  return visualInfo;
}

/////////////////////////////////////////////////////////////////////////////

// parent can be NULL for root window
Window XGraphicsSystem::createWindow( Window parent,
                                      int x, int y, int width, int height,
                                      boolean saveUnder )
{
  if( display == 0 || colormap == Colormap(0) )
    return Window(0);

  boolean root= false;
  if( parent == Window(0) )
    {
    parent= xRootWindow;
    root= true;
    }

  XSetWindowAttributes attr;
  unsigned long valuemask(0);

  // for root, this will be done just later
  if( ! root || hpCRMap != Colormap(0) )
    {
    valuemask |= CWColormap;
    attr.colormap= colormap;
    }

  // required for SGI OpenGL
  valuemask |= CWBorderPixel;
  attr.border_pixel= BlackPixelOfScreen(screen);

  if( saveUnder && GraphicsSystem::_supportSaveUnder )
    {
    valuemask |= CWSaveUnder;
    attr.save_under= True;
    }

/*
  if( need_overrideRedirect_windows )
    {
    valuemask |= CWOverrideRedirect;
    attr.override_redirect= True;
    }
*/

//  if( root )
//    {
//    // it's the application root window
//    valuemask |= CWBackPixel;
//    attr.background_pixel= BlackPixelOfScreen(screen);
//    }

//  valuemask |= CWCursor;
//  attr.cursor= defaultCursor;

  Window windowID;
  windowID= XCreateWindow( display,
                           parent,
                           x, y, width, height,
                           0, // border width
                           depth,
                           InputOutput,
                           visualInfo->visual,
                           valuemask, // valuemask
                           &attr      // XSetWindowAttributes*
                         );

  if( windowID == Window(0) )
    return windowID;

  if( root )
    {
    // Tell mwm to remove decoration
    // No matter if we're not running mwm
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

//XSetWMNormalHints( display, windowID, &hints );
    XMapWindow(display,windowID);

    // SetWindowPos
    XWindowChanges xwc;
    xwc.x= 0; xwc.y= 0;
    xwc.width= width; xwc.height= height;

    XConfigureWindow( display, windowID,
                      CWX | CWY | CWWidth | CWHeight, &xwc );

//    XWMHints* wmh= XAllocWMHints();
//    wmh->flags= 0;
//    XSetWMHints(display,windowID,wmh);
    }

//  XSync(display,False);

  if( root && hpCRMap == Colormap(0) )
    {
    // ok, window is black, now set the colormap
    // (this avoids flashing init screen)
    XSetWindowColormap( display, windowID, colormap );
    }

  long event_mask= ExposureMask |
                   ButtonPressMask | ButtonReleaseMask |
                   KeyPressMask | KeyReleaseMask |
                   ButtonMotionMask |
PointerMotionMask;
//                   EnterWindowMask | LeaveWindowMask ;
  if( root )
    event_mask |= StructureNotifyMask;

  XSelectInput( display, windowID, event_mask );

/*
if( ::need_overrideRedirect_windows )
{
// test it...
XSetWindowColormap( display, windowID, colormap );
XSetWMColormapWindows( display, windowID, &windowID, 1 );
}
*/

  // why not?
  // because it breaks
//  Atom atom_list[1];
//  atom_list[0]= XInternAtom( getDisplay(), "WM_DELETE_WINDOW", False );
//  if( atom_list[0] != None )
//    XSetWMProtocols( display, windowID, atom_list, 1 );

  return windowID;
}

/////////////////////////////////////////////////////////////////////////////

Window XGraphicsSystem::getRootWindowID() const
{
  if( rootWindow == 0 )
    return Window(0);

  XWindowDriver* wd= (XWindowDriver*) (rootWindow->getDriver());
  if( wd == 0 )
    return Window(0);

  return wd->getWindowID();
}

/////////////////////////////////////////////////////////////////////////////

Window XGraphicsSystem::getXRootWindowID() const
{
  return xRootWindow;
}

/////////////////////////////////////////////////////////////////////////////

Colormap XGraphicsSystem::getColormap() const
{
  return colormap;
}

/////////////////////////////////////////////////////////////////////////////

WindowDriver* XGraphicsSystem::newWindowDriver( SystemWindow* sw ) const
{
  return new XWindowDriver(sw);
}

/////////////////////////////////////////////////////////////////////////////

MLGLPixmap* XGraphicsSystem::newMLGLPixmap() const
{
  return new XMLGLPixmap;
}

/////////////////////////////////////////////////////////////////////////////

MLCursor* XGraphicsSystem::newMLCursor() const
{
  return new XMLCursor;
}

/////////////////////////////////////////////////////////////////////////////

// where is the cursor now?
// returned in MLEvent::window, fills: MLEvent::position, MLEvent::state
MLWindow* XGraphicsSystem::getCursorWindow( MLEvent* event )
{
  Window rootid= getRootWindowID();
  if( rootid == Window(0) )
    return 0;

  unsigned int mask_return= 0;
  Window root_return= Window(0);
  Window child_return= Window(0);
  int root_x_return= 0, root_y_return= 0;
  int win_x_return= 0, win_y_return= 0;
  Bool status= XQueryPointer( getDisplay(),
                              rootid,
                              &root_return, &child_return,
                              &root_x_return, &root_y_return,
                              &win_x_return, &win_y_return,
                              &mask_return );
  if( status != True )
    return 0;

  if( event != 0 )
    {
    event->window= 0;
    event->state= 0;
    if( (mask_return & Button1Mask) != 0 )
      event->state |= int(MLEvent::LEFT_BUTTON);
    if( (mask_return & Button2Mask) != 0 )
      event->state |= int(MLEvent::MIDDLE_BUTTON);
    if( (mask_return & Button3Mask) != 0 )
      event->state |= int(MLEvent::RIGHT_BUTTON);

    if( (mask_return & ShiftMask) != 0 )
      event->state |= int(MLEvent::SHIFT);
    if( (mask_return & ControlMask) != 0 )
      event->state |= int(MLEvent::CONTROL);
    if( (mask_return & Mod1Mask) != 0 )
      event->state |= int(MLEvent::ALT);
    }

  MLWindow* w= GraphicsSystem::findCursorWindow( IPoint( root_x_return,
                                                         root_y_return ) );
  if( event != 0 )
    {
    event->window= w;
    if( w != 0 )
      {
      IPoint p= w->getAbsPosition();
      event->position= IPoint( root_x_return - p.x(),
                               root_y_return - p.y() );
      }
    }

  return w;
}

/////////////////////////////////////////////////////////////////////////////

// for the XEventManager::unblockWaiting() function
/*
Display* XGraphicsSystem::getDisplay2() const
{
  return display2;
}
*/

/////////////////////////////////////////////////////////////////////////////

// for the XEventManager::unblockWaiting() function
/*
Atom XGraphicsSystem::getUnblockProtocol() const
{
  return unblock_protocol;
}
*/

/////////////////////////////////////////////////////////////////////////////

void XGraphicsSystem::setApplicationName( const char* name )
{
  if( name == 0 )
    return;

  ::applicationName= name;
  ::iconName= name;

  Window rootID= ((XWindowDriver*)(getRootWindow()->getDriver()))
                ->getWindowID();

  XSetStandardProperties( display,
                          rootID,
                          name, // window name
                          name, // icon name
                          None,  // icon pixmap
                          0, 0,  // argv, argc
                          0 );   // &hints
//printf("coucou!!! <%s>\n",name);

  static MLString __windowName;
  static MLString __iconName;
  __windowName= MLString(::applicationName);
  __iconName= MLString(::iconName);
  char* _windowName= __windowName.getBytes();
  char* _iconName= __iconName.getBytes();

  // name & class hints don't allow spaces
  // from: Marcelo E. Magallon <mmagallo@efis.ucr.ac.cr>
  int i;
  for( i= 0; i < __windowName.length(); ++i )
    if( _windowName[i] == ' ' )
      _windowName[i]= '_';

  XClassHint* classhints= XAllocClassHint();
  classhints->res_name= _windowName;
  classhints->res_class= _windowName;

  if( titleProperty.value != 0 )
    {
    XFree( titleProperty.value );
    titleProperty.value= 0;
    }
  if( XStringListToTextProperty( &_windowName, 1, &titleProperty ) == 0 )
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
  if( XStringListToTextProperty( &_iconName, 1, &iconnameProperty ) == 0 )
    {
    // error
    iconnameProperty.value= 0;
    return;
    }

  XSetWMProperties( display, rootID,
                    &titleProperty, &iconnameProperty,
                    0, 0, // argv, argc,
                    0, // sizehints
                    0, // wmhints
                    classhints  // classhints
                  );
  XSetWMName( display, rootID, &titleProperty );
  XSetWMIconName( display, rootID, &iconnameProperty );

  XSetStandardProperties( display, rootID,
                          _windowName,
                          _iconName,
                          Pixmap(0), // icon
                          0, 0, // argv, argc
                          0 // XSizeHints
                        );
}

/////////////////////////////////////////////////////////////////////////////

