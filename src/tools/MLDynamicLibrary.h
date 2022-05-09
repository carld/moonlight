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
  MLDynamicLibrary.h

  Stephane Rehel
  December 15 1998
*/

#ifndef __MLDynamicLibrary_h
#define __MLDynamicLibrary_h

#include <string>

/////////////////////////////////////////////////////////////////////////////

class MLDynamicLibrary
{
protected:
  std::string filename;
  std::string error_message;

  void* handle;

public:
  MLDynamicLibrary();

  virtual ~MLDynamicLibrary();

  void close();

  std::string getRealFilename(const std::string &filename)
    {
      return "lib" + filename + ".so";
    }

  // return true if success
  bool open( const std::string& filename );

  // return 0 if error
  void* getfct( const std::string& name );

  const std::string& getFilename() const
    {
    return filename;
    }

  const std::string& getErrorMessage() const
    {
    return error_message;
    }

  void resetErrorMessage()
    {
    error_message= "";
    }
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLDynamicLibrary_h
