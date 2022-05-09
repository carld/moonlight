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
  MLKernelVersion.h

  Stephane Rehel

  March 31 1998
*/

#ifndef __MLKernelVersion_h
#define __MLKernelVersion_h

/////////////////////////////////////////////////////////////////////////////

/**
 * This class contains many informations about the building host and the
 * used sources.
 */
class MLKernelVersion
{
public:
  /** the whole version string (major,minor,release) of moonlight */
  static const char* version();
  /** the build number (ie days 'til last release) */
  static int         build_number();
  /** time & date moonlight was built */
  static const char* build_time();
  /** the user who built moonlight */
  static const char* built_by();
  /** the host moonlight was built on */
  static const char* build_host();

  /** the cpu this build is optimized for */
  static const char* build_cpu();
  static const char* build_vendor();
  /** the operating system moonlight was built for */
  static const char* build_os();

  /** the compiler version used */
  static const char* build_compiler_version();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLKernelVersion_h
