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
  MLConfig.h

  Stephane Rehel

  March 7 1998
*/

#ifndef __MLConfig_h
#define __MLConfig_h

#ifndef __boolean_h
#include "boolean.h"
#endif

#ifndef __MLString_h
#include "MLString.h"
#endif

#ifndef __SIList_h
#include "SIList.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class MLConfig
{
  friend class MLConfigWriter;

protected:
  MLString filename;

public:
  struct Variable
    {
    MLString name;
    MLString value; // parsed value

    MLString initial_value;
    };

  struct Group
    {
    MLString name;
    SIList<Variable> vars;
    };

protected:
  int max_groups;
  int nGroups;
  Group** groups;

public:
  MLConfig();

  virtual ~MLConfig();

  void setFilename( const MLString& _filename );

  const MLString& getFilename() const
    {
    return filename;
    }

  int getNGroups() const;
  const Group* getGroup( int i ) const;

  boolean testGroup( const MLString& name ) const
    {
    return findGroup(name) != 0;
    }

  void setVariable( const MLString& group,
                    const MLString& name, const MLString& value );

  // return "" if not found
  const MLString& getVariable( const MLString& group, const MLString& name );

  boolean getIntegerVariable( const MLString& group,
                            const MLString& name, int& result );
  boolean getRealVariable( const MLString& group,
                         const MLString& name, double& result );

protected:
  static MLString trim_name( const MLString& name );

  Group* findGroup( const MLString& name ) const;

  Group* addFindGroup( const MLString& name );

  void setVariable( Group* group,
                    const MLString& name, const MLString& value );

  // return "" if not found
  const MLString& getVariable( Group* group, const MLString& name );

  MLString parseVariable( Group* group,
                         const MLString& name,
                         const MLString& _value );

public:
  static MLString getenv( const MLString& name );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLConfig_h
