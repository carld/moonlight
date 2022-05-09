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
  MLIOProgress.C

  Stephane Rehel
  May 7 1997
*/

#include <math.h>
#include <stdio.h>

#include "Common.h"
#include "Chrono.h"
#include "MLIOProgress.h"

/////////////////////////////////////////////////////////////////////////////

static const double update_delay= 0.1; // 1/10 second

/////////////////////////////////////////////////////////////////////////////

MLIOProgress::MLIOProgress()
{
  min_value= 0.;
  max_value= 1.;
  d= 0.;
  msg= "";
  last_time= 0.;
}

/////////////////////////////////////////////////////////////////////////////

MLIOProgress::~MLIOProgress()
{}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::setRange( double _min_value, double _max_value )
{
  min_value= min( _min_value, _max_value );
  max_value= max( _min_value, _max_value );

  if( fabs(min_value-max_value) < 1e-6 )
    max_value= min_value + 1e-6;
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::empty()
{
  set(min_value);
  _flush();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::full()
{
  set(max_value);
  _flush();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::set( double _d )
{
  d= (_d-min_value) / (max_value-min_value);
  if( d < 0. ) d= 0.;
  if( d > 1. ) d= 1.;
  _flush();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::add( double _d )
{
  d += _d / (max_value-min_value);
  if( d < 0. ) d= 0.;
  if( d > 1. ) d= 1.;
  _flush();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::setMessage( const MLString& _msg )
{
  msg= _msg;
  _flush();
}

/////////////////////////////////////////////////////////////////////////////

void MLIOProgress::_flush()
{
#ifndef NDEBUG
//  fprintf( stderr, "MLIOProgress: '%s' %2.1f%%\n",msg.get(),d*100.);
#endif

  double t= Chrono::clock();

  if( (t-last_time) > ::update_delay )
    {
    last_time= t;
    flush();
    }
}

/////////////////////////////////////////////////////////////////////////////
