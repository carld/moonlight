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
  MLModes.h

  Stephane Rehel

  December 9 1996
*/

#ifndef __MLModes_h
#define __MLModes_h

class MLModule;
class MLMode;

/////////////////////////////////////////////////////////////////////////////

class MLModes
{
protected:
  MLModule* module;

  MLMode** modes;
  int nModes;

public:
  MLModes( MLModule* _module );
  virtual ~MLModes();

  MLMode* get( int id ) const;

  // MLModes becomes owner of *mode
  void add( int id, MLMode* mode );
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MLModes_h
