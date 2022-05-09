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
  MLOperator.C

  Stephane Rehel
  March 24 1998
*/

#include "MLOperator.h"
#include "MLProperty.h"

/////////////////////////////////////////////////////////////////////////////

MLOperator::MLOperator( MLNode* _parent ): MLNode(_parent,"")
{
  need_to_be_updated= false;
  beeing_updated= false;
}

/////////////////////////////////////////////////////////////////////////////

MLOperator::~MLOperator()
{
  int i;

  for( i= sourceProps.length(); i > 0; --i )
    removeSourceProp( sourceProps.get(i) );

  for( i= destProps.length(); i > 0; --i )
    removeDestProp( destProps.get(i) );
}

/////////////////////////////////////////////////////////////////////////////

// this is for operators dependencies
void MLOperator::tree_to_be_updated()
{
  to_be_updated();

  // traverse destProps for further operators updating
  for( int i= 1; i <= destProps.length(); ++i )
    {
    MLProperty* p= destProps[i];
    if( p == 0 )
      continue;

    MLArray<MLOperator*>& destOps= p->destOps;
    for( int i= 1; i <= destOps.length(); ++i )
      {
      MLOperator* o= destOps[i];
      if( o == 0 )
        continue;

      if( o->need_to_be_updated )
        continue; // don't loop please

      o->tree_to_be_updated();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

// you should first call tree_to_be_updated() on all the operators
// before calling this function
boolean MLOperator::update( boolean& needed )
{
  if( beeing_updated )
    {
    needed= false;
    return true;
    }

  if( ! need_to_be_updated )
    {
    needed= false;
    return true;
    }

  beeing_updated= true;

  boolean error= false;

  // ask to source props if there are up to date
  for( int i= 1; i <= sourceProps.length(); ++i )
    {
    MLProperty* p= sourceProps[i];
    if( p == 0 )
      continue;

    MLOperator* sourceOp= p->sourceOp;
    if( sourceOp == 0 )
      continue; // this prop doesn't depend on an operator!?

    boolean n= false;
    boolean outcome= sourceOp->update(n);

    if( !outcome )
      {
      error= true;
      break;
      }
    }

  if( error )
    {
    beeing_updated= false;
    needed= true;
    return false;
    }

  if( ! _update(needed) )
    {
    beeing_updated= false;
    return false;
    }

  beeing_updated= false;
  need_to_be_updated= false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLOperator::addSourceProp( MLProperty* p )
{
  if( p == 0 )
    return;

  for( int i= 1; i <= sourceProps.length(); ++i )
    if( sourceProps.get(i) == p )
      return; // already have it

  ref(p);
  sourceProps.append(p);

  to_be_updated();

  p->addDestOp(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLOperator::removeSourceProp( MLProperty* p )
{
  if( p == 0 )
    return;

  int i;
  for( i= sourceProps.length(); i > 0; --i )
    if( sourceProps.get(i) == p )
      break; // found

  if( i < 0 )
    return; // not found

  sourceProps.remove(i);

  to_be_updated();

  p->removeDestOp(this);

  unref(p);
}

/////////////////////////////////////////////////////////////////////////////

void MLOperator::addDestProp( MLProperty* p )
{
  if( p == 0 )
    return;

  for( int i= 1; i <= destProps.length(); ++i )
    if( destProps.get(i) == p )
      return; // already have it

  ref(p);

  destProps.append(p);

  to_be_updated();

  p->setSourceOp(this);
}

/////////////////////////////////////////////////////////////////////////////

void MLOperator::removeDestProp( MLProperty* p )
{
  if( p == 0 )
    return;

  int i;
  for( i= destProps.length(); i > 0; --i )
    if( destProps.get(i) == p )
      break; // found

  if( i < 0 )
    return; // not found

  destProps.remove(i);

  to_be_updated();

  if( p->getSourceOp() == this )
    p->unsetSourceOp();

  unref(p);
}

/////////////////////////////////////////////////////////////////////////////

// static
// return true if at least one operator needs to be updated
boolean MLOperator::check_updating( MLArray<MLOperator*>& ops )
{
  boolean ok= false;

  int i;

  // check for the MLProperty::op_modified flag for updating operators
  // that depend on these properties
  for( i= 1; i <= ops.length(); ++i )
    {
    MLOperator* o= ops[i];
    if( o == 0 )
      continue;

    MLArray<MLProperty*>& sourceProps= o->sourceProps;
    boolean got_one= false;
    for( int j= 1; j <= sourceProps.length(); ++j )
      {
      MLProperty* p= sourceProps[j];
      if( p->op_modified )
        {
        got_one= true;
        break;
        }
      }

    if( got_one )
      {
      ok= true;
      o->to_be_updated();
      }
    }

  if( ! ok )
    return false;

  // clear all the MLProperty::op_modified flag
  for( i= 1; i <= ops.length(); ++i )
    {
    MLOperator* o= ops[i];
    if( o == 0 )
      continue;
    MLArray<MLProperty*>& sourceProps= o->sourceProps;
    for( int j= 1; j <= sourceProps.length(); ++j )
      sourceProps[j]->op_modified= false;
    }

  // set update flag in all operators from the dependency tree
  for( i= 1; i <= ops.length(); ++i )
    {
    MLOperator* o= ops[i];
    if( o == 0 )
      continue;
    if( o->need_to_be_updated )
      o->tree_to_be_updated();
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// static
// return true if at least one operator has been updated
boolean MLOperator::update( MLArray<MLOperator*>& ops )
{
  boolean got_one= false;

  // set update flag in all operators from the dependency tree
  for( int i= 1; i <= ops.length(); ++i )
    {
    MLOperator* o= ops[i];
    if( o == 0 )
      continue;
    if( o->need_to_be_updated )
      {
      boolean n;
      o->update(n);
      if( n )
        got_one= true;
      }
    }

  return got_one;
}

/////////////////////////////////////////////////////////////////////////////
