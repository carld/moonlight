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
  MLModelVersion.C

  Stephane Rehel

  August 3 1997
*/

#include "MLModelVersion.h"

#include "config.h"

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::version()
{
  return ML_VERSION;
}

/////////////////////////////////////////////////////////////////////////////

// static
int MLModelVersion::build_number()
{
  return ML_BUILD_NUMBER;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::build_time()
{
  return ML_BUILD_TIME;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::built_by()
{
  return ML_BUILT_BY;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::build_host()
{
  return ML_BUILD_HOST;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::build_cpu()
{
  return ML_BUILD_CPU;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::build_vendor()
{
  return ML_BUILD_VENDOR;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::build_os()
{
  return ML_BUILD_OS;
}

/////////////////////////////////////////////////////////////////////////////

// static
const char* MLModelVersion::build_compiler_version()
{
  return ML_BUILD_COMPILER_VERSION;
}

/////////////////////////////////////////////////////////////////////////////
