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
  MLProcedure.C

  Stephane Rehel
  March 30 1998
*/

#include "MLProcedure.h"
#include "MLProcProfile.h"

#include "MLAppRoot.h"
#include "MLNodeType.h"

/////////////////////////////////////////////////////////////////////////////

MLProcedure::MLProcedure( MLAppRoot* _approot,
                          MLNodeType::Type type /* = MLNodeType::APP_ROOT */ ):
  MLNode( _approot, _approot->getTypes()->getProcedures(type) )
{
  profile= new MLProcProfile(this);
  ref(profile);
}

/////////////////////////////////////////////////////////////////////////////

MLProcedure::~MLProcedure()
{
  unref(profile);
  profile= 0;
}

/////////////////////////////////////////////////////////////////////////////
