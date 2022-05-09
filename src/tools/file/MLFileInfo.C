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
  MLFileInfo.C

  Stephane Rehel

  September 28 1997
*/

#include "MLFileInfo.h"

#include "MLRFileBlock.h"
#include "MLWFileBlock.h"

#include "tools/Chrono.h"
#include "tools/MLFileEntry.h"

/////////////////////////////////////////////////////////////////////////////

static const int info_version= 2;

/////////////////////////////////////////////////////////////////////////////

MLFileInfo::MLFileInfo()
{
  version= "";
  user= "";
  cpu= "";
  vendor= "";
  os= "";
  timeofday= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLFileInfo::~MLFileInfo()
{}

/////////////////////////////////////////////////////////////////////////////

/*
// version = 1
// static
void MLFileInfo::write( const MLString& version,
                        const MLString& sysname,
                        const MLString& machine,
                        MLWFileBlock& block )
{
  MLString user= ListFiles::getUserName();
  unsigned long timeofday= Chrono::gettimeofday();

  block << int(::info_version);
  writeString( version, block );
  writeString( user, block );
  writeString( sysname, block );
  writeString( machine, block );

  block << timeofday;
}
*/

/////////////////////////////////////////////////////////////////////////////

// version = 2
// static
void MLFileInfo::write( const MLString& version,
                        const MLString& cpu,
                        const MLString& vendor,
                        const MLString& os,
                        MLWFileBlock& block )
{
  MLString user= MLFileEntry::getUserName();
  unsigned long timeofday= Chrono::gettimeofday();

  block << int(::info_version);

  writeString( version, block );
  writeString( user, block );
  writeString( cpu, block );
  writeString( vendor, block );
  writeString( os, block );

  block << timeofday;
}

/////////////////////////////////////////////////////////////////////////////

void MLFileInfo::read( MLRFileBlock& block )
{
  int v= block.getInteger(); // info_version

  switch(v)
    {
    case 1:
      {
      version= readString(block);
      user= readString(block);
      os= readString(block); // was sysname (version 1)
      cpu= readString(block); // was machine (version 1)
      vendor= os;
      break;
      }
    case 2:
    default:
      {
      version= readString(block);
      user= readString(block);
      cpu= readString(block);
      vendor= readString(block);
      os= readString(block);
      break;
      }
    }

  block >> timeofday;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLFileInfo::getTimeOfDay() const
{
  return MLFileEntry::getDateString( long(timeofday) );
}

/////////////////////////////////////////////////////////////////////////////

// static
void MLFileInfo::writeString( const MLString& s, MLWFileBlock& block )
{
  block << int(s.length());

  unsigned int u= s.length();
  for( int i= 0; i < s.length(); ++i )
    {
    unsigned int ch= (unsigned int) (unsigned char) s.charAt(i);
    ch += u;
    block << ch;
    u+= u * u * (unsigned int)(i+1);
    }
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLFileInfo::readString( MLRFileBlock& block )
{
  MLString s("");

  int length;
  block >> length;
  if( length <= 0 || length > 1024 )
    return s;

  s= MLString(length);
  unsigned int u= length;
  for( int i= 0; i < length; ++i )
    {
    unsigned int ch;
    block >> ch;
    ch -= u;
    s[i]= (unsigned char) ch;
    u+= u * u * (unsigned int)(i+1);
    }

  return s;
}

/////////////////////////////////////////////////////////////////////////////
