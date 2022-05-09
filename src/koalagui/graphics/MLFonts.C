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
  MLFonts.C

  Stephane Rehel
  July 18 1997
*/

#include "MLFonts.h"

/////////////////////////////////////////////////////////////////////////////

MLFont* normalFont= 0;
MLFont* smallFont= 0;
MLFont* widgetFont= 0;
MLFont* fixedWidgetFont= 0;
MLFont* widgetBoldFont= 0;
MLFont* widgetBigFont= 0;

/////////////////////////////////////////////////////////////////////////////

MLFont* InitFont_Small_1024();
MLFont* InitFont_Normal_1024();
MLFont* InitFont_Big_1024();

MLFont* InitFont_Widget_1024();
MLFont* InitFont_WidgetBold_1024();
MLFont* InitFont_WidgetBig_1024();
MLFont* InitFont_FixedWidget_1024();

/////////////////////////////////////////////////////////////////////////////

boolean MLFonts::init()
{
//  if( size.x() > 1000 )
    {
    normalFont= InitFont_Normal_1024();
    if( normalFont == 0 ) return false;
    }

    {
    smallFont= InitFont_Small_1024();
    if( smallFont == 0 ) return false;
    }

    {
    widgetFont= InitFont_Widget_1024();
    if( widgetFont == 0 ) return false;
    }

    {
    fixedWidgetFont= InitFont_FixedWidget_1024();
    if( fixedWidgetFont == 0 ) return false;
    }

    {
    widgetBoldFont= InitFont_WidgetBold_1024();
    if( widgetBoldFont == 0 ) return false;
    }

    {
    widgetBigFont= InitFont_WidgetBig_1024();
    if( widgetBigFont == 0 ) return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

