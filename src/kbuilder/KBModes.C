// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  KBModes.C

  Stephane Rehel

  October 19 1997
*/
#include <string.h>

#include "KBModes.h"

#include "interface/MLModes.h"

#include "modes/KBMSelect.h"
#include "modes/KBMDrag.h"
#include "modes/KBMClick.h"

/////////////////////////////////////////////////////////////////////////////

// static
void KBModes::init( KBModule* kb, MLModes* m )
{
  if( m == 0 )
    return;

  m->add( SELECT, new KBMSelect(kb) );
  m->add( DRAG,   new KBMDrag(kb) );
  m->add( CLICK,  new KBMClick(kb) );
}

/////////////////////////////////////////////////////////////////////////////
