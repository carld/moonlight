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
  KBModule.h

  Stephane Rehel

  October 19 1997
*/

#ifndef __KBModule_h
#define __KBModule_h

#ifndef __MLModule_h
#include "interface/MLModule.h"
#endif

class MLCommandLine;

class KBCanvas;
class MLScene;
class MLCommand;
class KBCommand;
class MLPicture;
class KBStatusBars;
class Canvas;
class KBPopups;
class KBModuleKeyboardHandler;

/////////////////////////////////////////////////////////////////////////////

class KBModule: public MLModule
{
protected:
  KBCanvas* mcanvas;

  KBStatusBars* status;
  KBPopups* popups;

  KBModuleKeyboardHandler* keyboardHandler;

public:
  KBModule( Interface* _interface );

  virtual ~KBModule();

  virtual boolean init( MLAppRoot* _approot );

  void updatePopupsFlags();

  void updateStatusFlags();
  void updateFlags();

  virtual void idle();

  virtual void map();

  virtual void draw();

  virtual void handleCommand( MLCommand* cmd );
  virtual void handleKBCommand( KBCommand* cmd );

  virtual void enterMode( MLMode* mm );
  virtual void leaveMode( MLMode* mm = 0 );

  int getCanvasIndex( MLCanvas* c ) const;

  enum REFRESH_TYPE
    {
    REFRESH_BUILDER= (1<<0),
    REFRESH_DIALOGS= (1<<1),
    REFRESH_ALL=     ~0
    };

  virtual void refreshScreen( unsigned int what );

  void resetAll();

  virtual boolean createDialog( const MLString& name );

  Canvas* getBuilderCanvas() const;

  // color: 0 = gray
  //        1 = white
  //        2 = yellow
  virtual void printMessage( const MLString& message, int color = 1 );
  virtual void pushMessage( const MLString& message, int color = 1 );
  virtual void popMessage();

  virtual void printWidgetMessage( const MLString& message );

protected:
  void handlePopupCommand( int label );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __KBModule_h
