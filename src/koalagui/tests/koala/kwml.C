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
  kwml.C

  koala test
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "koala/VScrollingCartouche.h"
#include "koala/VScrollingDialogs.h"

#include "Dialog1Dlg.h"
#include "Dialog2Dlg.h"
#include "Dialog3Dlg.h"
#include "kw/KWDialog.h"

void buildScrolling( SystemWindow* systemParent )
{
  int right_margin= 0;
  int width= VScrollingCartouche::probeCartoucheSize(
                              IVector(Dialog1Dlg::width,Dialog1Dlg::height),
                              true ).x();

  VScrollingDialogs* sc= new VScrollingDialogs;
  sc->create( systemParent,
              systemParent->getSize().x() - VScrollingDialogs::getScrollBarWidth()
                                          - width - right_margin,
              5,
              width, systemParent->getSize().y()-5*2 );
  sc->map();

  KWDialog* d1= new KWDialog;
  d1->setScrollingCartouche();
  d1->create( sc, 0, 0, Dialog1Dlg::data, 0 );
  sc->append(d1);

  KWDialog* d2= new KWDialog;
  d2->setScrollingCartouche();
  d2->create( sc, 0, 0, Dialog2Dlg::data, 0 );
  sc->append(d2);

  KWDialog* d3= new KWDialog;
  d3->setScrollingCartouche();
  d3->create( sc, 0, 0, Dialog3Dlg::data, 0 );
  sc->append(d3);

}
