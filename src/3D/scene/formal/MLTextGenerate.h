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
  MLTextGenerate.h

  Stephane Rehel

  February 8 1998
*/

#ifndef __MLTextGenerate_h
#define __MLTextGenerate_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __GLTTGlyphTriangulator_h
#include "gltt/GLTTGlyphTriangulator.h"
#endif

class MLString;
class MLPolygonArray;
class GLTTFont;
class MeshRep;
class FormalText;

/////////////////////////////////////////////////////////////////////////////

class MLTextGenerate
{
protected:
  int nVertices;
  int nTriangles;
  boolean count_triangles;

  int triangle_index;
  MLPolygonArray* polygons;

  double front_z, back_z;
  const FormalText* ft;

  int back_vertices;

public:
  MLTextGenerate();

  virtual ~MLTextGenerate();

  boolean generate( MeshRep* mesh,
                  GLTTFont& font,
                  const FormalText* _ft );

protected:
  void triangle( FTGlyphVectorizer::POINT* p1,
                 FTGlyphVectorizer::POINT* p2,
                 FTGlyphVectorizer::POINT* p3 );

  friend class MLGlyphTriangulator;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLTextGenerate_h
