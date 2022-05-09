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
  printRenderTime.C

  Stephane Rehel

  August 3 1997
*/

#include "tools/Chrono.h"

#include "graphics/SystemWindow.h"

#include "ModelCanvas.h"
#include "ModelEngine.h"

#include "ModelModule.h"

/////////////////////////////////////////////////////////////////////////////

void ModelModule::printRenderTime()
{
  ModelCanvas* canvas= mcanvas[2];

  if( canvas == 0 )
    return;

  if( ! canvas->mapped() )
    {
    printMessage("Main window is not visible!\n");
    return;
    }

  ModelEngine* engine= (ModelEngine*) canvas->getEngine();
  if( engine == 0 )
    return;

  const MLString& engineName= engine->getName();
  const IVector& size= engine->getGfxWindow()->getSize();

  pushWaitCursor();

  Chrono chrono;
  int n= 0;

  chrono.start();
  for(;;)
    {
    engine->draw();
    ++n;
    if( chrono.peek() > 2.0 )
      break;
    }
  chrono.stop();

  popWaitCursor();

  double delta= chrono.elapsed() / double(n);
  if( delta <= 0. )
    delta= 1e-5;

  MLString s= MLString::printf(
                  "Render time for \"%s\" (%dx%d) : %.3f s, %.2f fps\n",
                  engineName.get(),
                  size.x(), size.y(),
                  delta, 1./delta );

  printMessage(s);
}

/////////////////////////////////////////////////////////////////////////////
