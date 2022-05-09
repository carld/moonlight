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
   KeyCodes.C

   Stephane Rehel
   June 28, 1996
*/

#include "KeyCodes.h"

/////////////////////////////////////////////////////////////////////////////

// static
Key KeyCodes::NULL_KEY;

Key KeyCodes::CONTROL;
Key KeyCodes::SHIFT;
Key KeyCodes::ALT;

Key KeyCodes::ESC;
Key KeyCodes::ENTER;
Key KeyCodes::BACKSPACE;
Key KeyCodes::SPACE;
Key KeyCodes::TAB;
Key KeyCodes::INSERT;
Key KeyCodes::DELETE;
Key KeyCodes::HOME;
Key KeyCodes::END;
Key KeyCodes::UP;
Key KeyCodes::DOWN;
Key KeyCodes::RIGHT;
Key KeyCodes::LEFT;
Key KeyCodes::PAGEUP;
Key KeyCodes::PAGEDOWN;

Key KeyCodes::PRINTSCREEN;
Key KeyCodes::SCROLLLOCK;
Key KeyCodes::PAUSE;
Key KeyCodes::NUMLOCK;
Key KeyCodes::CAPSLOCK;

Key KeyCodes::BANG;
Key KeyCodes::DOUBLEQUOTE;
Key KeyCodes::POUND;
Key KeyCodes::DOLLAR;
Key KeyCodes::PERCENT;
Key KeyCodes::AMPERSAND;
Key KeyCodes::QUOTE;
Key KeyCodes::OPAREN;
Key KeyCodes::CPAREN;
Key KeyCodes::STAR;
Key KeyCodes::PLUS;
Key KeyCodes::COMMA;
Key KeyCodes::MINUS;
Key KeyCodes::DOT;
Key KeyCodes::SLASH;
Key KeyCodes::COLON;
Key KeyCodes::SEMICOLON;
Key KeyCodes::OANGLE;
Key KeyCodes::EQUAL;
Key KeyCodes::CANGLE;
Key KeyCodes::QUERY;
Key KeyCodes::AT;
Key KeyCodes::OBRACKET;
Key KeyCodes::BACKSLASH;
Key KeyCodes::CBRACKET;
Key KeyCodes::HAT;
Key KeyCodes::UNDERSCORE;
Key KeyCodes::BACKQUOTE;
Key KeyCodes::OBRACE;
Key KeyCodes::BAR;
Key KeyCodes::CBRACE;
Key KeyCodes::TILDE;

Key KeyCodes::D0;
Key KeyCodes::D1;
Key KeyCodes::D2;
Key KeyCodes::D3;
Key KeyCodes::D4;
Key KeyCodes::D5;
Key KeyCodes::D6;
Key KeyCodes::D7;
Key KeyCodes::D8;
Key KeyCodes::D9;

Key KeyCodes::A;
Key KeyCodes::B;
Key KeyCodes::C;
Key KeyCodes::D;
Key KeyCodes::E;
Key KeyCodes::F;
Key KeyCodes::G;
Key KeyCodes::H;
Key KeyCodes::I;
Key KeyCodes::J;
Key KeyCodes::K;
Key KeyCodes::L;
Key KeyCodes::M;
Key KeyCodes::N;
Key KeyCodes::O;
Key KeyCodes::P;
Key KeyCodes::Q;
Key KeyCodes::R;
Key KeyCodes::S;
Key KeyCodes::T;
Key KeyCodes::U;
Key KeyCodes::V;
Key KeyCodes::W;
Key KeyCodes::X;
Key KeyCodes::Y;
Key KeyCodes::Z;

Key KeyCodes::a;
Key KeyCodes::b;
Key KeyCodes::c;
Key KeyCodes::d;
Key KeyCodes::e;
Key KeyCodes::f;
Key KeyCodes::g;
Key KeyCodes::h;
Key KeyCodes::i;
Key KeyCodes::j;
Key KeyCodes::k;
Key KeyCodes::l;
Key KeyCodes::m;
Key KeyCodes::n;
Key KeyCodes::o;
Key KeyCodes::p;
Key KeyCodes::q;
Key KeyCodes::r;
Key KeyCodes::s;
Key KeyCodes::t;
Key KeyCodes::u;
Key KeyCodes::v;
Key KeyCodes::w;
Key KeyCodes::x;
Key KeyCodes::y;
Key KeyCodes::z;

