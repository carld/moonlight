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
  Md2Loader.h

  From Stephane Rehel's ASCLoader.h

  phooky, 2/6/98

*/

#ifndef __Md2Loader_h
#define __Md2Loader_h

#include <stdio.h>
#include "../../formats/md2types.h"

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

#define MD2_SCALE 0.1

class MLImporter;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

/**
 * A loader for MD2 files.
 */
class Md2Loader
{
private:
  MLImporter* importer;
  MLScene* scene;

  FILE* file;
  MLString filename;

  dmdl_t header;

public:
  /**
   * Constructor for this class. It takes an MLImporter
   * as argument to which the imported data is passed on.
   */
  Md2Loader( MLImporter* _importer );

  /**
   * Destructor for this class.
   */
  virtual ~Md2Loader();

  /**
   * This method does finial initalisation and let's the
   * MLImporter open the file the data is going to be read from.
   */
  boolean init();
  /**
   * This method actually loads the data from the given file.
   */
  boolean load();

private:
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Md2Loader_h
