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
   WKey.C

   Stephane Rehel
   July 6 1997
*/

#include <assert.h>

#include "Key.h"
#include "KeyCodes.h"
#include "WKey.h"

#ifdef __LINUX__
#define __FRENCH_KEYBOARD__
#endif

/////////////////////////////////////////////////////////////////////////////

struct KEYTRANSLATION
{
  Key translated;
  INT initial;
};

static KEYTRANSLATION* keysTranslation= 0;
static int nTranslations= 0;

/////////////////////////////////////////////////////////////////////////////

static void initWKeysTranslations()
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

 s( KeyCodes::CONTROL ,  -(VK_CONTROL) )
 s( KeyCodes::SHIFT   ,  -(VK_SHIFT) )
 s( KeyCodes::ALT     ,  -(VK_MENU) )

 s( KeyCodes::ESC      ,   -(VK_ESCAPE)   )
 s( KeyCodes::ENTER    ,   -(VK_RETURN)   )
 s( KeyCodes::BACKSPACE,   -(VK_BACK)     )
 s( KeyCodes::SPACE    ,   -(VK_SPACE)    )
 s( KeyCodes::TAB      ,   -(VK_TAB)      )
 s( KeyCodes::INSERT   ,   -(VK_INSERT)   )
 s( KeyCodes::DELETE   ,   -(VK_DELETE)   )
 s( KeyCodes::HOME     ,   -(VK_HOME)     )
 s( KeyCodes::END      ,   -(VK_END)      )
 s( KeyCodes::UP       ,   -(VK_UP)       )
 s( KeyCodes::DOWN     ,   -(VK_DOWN)     )
 s( KeyCodes::RIGHT    ,   -(VK_RIGHT)    )
 s( KeyCodes::LEFT     ,   -(VK_LEFT)     )
 s( KeyCodes::PAGEUP   ,   -(VK_PRIOR)    )
 s( KeyCodes::PAGEDOWN ,   -(VK_NEXT)     )
 s( KeyCodes::PAUSE     ,  -(VK_PAUSE)    )
 s( KeyCodes::SCROLLLOCK,  -(VK_SCROLL)   )
 s( KeyCodes::PRINTSCREEN, -(VK_SNAPSHOT) )

 s( KeyCodes::F1      ,         -(VK_F1)  )
 s( KeyCodes::F2      ,         -(VK_F2)  )
 s( KeyCodes::F3      ,         -(VK_F3)  )
 s( KeyCodes::F4      ,         -(VK_F4)  )
 s( KeyCodes::F5      ,         -(VK_F5)  )
 s( KeyCodes::F6      ,         -(VK_F6)  )
 s( KeyCodes::F7      ,         -(VK_F7)  )
 s( KeyCodes::F8      ,         -(VK_F8)  )
 s( KeyCodes::F9      ,         -(VK_F9)  )
 s( KeyCodes::F10     ,         -(VK_F10) )
 s( KeyCodes::F11     ,         -(VK_F11) )
 s( KeyCodes::F12     ,         -(VK_F12) )

 s( KeyCodes::D0,      int('0') )
 s( KeyCodes::D1,      int('1') )
 s( KeyCodes::D2,      int('2') )
 s( KeyCodes::D3,      int('3') )
 s( KeyCodes::D4,      int('4') )
 s( KeyCodes::D5,      int('5') )
 s( KeyCodes::D6,      int('6') )
 s( KeyCodes::D7,      int('7') )
 s( KeyCodes::D8,      int('8') )
 s( KeyCodes::D9,      int('9') )

 s( KeyCodes::A,      int('A') )
 s( KeyCodes::B,      int('B') )
 s( KeyCodes::C,      int('C') )
 s( KeyCodes::D,      int('D') )
 s( KeyCodes::E,      int('E') )
 s( KeyCodes::F,      int('F') )
 s( KeyCodes::G,      int('G') )
 s( KeyCodes::H,      int('H') )
 s( KeyCodes::I,      int('I') )
 s( KeyCodes::J,      int('J') )
 s( KeyCodes::K,      int('K') )
 s( KeyCodes::L,      int('L') )
 s( KeyCodes::M,      int('M') )
 s( KeyCodes::N,      int('N') )
 s( KeyCodes::O,      int('O') )
 s( KeyCodes::P,      int('P') )
 s( KeyCodes::Q,      int('Q') )
 s( KeyCodes::R,      int('R') )
 s( KeyCodes::S,      int('S') )
 s( KeyCodes::T,      int('T') )
 s( KeyCodes::U,      int('U') )
 s( KeyCodes::V,      int('V') )
 s( KeyCodes::W,      int('W') )
 s( KeyCodes::X,      int('X') )
 s( KeyCodes::Y,      int('Y') )
 s( KeyCodes::Z,      int('Z') )

 s( KeyCodes::a,      int('a') )
 s( KeyCodes::b,      int('b') )
 s( KeyCodes::c,      int('c') )
 s( KeyCodes::d,      int('d') )
 s( KeyCodes::e,      int('e') )
 s( KeyCodes::f,      int('f') )
 s( KeyCodes::g,      int('g') )
 s( KeyCodes::h,      int('h') )
 s( KeyCodes::i,      int('i') )
 s( KeyCodes::j,      int('j') )
 s( KeyCodes::k,      int('k') )
 s( KeyCodes::l,      int('l') )
 s( KeyCodes::m,      int('m') )
 s( KeyCodes::n,      int('n') )
 s( KeyCodes::o,      int('o') )
 s( KeyCodes::p,      int('p') )
 s( KeyCodes::q,      int('q') )
 s( KeyCodes::r,      int('r') )
 s( KeyCodes::s,      int('s') )
 s( KeyCodes::t,      int('t') )
 s( KeyCodes::u,      int('u') )
 s( KeyCodes::v,      int('v') )
 s( KeyCodes::w,      int('w') )
 s( KeyCodes::x,      int('x') )
 s( KeyCodes::y,      int('y') )
 s( KeyCodes::z,      int('z') )

 s( KeyCodes::BANG,        int('!') )
 s( KeyCodes::DOUBLEQUOTE, int('"') )
 s( KeyCodes::POUND,       int('#') )
 s( KeyCodes::DOLLAR,      int('$') )
 s( KeyCodes::PERCENT,     int('%') )
 s( KeyCodes::AMPERSAND,   int('&') )
 s( KeyCodes::QUOTE,       int('\''))
 s( KeyCodes::OPAREN,      int('(') )
 s( KeyCodes::CPAREN,      int(')') )
 s( KeyCodes::STAR,        int('*') )
 s( KeyCodes::PLUS,        int('+') )
 s( KeyCodes::COMMA,       int(',') )
 s( KeyCodes::MINUS,       int('-') )
 s( KeyCodes::DOT,         int('.') )
 s( KeyCodes::SLASH,       int('/') )
 s( KeyCodes::COLON,       int(':') )
 s( KeyCodes::SEMICOLON,   int(';') )
 s( KeyCodes::OANGLE,      int('<') )
 s( KeyCodes::EQUAL,       int('=') )
 s( KeyCodes::CANGLE,      int('>') )
 s( KeyCodes::QUERY,       int('?') )
 s( KeyCodes::AT,          int('@') )
 s( KeyCodes::OBRACKET,    int('[') )
 s( KeyCodes::BACKSLASH,   int('\\'))
 s( KeyCodes::CBRACKET,    int(']') )
 s( KeyCodes::HAT,         int('^') )
 s( KeyCodes::UNDERSCORE,  int('_') )
 s( KeyCodes::BACKQUOTE,   int('`') )
 s( KeyCodes::OBRACE,      int('{') )
 s( KeyCodes::BAR,         int('|') )
 s( KeyCodes::CBRACE,      int('}') )
 s( KeyCodes::TILDE,       int('~') )
 }

