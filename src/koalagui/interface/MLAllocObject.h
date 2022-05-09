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
   MLAllocObject.h

   Stephane Rehel

   November 28 1996
*/

#ifndef __MLAllocObject_h
#define __MLAllocObject_h

/////////////////////////////////////////////////////////////////////////////

/**
 * A simple base class managing the ID of an object.
 *
 * Note: All code from this class is in MLAllocObject.h.
 * The MLAllocObject.C file is only a dummy.
 */
class MLAllocObject
{
  friend class MLAllocator;

private:
  /**
   * The ID of this object. It is set by MLAllocate::allocScrollingDialog()
   */
  int alloc_object_id;

public:
  /**
   * The constructor just sets alloc_object_id to 0.
   */
  MLAllocObject()
    {
    alloc_object_id= 0;
    }
  /**
   * The destructor just sets alloc_object_id to 0.
   */
  virtual ~MLAllocObject()
    {
    alloc_object_id= 0;
    }

  /**
   * Just returns the value of alloc_object_id.
   */

  inline int getID() const
    {
    return alloc_object_id;
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLAllocObject_h

