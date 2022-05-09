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
  Canvas.h

  Stephane Rehel
  July 30 1996
*/

#ifndef __Canvas_h
#define __Canvas_h

#include "tools/SIList.h"
#include "tools/IRectangle.h"
#include "tools/MLString.h"
#include "graphics/MLEventHandler.h"
#include "graphics/MLKeyboardHandler.h"
#include "kw/KWCallbackClass.h"

class VDialog;
class VWindow;
class MLWindow;
class MLEvent;
class VWidget;
class GWidget;
class SystemWindow;
class VCartouche;
class KWDialog;
class VCard;

class Xinfo;

class KBModule;
class KBCanvas;

/////////////////////////////////////////////////////////////////////////////

class Canvas: public MLEventHandler, public MLKeyboardHandler,
              public KWCallbackClass
{
  friend class GWidget;

public:
  static const int xorRectangleSize;
  static const int NOT_INTO;
  static const int INTO;
  static const int UP;
  static const int DOWN;
  static const int RIGHT;
  static const int LEFT;

  static MLString canvasExtension;

protected:
  Xinfo* xinfo;
  SystemWindow* root;
  SystemWindow* systemWindow;
  VCartouche* cartouche;
  KWDialog* popup;

  VDialog* dialog;
  VCard* activeCard;

  SIList<GWidget> widgets;
  SIList<GWidget> selectedWidgets;
  SIList<GWidget> clipboard;

  typedef SIList<GWidget> GGroup;
  SIList<GGroup> groups;

  IPoint dialogPosition;
  IVector dialogSize;
  boolean frame;
  boolean killButton;
  MLString title;
  MLString filename, name;
  MLString C_filename, h_filename;
  boolean has_C_filename, has_h_filename;
  boolean write_C_file;
  boolean validateCancel;
  int validateID, cancelID;
  int granularity, defaultGranularity;
  boolean drawGroups;

  IRectangle cardsRectangle;

  int mousePos;
  IVector drag_pos;
  IVector tmp_drag_pos;
  IPoint drag_origin;
  IPoint drag_origin2;
  int drag_pos_gravity;
  int dragging;

  boolean modified, h_modified;
  int top_margin, bottom_margin, right_margin, left_margin;

  KBCanvas* kbCanvas;
  KBModule* kbModule;

public:
  Canvas( SystemWindow* _root, KBCanvas* _kbCanvas );
  virtual ~Canvas();

  void destroy();
  void recreate();
  void create();

  void newDialog();

  // in CanvasDraw.C
  void drawInfos();
  void draw_now();
  void draw();
  void drawXORRectangle( const IRectangle& _rect,
                         boolean correct = true  );
  void xorRectangle( GWidget* gw, const IRectangle& rect );
  void xorSelected( GWidget* gw );
  void xorSelected();

  boolean isRadioBoxGroup( GGroup* g );
  GGroup* findGroup( GWidget* gw );
  void removeFromGroup( GWidget* gw );
  void selectGroup( GGroup* g, boolean toggle );
  GGroup* createGroup();

  GWidget* findWidget( int id );

  SIList<GWidget>& getWidgets()
    {
    return widgets;
    }

  // in CanvasMouse.C
  GWidget* getMousePosition( const IPoint& pos, int& mousePos );
  void mousePress( MLEvent& event );
  void mouseMotion( MLEvent& event );
  void mouseRelease( MLEvent& event );

  void moveWidgets( const IVector& displ );

  boolean eventHandler( MLWindow*, MLEvent& );
  boolean handleMLKeyboard( MLEvent& event );

  const MLString& getFilename() const
    {
    return filename;
    }

  void writeCanvasAs();
  void writeCanvas();
  boolean writeCanvas( const MLString& _filename );

  void readCanvas();
  boolean readCanvas( const MLString& _filename );

private:
  void _encodeCanvas();

public:
  int findEncodingLength();
  boolean encodeCanvas_C();
  boolean encodeCanvas_h();

  boolean hasCards();
  IRectangle getBounding();
  void changeDialogSize( const IVector& newSize );

  void makeLinks();

  virtual boolean kwCallback( KWEvent& e );

  static MLString getCardName( VCard* card );

  boolean editButtonCallback( KWEvent& e );
  boolean editButton( GWidget* gw );
  boolean editLabel( GWidget* gw );
  boolean editCheckBox( GWidget* gw );
  boolean editGroup( GWidget* gw );

  boolean editEditCallback( KWEvent& e );
  boolean editEdit( GWidget* gw );

  boolean editDialogCallback( KWEvent& e );
  boolean editDialog();

  boolean editLine( GWidget* gw );
  boolean editTextBox( GWidget* gw );

  boolean editListCallback( KWEvent& e );
  boolean editList( GWidget* gw );

  boolean editFolderListCallback( KWEvent& e );
  boolean editFolderList( GWidget* gw );

  boolean editArea( GWidget* gw );
  boolean editProgress( GWidget* gw );
  boolean editBar( GWidget* gw );
  boolean editRadioButton( GWidget* gw );

  boolean editAlignCallback( KWEvent& e );
  boolean editAlign();

  boolean editValueEditCallback( KWEvent& e );
  boolean editValueEdit( GWidget* gw );

//  void createPopup();
//  boolean popupCallback( KWEvent& e );
  int getGranularity() const
    {
    return granularity;
    }
  int getDefaultGranularity() const
    {
    return defaultGranularity;
    }
  void setGranularity( int g );
  void setActiveCard( VCard* card );
  VCard* getActiveCard() const
    {
    return activeCard;
    }

  void sortCards();

  boolean editPrefs();

  boolean editPicture( GWidget* gw );

  boolean editChooserCallback( KWEvent& e );
  boolean editChooser( GWidget* gw );

  boolean editRangeBar( GWidget* gw );

  boolean editValueBarCallback( KWEvent& e );
  boolean editValueBar( GWidget* gw );

// actions
  void deleteWidgets();
  void deleteCard();
  void end_newWidget( GWidget* w );
  void newCard( boolean insertIt = false );
  void newPicture();
  void newButton();
  void newChooser();
  void newLabel();
  void newCheckBox();
  void newEdit();
  void newValueEdit();
  void newRadioButton();
  void newVertBar();
  void newHorizBar();
  void newTextBox();
  void newList();
  void newFolderList();
  void newHorizLine();
  void newVertLine();
  void newGroup();
  void newArea();
  void newProgress();
  void newRangeBar();
  void newValueBar();
  void expandCanvas();
  void shrinkCanvas();
  void duplicateWidgets();
  void availableWidgets();
  void editWidget();
  void editDialogAction();
  void centerAllWidgets();
  void raiseWidget( boolean yes, GWidget* gw );
  void raiseWidgets( boolean yes );
  void groupWidgets();
  void ungroupWidgets();
  void alignWidgets( int horiz, int vert );
  boolean editCard();

  void copy_to_clipboard();
  void copy_from_clipboard();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Canvas_h
