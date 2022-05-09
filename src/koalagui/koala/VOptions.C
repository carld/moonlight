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
   VOptions.C

   Creation: SR, July 26th, 1995
   Revisions:
     + SR, July 5 1996
*/

#include "GL/opengl.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/MLFont.h"
#include "graphics/MLFonts.h"

#include "VOptions.h"

/////////////////////////////////////////////////////////////////////////////

VOptions* vOptions= 0;

/////////////////////////////////////////////////////////////////////////////

VOptions::VOptions()
{}

/////////////////////////////////////////////////////////////////////////////

VOptions::~VOptions()
{}

/////////////////////////////////////////////////////////////////////////////

void VOptions::gray0() const { glColor3ubv( colorGray0.getConstCoord() ); }
void VOptions::gray1() const { glColor3ubv( colorGray1.getConstCoord() ); }
void VOptions::gray2() const { glColor3ubv( colorGray2.getConstCoord() ); }
void VOptions::gray3() const { glColor3ubv( colorGray3.getConstCoord() ); }

/////////////////////////////////////////////////////////////////////////////

int VOptions::getFontHeight( MLFont* font )
{
  if( font == 0 )
    return 0;

  return font->getHeight();
}

/////////////////////////////////////////////////////////////////////////////

void VOptions::init()
{
  GraphicsSystem* gs= GraphicsSystem::getGraphicsSystem();

  if( gs == 0 )
    return; // error

  colorGray0= Color8( gray_level_0, gray_level_0, gray_level_0 );
  colorGray1= Color8( gray_level_1, gray_level_1, gray_level_1 );
  colorGray2= Color8( gray_level_2, gray_level_2, gray_level_2 );
  colorGray3= Color8( gray_level_3, gray_level_3, gray_level_3 );

  doubleClickDelay= 200./1000.; // seconds

  unit= 18 * widgetFont->getHeight() / 14;
  fixedFont= fixedWidgetFont;
  bigFont= widgetBigFont;

  //
  // DIALOG
  //

// Fonte du menu de netscape:
// -adobe-helvetica-bold-r-normal-*-*-120-*-*-p-*-iso8859-*

//  dialog.font_name= "Screen15";
//  dialog.font_name= "-adobe-helvetica-medium-r-normal--17-120-100-100-p-88-iso8859-1";

// v0.1
//  dialog.font_name= "-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1";

//  dialog.font_name= "haebfix15";
  dialog.z= 0; //2;

//  dialog.title_font_name= "-*-helvetica-medium-r-*-*-16-*-*-*-*-*-*-*";
//  dialog.titleHeight= dialog.z*2 + getFontHeight(dialog.title_font_name);


// definitive choice:
//  dialog.font_name= "-adobe-helvetica-bold-r-normal-*-12-120-*-*-p-*-iso8859-*";
  dialog.font= widgetFont;

  //
  // BUTTON
  //

  button.font= dialog.font;
  button.pressingDuration= 50./1000.; // in seconds
  button.firstPressingDuration= 300./1000.; // in seconds
  button.repeatPressingDuration= 30./1000.; // in seconds

  button.hotCharMark= '&';

  button.z= 2; // `thickness' of buttons

  //
  // LABEL
  //

  label.font= dialog.font;
  label.bg_z= 2;

  label.fontHeight= getFontHeight(label.font);

  //
  // ARROW
  //

  arrow.z= 3; // `thickness' of arrow
  arrow.bg_z= 1; // `thickness' of scrollbar background

  //
  // SCROLLBAR
  //

  scrollBar.width= 16;
  scrollBar.arrowsHeight= scrollBar.width;

  scrollBar.z= 2; // `thickness' of buttons
  scrollBar.arrows_z= 3; // `thickness' of arrows
  scrollBar.bg_z= 2; // `thickness' of scrollbar background

  scrollBar.pressingDuration= button.pressingDuration;

  //
  // LIST
  //
// Definitive Choice
//  list.font_name= "-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1";
//  list.font_name_i= "-adobe-helvetica-medium-o-normal--14-140-75-75-p-77-iso8859-1";
//  list.font_name_b= "-adobe-helvetica-bold-r-normal--14-140-75-75-p-77-iso8859-1";
//  list.font_name_bi= "-adobe-helvetica-bold-o-normal--14-140-75-75-p-77-iso8859-1";
  list.font= widgetFont;
  list.font_i= widgetFont;
  list.font_b= widgetFont;
  list.font_bi= widgetFont;

  list.slowScrollingDelay= 200./1000.; // seconds
  list.fastScrollingDelay= 20./1000.; // seconds

  list.bg_z= 1;
  list.horizMargin= 5;
  list.vertMargin= 1;

  //
  // CHECKBOX
  //
  checkbox.z= 2;

  //
  // EDIT
  //
  edit.font= dialog.font;

  edit.z= 2; // `thickness'
  edit.horizMargin= 4;
  edit.vertMargin= 1;

  //
  // CIRCLEBUTTON
  //
  circleButton.z= 2;

  //
  // POPUP
  //

  // v0.1
  //popup.font_name= "-adobe-helvetica-medium-*-*--14-*-*-*-*-*-*-*";

// Definitive choice:
//  popup.font_name= "-adobe-helvetica-bold-r-normal--12-120-*-*-p-*-iso8859-*";
  popup.font= widgetFont;
  popup.fontHeight= getFontHeight(popup.font)+3;

  popup.z= 2; // `thickness'
  popup.horizMargin= 5;
  popup.vertMargin= 0;
  popup.selectionDelay= 100./1000.; // in seconds

  popup.with_a_frame= 0; // false

  //
  // STATUSBAR
  //

  //v0.1
  //statusbar.font_name= dialog.font_name;

// Definitive Choice:
//  statusbar.font_name= "-adobe-helvetica-bold-r-normal-*-12-120-*-*-p-*-iso8859-*";
  statusbar.font= widgetFont;
  statusbar.fontHeight= getFontHeight(statusbar.font)+1;

  statusbar.horizMargin= 2;
  statusbar.vertMargin= 2;

  //
  // TEXTIO
  //
//  textio.font_name= "-adobe-courier-medium-r-*--14-*-*-*-*-*-*-*";
//  textio.font_name= "-misc-fixed-medium-r-*-*-14-*-*-*-*-*-*-*";
  // v0.1
  //textio.font_name= "6x13";

// Definitive Choice:
//  textio.font_name= "-misc-fixed-medium-r-normal-*-*-120-*-*-c-*-iso8859-*";
  textio.font= normalFont;

  textio.z= 2;
  textio.horizMargin= 2;
  textio.vertMargin= 2;

  //
  // SCROLLINGDIALOGS
  //

  scrollingDialogs.z= 2; // `thickness'

  scrollingDialogs.titles.z= 1; // titles `thickness'
  scrollingDialogs.titles.x_margin= 4;
  scrollingDialogs.titles.y_margin= 2;

//  scrollingDialogs.titles.font_name=
//             "-adobe-helvetica-medium-o-normal-*-14-140-*-*-p-*-iso8859-*";

// Definitive Choice:
//  scrollingDialogs.titles.font_name=
//              "-adobe-helvetica-bold-r-normal--12-120-*-*-p-*-iso8859-*";

  scrollingDialogs.titles.font= widgetFont;
}

/////////////////////////////////////////////////////////////////////////////

