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
  MLProcProfile.C

  Stephane Rehel
  April 6 1998
*/

#include <stdarg.h>

#include "MLProcProfile.h"
#include "MLProcedure.h"

#include "MLProcArgs.h"
#include "MLPropertyType.h"
#include "MLProperty.h"

/////////////////////////////////////////////////////////////////////////////

MLProcProfile::MLProcProfile( MLProcedure* _procedure ):
  MLNode(_procedure,"profile")
{
//  MLNode::setName("profile");
}

/////////////////////////////////////////////////////////////////////////////

MLProcProfile::~MLProcProfile()
{
  for( int i= params.length(); i > 0; --i )
    {
    Param* p= params[i];
    params.remove(i);
    delete p;
    }
}

/////////////////////////////////////////////////////////////////////////////

int MLProcProfile::getNParams() const
{
  return params.length();
}

/////////////////////////////////////////////////////////////////////////////

void MLProcProfile::addParam( const MLString& name, int type )
{
  Param* p= new Param;
  p->name= name;
  p->type= type;
  params.append(p);
}

/////////////////////////////////////////////////////////////////////////////

MLProcArgs* MLProcProfile::buildArgs( va_list ap )
{
  MLProcArgs* args= new MLProcArgs(getKernel());

  for( int i= 1; i <= params.length(); ++i )
    {
    Param* p= params[i];
    switch(p->type)
      {
      case MLPropertyType::INTEGER:
        {
        args->newInteger(p->name,va_arg(ap,int));
        break;
        }
      }
    }

  return args;
}

/////////////////////////////////////////////////////////////////////////////
