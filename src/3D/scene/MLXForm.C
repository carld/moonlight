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
  MLXForm.C

  Simple ML transformation matrix

  Stephane Rehel

  December 13 1996
*/

#include "MLXForm.h"

/////////////////////////////////////////////////////////////////////////////

MLXForm::MLXForm()
{
  scale= Vector(1,1,1);
  rotate= Vector(0,0,0);
  translate= Vector(0,0,0);
}

/////////////////////////////////////////////////////////////////////////////

MLXForm& MLXForm::operator = ( const MLXForm& fx )
{
  preXForm= fx.preXForm;

  scale= fx.scale;
  rotate= fx.rotate;
  translate= fx.translate;

  transform= fx.transform;

  return *this;
}

/////////////////////////////////////////////////////////////////////////////

void MLXForm::update()
{
  transform.reset();

  transform.scale(scale);
  transform.rotateX(rotate.x());
  transform.rotateY(rotate.y());
  transform.rotateZ(rotate.z());
  transform.translate(translate);

  transform.set( preXForm.get_o2w() * transform.get_o2w(),
                 transform.get_w2o()* preXForm.get_w2o() );
}

/////////////////////////////////////////////////////////////////////////////