Key KeyCodes::SHIFT_ESC;
Key KeyCodes::SHIFT_ENTER;
Key KeyCodes::SHIFT_BACKSPACE;
Key KeyCodes::SHIFT_SPACE;
Key KeyCodes::SHIFT_TAB;
Key KeyCodes::SHIFT_INSERT;
Key KeyCodes::SHIFT_DELETE;
Key KeyCodes::SHIFT_HOME;
Key KeyCodes::SHIFT_END;
Key KeyCodes::SHIFT_UP;
Key KeyCodes::SHIFT_DOWN;
Key KeyCodes::SHIFT_RIGHT;
Key KeyCodes::SHIFT_LEFT;
Key KeyCodes::SHIFT_PAGEUP;
Key KeyCodes::SHIFT_PAGEDOWN;

Key KeyCodes::CTRL_ESC;
Key KeyCodes::CTRL_ENTER;
Key KeyCodes::CTRL_BACKSPACE;
Key KeyCodes::CTRL_SPACE;
Key KeyCodes::CTRL_TAB;
Key KeyCodes::CTRL_INSERT;
Key KeyCodes::CTRL_DELETE;
Key KeyCodes::CTRL_HOME;
Key KeyCodes::CTRL_END;
Key KeyCodes::CTRL_UP;
Key KeyCodes::CTRL_DOWN;
Key KeyCodes::CTRL_RIGHT;
Key KeyCodes::CTRL_LEFT;
Key KeyCodes::CTRL_PAGEUP;
Key KeyCodes::CTRL_PAGEDOWN;

Key KeyCodes::ALT_ESC;
Key KeyCodes::ALT_ENTER;
Key KeyCodes::ALT_BACKSPACE;
Key KeyCodes::ALT_SPACE;
Key KeyCodes::ALT_TAB;
Key KeyCodes::ALT_INSERT;
Key KeyCodes::ALT_DELETE;
Key KeyCodes::ALT_HOME;
Key KeyCodes::ALT_END;
Key KeyCodes::ALT_UP;
Key KeyCodes::ALT_DOWN;
Key KeyCodes::ALT_RIGHT;
Key KeyCodes::ALT_LEFT;
Key KeyCodes::ALT_PAGEUP;
Key KeyCodes::ALT_PAGEDOWN;

Key KeyCodes::F1;
Key KeyCodes::F2;
Key KeyCodes::F3;
Key KeyCodes::F4;
Key KeyCodes::F5;
Key KeyCodes::F6;
Key KeyCodes::F7;
Key KeyCodes::F8;
Key KeyCodes::F9;
Key KeyCodes::F10;
Key KeyCodes::F11;
Key KeyCodes::F12;

Key KeyCodes::SHIFT_F1;
Key KeyCodes::SHIFT_F2;
Key KeyCodes::SHIFT_F3;
Key KeyCodes::SHIFT_F4;
Key KeyCodes::SHIFT_F5;
Key KeyCodes::SHIFT_F6;
Key KeyCodes::SHIFT_F7;
Key KeyCodes::SHIFT_F8;
Key KeyCodes::SHIFT_F9;
Key KeyCodes::SHIFT_F10;
Key KeyCodes::SHIFT_F11;
Key KeyCodes::SHIFT_F12;

Key KeyCodes::CTRL_F1;
Key KeyCodes::CTRL_F2;
Key KeyCodes::CTRL_F3;
Key KeyCodes::CTRL_F4;
Key KeyCodes::CTRL_F5;
Key KeyCodes::CTRL_F6;
Key KeyCodes::CTRL_F7;
Key KeyCodes::CTRL_F8;
Key KeyCodes::CTRL_F9;
Key KeyCodes::CTRL_F10;
Key KeyCodes::CTRL_F11;
Key KeyCodes::CTRL_F12;

Key KeyCodes::ALT_F1;
Key KeyCodes::ALT_F2;
Key KeyCodes::ALT_F3;
Key KeyCodes::ALT_F4;
Key KeyCodes::ALT_F5;
Key KeyCodes::ALT_F6;
Key KeyCodes::ALT_F7;
Key KeyCodes::ALT_F8;
Key KeyCodes::ALT_F9;
Key KeyCodes::ALT_F10;
Key KeyCodes::ALT_F11;
Key KeyCodes::ALT_F12;

Key KeyCodes::CTRL_0;
Key KeyCodes::CTRL_1;
Key KeyCodes::CTRL_2;
Key KeyCodes::CTRL_3;
Key KeyCodes::CTRL_4;
Key KeyCodes::CTRL_5;
Key KeyCodes::CTRL_6;
Key KeyCodes::CTRL_7;
Key KeyCodes::CTRL_8;
Key KeyCodes::CTRL_9;

