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
  GWidget.h

  Stephane Rehel
  July 31 1996
*/

#ifndef __GWidget_h
#define __GWidget_h

#ifndef __VWidget_h
#include "koala/VWidget.h"
#endif

class Canvas;
class MLPicture;

/////////////////////////////////////////////////////////////////////////////

class GWidget
{
public:
  Canvas* canvas;
  VDialog* dialog;

  GWidget( Canvas* _canvas );
  virtual ~GWidget();
  void destroyWidget();

  void set( GWidget* gw );

  VWidget::TYPE type;
  boolean isCard; // speed up
  VCard* card;
  int cardIndex; // used by Canvas::recreate()

  VWidget* widget;

  MLString name;
  int id, linkWidget;
  IPoint position;
  IVector size;
  IVector minSize, maxSize, sizeable;

  MLString message;

  boolean available;
  boolean fixedFont, bigFont;
  boolean textList;
  double minValue, maxValue;
  int nDigits;
  double minValueBar, maxValueBar; // for ValueBar
  int editWidth; // for ValueBar
  int folderPlusMinus; // for FolderList

  MLPicture* picture;
  MLString pictureFilename;

// radio box
  SIList<MLString> list;

// label, checkbox, radiobox
  int margin;
  enum Justify { LEFT= 1, CENTER= 2, RIGHT= 3 };
  Justify justify;

// line, bar
  boolean vertical;

// list
  boolean scrolling;

// button
  boolean enterPXM;

  // METHODS //
  void newCard( const MLString& _message );
  void newPicture( const MLString& filename );
  void newButton( const MLString& _message );
  void newChooser();
  void newLabel( const MLString& _message );
  void newCheckBox( const MLString& _message );
  void newEdit();
  void newValueEdit();
  void newRadioButton( const MLString& _message );
  void newVertBar();
  void newHorizBar();
  void newVertScrollBar();
  void newHorizScrollBar();
  void newTextBox( const MLString& _message );
  void newScrollList();
  void newFolderList();
  void newHorizLine();
  void newVertLine();
  void newGroup( const MLString& _message );
  void newArea();
  void newProgress();
  void newRangeBar();
  void newValueBar();

  void create( boolean first = false, boolean alignIt = true );

  void updatePicture();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __GWidget_h

