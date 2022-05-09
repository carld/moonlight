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
  MLAllocator.h

  Stephane Rehel

  November 28 1996
*/

#ifndef __MLAllocator_h
#define __MLAllocator_h

class MLModule;
class MLCanvas;
class MLEngine;
class MLScrollingDialog;

class MLKernel;
class Interface;

/////////////////////////////////////////////////////////////////////////////

/**
 * This class keeps references to all MLScrollingDialog objects.
 *
 * This class only contains static members. Maybe it should be converted
 * into a namespace (and besides, this code really is a mess).
 */
class MLAllocator
{
  friend class MLScrollingDialog;

private:
  static Interface* interface;
  static MLKernel* kernel;

public:
  /**
   * This constructor is not used (it is an empty function).
   */
  MLAllocator();

  /**
   * This destructor is not used (it is an empty function).
   */
  virtual ~MLAllocator();

  /**
   * Initialize the MLAllocator with the given Interface and
   * create an array of pointers to MLScrollingDialog classes.
   */
  static void init( Interface* _interface );

  /**
   * Register a given MLScrollingDialog in the internal array
   * and return the index. It also sets the ID of the
   * MLScrollingDialog to the index value.
   */
  static int allocScrollingDialog( MLScrollingDialog* sd );

  /**
   * Retrieves the node with the given ID from the kernel and
   * returns a pointer to it if it is a MLModule. Otherwise
   * this function returns NULL.
   */
  static MLModule* getModule( int id );
  /**
   * Retrieves the node with the given ID from the kernel and
   * returns a pointer to it if it is a MLCanvas. Otherwise
   * this function returns NULL.
   */
  static MLCanvas* getCanvas( int id );
  /**
   * Retrieves the node with the given ID from the kernel and
   * returns a pointer to it if it is a MLEngine. Otherwise
   * this function returns NULL.
   */
  static MLEngine* getEngine( int id );
  /**
   * Returns the MLScrollingDialog taking the pointer from the
   * internal array from the index given by id.
   */
  static MLScrollingDialog* getScrollingDialog( int id );

  /**
   * Unregisters the given MLScrollingDialog from within the
   * internal array.
   */
  static void freeScrollingDialog( int id );

private:
  /**
   * Unregisters the given MLScrollingDialog from within the
   * internal array. This function is uses by
   * MLScrollingDialog::~MLScrollingDialog(), according the the
   * original comment
   */
  static void freeScrollingDialog( MLScrollingDialog* sd );
                                   // called by ~MLScrollingDialog

  /**
   * Retrieves the array index of a the given MLScrollingDialog.
   */
  static int findScrollingDialog( MLScrollingDialog* sd );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAllocator_h