Key KeyCodes::ALT_0;
Key KeyCodes::ALT_1;
Key KeyCodes::ALT_2;
Key KeyCodes::ALT_3;
Key KeyCodes::ALT_4;
Key KeyCodes::ALT_5;
Key KeyCodes::ALT_6;
Key KeyCodes::ALT_7;
Key KeyCodes::ALT_8;
Key KeyCodes::ALT_9;

Key KeyCodes::CTRL_A;
Key KeyCodes::CTRL_B;
Key KeyCodes::CTRL_C;
Key KeyCodes::CTRL_D;
Key KeyCodes::CTRL_E;
Key KeyCodes::CTRL_F;
Key KeyCodes::CTRL_G;
Key KeyCodes::CTRL_H;
Key KeyCodes::CTRL_I;
Key KeyCodes::CTRL_J;
Key KeyCodes::CTRL_K;
Key KeyCodes::CTRL_L;
Key KeyCodes::CTRL_M;
Key KeyCodes::CTRL_N;
Key KeyCodes::CTRL_O;
Key KeyCodes::CTRL_P;
Key KeyCodes::CTRL_Q;
Key KeyCodes::CTRL_R;
Key KeyCodes::CTRL_S;
Key KeyCodes::CTRL_T;
Key KeyCodes::CTRL_U;
Key KeyCodes::CTRL_V;
Key KeyCodes::CTRL_W;
Key KeyCodes::CTRL_X;
Key KeyCodes::CTRL_Y;
Key KeyCodes::CTRL_Z;

Key KeyCodes::ALT_A;
Key KeyCodes::ALT_B;
Key KeyCodes::ALT_C;
Key KeyCodes::ALT_D;
Key KeyCodes::ALT_E;
Key KeyCodes::ALT_F;
Key KeyCodes::ALT_G;
Key KeyCodes::ALT_H;
Key KeyCodes::ALT_I;
Key KeyCodes::ALT_J;
Key KeyCodes::ALT_K;
Key KeyCodes::ALT_L;
Key KeyCodes::ALT_M;
Key KeyCodes::ALT_N;
Key KeyCodes::ALT_O;
Key KeyCodes::ALT_P;
Key KeyCodes::ALT_Q;
Key KeyCodes::ALT_R;
Key KeyCodes::ALT_S;
Key KeyCodes::ALT_T;
Key KeyCodes::ALT_U;
Key KeyCodes::ALT_V;
Key KeyCodes::ALT_W;
Key KeyCodes::ALT_X;
Key KeyCodes::ALT_Y;
Key KeyCodes::ALT_Z;

/////////////////////////////////////////////////////////////////////////////

