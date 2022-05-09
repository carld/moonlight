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
  FormalText.h

  Stephane Rehel

  February 8 1998
*/

#ifndef __FormalText_h
#define __FormalText_h

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#ifndef __FormalRep_h
#include "FormalRep.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalText: public FormalRep
{
public:
  MLString text;
  MLString ttf_name;
  double precision;

  boolean side, front, back;
  double extrude_height;

  boolean reversed_sides;

public:
  FormalText( MLScene* _scene );

  virtual ~FormalText();

  virtual int getSizeOf() const;

  virtual FormalRep::TYPE getType() const;

  virtual void generate( MeshRep* mesh ) const;

  virtual FormalRep* duplicate();

protected:
  virtual void _read( MLRFileBlock* block );
  virtual void _write( MLWFileBlock* block );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalText_h
