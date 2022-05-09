/* mwmborder.c */


/*
 * This function shows how to remove the border, title bar, resize button,
 * etc from a Motif window frame from inside an Xlib-based application.
 *
 * Brian Paul  19 Sep 1995   brianp@ssec.wisc.edu
 * 
 * disabled some of them for now as problems arose from them
 * 
 * Patrick Mauritz, 2002 
 */

#include <X11/Xlib.h>
#include <X11/Xatom.h>

/* bit definitions for MwmHints.flags */
#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_INPUT_MODE    (1L << 2)
#define MWM_HINTS_STATUS        (1L << 3)

/* bit definitions for MwmHints.decorations */
#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)

typedef struct
{
    unsigned long       flags;
    unsigned long       functions;
    unsigned long       decorations;
    long                inputMode;
    unsigned long       status;
} PropMotifWmHints;

#define PROP_MOTIF_WM_HINTS_ELEMENTS    5

/*
 * Remove all window decorations.
 *
 * Input:  dpy - the X display
 *         w - the X window
 */
void set_mwm_border( Display *dpy, Window w )
{
  Atom wm_hints;
  
  // set MWM hints
 
  PropMotifWmHints mwm_hints = { MWM_DECOR_BORDER, 0, 0, 0, 0 };
  
  wm_hints = XInternAtom( dpy, "_MOTIF_WM_HINTS", True );
  if( wm_hints != None )
  {
    XChangeProperty( dpy, w,
                     wm_hints, wm_hints, 32,
                     PropModeReplace,
                     (unsigned char*)&mwm_hints,
                     PROP_MOTIF_WM_HINTS_ELEMENTS );
  }

  // set KWM hints

  long kwm_hints = 0;
  
  wm_hints = XInternAtom( dpy, "KWM_WIN_DECORATION", True );
  if( wm_hints != None )
  {
    XChangeProperty( dpy, w,
                     wm_hints, wm_hints, 32,
                     PropModeReplace,
                     (unsigned char*)&kwm_hints,
                     1 );
  }
  
  // set GNOME hints

  long gnome_hints = 0;
  
  wm_hints = XInternAtom( dpy, "_WIN_HINTS", True );
  if( wm_hints != None )
  {
    XChangeProperty( dpy, w,
                     wm_hints, wm_hints, 32,
                     PropModeReplace,
                     (unsigned char*)&kwm_hints,
                     1 );
  }

  // If everithing fails, finally set WM_TRANSIENT_FOR hint
  // for this window in respect to root, but at the moment
  // it is much too much hassle to extract root Window from
  // XGraphicsSystem, and those hints should cover about
  // all modern window managers anyway.
}  

