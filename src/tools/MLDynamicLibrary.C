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
  MLDynamicLibrary.C

  Stephane Rehel
  December 15 1998
*/

#include "config.h"

#ifdef __CYGWIN32__
 #include "dlfcn-cygnus.h"
#else
 #ifndef HAVE_DLFCN_H
  #error ok guy, it appears that you do not have dlfcn.h. You might be on a HP/UX. Please port tools/MLDynamicLibrary.C !!
 #endif
 #include <dlfcn.h>
#endif

#include "MLDynamicLibrary.h"

/////////////////////////////////////////////////////////////////////////////

MLDynamicLibrary::MLDynamicLibrary()
{
  error_message= "";
  filename= "";
  handle= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLDynamicLibrary::~MLDynamicLibrary()
{
  close();
  filename= "";
  error_message= "";
}

/////////////////////////////////////////////////////////////////////////////

void MLDynamicLibrary::close()
{
  if( handle != 0 )
    {
    dlclose(handle);
    handle= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

bool MLDynamicLibrary::open( const std::string& filename )
{
  close();

  this->filename=filename;

  handle= dlopen( filename.c_str(), RTLD_NOW ); // RTLD_GLOBAL?

  if( handle == 0 )
    {
    error_message= dlerror();
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void* MLDynamicLibrary::getfct( const std::string& name )
{
  if( handle == 0 )
    {
    error_message= "Library is not open";
    return 0;
    }

  void* f= dlsym( handle, name.c_str() );

  if( f == 0 )
    {
    error_message= dlerror();
    return 0;
    }

  return f;
}

/////////////////////////////////////////////////////////////////////////////
