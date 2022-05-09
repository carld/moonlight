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
   KeyCodes.h

   Stephane Rehel
   June 28, 1996
*/


#ifndef __KeyCodes_h
#define __KeyCodes_h

#ifndef __Key_h
#include "Key.h"
#endif

#ifdef DELETE
#undef DELETE
#endif

/////////////////////////////////////////////////////////////////////////////

class KeyCodes
{
public:
  static Key NULL_KEY;

  static Key CONTROL;
  static Key SHIFT;
  static Key ALT;

  static Key ESC;
  static Key ENTER;
  static Key BACKSPACE;
  static Key SPACE;
  static Key TAB;
  static Key INSERT;
  static Key DELETE;
  static Key HOME;
  static Key END;
  static Key UP;
  static Key DOWN;
  static Key RIGHT;
  static Key LEFT;
  static Key PAGEUP;
  static Key PAGEDOWN;

  static Key PRINTSCREEN;
  static Key SCROLLLOCK;
  static Key PAUSE;
  static Key NUMLOCK;
  static Key CAPSLOCK;

  static Key BANG;
  static Key DOUBLEQUOTE;
  static Key POUND;
  static Key DOLLAR;
  static Key PERCENT;
  static Key AMPERSAND;
  static Key QUOTE;
  static Key OPAREN;
  static Key CPAREN;
  static Key STAR;
  static Key PLUS;
  static Key COMMA;
  static Key MINUS;
  static Key DOT;
  static Key SLASH;
  static Key COLON;
  static Key SEMICOLON;
  static Key OANGLE;
  static Key EQUAL;
  static Key CANGLE;
  static Key QUERY;
  static Key AT;
  static Key OBRACKET;
  static Key BACKSLASH;
  static Key CBRACKET;
  static Key HAT;
  static Key UNDERSCORE;
  static Key BACKQUOTE;
  static Key OBRACE;
  static Key BAR;
  static Key CBRACE;
  static Key TILDE;

  static Key D0;
  static Key D1;
  static Key D2;
  static Key D3;
  static Key D4;
  static Key D5;
  static Key D6;
  static Key D7;
  static Key D8;
  static Key D9;

  static Key A;
  static Key B;
  static Key C;
  static Key D;
  static Key E;
  static Key F;
  static Key G;
  static Key H;
  static Key I;
  static Key J;
  static Key K;
  static Key L;
  static Key M;
  static Key N;
  static Key O;
  static Key P;
  static Key Q;
  static Key R;
  static Key S;
  static Key T;
  static Key U;
  static Key V;
  static Key W;
  static Key X;
  static Key Y;
  static Key Z;

  static Key a;
  static Key b;
  static Key c;
  static Key d;
  static Key e;
  static Key f;
  static Key g;
  static Key h;
  static Key i;
  static Key j;
  static Key k;
  static Key l;
  static Key m;
  static Key n;
  static Key o;
  static Key p;
  static Key q;
  static Key r;
  static Key s;
  static Key t;
  static Key u;
  static Key v;
  static Key w;
  static Key x;
  static Key y;
  static Key z;

  static Key SHIFT_ESC;
  static Key SHIFT_ENTER;
  static Key SHIFT_BACKSPACE;
  static Key SHIFT_SPACE;
  static Key SHIFT_TAB;
  static Key SHIFT_INSERT;
  static Key SHIFT_DELETE;
  static Key SHIFT_HOME;
  static Key SHIFT_END;
  static Key SHIFT_UP;
  static Key SHIFT_DOWN;
  static Key SHIFT_RIGHT;
  static Key SHIFT_LEFT;
  static Key SHIFT_PAGEUP;
  static Key SHIFT_PAGEDOWN;

  static Key CTRL_ESC;
  static Key CTRL_ENTER;
  static Key CTRL_BACKSPACE;
  static Key CTRL_SPACE;
  static Key CTRL_TAB;
  static Key CTRL_INSERT;
  static Key CTRL_DELETE;
  static Key CTRL_HOME;
  static Key CTRL_END;
  static Key CTRL_UP;
  static Key CTRL_DOWN;
  static Key CTRL_RIGHT;
  static Key CTRL_LEFT;
  static Key CTRL_PAGEUP;
  static Key CTRL_PAGEDOWN;

