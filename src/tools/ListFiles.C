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
   ListFiles.C

   Creation: SR, August 23rd, 1995
   Revisions:
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>
#include <time.h>

#include "ListFiles.h"

// for path separator
#include "MLPath.h"

/////////////////////////////////////////////////////////////////////////////

MLString ListFiles::FileItem::getUser()
{
  return ListFiles::getUserName( (unsigned long) uid );
}

/////////////////////////////////////////////////////////////////////////////

MLString ListFiles::FileItem::getDateString()
{
  return ListFiles::getDateString(mtime);
}

/////////////////////////////////////////////////////////////////////////////

ListFiles::ListFiles()
{
  ngroups= 0;
  groups= 0;
}

/////////////////////////////////////////////////////////////////////////////

ListFiles::~ListFiles()
{
  while( ! files.empty() )
    {
    FileItem* fi= files.getFirst();
    files.removeFirst();
    delete fi;
    }

  delete groups;
  groups= 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString ListFiles::getUserName( unsigned long uid )
{
  struct passwd* entry= getpwuid( (uid_t) uid );

  if( entry == 0 )
    return MLString("unknown");

  if( entry->pw_name == 0 )
    return MLString("unknown");

  return MLString(entry->pw_name);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString ListFiles::getUserName()
{
  return ListFiles::getUserName( (unsigned long) getuid() );
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString ListFiles::getDateString( time_t tt )
{
  struct tm* t= localtime(&tt);
  if( t == 0 )
    return MLString("");

  char* months[12] = { "Jan",
                       "Feb",
                       "Mar",
                       "Apr",
                       "May",
                       "Jun",
                       "Jul",
                       "Aug",
                       "Sep",
                       "Oct",
                       "Nov",
                       "Dec" };

  // Returns "Apr 21 1997, 18:25"
  return MLString::printf( "%s %d %d, %02d:%02d",
                           months[t->tm_mon],
                           t->tm_mday,
                           t->tm_year + 1900, // Y2K?:-)
                           t->tm_hour,
                           t->tm_min );
}

/////////////////////////////////////////////////////////////////////////////

boolean ListFiles::inGroups( gid_t gid )
{
#ifdef __WINDOWS__
  return true;
#else
  if( groups == 0 )
    {
    ngroups= getgroups( 0, 0 );
    if( ngroups == -1 )
      return false;
    groups= new gid_t [ ngroups ];
    getgroups( ngroups, groups );
    }

  for( int i= 0; i < ngroups; ++i )
    if( groups[i] == gid )
      return true;
  return false;
#endif
}

/////////////////////////////////////////////////////////////////////////////

ListFiles::STATUS ListFiles::list( MLString& directory,
                                   const MLString& pattern,
                                   boolean alwaysIncludeDirs /* = true */ )
{
  while( ! files.empty() )
    {
    FileItem* fi= files.getFirst();
    files.removeFirst();
    delete fi;
    }

  int status;

  status= chdir(directory.get());
  if( status != 0 )
    return BAD_DIRECTORY;
   else
    {
    char d[2048];
    getcwd( d, 2048-1 );
    directory= d;
    }

  status= chdir(directory.get());
  if( status != 0 )
    return BAD_DIRECTORY;

  DIR* current_directory= opendir(directory.get());
  if( current_directory == 0 )
    return UNABLE_TO_OPENDIR;

  uid_t uid= getuid();

  for(;;)
    {
    struct dirent* entry=  readdir(current_directory);
    if( entry == 0 )
      break;
    MLString name(entry->d_name);
    assert( name.length() > 0 );

    struct stat file_info;
    stat( entry->d_name, &file_info );

    if( (S_ISDIR(file_info.st_mode) && alwaysIncludeDirs) ||
        name.glob(pattern) )
      {
      FileItem* fi= new FileItem;

      fi->name= name;
      
      fi->mode  = file_info.st_mode;
      fi->uid   = file_info.st_uid;
      fi->gid   = file_info.st_gid;
      fi->size  = file_info.st_size;
      fi->mtime = file_info.st_mtime;
      
      fi->isDirectory= S_ISDIR(file_info.st_mode) != 0;
      fi->isLink= S_ISLNK(file_info.st_mode) != 0;

      int in_groups= -1;

      if( fi->uid == uid )
        fi->canRead= (fi->mode & S_IRUSR) != 0;
       else
      if( (fi->mode & S_IROTH) != 0 )
        fi->canRead= true;
       else
      if( (fi->mode & S_IRGRP) != 0 )
        fi->canRead= (in_groups==-1) ? (in_groups=inGroups(fi->gid))
                                     : in_groups;
       else
        fi->canRead= false;

      if( fi->uid == uid )
        fi->canWrite= (fi->mode & S_IWUSR) != 0;
       else
      if( (fi->mode & S_IWOTH) != 0 )
        fi->canWrite= true;
       else
      if( (fi->mode & S_IWGRP) != 0 )
        fi->canWrite= (in_groups==-1) ? (in_groups=inGroups(fi->gid))
                                      : in_groups;
       else
        fi->canWrite= false;

      if( fi->uid == uid )
        fi->canExecute= (fi->mode & S_IXUSR) != 0;
       else
      if( (fi->mode & S_IXOTH) != 0 )
        fi->canExecute= true;
       else
      if( (fi->mode & S_IXGRP) != 0 )
        fi->canExecute= (in_groups==-1) ? (in_groups=inGroups(fi->gid))
                                        : in_groups;
       else
        fi->canExecute= false;

      IListElement<FileItem>* next= 0;
      IListIterator<FileItem> li(files);
      while( ! li.eol() )
        {
        IListElement<FileItem>* ile= li.nextElement();
        if( fi->name.compareTo(ile->element->name) <= 0 )
          {
          next= ile;
          break;
          }
        }
      if( next == 0 )
        files.append(fi);
       else
        files.insert( next, fi );
      }
    } // end for(;;)

  closedir(current_directory);

  return SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean ListFiles::canRead( const MLString& filename )
{
  int outcome= ::access( filename.get(), R_OK );

  return outcome == 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean ListFiles::canWrite( const MLString& filename )
{
  int outcome= ::access( filename.get(), W_OK );

  return outcome == 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean ListFiles::canExecute( const MLString& filename )
{
  int outcome= ::access( filename.get(), X_OK );

  return outcome == 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean ListFiles::exist( const MLString& filename )
{
  int outcome= ::access( filename.get(), F_OK );

  return outcome == 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean ListFiles::isAccessibleDirectory( const MLString& filename )
{
  struct stat file_info;

  int outcome= stat( filename.get(), &file_info );

  if( outcome != 0 )
    return false;

  if( S_ISDIR(file_info.st_mode) )
    return canRead(filename) && canExecute(filename);

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// static
boolean ListFiles::removeFile( const MLString& filename )
{
  return unlink( filename.get() ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

// static

// split a complete filename
// We always have: long_filename = path + filename
// Ex:
//    long_filename = "/home/foousr/boo.c"
//    -> path = "/home/foousr/"
//       filename= "boo.c"
// Ex:
//    long_filename = "/home/foousr/"
//    -> path = "/home/foousr/"
//       filename= ""
// Ex:
//    long_filename = "/home/foousr"
//    -> path = "/home/"
//       filename= "foousr"
// Ex:
//    long_filename = "boo.c"
//    -> path = ""
//       filename= "boo.c"
void ListFiles::splitFilename( const MLString& long_filename,
                               MLString& path,
                               MLString& filename )
{
  int i= long_filename.length() - 1;

  while( i >= 0 )
    {
    if( long_filename.charAt(i) == MLPath::separator )
      break;
    --i;
    }

  path= long_filename.substring( 0, i );
  filename= long_filename.substring( i+1, long_filename.length()-1 );
}


/////////////////////////////////////////////////////////////////////////////

// static

// from ImageMagick 3.6.5: magick/utility.c: ExpandFilename()
MLString ListFiles::expandFilename( const MLString& filename,
                                   boolean expand_current /* = true */ )
{
#ifdef vms
  return filename;
#else

  if( filename.length() == 0 )
    return filename;

  if( filename.charAt(0) != '~' )
    {
    if( filename.charAt(0) == MLPath::separator )
      return filename;

#ifdef __WINDOWS__
    if( filename.length() >= 2 )
      {
      if( filename.charAt(1) == ':' )
        {
        MLString wf= MLString("//") +
                     MLString::singleChar(filename.charAt(0));
        if( filename.charAt(2) != '/' )
          wf += MLString("/");
        wf += filename.substring(2,filename.length()-1);
        return wf;
        }
      }
#endif

    if( expand_current )
      return getCurrentDirectory() + filename;
     else
      return filename;
    }

  if( filename.length() == 1 )
    return getHomeDirectory();

  if( filename.charAt(1) == MLPath::separator )
    {
    // Substitute ~ with $HOME
    return getHomeDirectory() + filename.substring(2,filename.length()-1);
    }

  // Substitute ~ by home directory from password file

  MLString username("");
  int i= 1;
  for(;;)
    {
    username+= filename.charAt(i);
    ++i;
    if( i == int(filename.length()) )
      break;
    if( filename.charAt(i) == MLPath::separator )
      break;
    }

  struct passwd* entry= getpwnam( username.get() );

  if( entry == 0 )
    return filename; // unknown user...

  return MLString(entry->pw_dir) + filename.substring(i,filename.length()-1);

#endif
}

/////////////////////////////////////////////////////////////////////////////

// static
// for '/etc/rc.d/toto.foo.bar' returns: '.bar'
// for 'foo' returns: ''
// for 'foo.' returns: '.'
// for '.a' returns: '.a'
MLString ListFiles::getExtension( const MLString& filename )
{
  MLString path, fn;
  ListFiles::splitFilename( filename, path, fn );

  // now, fn does not contain any '/' chars

  int i= fn.length()-1;
  while( i >= 0 )
    {
    if( fn[i] == '.' )
      {
      // ok
      return fn.substring(i,fn.length()-1);
      }
    --i;
    }

  return MLString("");
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString ListFiles::getHomeDirectory()
{
  const char* home= getenv( "HOME" );
  if( home == 0 )
    {
problem:
    // return "/"
    char s[2];
    s[0]= MLPath::separator;
    s[1]= '\0';
    return MLString(s);
    }

  if( home[0] == '\0' )
    goto problem;

  if( home[ strlen(home)-1 ] != MLPath::separator )
    return MLString(home) + MLPath::separator;
   else
    return MLString(home);
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString ListFiles::getCurrentDirectory()
{
  char d[512+1];
  getcwd( d, 512 );

  if( d[0] == '\0' )
    {
    // Problem: return "/"
    char s[2];
    s[0]= MLPath::separator;
    s[1]= '\0';
    return MLString(s);
    }

  if( d[ strlen(d)-1 ] != MLPath::separator )
    return MLString(d) + MLPath::separator;
   else
    return MLString(d);
}

/////////////////////////////////////////////////////////////////////////////

// returns false if failed
// static
boolean ListFiles::changeToDirectory( const MLString& newDirectory )
{
  int status;
  status= chdir(newDirectory.get());

  return status == 0;
}

/////////////////////////////////////////////////////////////////////////////

// if error, return FALSE
boolean ListFiles::sameFile( const MLString& file1, const MLString& file2 )
{
  struct stat file1_info;
  struct stat file2_info;

  if( stat( file1.get(), &file1_info ) == -1 )
    return false;
  if( stat( file2.get(), &file2_info ) == -1 )
    return false;

  return (file1_info.st_ino == file2_info.st_ino) &&
         (file1_info.st_dev == file2_info.st_dev);
}

/////////////////////////////////////////////////////////////////////////////

// if error, return 0.
// in seconds
// static
double ListFiles::getLastModTime( const MLString& filename )
{
  struct stat file_info;
  if( stat( filename.get(), &file_info ) < 0 )
    return 0.;

  time_t last_modification= file_info.st_mtime;

  return double(last_modification);
}

/////////////////////////////////////////////////////////////////////////////

