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
  InputStream.h

  Input Stream
  Stephane Rehel, January 22nd, 1996
*/

#ifndef __InputStream_h
#define __InputStream_h

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////

class InputStream
{
protected:
  const char* string;
  int string_index;

  FILE* file; // text or binary file
  long initial_file_position;

  int lineNumber;

  static const int buffer_size;
  int* buffer;
  int buffer_index;
  int* back_buffer;
  int back_buffer_nchars;

public:

  InputStream( const char* _string );

  InputStream( FILE* _file );

  virtual ~InputStream()
    {
    delete buffer; buffer= 0;
    delete back_buffer; back_buffer= 0;
    }

  int get(); // return -1 if eof reached

  void reset(); // reset to initial position

  void close(); // close file if a file is open

  int getLine() const
    {
    return lineNumber;
    }

  void backward();

  void unget( int ch );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __InputStream_h