// static
void KeyCodes::init_keys()
{
  NULL_KEY= Key(Key::KEY(0));

  CONTROL=   Key(Key::KEY(0x10000));
  SHIFT=     Key(Key::KEY(0x20000));
  ALT=       Key(Key::KEY(0x40000));

  ESC=       Key(Key::KEY(0x001b));
  ENTER=     Key(Key::KEY(0x000d));
  BACKSPACE= Key(Key::KEY(0x0008));
  SPACE=     Key(Key::KEY(0x0020));
  TAB=       Key(Key::KEY(0x0009));

  INSERT=    Key(Key::KEY(0x5200)); // remember the old good time...
  DELETE=    Key(Key::KEY(0x5300));
  HOME=      Key(Key::KEY(0x4700));
  END=       Key(Key::KEY(0x4f00));
  UP=        Key(Key::KEY(0x4800));
  DOWN=      Key(Key::KEY(0x5000));
  RIGHT=     Key(Key::KEY(0x4d00));
  LEFT=      Key(Key::KEY(0x4b00));
  PAGEUP=    Key(Key::KEY(0x4900));
  PAGEDOWN=  Key(Key::KEY(0x5100));

  PRINTSCREEN= Key(Key::KEY(0x0700));
  SCROLLLOCK=  Key(Key::KEY(0x0600));
  PAUSE=       Key(Key::KEY(0x0100));
  NUMLOCK=     Key(Key::KEY(0x0200));
  CAPSLOCK=    Key(Key::KEY(0x0300));

  BANG=        Key('!');
  DOUBLEQUOTE= Key('"');
  POUND=       Key('#');
  DOLLAR=      Key('$');
  PERCENT=     Key('%');
  AMPERSAND=   Key('&');
  QUOTE=       Key('\'');
  OPAREN=      Key('(');
  CPAREN=      Key(')');
  STAR=        Key('*');
  PLUS=        Key('+');
  COMMA=       Key(',');
  MINUS=       Key('-');
  DOT=         Key('.');
  SLASH=       Key('/');
  COLON=       Key(':');
  SEMICOLON=   Key(';');
  OANGLE=      Key('<');
  EQUAL=       Key('=');
  CANGLE=      Key('>');
  QUERY=       Key('?');
  AT=          Key('@');
  OBRACKET=    Key('[');
  BACKSLASH=   Key('\\');
  CBRACKET=    Key(']');
  HAT=         Key('^');
  UNDERSCORE=  Key('_');
  BACKQUOTE=   Key('`');
  OBRACE=      Key('{');
  BAR=         Key('|');
  CBRACE=      Key('}');
  TILDE=       Key('~');

  D0= Key('0');
  D1= Key('1');
  D2= Key('2');
  D3= Key('3');
  D4= Key('4');
  D5= Key('5');
  D6= Key('6');
  D7= Key('7');
  D8= Key('8');
  D9= Key('9');

  A= Key('A');
  B= Key('B');
  C= Key('C');
  D= Key('D');
  E= Key('E');
  F= Key('F');
  G= Key('G');
  H= Key('H');
  I= Key('I');
  J= Key('J');
  K= Key('K');
  L= Key('L');
  M= Key('M');
  N= Key('N');
  O= Key('O');
  P= Key('P');
  Q= Key('Q');
  R= Key('R');
  S= Key('S');
  T= Key('T');
  U= Key('U');
  V= Key('V');
  W= Key('W');
  X= Key('X');
  Y= Key('Y');
  Z= Key('Z');

  a= Key('a');
  b= Key('b');
  c= Key('c');
  d= Key('d');
  e= Key('e');
  f= Key('f');
  g= Key('g');
  h= Key('h');
  i= Key('i');
  j= Key('j');
  k= Key('k');
  l= Key('l');
  m= Key('m');
  n= Key('n');
  o= Key('o');
  p= Key('p');
  q= Key('q');
  r= Key('r');
  s= Key('s');
  t= Key('t');
  u= Key('u');
  v= Key('v');
  w= Key('w');
  x= Key('x');
  y= Key('y');
  z= Key('z');

  SHIFT_ESC=       ESC.shift();
  SHIFT_ENTER=     ENTER.shift();
  SHIFT_BACKSPACE= BACKSPACE.shift();
  SHIFT_SPACE=     SPACE.shift();
  SHIFT_TAB=       TAB.shift();
  SHIFT_INSERT=    INSERT.shift();
  SHIFT_DELETE=    DELETE.shift();
  SHIFT_HOME=      HOME.shift();
  SHIFT_END=       END.shift();
  SHIFT_UP=        UP.shift();
  SHIFT_DOWN=      DOWN.shift();
  SHIFT_RIGHT=     RIGHT.shift();
  SHIFT_LEFT=      LEFT.shift();
  SHIFT_PAGEUP=    UP.shift();
  SHIFT_PAGEDOWN=  DOWN.shift();

  CTRL_ESC=       ESC.control();
  CTRL_ENTER=     ENTER.control();
  CTRL_BACKSPACE= BACKSPACE.control();
  CTRL_SPACE=     SPACE.control();
  CTRL_TAB=       TAB.control();
  CTRL_INSERT=    INSERT.control();
  CTRL_DELETE=    DELETE.control();
  CTRL_HOME=      HOME.control();
  CTRL_END=       END.control();
  CTRL_UP=        UP.control();
  CTRL_DOWN=      DOWN.control();
  CTRL_RIGHT=     RIGHT.control();
  CTRL_LEFT=      LEFT.control();
  CTRL_PAGEUP=    UP.control();
  CTRL_PAGEDOWN=  DOWN.control();

  ALT_ESC=       ESC.alt();
  ALT_ENTER=     ENTER.alt();
  ALT_BACKSPACE= BACKSPACE.alt();
  ALT_SPACE=     SPACE.alt();
  ALT_TAB=       TAB.alt();
  ALT_INSERT=    INSERT.alt();
  ALT_DELETE=    DELETE.alt();
  ALT_HOME=      HOME.alt();
  ALT_END=       END.alt();
  ALT_UP=        UP.alt();
  ALT_DOWN=      DOWN.alt();
  ALT_RIGHT=     RIGHT.alt();
  ALT_LEFT=      LEFT.alt();
  ALT_PAGEUP=    UP.alt();
  ALT_PAGEDOWN=  DOWN.alt();

  F1=  Key::function( 1);
  F2=  Key::function( 2);
  F3=  Key::function( 3);
  F4=  Key::function( 4);
  F5=  Key::function( 5);
  F6=  Key::function( 6);
  F7=  Key::function( 7);
  F8=  Key::function( 8);
  F9=  Key::function( 9);
  F10= Key::function(10);
  F11= Key::function(11);
  F12= Key::function(12);

  SHIFT_F1=  Key::function( 1).shift();
  SHIFT_F2=  Key::function( 2).shift();
  SHIFT_F3=  Key::function( 3).shift();
  SHIFT_F4=  Key::function( 4).shift();
  SHIFT_F5=  Key::function( 5).shift();
  SHIFT_F6=  Key::function( 6).shift();
  SHIFT_F7=  Key::function( 7).shift();
  SHIFT_F8=  Key::function( 8).shift();
  SHIFT_F9=  Key::function( 9).shift();
  SHIFT_F10= Key::function(10).shift();
  SHIFT_F11= Key::function(11).shift();
  SHIFT_F12= Key::function(12).shift();

  CTRL_F1=  Key::function( 1).control();
  CTRL_F2=  Key::function( 2).control();
  CTRL_F3=  Key::function( 3).control();
  CTRL_F4=  Key::function( 4).control();
  CTRL_F5=  Key::function( 5).control();
  CTRL_F6=  Key::function( 6).control();
  CTRL_F7=  Key::function( 7).control();
  CTRL_F8=  Key::function( 8).control();
  CTRL_F9=  Key::function( 9).control();
  CTRL_F10= Key::function(10).control();
  CTRL_F11= Key::function(11).control();
  CTRL_F12= Key::function(12).control();

  ALT_F1=  Key::function( 1).alt();
  ALT_F2=  Key::function( 2).alt();
  ALT_F3=  Key::function( 3).alt();
  ALT_F4=  Key::function( 4).alt();
  ALT_F5=  Key::function( 5).alt();
  ALT_F6=  Key::function( 6).alt();
  ALT_F7=  Key::function( 7).alt();
  ALT_F8=  Key::function( 8).alt();
  ALT_F9=  Key::function( 9).alt();
  ALT_F10= Key::function(10).alt();
  ALT_F11= Key::function(11).alt();
  ALT_F12= Key::function(12).alt();

  CTRL_0= D0.control();
  CTRL_1= D1.control();
  CTRL_2= D2.control();
  CTRL_3= D3.control();
  CTRL_4= D4.control();
  CTRL_5= D5.control();
  CTRL_6= D6.control();
  CTRL_7= D7.control();
  CTRL_8= D8.control();
  CTRL_9= D9.control();

  ALT_0= D0.alt();
  ALT_1= D1.alt();
  ALT_2= D2.alt();
  ALT_3= D3.alt();
  ALT_4= D4.alt();
  ALT_5= D5.alt();
  ALT_6= D6.alt();
  ALT_7= D7.alt();
  ALT_8= D8.alt();
  ALT_9= D9.alt();

  CTRL_A= A.control();
  CTRL_B= B.control();
  CTRL_C= C.control();
  CTRL_D= D.control();
  CTRL_E= E.control();
  CTRL_F= F.control();
  CTRL_G= G.control();
  CTRL_H= H.control();
  CTRL_I= I.control();
  CTRL_J= J.control();
  CTRL_K= K.control();
  CTRL_L= L.control();
  CTRL_M= M.control();
  CTRL_N= N.control();
  CTRL_O= O.control();
  CTRL_P= P.control();
  CTRL_Q= Q.control();
  CTRL_R= R.control();
  CTRL_S= S.control();
  CTRL_T= T.control();
  CTRL_U= U.control();
  CTRL_V= V.control();
  CTRL_W= W.control();
  CTRL_X= X.control();
  CTRL_Y= Y.control();
  CTRL_Z= Z.control();

  ALT_A= A.alt();
  ALT_B= B.alt();
  ALT_C= C.alt();
  ALT_D= D.alt();
  ALT_E= E.alt();
  ALT_F= F.alt();
  ALT_G= G.alt();
  ALT_H= H.alt();
  ALT_I= I.alt();
  ALT_J= J.alt();
  ALT_K= K.alt();
  ALT_L= L.alt();
  ALT_M= M.alt();
  ALT_N= N.alt();
  ALT_O= O.alt();
  ALT_P= P.alt();
  ALT_Q= Q.alt();
  ALT_R= R.alt();
  ALT_S= S.alt();
  ALT_T= T.alt();
  ALT_U= U.alt();
  ALT_V= V.alt();
  ALT_W= W.alt();
  ALT_X= X.alt();
  ALT_Y= Y.alt();
  ALT_Z= Z.alt();
}

/////////////////////////////////////////////////////////////////////////////

