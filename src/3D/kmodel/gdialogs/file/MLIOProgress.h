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
  MLIOProgress.h

  Stephane Rehel
  May 5 1997
*/

#ifndef __MLIOProgress_h
#define __MLIOProgress_h

class ModelModule;
class KWDialog;
class MLGlobalDialog;
class MLString;

/////////////////////////////////////////////////////////////////////////////

class MLIOProgress
{
protected:
  ModelModule* model;
  KWDialog* kwDialog;
  MLGlobalDialog* dialog;

  int progress;
  int label;

public:
  MLIOProgress( ModelModule* _model );
  virtual ~MLIOProgress();

  void create( const unsigned char* data, int _progress, int _label );
  void close();

  void empty();
  void full();

  void set( double d ); // d=0..1

  void setMessage( const MLString& msg );

  void flush();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifnef __MLIOProgress_h