  static Key ALT_ESC;
  static Key ALT_ENTER;
  static Key ALT_BACKSPACE;
  static Key ALT_SPACE;
  static Key ALT_TAB;
  static Key ALT_INSERT;
  static Key ALT_DELETE;
  static Key ALT_HOME;
  static Key ALT_END;
  static Key ALT_UP;
  static Key ALT_DOWN;
  static Key ALT_RIGHT;
  static Key ALT_LEFT;
  static Key ALT_PAGEUP;
  static Key ALT_PAGEDOWN;

  static Key F1;
  static Key F2;
  static Key F3;
  static Key F4;
  static Key F5;
  static Key F6;
  static Key F7;
  static Key F8;
  static Key F9;
  static Key F10;
  static Key F11;
  static Key F12;

  static Key SHIFT_F1;
  static Key SHIFT_F2;
  static Key SHIFT_F3;
  static Key SHIFT_F4;
  static Key SHIFT_F5;
  static Key SHIFT_F6;
  static Key SHIFT_F7;
  static Key SHIFT_F8;
  static Key SHIFT_F9;
  static Key SHIFT_F10;
  static Key SHIFT_F11;
  static Key SHIFT_F12;

  static Key CTRL_F1;
  static Key CTRL_F2;
  static Key CTRL_F3;
  static Key CTRL_F4;
  static Key CTRL_F5;
  static Key CTRL_F6;
  static Key CTRL_F7;
  static Key CTRL_F8;
  static Key CTRL_F9;
  static Key CTRL_F10;
  static Key CTRL_F11;
  static Key CTRL_F12;

  static Key ALT_F1;
  static Key ALT_F2;
  static Key ALT_F3;
  static Key ALT_F4;
  static Key ALT_F5;
  static Key ALT_F6;
  static Key ALT_F7;
  static Key ALT_F8;
  static Key ALT_F9;
  static Key ALT_F10;
  static Key ALT_F11;
  static Key ALT_F12;

  static Key CTRL_0;
  static Key CTRL_1;
  static Key CTRL_2;
  static Key CTRL_3;
  static Key CTRL_4;
  static Key CTRL_5;
  static Key CTRL_6;
  static Key CTRL_7;
  static Key CTRL_8;
  static Key CTRL_9;

  static Key ALT_0;
  static Key ALT_1;
  static Key ALT_2;
  static Key ALT_3;
  static Key ALT_4;
  static Key ALT_5;
  static Key ALT_6;
  static Key ALT_7;
  static Key ALT_8;
  static Key ALT_9;

  static Key CTRL_A;
  static Key CTRL_B;
  static Key CTRL_C;
  static Key CTRL_D;
  static Key CTRL_E;
  static Key CTRL_F;
  static Key CTRL_G;
  static Key CTRL_H;
  static Key CTRL_I;
  static Key CTRL_J;
  static Key CTRL_K;
  static Key CTRL_L;
  static Key CTRL_M;
  static Key CTRL_N;
  static Key CTRL_O;
  static Key CTRL_P;
  static Key CTRL_Q;
  static Key CTRL_R;
  static Key CTRL_S;
  static Key CTRL_T;
  static Key CTRL_U;
  static Key CTRL_V;
  static Key CTRL_W;
  static Key CTRL_X;
  static Key CTRL_Y;
  static Key CTRL_Z;

  static Key ALT_A;
  static Key ALT_B;
  static Key ALT_C;
  static Key ALT_D;
  static Key ALT_E;
  static Key ALT_F;
  static Key ALT_G;
  static Key ALT_H;
  static Key ALT_I;
  static Key ALT_J;
  static Key ALT_K;
  static Key ALT_L;
  static Key ALT_M;
  static Key ALT_N;
  static Key ALT_O;
  static Key ALT_P;
  static Key ALT_Q;
  static Key ALT_R;
  static Key ALT_S;
  static Key ALT_T;
  static Key ALT_U;
  static Key ALT_V;
  static Key ALT_W;
  static Key ALT_X;
  static Key ALT_Y;
  static Key ALT_Z;

  static void init_keys();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KeyCodes_h
