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
  MLGlobalDialog.h

  Stephane Rehel
  April 20 1997
*/

#ifndef __MLGlobalDialog_h
#define __MLGlobalDialog_h

#ifndef __KWCallbackClass_h
#include "kw/KWCallbackClass.h"
#endif

class Interface;
class MLModule;
class KWDialog;

/////////////////////////////////////////////////////////////////////////////

/**
 * This class implements the basic event handling and management for global dialogs
 * that show up in the center of the screen.
 */
class MLGlobalDialog: public KWCallbackClass
{
protected:
  Interface* interface;
  MLModule* module;
  KWDialog* kwDialog;

public:
  /** 
   * Constructor for this class. Takes a pointer to an Interface.
   */
  MLGlobalDialog( Interface* _interface );
  /**
   * Constructor for this class. Takes a pointer to an MLModule.
   */
  MLGlobalDialog( MLModule* _module );

  /**
   * Destructor for this class. Calls close().
   */
  virtual ~MLGlobalDialog();

  boolean create( int x, int y, const unsigned char* dialogMap );
  boolean create( const unsigned char* dialogMap );

  /**
   * Destroys the KWDialog this global dialog owns.
   */
  void close();

  /**
   * Returns the KWDialog this global dialog owns.
   */
  KWDialog* getDialog() const
    {
    return kwDialog;
    }

  /**
   * Resets the buffer of registered dialogs, which is kept outside
   * this class, though.
   */
  static void resetAll();

protected:
  /**
   * Reimplements KWCallback::kwCallback, but only calls the parent's
   * function.
   */
  virtual boolean kwCallback( KWEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLGlobalDialog_h
