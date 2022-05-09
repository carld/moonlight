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
  MLScrollingDialog.h

  Stephane Rehel

  December 18 1996
*/

#ifndef __MLScrollingDialog_h
#define __MLScrollingDialog_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __KWCallbackClass_h
#include "kw/KWCallbackClass.h"
#endif

#ifndef __MLAllocObject_h
#include "MLAllocObject.h"
#endif

class KWDialog;
class MLModule;

/////////////////////////////////////////////////////////////////////////////

/**
 * This class implements the basic management and event handling for one of the
 * dialogs in the bar on the right side of the Moonlight3D window.
 */
class MLScrollingDialog: public MLAllocObject, public KWCallbackClass
{
  friend class MLScrollingDialogs;

protected:
  MLString name;
  const unsigned char* data; // dialog data
  KWDialog* kwDialog;
  MLModule* mlmodule;
  unsigned int updateFlags;

public:
  /**
   * The constructor takes a pointer to a MLModule.
   */
  MLScrollingDialog( MLModule* _mlmodule );

  /**
   * The destructor unregisters this class from the MLAllocator.
   */ 
  virtual ~MLScrollingDialog();

  /**
   * Returns the name of the dialog (which would be an empty string
   * for this class).
   */
  const MLString& getName() const
    {
    return name;
    }

  /**
   * This function is overloaded in the subclasses of MLScrollingDialog
   * to repaint the dialog window.
   */
  virtual void update() = 0;

  /**
   * This function is reimlemented from KWCallbackClass to implement
   * window closing behaviour.
   */
  virtual boolean kwCallback( KWEvent& );

  /**
   * Tells the MLModule instance this MLScrollingDialog is registered with
   * to remove this dialog by generating a MLInterfaceCommand.
   */
  void remove();

  /**
   * Returns the ID of the MLModule this MLScrollingDialog is registered with
   */
  int getModuleID() const;

  /**
   * Returns updateFlags, which is just 0 for this class
   */
  unsigned int getUpdateFlags() const
    {
    return updateFlags;
    }

  /**
   * An empty virtual function "called just after dialog creation" (original
   * comment).
   */
  virtual void justCreated()
    {}
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLScrollingDialog_h
