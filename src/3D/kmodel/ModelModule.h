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
  ModelModule.h

  Stephane Rehel

  November 15 1996
*/

#ifndef __ModelModule_h
#define __ModelModule_h

#ifndef __MLModule_h
#include "interface/MLModule.h"
#endif

class MLCommandLine;

class ModelCanvas;
class MLScene;
class MLCommand;
class ModelCommand;
class ModelScene;
class MLCamera;
class MLLightingProcess;
class MLThread;
class MLDatabase;
class MLPicture;
class ModelStatusBars;
class ModelFlags;
class ModelRender;
class ModelPopups;
class ModelLogListener;
class MLLog;
class ModelCommandLine;
class MLStatusLineEntryLogging;

/////////////////////////////////////////////////////////////////////////////

class ModelModule: public MLModule
{
  friend class ModelModuleIO;
  friend class MLLoadProject;
  friend class MLSaveProject;
  friend class MLDSavePackaged;
  friend class ModelPopups;

protected:
  ModelCanvas* mcanvas[5];
  MLScene* scene;
  ModelScene* modelScene;

  ModelStatusBars* status;

  MLLightingProcess* lightingProcess;
  MLThread* lightingThread;

  ModelFlags* flags;
  ModelPopups* popups;

  ModelRender* modelRender;

  ModelLogListener* modelLogListener;
  ModelCommandLine* modelCommandLine;
  MLStatusLineEntryLogging* entryLogging;
  MLLog* log;

public:
  enum LIGHTING_STATUS
    {
    LS_DISABLED= 1, // no lighting process at all
    LS_ASLEEP,      // lighting process on, but nothing to do
    LS_WORKING1,     // lighting process on and working
    LS_WORKING2      // lighting process on and working
    };

public:
  ModelModule( Interface* _interface );

  virtual ~ModelModule();

  MLScene* getScene() const
    {
    return scene;
    }
  ModelScene* getModelScene() const
    {
    return modelScene;
    }
  ModelFlags* getFlags() const
    {
    return flags;
    }

  MLLog* getLog() const
    {
    return log;
    }
  MLStatusLineEntryLogging* getEntryLogging() const
    {
    return entryLogging;
    }

  virtual boolean init();

  void updatePopupsFlags();
  void updateStatusFlags();
  void updateFlags();

  virtual void idle();

  virtual void map();

  virtual void draw();

  virtual void handleCommand( MLCommand* cmd );
  virtual void handleModelCommand( ModelCommand* cmd );

  virtual void enterMode( MLMode* mm );
  virtual void leaveMode( MLMode* mm = 0 );

  int getCanvasIndex( MLCanvas* c ) const;

  enum REFRESH_TYPE
    {
    REFRESH_CAMERA=       (1<<0),
    REFRESH_GEOMETRY=     (1<<1),
    REFRESH_TRANSFORM=    (1<<2),
    REFRESH_HIER=         (1<<3),
    REFRESH_ILLUM=        (1<<4),
    REFRESH_SELECTION=    (1<<5),
    REFRESH_RAYTRACE=     (1<<6),
    REFRESH_DISPLAY_TYPE= (1<<7),
    REFRESH_MAIN_CAMERA=  (1<<8),
    REFRESH_NAMES=        (1<<9),
    REFRESH_DIALOGS=      (1<<10),
    REFRESH_MATERIALS=    (1<<11),
    REFRESH_WIRE=         (1<<12),
    REFRESH_PREVIEW=      (1<<13),
    REFRESH_ALL=          ~0
    };

  virtual void refreshScreen( unsigned int what );
  void refreshMainCamera( const MLCamera& camera );

  virtual boolean createDialog( const MLString& name );

  const MLCamera& getMainCamera() const;

  // f==0 if leaving current
  void enteringFunction( MLFunction* );

  // return 0 if none selected or more than a single one
  int getSingleSelected();

  boolean isProjectEmpty();

  MLLightingProcess* getLightingProcess() const
    {
    return lightingProcess;
    }
  void stopLightingProcess();
  void runLightingProcess();
  void disableLighting();
  void enableLighting();

  // color: 0 = gray
  //        1 = white
  //        2 = yellow
  virtual void printMessage( const MLString& message, int color = 1 );
  virtual void pushMessage( const MLString& message, int color = 1 );
  virtual void popMessage();

protected:
  void handlePopupCommand( int label );

  // in ModelModuleFile.C
//  virtual void write( MLWFileBlock& block );
//  virtual void read( MLRFileBlock& block );
//  MLString getProjectFilename( MLDatabase* database );
//  boolean write( MLDatabase* database );
//  boolean read( const MLString& filename, MLDatabase* database );
//  MLPicture* readSnapshot( const MLString& filename );

  void resetFlags();

public:
  virtual void resetAll();

protected:
  void printRenderTime(); // in printRenderTime.C
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ModelModule_h
