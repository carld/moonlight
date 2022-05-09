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
  MLPixmaps.h

  Stephane Rehel
  July 9 1996
*/

#ifndef __MLPixmaps_h
#define __MLPixmaps_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

class MLPixmap;

/////////////////////////////////////////////////////////////////////////////

class MLPixmaps
{
public:
  static MLPixmap* check;
  static MLPixmap* checkedBox;
  static MLPixmap* uncheckedBox;
  static MLPixmap* emptyCircle;
  static MLPixmap* filledCircle;
  static MLPixmap* upArrow;
  static MLPixmap* downArrow;
  static MLPixmap* leftArrow;
  static MLPixmap* rightArrow;
  static MLPixmap* upArrowGray;
  static MLPixmap* downArrowGray;
  static MLPixmap* leftArrowGray;
  static MLPixmap* rightArrowGray;
  static MLPixmap* cross;
  static MLPixmap* emptyCross;
  static MLPixmap* window;
  static MLPixmap* sphere;
  static MLPixmap* smallWhiteArrow;
  static MLPixmap* smallGrayArrow;
  static MLPixmap* enterArrow;
  static MLPixmap* littleButton;
  static MLPixmap* littlePressedButton;

  static boolean init();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifnef __MLPixmaps_h

