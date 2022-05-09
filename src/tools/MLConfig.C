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
  MLConfig.C

  Stephane Rehel

  March 7 1998
*/

// getenv()
#include <stdlib.h>

#include <stdio.h>

#include "MLConfig.h"

/////////////////////////////////////////////////////////////////////////////

MLConfig::MLConfig()
{
  filename= "";

  nGroups= 0;
  max_groups= 8;
  groups= new Group* [ max_groups ];

  for( int i= 0; i < max_groups; ++i )
    groups[i]= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLConfig::~MLConfig()
{
  if( groups != 0 )
    {
    for( int i= 0; i < nGroups; ++i )
      {
      Group* g= groups[i];
      if( g == 0 )
        continue;

      SIList_destroyElements(Variable,g->vars)

      delete g;
      groups[i]= 0;
      }

    delete groups;
    groups= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLConfig::setFilename( const MLString& _filename )
{
  filename= _filename;
}

/////////////////////////////////////////////////////////////////////////////

int MLConfig::getNGroups() const
{
  return nGroups;
}

/////////////////////////////////////////////////////////////////////////////

const MLConfig::Group* MLConfig::getGroup( int i ) const
{
  if( groups == 0 )
    return 0;

  if( i < 0 || i >= nGroups )
    return 0;

  return groups[i];
}

/////////////////////////////////////////////////////////////////////////////

void MLConfig::setVariable( const MLString& _group,
                            const MLString& _name, const MLString& value )
{
  MLString group= trim_name(_group);
  MLString name= trim_name(_name);

  Group* g= addFindGroup(group);

  if( g == 0 )
    return;

  setVariable(g,name,value);
}

/////////////////////////////////////////////////////////////////////////////

// return "" if not found
const MLString& MLConfig::getVariable( const MLString& _group,
                                      const MLString& _name )
{
  MLString group= trim_name(_group);
  MLString name= trim_name(_name);

  Group* g= findGroup(group);

  return getVariable(g,name);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLConfig::getIntegerVariable( const MLString& group,
                                    const MLString& name, int& result )

{
  result= 0;

  const MLString& v= getVariable(group,name);
  if( v.length() == 0 )
    return false;

  if( sscanf(v.get(),"%d",&result) != 1 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLConfig::getRealVariable( const MLString& group,
                                 const MLString& name, double& result )
{
  result= 0.;

  const MLString& v= getVariable(group,name);
  if( v.length() == 0 )
    return false;

  if( sscanf(v.get(),"%lf",&result) != 1 )
    return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLConfig::trim_name( const MLString& name )
{
  return name.trim();
}

/////////////////////////////////////////////////////////////////////////////

MLConfig::Group* MLConfig::findGroup( const MLString& _name ) const
{
  MLString name= trim_name(_name);

  for( int i= 0; i < nGroups; ++i )
    {
    Group* g= groups[i];
    if( g == 0 )
      continue;

    if( g->name == name )
      return g;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

MLConfig::Group* MLConfig::addFindGroup( const MLString& _name )
{
  MLString name= trim_name(_name);

  Group* g= findGroup(name);

  if( g != 0 )
    return g;

  if( nGroups >= max_groups )
    {
    int new_max_groups= max_groups + max_groups / 2;
    Group** new_groups= new Group* [ new_max_groups ];
    int i;
    for( i= 0; i < max_groups; ++i )
      new_groups[i]= groups[i];
    for( i= max_groups; i < new_max_groups; ++i )
      new_groups[i]= 0;

    delete groups;
    groups= new_groups;

    max_groups= new_max_groups;
    }

  g= new Group;
  g->name= name;

  groups[nGroups]= g;
  ++nGroups;

  return g;
}

/////////////////////////////////////////////////////////////////////////////

void MLConfig::setVariable( Group* group,
                            const MLString& _name, const MLString& value )
{
  MLString name= trim_name(_name);

  if( group == 0 || _name.length() == 0 )
    return; // silly boy

  MLString parsed_value= parseVariable(group,name,value);

  SIListIterator<Variable> li(group->vars);
  while( ! li.eol() )
    {
    Variable* v= li++;
    if( v->name == name )
      {
      v->value= parsed_value;
      v->initial_value= value;
      return;
      }
    }

  Variable* v= new Variable;
  v->name= name;
  v->value= parsed_value;
  v->initial_value= value;

  group->vars.append(v);
}

/////////////////////////////////////////////////////////////////////////////

// return "" if not found
const MLString& MLConfig::getVariable( Group* group, const MLString& _name )
{
  if( group == 0 )
    return MLString::empty();

  MLString name= trim_name(_name);

  SIListIterator<Variable> li(group->vars);
  while( ! li.eol() )
    {
    Variable* v= li++;

    if( v->name == name )
      return v->value;
    }

  return MLString::empty();
}

/////////////////////////////////////////////////////////////////////////////

// Variable syntax:
//
//  var=value -> var={value}
//  var=$HOME/a -> var={/home/foo/a}
//  var1=%var/a -> var1={value/a}
//  var1=foo$$bar -> var1={foo$bar}
//
MLString MLConfig::parseVariable( Group* group,
                                 const MLString& _name,
                                 const MLString& _value )
{
  MLString name= trim_name(_name);

  MLString value= _value.trim();
  MLString result= "";

  for( int i= 0; i < value.length(); ++i )
    {
    char ch= value.charAt(i);

    if( ch == '$' || ch == '%' )
      {
      ++i;
      if( i >= value.length() )
        continue;

      if( value.charAt(i) == ch )
        {
        // this is a $$ or %%
        result += ch;
        continue;
        }

      // get var name
      MLString v= "";
      while( i < value.length() )
        {
        char ch1= value.charAt(i);
        boolean ok= (ch1 >= '0' && ch1 <= '9') ||
                  (ch1 >= 'A' && ch1 <= 'Z') ||
                  (ch1 >= 'a' && ch1 <= 'z') || (ch1 == '_');
        if( ! ok )
          break;
        v += ch1;
        ++i;
        }

      --i;
      if( ch == '$' )
        result += getenv(v);
       else
        {
        if( v != name )
          result += getVariable(group,v);
        }
      continue;
      }

    result += ch;
    }

  return result;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString MLConfig::getenv( const MLString& name )
{
  if( name.length() == 0 )
    return "";

  char* s= ::getenv(name.get());
  if( s == 0 )
    return "";

  return MLString(s);
}

/////////////////////////////////////////////////////////////////////////////
