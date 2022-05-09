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
  InputStream.C

  Input Stream
  Stephane Rehel, January 22nd, 1996
*/

#include <assert.h>

#include "InputStream.h"

/////////////////////////////////////////////////////////////////////////////

// static
const int InputStream::buffer_size= 16; // power of 2 please

/////////////////////////////////////////////////////////////////////////////

InputStream::InputStream( const char* _string )
{
  string= _string;
  string_index= 0;

  file= 0;
  initial_file_position= long(0);

  lineNumber= 1;

  buffer= new int [ buffer_size ];
  buffer_index= 0;

  back_buffer= new int [ buffer_size ];
  back_buffer_nchars= 0;
}

/////////////////////////////////////////////////////////////////////////////

InputStream::InputStream( FILE* _file )
{
  file= _file;
  if( file == 0 )
    initial_file_position= long(0);
   else
    initial_file_position= ftell(file);

  string= 0;
  string_index= 0;

  lineNumber= 1;

  buffer= new int [ buffer_size ];
  buffer_index= 0;

  back_buffer= new int [ buffer_size ];
  back_buffer_nchars= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return -1 if eof reached
int InputStream::get()
{
  int ch;

again:
  if( back_buffer_nchars > 0 )
    {
    ch= back_buffer[ --back_buffer_nchars ];
    goto ok;
    }

  if( file == 0 )
    {
    if( string == 0 )
      return -1;

    ch= int( (unsigned char) string[string_index] );
    if( ch == 0 )
      return -1;
    ++string_index;

    goto ok;
    }

  if( feof(file) )
    return -1;

  char ch0;
  if( fread( (void*) &ch0, sizeof(char), 1, file ) < 1 )
    return -1; // eof or error
  ch= (int) (unsigned char) ch0;

ok:
  if( ch == 13 ) // == ^M
    goto again; // MSDOS text file

  if( ch == int('\n') )
    ++lineNumber;

  if( ch == int('\t') )
    ch= int(' ');

  buffer[ buffer_index++ ]= ch;
  buffer_index &= buffer_size - 1;

  return ch;
}

/////////////////////////////////////////////////////////////////////////////

// reset to initial position
void InputStream::reset()
{
  if( file == 0 )
    {
    if( string == 0 )
      return;
    string_index= 0;
    }
   else
    fseek( file, initial_file_position, SEEK_SET );

  back_buffer_nchars= 0;
  buffer_index= 0;
}

/////////////////////////////////////////////////////////////////////////////

void InputStream::close()
{
  if( file != 0 && file != stdin )
    fclose(file);

  file= 0;
  initial_file_position= long(0);

  string= 0;
  string_index= 0;
}

/////////////////////////////////////////////////////////////////////////////

void InputStream::backward()
{
  assert( back_buffer_nchars < buffer_size );

  if( buffer_index == 0 )
    buffer_index= buffer_size - 1;
   else
    --buffer_index;
  int ch= buffer [ buffer_index ];

  unget(ch);
}

/////////////////////////////////////////////////////////////////////////////

void InputStream::unget( int ch )
{
  assert( back_buffer_nchars < buffer_size );

  back_buffer [ back_buffer_nchars++ ] = ch;

  if( ch == int('\n') )
    --lineNumber;
}

/////////////////////////////////////////////////////////////////////////////
