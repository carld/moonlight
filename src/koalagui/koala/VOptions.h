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
   VOptions.h

   Creation: SR, July 26th, 1995
   Revisions:
     + SR, July 5 1996
*/

#ifndef __VOptions_h
#define __VOptions_h

#ifndef __Color8_h
#include "tools/Color8.h"
#endif

class MLFont;

/////////////////////////////////////////////////////////////////////////////

struct VOptions
{
  Color8 colorGray0, colorGray1, colorGray2, colorGray3;
  double doubleClickDelay; // in seconds

  void gray0() const;
  void gray1() const;
  void gray2() const;
  void gray3() const;

  int unit;
  MLFont* fixedFont;
  MLFont* bigFont;

  struct Dialog
    {
    MLFont* font;
    int z; // thickness of dialogs
    } dialog;

  struct Button
    {
    MLFont* font;
    double pressingDuration; // in seconds
    double firstPressingDuration;
    double repeatPressingDuration;

    int z; // `thickness' of buttons
    char hotCharMark;
    } button;

  struct Label
    {
    MLFont* font;
    int bg_z;
    int fontHeight;
    } label;

  struct Arrow
    {
    int z; // `thickness' of arrow
    int bg_z; // `thickness' of background
    } arrow;

  struct ScrollBar
    {
    int width;
    int arrowsHeight;
    double pressingDuration; // in seconds

    int z; // `thickness' of buttons
    int arrows_z; // `thickness' of arrows

    int bg_z; // `thickness' of scrollbar background
    } scrollBar;

  struct List
    {
    MLFont* font;
    MLFont* font_i;
    MLFont* font_b;
    MLFont* font_bi;
    double slowScrollingDelay; // in seconds
    double fastScrollingDelay; // in seconds

    int bg_z; // `thickness'
    int horizMargin, vertMargin;
    } list;

  struct CheckBox
    {
    int z;
    } checkbox;

  struct Edit
    {
    MLFont* font;

    int z; // `thickness'
    int horizMargin, vertMargin;
    } edit;

  struct CircleButton
    {
    int z;
    } circleButton;

  struct Popup
    {
    MLFont* font;
    int fontHeight;
    double selectionDelay; // in seconds

    int z; // `thickness'
    int with_a_frame;
    int horizMargin, vertMargin;
    } popup;

  struct StatusBar
    {
    MLFont* font;
    int fontHeight;

    int z; // `thickness'
    int horizMargin, vertMargin;
    } statusbar;

  struct TextIO
    {
    MLFont* font;

    int z; // `thickness'
    int horizMargin, vertMargin;
    } textio;

  struct ScrollingDialogs
    {
    int z; // `thickness'

    struct
      {
      int z, x_margin, y_margin;
      MLFont* font;
      } titles;
    } scrollingDialogs;


private:
  int getFontHeight( MLFont* font );

public:
  VOptions();
  ~VOptions();

  void init();
};

/////////////////////////////////////////////////////////////////////////////

extern VOptions* vOptions;

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VOptions_h

