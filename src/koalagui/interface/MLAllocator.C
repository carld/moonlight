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
  MLAllocator.C

  Stephane Rehel

  November 28 1996
*/

#include <stdio.h>

#include "kernel/MLKernel.h"

#include "Interface.h"
#include "KoalaNodeTypes.h"

#include "MLAllocator.h"

#include "MLModule.h"
#include "MLCanvas.h"
#include "MLEngine.h"
#include "MLScrollingDialog.h"

/////////////////////////////////////////////////////////////////////////////

// never called
MLAllocator::MLAllocator()
{}

/////////////////////////////////////////////////////////////////////////////

// never called
MLAllocator::~MLAllocator()
{}

/////////////////////////////////////////////////////////////////////////////

static const int max_scrolling_dialogs= 1000;
static MLScrollingDialog** scrolling_dialogs= 0;
static int nScrollingDialogs= 0;

Interface* MLAllocator::interface= 0;
MLKernel* MLAllocator::kernel= 0;

/////////////////////////////////////////////////////////////////////////////

// static
void MLAllocator::init( Interface* _interface )
{
  interface= _interface;
  kernel= _interface->getKernel();

  int i;

  assert( scrolling_dialogs == 0 );
  scrolling_dialogs= new MLScrollingDialog* [ max_scrolling_dialogs + 1 ];
  nScrollingDialogs= 0;
  for( i= 0; i <= max_scrolling_dialogs; ++i )
    scrolling_dialogs[i]= 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
int MLAllocator::allocScrollingDialog( MLScrollingDialog* sd )
{
  if( sd == 0 )
    return 0;

  if( nScrollingDialogs >= max_scrolling_dialogs )
    {
    fprintf( stderr, "ML panic: too many dialogs (%d)\n",
                     max_scrolling_dialogs );
    return 0;
    }

  if( scrolling_dialogs[nScrollingDialogs+1] == 0 )
    {
    int i= nScrollingDialogs+1;
    scrolling_dialogs[i]= sd;
    ++nScrollingDialogs;
    if( nScrollingDialogs >= max_scrolling_dialogs )
      nScrollingDialogs= 1;

    sd->alloc_object_id= i;
    return i;
    }

  for( int i= 1; i <= max_scrolling_dialogs; ++i )
    {
    if( scrolling_dialogs[i] != 0 )
      {
      scrolling_dialogs[i]= sd;
      ++nScrollingDialogs;
      sd->alloc_object_id= i;
      return i;
      }
    }

  return 0; // never reached
}

/////////////////////////////////////////////////////////////////////////////

// static
MLModule* MLAllocator::getModule( int id )
{
  MLNode* node= kernel->getNode(id);
  if( node == 0 )
    return 0;

  KoalaNodeTypes* types= interface->getKoalaNodeTypes();

  if( node->getNodeType() != types->module )
    return 0;

  return (MLModule*) node;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLCanvas* MLAllocator::getCanvas( int id )
{
  MLNode* node= kernel->getNode(id);
  if( node == 0 )
    return 0;

  KoalaNodeTypes* types= interface->getKoalaNodeTypes();

  if( node->getNodeType() != types->canvas )
    return 0;

  return (MLCanvas*) node;

}

/////////////////////////////////////////////////////////////////////////////

// static
MLEngine* MLAllocator::getEngine( int id )
{
  MLNode* node= kernel->getNode(id);
  if( node == 0 )
    return 0;

  KoalaNodeTypes* types= interface->getKoalaNodeTypes();

  if( node->getNodeType() != types->engine )
    return 0;

  return (MLEngine*) node;

}

/////////////////////////////////////////////////////////////////////////////

// static
MLScrollingDialog* MLAllocator::getScrollingDialog( int id )
{
  if( id < 1 || id > max_scrolling_dialogs )
    return 0;

  return scrolling_dialogs[id];
}

/////////////////////////////////////////////////////////////////////////////

// static
void MLAllocator::freeScrollingDialog( int id )
{
  if( id >= 1 && id <= max_scrolling_dialogs )
    scrolling_dialogs[id]= 0;
}

/////////////////////////////////////////////////////////////////////////////

// private static
void MLAllocator::freeScrollingDialog( MLScrollingDialog* sd )
{
  if( sd == 0 )
    return;

  assert( findScrollingDialog(sd) == sd->getID() ); // is it the good one?

  freeScrollingDialog(sd->getID());
}

/////////////////////////////////////////////////////////////////////////////

// private static
int MLAllocator::findScrollingDialog( MLScrollingDialog* sd )
{
  if( sd == 0 )
    return 0;

  for( int i= 1; i <= max_scrolling_dialogs; ++i )
    if( scrolling_dialogs[i] == sd )
      return i;

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

