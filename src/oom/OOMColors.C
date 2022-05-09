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
  OOMColors.C

  Stephane Rehel

  December 2 1997
*/

#include "tools/MLGammaCorrection.h"

#include "OOMColors.h"

#include "OOMModule.h"

/////////////////////////////////////////////////////////////////////////////

// static
Color OOMColors::wire_background;
Color OOMColors::highlighted;
Color OOMColors::selected;
Color OOMColors::unselected;

/////////////////////////////////////////////////////////////////////////////

// static
void OOMColors::init( OOMModule* module )
{
#define GColor(r,g,b) Color(gc->do_f(r),gc->do_f(g),gc->do_f(b))

  MLGammaCorrection* gc= module->getGammaCorrection();

  double w_bk= gc->do_f(128.5/255.);
  wire_background= Color(w_bk,w_bk,w_bk);

  highlighted=         GColor(1,0,0);
  selected=            GColor(1,1,1);
  unselected=          GColor(0,0,0);

#undef GColor
}

/////////////////////////////////////////////////////////////////////////////
