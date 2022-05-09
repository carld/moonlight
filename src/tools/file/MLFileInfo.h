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
  MLFileInfo.h

  Stephane Rehel

  September 28 1997
*/

#ifndef __MLFileInfo_h
#define __MLFileInfo_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLWFileBlock;
class MLRFileBlock;

/////////////////////////////////////////////////////////////////////////////

class MLFileInfo
{
public:
  MLString version; // which version?
  MLString user;    // who?

  MLString cpu; // which cpu?
  MLString vendor; // which vendor?
  MLString os; // which OS?

  unsigned long timeofday;  // when?

public:
  MLFileInfo();

  virtual ~MLFileInfo();

/*
  // version = 1
  static void write( const MLString& version,
                     const MLString& sysname,
                     const MLString& machine,
                     MLWFileBlock& block );
*/

  // version = 2
  static void write( const MLString& version,
                     const MLString& cpu,
                     const MLString& vendor,
                     const MLString& os,
                     MLWFileBlock& block );

  void read( MLRFileBlock& block );

  MLString getTimeOfDay() const;

protected:
  static MLString readString( MLRFileBlock& block );
  static void writeString( const MLString& s, MLWFileBlock& block );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLFileInfo_h
