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
  MLIDs.C

  Stephane Rehel
  May 2 1997
*/

#include "MLAbstractMeshIDs.h"
#include "MLObjectIDs.h"
#include "MLSceneIDs.h"
#include "MLMaterialIDs.h"

/////////////////////////////////////////////////////////////////////////////

// static

const unsigned int MLAbstractMeshID::signature= 0x5f4d25e0;
const unsigned int         MLObjectID::signature= 0x5f4d25e1;
const unsigned int          MLSceneID::signature= 0x5f4d25e2;
//const unsigned int        modelModuleSignature= 0x5f4d25e3;
const unsigned int       MLMaterialID::signature= 0x5f4d25e4;
//const unsigned int       MLPackagedID::signature= 0x5f4d25e5;

/////////////////////////////////////////////////////////////////////////////

