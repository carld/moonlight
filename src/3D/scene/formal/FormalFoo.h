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
   FormalFoo.h

   March 1 1997
   Stephane Rehel
*/

#ifndef __FormalFoo_h
#define __FormalFoo_h

#ifndef __FormalRep_h
#include "FormalRep.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class FormalFoo: public FormalRep
{
  friend class FormalRep;

protected:
  FormalFoo( MLScene* _scene );

public:
  virtual ~FormalFoo();

  FormalRep::TYPE getType() const;
  void generate( MeshRep* mesh ) const;

  FormalRep* duplicate();

protected:
  virtual void _read( MLRFileBlock* /*block*/ ) {}
  virtual void _write( MLWFileBlock* /*block*/ ) {}
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __FormalFoo_h
