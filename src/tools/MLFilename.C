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
  MLFilename.C

  Stephane Rehel
  April 12 1997
*/

#include <pwd.h>
#include <sys/types.h>

#include "MLFilename.h"
#include "MLFileEntry.h"

/////////////////////////////////////////////////////////////////////////////

MLFilename::MLFilename()
{
  path= "";
  filename= "";
  extension= "";
}

/////////////////////////////////////////////////////////////////////////////

MLFilename::MLFilename( const MLString& fullname )
{
  MLString fn;
  MLFilename::split( fullname, path, fn );

  // now, fn does not contain any '/' char

  extension= "";

  int i= fn.length()-1;
  while( i >= 0 )
    {
    if( fn.charAt(i) == '.' )
      {
      // ok
      extension= fn.substring( i, fn.length()-1 );
      break;
      }
    --i;
    }

  filename= fn.substring( 0, fn.length() - extension.length() - 1 );
}

/////////////////////////////////////////////////////////////////////////////

MLFilename::MLFilename( const MLFilename& fn )
{
  operator = (fn);
}

/////////////////////////////////////////////////////////////////////////////

MLFilename& MLFilename::operator = ( const MLFilename& fn )
{
  path= fn.path;
  filename= fn.filename;
  extension= fn.extension;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLFilename::isAbsolute() const
{
  return path.isAbsolute();
}

/////////////////////////////////////////////////////////////////////////////

// split a complete filename
// We always have: fullname = path + filename
// Ex:
//    fullname = "/home/foousr/boo.c"
//    -> path = "/home/foousr/"
//       filename= "boo.c"
// Ex:
//    fullname = "/home/foousr/"
//    -> path = "/home/foousr/"
//       filename= ""
// Ex:
//    fullname = "/home/foousr"
//    -> path = "/home/"
//       filename= "foousr"
// Ex:
//    fullname = "boo.c"
//    -> path = ""
//       filename= "boo.c"

// static
void MLFilename::split( const MLString& fullname,
                        MLPath& path, MLString& filename )
{
  int i= fullname.length() - 1;

  while( i >= 0 )
    {
    if( fullname.charAt(i) == MLPath::separator )
      break;
    --i;
    }

  path= fullname.substring( 0, i );
  filename= fullname.substring( i+1, fullname.length()-1 );
}

/////////////////////////////////////////////////////////////////////////////

// if error, return FALSE
boolean MLFilename::operator == ( const MLFilename& _file2 ) const
{
  if( &_file2 == this )
    return true;

  MLString file1= (*this).get();
  MLString file2= _file2.get();

  if( file1 == file2 )
    return true;

  return MLFileEntry::sameFile(file1,file2);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLFilename::expand( const MLString& _filename,
                             boolean expand_current /* = true */ )
{
#ifdef vms
  return _filename;
#else

  if( _filename.length() == 0 )
    return _filename;

  if( _filename.charAt(0) != '~' )
    {
    if( _filename.charAt(0) == MLPath::separator )
      return _filename;

#ifdef __WINDOWS__
    if( _filename.length() >= 2 )
      {
      if( _filename.charAt(1) == ':' )
        {
	MLString wf= MLString("//") + 
                     MLString::singleChar(_filename.charAt(0)) +
                     MLString("/") +
                     _filename.substring(2,_filename.length()-1);
        return wf;
	}
      }
#endif

    if( expand_current )
      return MLPath::getCurrentDirectory().get() + _filename;
     else
      return _filename;
    }

  if( _filename.length() == 1 )
    return MLPath::getHomeDirectory().get();

  if( _filename.charAt(1) == MLPath::separator )
    {
    // Substitute ~ with $HOME
    return MLPath::getHomeDirectory().get()
           + _filename.substring(2,_filename.length()-1);
    }

  // Substitute ~ by home directory from password file

  MLString username("");
  int i= 1;
  for(;;)
    {
    username+= _filename.charAt(i);
    ++i;
    if( i == int(_filename.length()) )
      break;
    if( _filename.charAt(i) == MLPath::separator )
      break;
    }

  struct passwd* entry= ::getpwnam( username.get() );

  if( entry == 0 )
    return _filename; // unknown user...

  return MLString(entry->pw_dir)
         + _filename.substring(i,_filename.length()-1);

#endif
}

/////////////////////////////////////////////////////////////////////////////
