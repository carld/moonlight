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
  DXFReader.h

  DXF file reader and converter:
     DXF file -->  DXFImport  -->  DXFExport --> any output (file, screen...)

  Revision:
    Stephane Rehel
    April 25 1997
*/

#ifndef __DXFReader_h
#define __DXFReader_h

#include <stdio.h>
#include <string.h>

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

#ifndef __Vector2_h
#include "tools/Vector2.h"
#endif

#ifndef __DXFPolyline_
#include "DXFPolyline.h"
#endif

class DXFExport;

/////////////////////////////////////////////////////////////////////////////

// DXFReader class
// Reads a dxf file, and send objects, entities... to DXFExport class
class DXFReader
{
private:
  FILE* fstderr;
  FILE* file;
  DXFExport* dxfExport;
  static const int maxLineLength;

  char* textLine;
  long int lineN;
  int groupCode;
  char curObject[81];
  char insert_name[81];
  char name[81];
/*
  boolean inBlock; // currently reading a block?
  boolean exportingBlock; // DXFExport::start_BLOCK() already called?
  boolean skipThisBlock; // skip current block?? i.e. AVE_RENDER for example
                      // skip: in BLOCK and INSERT commands
*/

  DXFPolyline polyline;

  struct COLOR
         {
         int index; // AutoCAD color index
         float r, g, b; // rgb values
         }
         color;

  Point coords[10];
  double floats[10], angles[10];
  int integers[10];

  struct {
         int xcoords, ycoords, zcoords, evelation, thickness,
             floats, repeatedValue, angles, followFlag, integers,
             extrusion, comments, cardinals, color;
         }
         entities;

  struct {
         double height, width;
         Point2 centerPoint;
         Vector direction;
         Point target;
         double lensLength, frontClip, backClip, twistAngle;
         }
         view;

  Vector blockBasePoint;

public:
  struct LAYER
         {
         int index;
         char* name;
         boolean off, frozen, locked;
         struct {
                int index; // AutoCAD color index
                float r, g, b; // rgb values
                }
                color;
         };

private:
  int nLayers;
  LAYER* layers;
  LAYER* curLayer;
  LAYER* defaultLayer;

public:
  DXFReader();
  ~DXFReader();

  void init( FILE* _file, DXFExport* _dxfExport, FILE* _fstderr= 0 );
  void flushObject();
  void read();
  void printStats();
  const Point getColor() const
    {
    return Point( double(color.r), double(color.g), double(color.b) );
//return Point( 1., 1., 1. );
    }
  LAYER* getLayer( const char* layerName );
  LAYER* getLayer( int layerIndex );
  LAYER* getLayer()
    {
    assert( curLayer != 0 );
    return curLayer;
    }
  int getNLayers() const
    {
    return nLayers;
    }
  int group70() const
    {
    return integers[0];
    }

private:
  boolean getLine();

  // find substrg in textLine; used for searching SECTION, ENDSEC, HEADER,...
  boolean find( const char* substrg ) const
    {
    return strstr( textLine, substrg ) != 0;
    }

  // find substrg in curObject; used for searching LINE, CIRCLE,...
  boolean findObject( const char* substrg ) const
    {
    return strstr( curObject, substrg ) != 0;
    }

  boolean readSection();
  boolean readTablesSection();
  boolean readViewPort();
  boolean readLayer();

  boolean readObject( boolean waitForFirst = false );
  boolean readPolyLine();
  boolean skipBlockTest( const char* blockName );
  boolean skipObjectTest( const char* objectName );
  boolean readEntitiesSection();
  boolean readBlocksSection();
  boolean readCardinalGroup();
  boolean readXCoord( int index );
  boolean readYCoord( int index );
  boolean readZCoord( int index );
  boolean readColor();
  boolean writePolylineObject();
  boolean writeInsertObject();
  boolean writeObject();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DXFReaderh
