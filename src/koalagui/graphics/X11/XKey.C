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
   XKey.C

   Stephane Rehel
   June 29, 1996
*/

#include <assert.h>

#include "Key.h"
#include "KeyCodes.h"
#include "XKey.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#ifdef __LINUX__
#define __FRENCH_KEYBOARD__
#endif

/////////////////////////////////////////////////////////////////////////////

struct KEYTRANSLATION
{
  Key translated;
  KeySym initial;
};

static KEYTRANSLATION* keysTranslation= 0;
static int nTranslations= 0;

/////////////////////////////////////////////////////////////////////////////

static void initXKeysTranslations()
{
  assert( keysTranslation == 0 );

  for( int pass= 0; pass <= 1; ++pass )
  {
  int i= 0;
  if( pass == 0 )
    nTranslations= 0;
   else
    {
    keysTranslation= new KEYTRANSLATION [ nTranslations ];
    }

#define s(tr,in) { \
                 if( pass == 0 ) \
                   ++nTranslations; \
                  else \
                   { \
                   assert( i < nTranslations ); \
                   keysTranslation[i].translated=(tr); \
                   keysTranslation[i].initial=(in); \
                   ++i; \
                   } \
                 }

 s( KeyCodes::CONTROL ,  XK_Control_R )
 s( KeyCodes::CONTROL ,  XK_Control_L )
 s( KeyCodes::SHIFT   ,  XK_Shift_R )
 s( KeyCodes::SHIFT   ,  XK_Shift_L )
 s( KeyCodes::ALT     ,  XK_Alt_L )
 s( KeyCodes::ALT     ,  XK_Alt_R )
 s( KeyCodes::ALT     ,  XK_Meta_L )
 s( KeyCodes::ALT     ,  XK_Meta_R )

 s( KeyCodes::ESC      ,        XK_Escape    )
 s( KeyCodes::ENTER    ,        XK_Return    )
 s( KeyCodes::BACKSPACE,        XK_BackSpace )
 s( KeyCodes::SPACE    ,        XK_space     )
 s( KeyCodes::TAB      ,        XK_Tab    )
 s( KeyCodes::INSERT   ,        XK_Insert )
 s( KeyCodes::DELETE   ,        XK_Delete )
 s( KeyCodes::HOME     ,        XK_Home  )
 s( KeyCodes::END      ,        XK_End   )
 s( KeyCodes::UP       ,        XK_Up    )
 s( KeyCodes::DOWN     ,        XK_Down  )
 s( KeyCodes::RIGHT    ,        XK_Right )
 s( KeyCodes::LEFT     ,        XK_Left  )
 s( KeyCodes::PAGEUP   ,        XK_Prior )
 s( KeyCodes::PAGEDOWN ,        XK_Next  )

 s( KeyCodes::PAUSE     ,         XK_Pause  )
 s( KeyCodes::SCROLLLOCK,         XK_Scroll_Lock )
 s( KeyCodes::PRINTSCREEN,        XK_Print )

 s( KeyCodes::F1      ,         XK_F1  )
 s( KeyCodes::F2      ,         XK_F2  )
 s( KeyCodes::F3      ,         XK_F3  )
 s( KeyCodes::F4      ,         XK_F4  )
 s( KeyCodes::F5      ,         XK_F5  )
 s( KeyCodes::F6      ,         XK_F6  )
 s( KeyCodes::F7      ,         XK_F7  )
 s( KeyCodes::F8      ,         XK_F8  )
 s( KeyCodes::F9      ,         XK_F9  )
 s( KeyCodes::F10     ,         XK_F10 )
 s( KeyCodes::F11     ,         XK_F11 )
 s( KeyCodes::F12     ,         XK_F12 )

 s( KeyCodes::SPACE,   XK_KP_Space )
 s( KeyCodes::TAB,     XK_KP_Tab   )
 s( KeyCodes::ENTER,   XK_KP_Enter )
 s( KeyCodes::EQUAL,   XK_KP_Equal    )
 s( KeyCodes::SLASH,   XK_KP_Divide   )
 s( KeyCodes::STAR,    XK_KP_Multiply )
 s( KeyCodes::PLUS,    XK_KP_Add      )
 s( KeyCodes::MINUS,   XK_KP_Subtract )
 s( KeyCodes::DOT,     XK_KP_Decimal  )
 s( KeyCodes::D0,      XK_KP_0 )
 s( KeyCodes::D1,      XK_KP_1 )
 s( KeyCodes::D2,      XK_KP_2 )
 s( KeyCodes::D3,      XK_KP_3 )
 s( KeyCodes::D4,      XK_KP_4 )
 s( KeyCodes::D5,      XK_KP_5 )
 s( KeyCodes::D6,      XK_KP_6 )
 s( KeyCodes::D7,      XK_KP_7 )
 s( KeyCodes::D8,      XK_KP_8 )
 s( KeyCodes::D9,      XK_KP_9 )

 s( KeyCodes::D0,      XK_0 )
 s( KeyCodes::D1,      XK_1 )
 s( KeyCodes::D2,      XK_2 )
 s( KeyCodes::D3,      XK_3 )
 s( KeyCodes::D4,      XK_4 )
 s( KeyCodes::D5,      XK_5 )
 s( KeyCodes::D6,      XK_6 )
 s( KeyCodes::D7,      XK_7 )
 s( KeyCodes::D8,      XK_8 )
 s( KeyCodes::D9,      XK_9 )

 s( KeyCodes::A,      XK_A )
 s( KeyCodes::B,      XK_B )
 s( KeyCodes::C,      XK_C )
 s( KeyCodes::D,      XK_D )
 s( KeyCodes::E,      XK_E )
 s( KeyCodes::F,      XK_F )
 s( KeyCodes::G,      XK_G )
 s( KeyCodes::H,      XK_H )
 s( KeyCodes::I,      XK_I )
 s( KeyCodes::J,      XK_J )
 s( KeyCodes::K,      XK_K )
 s( KeyCodes::L,      XK_L )
 s( KeyCodes::M,      XK_M )
 s( KeyCodes::N,      XK_N )
 s( KeyCodes::O,      XK_O )
 s( KeyCodes::P,      XK_P )
 s( KeyCodes::Q,      XK_Q )
 s( KeyCodes::R,      XK_R )
 s( KeyCodes::S,      XK_S )
 s( KeyCodes::T,      XK_T )
 s( KeyCodes::U,      XK_U )
 s( KeyCodes::V,      XK_V )
 s( KeyCodes::W,      XK_W )
 s( KeyCodes::X,      XK_X )
 s( KeyCodes::Y,      XK_Y )
 s( KeyCodes::Z,      XK_Z )

 s( KeyCodes::a,      XK_a )
 s( KeyCodes::b,      XK_b )
 s( KeyCodes::c,      XK_c )
 s( KeyCodes::d,      XK_d )
 s( KeyCodes::e,      XK_e )
 s( KeyCodes::f,      XK_f )
 s( KeyCodes::g,      XK_g )
 s( KeyCodes::h,      XK_h )
 s( KeyCodes::i,      XK_i )
 s( KeyCodes::j,      XK_j )
 s( KeyCodes::k,      XK_k )
 s( KeyCodes::l,      XK_l )
 s( KeyCodes::m,      XK_m )
 s( KeyCodes::n,      XK_n )
 s( KeyCodes::o,      XK_o )
 s( KeyCodes::p,      XK_p )
 s( KeyCodes::q,      XK_q )
 s( KeyCodes::r,      XK_r )
 s( KeyCodes::s,      XK_s )
 s( KeyCodes::t,      XK_t )
 s( KeyCodes::u,      XK_u )
 s( KeyCodes::v,      XK_v )
 s( KeyCodes::w,      XK_w )
 s( KeyCodes::x,      XK_x )
 s( KeyCodes::y,      XK_y )
 s( KeyCodes::z,      XK_z )

 s( KeyCodes::BANG,        XK_exclam       )
 s( KeyCodes::DOUBLEQUOTE, XK_quotedbl     )
 s( KeyCodes::POUND,       XK_numbersign   )
 s( KeyCodes::DOLLAR,      XK_dollar       )
 s( KeyCodes::PERCENT,     XK_percent      )
 s( KeyCodes::AMPERSAND,   XK_ampersand    )
 s( KeyCodes::QUOTE,       XK_apostrophe   )
 s( KeyCodes::OPAREN,      XK_parenleft    )
 s( KeyCodes::CPAREN,      XK_parenright   )
 s( KeyCodes::STAR,        XK_asterisk     )
 s( KeyCodes::PLUS,        XK_plus         )
 s( KeyCodes::COMMA,       XK_comma        )
 s( KeyCodes::MINUS,       XK_minus        )
 s( KeyCodes::DOT,         XK_period       )
 s( KeyCodes::SLASH,       XK_slash        )
 s( KeyCodes::COLON,       XK_colon        )
 s( KeyCodes::SEMICOLON,   XK_semicolon    )
 s( KeyCodes::OANGLE,      XK_less         )
 s( KeyCodes::EQUAL,       XK_equal        )
 s( KeyCodes::CANGLE,      XK_greater      )
 s( KeyCodes::QUERY,       XK_question     )
 s( KeyCodes::AT,          XK_at           )
 s( KeyCodes::OBRACKET,    XK_bracketleft  )
 s( KeyCodes::BACKSLASH,   XK_backslash    )
 s( KeyCodes::CBRACKET,    XK_bracketright )
 s( KeyCodes::HAT,         XK_asciicircum  )
 s( KeyCodes::UNDERSCORE,  XK_underscore   )
 s( KeyCodes::BACKQUOTE,   XK_grave        )
 s( KeyCodes::OBRACE,      XK_braceleft    )
 s( KeyCodes::BAR,         XK_bar          )
 s( KeyCodes::CBRACE,      XK_braceright   )
 s( KeyCodes::TILDE,       XK_asciitilde   )

 }


