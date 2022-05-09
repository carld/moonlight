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
  OOMModule.h

  Stephane Rehel

  November 3 1997
*/

#ifndef __OOMModule_h
#define __OOMModule_h

#ifndef __MLModule_h
#include "interface/MLModule.h"
#endif

class Command;

class OOMCanvas;
class MLScene;
class MLCommand;
class OOMCommand;
class MLPicture;
class OOMStatusBars;
class OOMPopups;
class OOModel;

/////////////////////////////////////////////////////////////////////////////

class OOMModule: public MLModule
{
protected:
  OOMCanvas* mcanvas[5];

  OOMStatusBars* status;

  OOMPopups* popups;

  OOModel* model;

public:
  OOMModule( Interface* _interface );

  virtual ~OOMModule();

  virtual boolean init( MLAppRoot* _approot );

  OOModel* getModel() const
    {
    return model;
    }

  void updatePopupsFlags();
  void updateStatusFlags();
  void updateFlags();

  virtual void idle();

  virtual void map();

  virtual void draw();

  virtual void handleCommand( MLCommand* cmd );
  virtual void handleOOMCommand( OOMCommand* cmd );

  virtual void enterMode( MLMode* mm );
  virtual void leaveMode( MLMode* mm = 0 );

  int getCanvasIndex( MLCanvas* c ) const;

  enum REFRESH_TYPE
    {
    REFRESH_COMPONENTS=       (1<<0),
    REFRESH_ALL=         ~0
    };

  virtual void refreshScreen( unsigned int what );

  void resetAll();

  virtual boolean createDialog( const MLString& name );

  // color: 0 = gray
  //        1 = white
  //        2 = yellow
  virtual void printMessage( const MLString& message, int color = 1 );
  virtual void pushMessage( const MLString& message, int color = 1 );
  virtual void popMessage();

protected:
  void handlePopupCommand( int label );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOMModule_h