#undef s
}

/////////////////////////////////////////////////////////////////////////////

static const KEYTRANSLATION* find_sys_key( INT sys_key )
{
  const KEYTRANSLATION* kt= keysTranslation;

  for( int i= 0; i < nTranslations; ++i, ++kt )
    if( kt->initial == sys_key )
      return kt;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean translate_wkey_event( MSG* msg, Key& key )
{
  key= KeyCodes::NULL_KEY;

  if( msg == 0 )
    return false;

  UINT message= msg->message;

  if( message != WM_KEYUP    && message != WM_KEYDOWN &&
      message != WM_SYSKEYUP && message != WM_SYSKEYDOWN )
    return false;

  INT nVirtKey= (int) msg->wParam;
  LONG lKeyData= msg->lParam;

  if( message == WM_KEYDOWN || message == WM_SYSKEYDOWN )
    if( (lKeyData & (LONG(1)<<30)) != 0 )
      return 0; // auto repeat is ignored

  if( keysTranslation == 0 )
    initWKeysTranslations();

  // first, try to map the virtual key
  const KEYTRANSLATION* kt= find_sys_key( -nVirtKey );

  if( kt == 0 )
    {
    // not found, try to map into ascii

    UINT scan_code= MapVirtualKey(nVirtKey,0);
    static BYTE bKeyState[256];
    if( ! GetKeyboardState(bKeyState) )
      return false;
    static WORD tkey[10];
    int res= ToAscii( nVirtKey, scan_code, bKeyState, tkey, 0 );

    if( res == 2 || res == -1 )
      {
      // special case of '~' and '`' (due to french keyboard mapping??)
      if( int(tkey[0]) != '~' && int(tkey[0]) != '`' )
        return false;
      if( res == -1 )
        msg->message= WM_KEYDOWN;
       else
        msg->message= WM_KEYUP;
      res= 1;
      }

    if( res != 1 )
      return false;

    if( INT(tkey[0]) >= 1 && INT(tkey[0]) <= 26 )
      tkey[0] += INT('A') - 1;

    kt= find_sys_key( INT(tkey[0]) );
    if( kt == 0 )
      return false;
    }

  key= kt->translated;

  if( GetKeyState(VK_CONTROL) < 0 )
    {
    if( key.getBase() != int('~') && key.getBase() != int('`') )
      key= key.upperCase().control();
    }

  if( GetKeyState(VK_SHIFT) < 0 )
    {
    if( key.getBase() <= int(' ') || key.getBase() >= 127 )       
      key= key.shift();
    }

  if( message == WM_SYSKEYDOWN || message == WM_SYSKEYUP )
    {
    if( key.getBase() == KeyCodes::F10.getBase() )
      {
      // special case of F10 key
      if( (lKeyData & (1<<29)) != 0 )
        {
        if( ! key.hasControl() )
          key= key.upperCase().alt();
        }
      }
     else
      key= key.upperCase().alt();
    }

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
