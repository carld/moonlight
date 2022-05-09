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
  KWDialog.h

  The Koala Widget API (kWAPI)

  Stephane Rehel
  August 4 1996
*/

#ifndef __KWDialog_h
#define __KWDialog_h

#ifndef __kw_h
#include "kw.h"
#endif

#ifndef __SIList_h
#include "tools/SIList.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __IRectangle_h
#include "tools/IRectangle.h"
#endif

class VDialog;
class VWidget;
class SystemWindow;
class MLKeyboardHandler;
class MLEventGrabber;
class SystemWindow;
class VWindow;
class VButton;
class VCartouche;
class VScrollingCartouche;
class MLFont;

typedef unsigned int KWDialogID;

/////////////////////////////////////////////////////////////////////////////

class KWDialog: public KWCallbackClass
{
  friend class VDialog; // for handleCancelValidateKey()
  friend class VWidget; // for cancelID and validateID
  friend class KWDialogAllocator;
  friend class VScrollingDialogs;
  friend class VScrollingCartouche;

public:
  // exit code from Validate/Cancel dialogs
  enum
    {
    CANCEL= 0,
    VALIDATE= 1
    };

protected:
  KWDialogID dialogID;

  MLKeyboardHandler* previousKH;
  MLEventGrabber* previousEG;

  VCartouche* cartouche;
  VScrollingCartouche* scartouche;
  boolean scrolling;

  VDialog* dialog;

  KWCallbackClass* callbackClass;

  int exitCode;
  boolean quitMe;

  boolean validateCancel;
  int validateID, cancelID;

  struct KWGroup
    {
    int size;
    KWidget* widgets;
    };
  int nGroups;
  KWGroup* groups;
  MLString name;
  boolean create_unmapped;

public:
  KWDialog();
  virtual ~KWDialog();

  void setScrollingCartouche();

  // parent may be NULL for root
  boolean          create( SystemWindow* parent, int x, int y,
                         const unsigned char* dialogMap,
                         KWCallbackClass* _callbackClass,
                         boolean map_it = true );

  // parent may be NULL for root
  boolean          create( SystemWindow* parent,
                         const unsigned char* dialogMap,
                         KWCallbackClass* _callbackClass,
                         boolean map_it = true );

  void           drawDialog    ();
  int            run           ();
  void           quit          ( int _exitCode = 0 );

  const MLString& getName       () const { return name; }
  KWDialogID     getID         () const { return dialogID; }
  void           map           ( KWidget widget );
  void           unmap         ( KWidget widget );
  boolean          mapped        ( KWidget widget );

  void           setText       ( KWidget widget, const MLString& text );
  void           setChecked    ( KWidget widget,  boolean yes = true );
  void           setAvailable  ( KWidget widget, boolean yes = true );
  void           focus         ( KWidget widget );
  void           setValue      ( KWidget widget, double value );
  void           setInteger    ( KWidget widget, int i );
  boolean          setListText   ( KWidget widget, int i, const MLString& text );
  boolean          appendListText( KWidget widget, const MLString& text );
  boolean          insertListText( KWidget widget, int i, const MLString& text );
  boolean          deleteListText( KWidget widget, int i );
  void           clearList     ( KWidget widget );
  void*          getListUserData( KWidget widget, int i );
  int            getListUserDataIndex( KWidget widget, void* user_data );

  int            appendFolder    ( KWidget widget,
                                   int folder_id,
                                   const MLString& text,
                                   void* user_data = 0 );
  boolean          appendFolderItem( KWidget widget,
                                   int folder_id,
                                   const MLString& text,
                                   void* user_data = 0 );
  int            insertFolder    ( KWidget widget,
                                   int folder_id,
                                   int i,
                                   const MLString& text,
                                   void* user_data = 0 );
  boolean          insertFolderItem( KWidget widget,
                                   int folder_id,
                                   int i,
                                   const MLString& text,
                                   void* user_data = 0 );
  boolean          setFolderData( KWidget widget,
                                int folder_id,
                                int i, // i==0 for folder text
                                const MLString& text,
                                void* user_data = 0 );
  void*          getFolderUserData( KWidget widget, int folder_id, int i );
  int            getFolderUserDataIndex( KWidget widget,
                                         int folder_id,
                                         void* user_data );
  int            getFolderID( KWidget widget, int i );
  int            getFolderID( KWidget widget, int folder_id, int i );
  int            getFolderLength( KWidget widget, int folder_id );
  boolean          deleteFolder( KWidget widget, int folder_id );

  void           openFolder      ( KWidget widget,
                                   int folder_id,
                                   boolean yes = true );
  void           closeFolder     ( KWidget widget,
                                   int folder_id );

  void           setValueEditRanges( KWidget widget,
                                     double minValue,
                                     double maxValue,
                                     int nDigits );
  void           setRangeBarRanges( KWidget widget,
                                    double minValue, double maxValue );
  void           repaintArea( KWidget widget );

  IRectangle     getRectangle  ( KWidget widget );
  const MLString& getText       ( KWidget widget );
  boolean          getChecked    ( KWidget widget );
  boolean          getAvailable  ( KWidget widget );
  KWidget        getRadioChecked( KWidget radioFromGroup );
  double         getValue      ( KWidget widget );
  int            getInteger    ( KWidget widget );
  int            getListLength ( KWidget widget );
  const MLString&
                 getListText   ( KWidget widget, int i );
  int            getListTextIndex( KWidget widget, const MLString& text );

  void           activateCard  ( KWidget card );
  KWidget        getActiveCard ();

  static KWDialog* find( KWDialogID id );

  VWidget* getWidget( KWidget widget );

private:
  void validateDialog();
  void removeWidget( VWidget* w );
  KWGroup* getRadioGroup( KWidget widget );

  boolean handleValidateCancelKey( Key key );

  boolean kwDialogCallback( KWEvent& event );

public:
  static MLString** splitString( MLFont* font,
                                const MLString& string,
                                int max_width,
                                int nLines );
};

/////////////////////////////////////////////////////////////////////////////

// in KWError.C
// title + 4-line-message + cancel button
void KWError( const MLString& title, const MLString& message );

// in KWError.C
// title + 4-line-message + cancel button
void KWErrorf( const MLString& title, const char* format, ... );

// in KWWarningOkCancel.C
// title + 4-line-message + cancel button
// return boolean is Ok pressed
boolean KWWarningOkCancel( const MLString& message );
boolean KWWarningOkCancelf( const char* format, ... );

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KWDialog_h
