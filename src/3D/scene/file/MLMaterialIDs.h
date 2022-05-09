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
  MLMaterialIDs.h

  Stephane Rehel

  September 14 1997
*/

#ifndef __MLMaterialIDs_h
#define __MLMaterialIDs_h

/////////////////////////////////////////////////////////////////////////////

class MLMaterialID
{
public:

  static const unsigned int signature;

  enum {
       FIRST                 = 0,
       DRIVER_VERSION        = 1,
       MATERIAL_NAMES        = 3,
       MATERIAL_DEF          = 4,
       MATERIAL_EMITTANCE    = 5,
       MATERIAL_DIFFUSE      = 6,
       MATERIAL_SPECULAR     = 7,
       MATERIAL_REFLECTION   = 8,
       MATERIAL_TRANSMISSION = 9,
       MATERIAL_INFO         = 10,
       LAST
       };
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLMaterialIDs_h
