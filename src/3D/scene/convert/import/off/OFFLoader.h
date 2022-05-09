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
  OFFLoader.h

  Stephane Rehel
  April 29 1997
*/

#ifndef __OFFLoader_h
#define __OFFLoader_h

#include <stdio.h>

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

class MLImporter;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

/**
 * A loader for MD2 files.
 */
class OFFLoader
{
private:
  MLImporter* importer;
  MLScene* scene;

  FILE* file;
  MLString filename;
  int lineN;
  char line[1024];

public:
  /**
   * Constructor for this class. It takes an MLImporter
   * as argument to which the imported data is passed on.
   */
  OFFLoader( MLImporter* _importer );

  /**
   * Destructor for this class.
   */
  virtual ~OFFLoader();

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
  /**
   * Helper used for parsing the input.
   */
  char* getline();
  /**
   * Helper used for parsing the input.
   */
  int getint( char*& l, int& i );
  /**
   * Helper used to output parsing errors.
   */
  void fatal_error();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __OFFLoader_h
