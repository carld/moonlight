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
  ASCLoader.h

  Creation: SR, February 9th, 1996

  Revision:
    Stephane Rehel
    April 27 1997
*/

/*
 * Rewrote the loader to use std::vector instead of the home-grown Array1
 * container class. Changes are still untested. -- Gregor, 2002-04-27
 */

#ifndef __ASCLoader_h
#define __ASCLoader_h

#include <vector>

#include <stdio.h>

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __MLString_h
#include "tools/MLString.h"
#endif

using namespace std;

class MLImporter;
class MLScene;

/////////////////////////////////////////////////////////////////////////////

/**
 * A loader for ASC files
 */
class ASCLoader
{
private:
  MLImporter* importer;
  MLScene* scene;

  FILE* file;
  MLString filename;
  int lineN;

public:
  /**
   * Constructor for this class. It takes an MLImporter
   * as argument to which the imported data is passed on.
   */
  ASCLoader( MLImporter* _importer );

  /**
   * Destructor for this class.
   */
  virtual ~ASCLoader();

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
   * Helper function for parsing the input
   */
  char* getLine();
  /**
   * Helper function for parsing the input
   */
  char* getNewLine();
  /**
   * Helper to find the given named object within the input
   */
  boolean findObject( MLString& name );
  /**
   * Reads a list of vertices from the input into the vector
   * of points passed to it.
   */
  boolean readVertexList( vector<Point>& vertices );
  /**
   * Reads a list of faces as indices into the list of vertices
   * read earlier by readVertexList. These faces are immediately
   * passed on the the instance MLImporter passed to the constructor
   * of this class.
   */
  boolean readFaceList( const MLString& object_name,
                      const vector<Point>& vertices, int nFaces );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __ASCLoader_h