#undef s
}

/////////////////////////////////////////////////////////////////////////////

boolean translate_xkey_event( XKeyEvent* event, Key& key )
{
  if( event == 0 )
    return false;

  int count;
  static char keybuffer[101];
  KeySym keysym;
  XComposeStatus compose;

  count= XLookupString( event, keybuffer, 100, &keysym, &compose );

  if( keysTranslation == 0 )
    initXKeysTranslations();

  const KEYTRANSLATION* kt= keysTranslation;
  static const int length= nTranslations;

  key= KeyCodes::NULL_KEY;

  for( int i= 0; i< nTranslations; ++i, ++kt )
    if( kt->initial == keysym )
      {
      key= kt->translated;
      break;
      }

  if( key == KeyCodes::NULL_KEY )
    return false;

  if( (event->state & ShiftMask) != 0 )
    {
    if( key.getBase() <= int(' ') || key.getBase() >= 127 )
      key= key.shift();
    }

  if( (event->state & Mod1Mask) != 0 )
    key= key.upperCase().alt();

  if( (event->state & ControlMask) != 0 )
    key= key.upperCase().control();

/*
  if( (event->state & ControlMask) != 0 )
    {
#ifdef __FRENCH_KEYBOARD__
    if( xkey == KEY('&') ) xkey= KeyCodes::ALT1; else
    if( xkey == KEY(0xe9)) xkey= KeyCodes::ALT2; else
    if( xkey == KEY('"') ) xkey= KeyCodes::ALT3; else
    if( xkey == KEY('\'')) xkey= KeyCodes::ALT4; else
    if( xkey == KEY('(') ) xkey= KeyCodes::ALT5; else
    if( xkey == KEY('-') ) xkey= KeyCodes::ALT6; else
    if( xkey == KEY(0xe8)) xkey= KeyCodes::ALT7; else
    if( xkey == KEY('_') ) xkey= KeyCodes::ALT8; else
    if( xkey == KEY(0xe7)) xkey= KeyCodes::ALT9; else
    if( xkey == KEY(0xe0)) xkey= KeyCodes::ALT0; else
#else
    if( xkey == KEY('1') ) xkey= KeyCodes::ALT1; else
    if( xkey == KEY('2') ) xkey= KeyCodes::ALT2; else
    if( xkey == KEY('3') ) xkey= KeyCodes::ALT3; else
    if( xkey == KEY('4') ) xkey= KeyCodes::ALT4; else
    if( xkey == KEY('5') ) xkey= KeyCodes::ALT5; else
    if( xkey == KEY('6') ) xkey= KeyCodes::ALT6; else
    if( xkey == KEY('7') ) xkey= KeyCodes::ALT7; else
    if( xkey == KEY('8') ) xkey= KeyCodes::ALT8; else
    if( xkey == KEY('9') ) xkey= KeyCodes::ALT9; else
    if( xkey == KEY('0') ) xkey= KeyCodes::ALT0; else
#endif
    key= Key::CTRL( Key::UPPERCASE(xkey) );
    }
*/

  return true;
}

/////////////////////////////////////////////////////////////////////////////
