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
  XDRMem.h

  Stephane Rehel
  Creation: SR, March 1st, 1996
  Revisions:
   SR August 4 1996
*/

#ifndef __XDRMem_h
#define __XDRMem_h

#ifndef __XDROp_h
#include "XDROp.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class XDRMem: public XDROp
{
protected:
  unsigned int size;
  void* mem;

public:
  XDRMem();
  virtual ~XDRMem();

  boolean open_for_reading( void* _mem, unsigned int _size );
  boolean open_for_writing( void* _mem, unsigned int _size );

protected:
  void destroy();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __XDRMem_h
