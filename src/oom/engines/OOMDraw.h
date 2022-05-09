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
  OOMDraw.h

  Stephane Rehel

  December 2 1997
*/

#ifndef __OOMDraw_h
#define __OOMDraw_h

class OOMECompose;
class OOPackage;
class OOClass;
class OORelation;

/////////////////////////////////////////////////////////////////////////////

class OOMDraw
{
protected:
  OOMECompose* compose;

public:
  OOMDraw( OOMECompose* _compose );

  virtual ~OOMDraw();

  void draw();

protected:
  void drawPackage( OOPackage* package );
  void drawClass( OOClass* the_class );
  void drawRelation( OORelation* relation );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OOMDraw_h
