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
   VValueEdit.C

   Stephane Rehel
   August 13 1996
*/

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "graphics/MLFont.h"
#include "graphics/KeyCodes.h"

#include "VOptions.h"
#include "VValueEdit.h"

/////////////////////////////////////////////////////////////////////////////

VValueEdit::VValueEdit( VWindow* _parent ): VEdit(_parent)
{
  minValue= 0.;
  maxValue= 1.;
  nDigits= 3;
  value= 0.;
}

/////////////////////////////////////////////////////////////////////////////

VValueEdit::~VValueEdit()
{}

/////////////////////////////////////////////////////////////////////////////

void VValueEdit::create( int x, int y, int _width,
                         double _minValue,
                         double _maxValue,
                         int _nDigits,
                         double _value )
{
  VEdit::create( x, y, _width );

  value= _value;
  setRanges( _minValue, _maxValue, _nDigits );

  changeValue(value);
}

/////////////////////////////////////////////////////////////////////////////

void VValueEdit::setRanges( double _minValue,
                            double _maxValue,
                            int _nDigits )
{
  nDigits= max( 0, min( 80, _nDigits ) );

  minValue= min( _maxValue, _minValue );
  maxValue= max( _maxValue, _minValue );

  value= min( maxValue, max( value, minValue ) );

  changeValue(value);
}

/////////////////////////////////////////////////////////////////////////////

void VValueEdit::changeValue( double _value )
{
  value= min( maxValue, max( _value, minValue ) );

  if( fabs(value) > 100000. )
    {
    VEdit::changeEdit( MLString::printf( "%g", value ) );
    return;
    }

  if( nDigits == 0 )
    value= (value<0.) ? double(int(value-0.5)) : double(int(value+0.5));

  char buffer[100];
  sprintf( buffer, "%.*f", nDigits, value );

  if( strchr(buffer,'.') != 0 )
    {
    // remove trailing zero's
    int len= strlen(buffer);
    while( buffer[len-1] == '0' )
      {
      buffer[len-1]= '\0';
      --len;
      if( len == 0 )
        break;
      }

    if( len > 0 )
      if( buffer[len-1] == '.' )
        buffer[len-1]= '\0';
    }

  if( buffer[0] == '\0' )
    strcpy( buffer, "0" );

  VEdit::changeEdit(buffer);
}

/////////////////////////////////////////////////////////////////////////////

boolean VValueEdit::handleKey( Key key )
{
  boolean ok= false;

  if( key.isAscii() )
    {
    ok= (key.isDigit()          ||
         key == KeyCodes::PLUS  ||
         key == KeyCodes::MINUS ||
         key == Key('e')        ||
         key == Key('E')        ||
         key == KeyCodes::DOT   );
    if( getEdit().length() >= 32 )
      ok= false;
//    if( minValue >= 0 && key == KeyCodes::MINUS )
//      ok= false;
    if( nDigits == 0 && key == KeyCodes::DOT )
      ok= false;
    }
   else
    ok= true;

  if( key == KeyCodes::ENTER )
    parseValue();

  if( ok )
    return VEdit::handleKey(key);

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean VValueEdit::parseValue()
{
  const MLString& string= getEdit();

  boolean ok= false;

  if( string.length() == 0 )
    value= minValue;
   else
    {
    double d;
    int outcome= sscanf( string.get(), "%lf", &d );
    if( outcome != 1 )
      value= minValue;
     else
      {
      ok= true;
      value= d;
      }
    }

  changeValue(value);

  return ok;
}

/////////////////////////////////////////////////////////////////////////////

void VValueEdit::setCurrent( boolean yes )
{
  if( (isCurrent()?1:0) == (yes?1:0) )
    return;

  if( ! yes )
    parseValue();
  VWidget::setCurrent(yes);
  enableSelection(yes);
}

/////////////////////////////////////////////////////////////////////////////

