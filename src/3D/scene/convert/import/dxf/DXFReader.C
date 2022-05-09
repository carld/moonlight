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
  DXFReader.C

  DXF file reader and converter:
     DXF file -->  DXFImport  -->  DXFExport --> any output (file, screen...)

  Revision:
    Stephane Rehel
    April 25 1997
*/

#include <stdlib.h>

#include "DXFExport.h"
#include "DXFPolyline.h"

#include "DXFReader.h"

/////////////////////////////////////////////////////////////////////////////

const int DXFReader::maxLineLength= 1024;

/////////////////////////////////////////////////////////////////////////////

DXFReader::DXFReader()
{
  file = 0;
  textLine= 0;
  polyline.vertices= 0;
  polyline.points= 0;
  layers= 0;
  defaultLayer= 0;
}

/////////////////////////////////////////////////////////////////////////////

DXFReader::~DXFReader()
{
  delete textLine;
  textLine= 0;
  delete polyline.vertices;
  polyline.vertices= 0;
  delete polyline.points;
  polyline.points= 0;

  if( layers != 0 )
    {
    for( int i= 0; i< nLayers; ++i )
      free( layers[i].name );
    delete layers;
    layers= 0;
    }

  delete defaultLayer;
  defaultLayer= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return false if error
// _dxfExport is assumed to be initialized
void DXFReader::init( FILE* _file, DXFExport* _dxfExport,
                      FILE* _fstderr /*= 0*/ )
{
  assert( _file != 0 );
  file= _file;
  fstderr= _fstderr;
  polyline.fstderr= fstderr;

  if( textLine == 0 )
    textLine= new char [ maxLineLength +1 ];
  assert( textLine != 0 );
  dxfExport= _dxfExport;

  polyline.vertices= new DXFPolyline::Vertex [ DXFPolyline::maxVertices ];
  assert( polyline.vertices != 0 );
  polyline.nVertices= 0;

  lineN= 1;
  groupCode= 0;

  entities.xcoords= entities.ycoords= entities.zcoords=
  entities.evelation= entities.thickness= entities.floats=
  entities.repeatedValue= entities.angles= entities.followFlag=
  entities.integers= entities.extrusion= entities.comments=
  entities.cardinals= entities.color= 0;

  blockBasePoint.zero();

  layers= 0;
  nLayers= 0;
  defaultLayer= new LAYER;
  assert( defaultLayer != 0 );

  defaultLayer->index= 0;
  defaultLayer->name= "0";
  defaultLayer->off= false;
  defaultLayer->frozen= false;
  defaultLayer->locked= false;
  defaultLayer->color.index= 7;
  defaultLayer->color.r= 1.;
  defaultLayer->color.g= 1.;
  defaultLayer->color.b= 1.;

  flushObject();
}

/////////////////////////////////////////////////////////////////////////////

void DXFReader::flushObject()
{
  curLayer= defaultLayer;
  *curObject= 0;
  color.index= 7; // default color: white
  color.r= color.g= color.b= 1.;
  for( int i= 0; i< 10; ++i )
    {
    coords[i]= Point( 0., 0., 0. );
    floats[i]= 0.;
    angles[i]= 0.;
    integers[i]= 0;
    }
  polyline.thickness= 0.;
  polyline.extrusionDir= Vector(0.,0.,1.);
}

/////////////////////////////////////////////////////////////////////////////

// read group code and the next line
// return false if error
boolean DXFReader::getLine()
{
  fgets( textLine, maxLineLength-1, file );
  ++lineN;
  if( feof( file ) )
    {
reach_eof:
    if( fstderr != 0 )
      {
      fprintf( fstderr, "unexpected end of file\n" );
      fflush(fstderr);
      }
    return false;
    }
  char* s= textLine;
  while( *s == ' ' ) ++s;
  sscanf( s, "%d", &groupCode );
  fgets( textLine, maxLineLength+1, file );
  ++lineN;
  if( feof( file ) )
    goto reach_eof;
  int len= strlen( textLine );
  if( len > 0 )
    if( textLine[ len - 1 ] == '\n' )
      textLine[ len - 1 ]= 0;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

// read dxf file, and send objects to dxfExport
// return false if error
void DXFReader::read()
{
  for(;;)
    {
    if( ! getLine() )
      {
PrintError:
      if( fstderr != 0 )
        {
        fprintf( fstderr, "Error line %ld\n", lineN );
        fflush(fstderr);
        }

      return;
      }
    if( groupCode == 0 )
      {
      if( find( "EOF" ) )
        break;
      if( find( "SECTION" ) )
        {
        if( !readSection() )
          goto PrintError;
        goto next;
        }
      }
     else
      {
      if( fstderr != 0 )
        {
        fprintf( fstderr, "Warning: ignored group code %d (need 0) line %ld\n",
                 groupCode, lineN-2 );
        fflush(fstderr);
        }
      }
next:;
    }
  dxfExport->end_MAIN();
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readSection()
{
  getLine();
  assert( groupCode == 2 );

  if( find( "TABLES" ) )
    {
    readTablesSection();
    goto theEnd;
    }
  if( find( "ENTITIES" ) )
    {
    dxfExport->start_MAIN();
    readObject(); // start to read the first entity name
                  // eg. 3DFACE, POLYLINE...
    readEntitiesSection();
    goto theEnd;
    }
  if( find( "BLOCKS" ) )
    {
    readBlocksSection();
    goto theEnd;
    }

  if( fstderr != 0 )
    {
    fprintf( fstderr, "Warning: skipping %s section found line %ld\n",
             textLine, lineN-1 );
    fflush(fstderr);
    }
  // now wkip this section: wait for ENDSEC
  do
    getLine();
  while( !( groupCode == 0 && find( "ENDSEC" ) ) );

theEnd:
  assert( find( "ENDSEC" ) );
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readTablesSection()
{
  for(;;)
    {
    readObject();

    if( find( "ENDSEC" ) )
      break;

    if( find( "TABLE" ) )
      {
      getLine();
      assert( groupCode == 2 );
      if( find( "VPORT" ) )
        {
        if( ! readViewPort() )
          return false;
        goto next;
        }
      if( find( "LAYER" ) )
        {
        if( ! readLayer() )
          return false;
        goto next;
        }

      if( fstderr != 0 )
        {
        fprintf( fstderr, "Warning: skipping %s table found line %ld\n",
                 textLine, lineN-1 );
        fflush(fstderr);
        }
      do
        readObject();
      while( ! find("ENDTAB" ) ); // wait for end of current table
      }
next:;
    }


  return true;
}

/////////////////////////////////////////////////////////////////////////////

// read VPORT header in TABLES section
boolean DXFReader::readViewPort()
{
  readObject();
//  int nViewPorts= integers[0]; // group 70

  for(;;)
    {
    if( find( "ENDTAB" ) )
      break;

    if( find( "VPORT" ) )
      {
      floats[0]= floats[1]= 1.; // 40 & 41 group codes = height & width
      coords[0]= Point( 0., 0., 0. ); // 10 & 20 = view center point
      coords[1]= Point( 1., 1., 1. ); // 11 & 21 & 31 = direction from target
      coords[2]= Point( 0., 0., 0. ); // 12 & 22 & 32 = target point
      floats[2]= 0.5; // 42 = lens length... (?)
      floats[3]= -100.; // 43 & 44= front & back clipping plans-offsets
      floats[4]= +100.;
      angles[0]= 0.; // 50 = twist angle
//      integers[0] == 70 group code = VIEWMODE, ignored...

      readObject();
      if( strcmp( name, "*ACTIVE" ) == 0 ) // active view port
        {
        view.height= floats[0];
        view.width = floats[1];
        view.centerPoint= Point2( coords[0].x(), coords[0].y() );
        view.direction= Vector( coords[1] );
        view.target= coords[2];
        view.lensLength= floats[2];
        view.frontClip= floats[3];
        view.backClip= floats[4];
        view.twistAngle= angles[0];
        }
      }
     else
      readObject();
     }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// read LAYER header in TABLES section
boolean DXFReader::readLayer()
{
  readObject();
  nLayers= integers[0]; // group 70
  if( nLayers <= 0 )
    {
    // we create at least one '0' layer
    layers= new LAYER;
    layers->index= 1;
    layers->name= strdup( "0" );
    layers->color.index= 7;
    layers->color.r= 1.;
    layers->color.g= 1.;
    layers->color.b= 1.;
    layers->off= false;
    layers->frozen= false;
    layers->locked= false;
    nLayers= 1;
    return true;
    }

  layers= new LAYER [ nLayers ];
  assert( layers != 0 );

  int l= 0;

  for(;;)
    {
    if( find( "ENDTAB" ) )
      break;

    if( find( "LAYER" ) )
      {
      assert( layers != 0 );
//printf("%d %d\n",l,nLayers);
//      assert( l < nLayers );
if( l >= nLayers ) l= nLayers-1;
      color.index= 7;
      color.r= color.g= color.b= 1.;

      readObject();
      layers[l].index= l+1;
      layers[l].name= strdup( name );
      assert( layers[l].name != 0 );
      layers[l].color.index= abs(color.index);
      layers[l].color.r= color.r;
      layers[l].color.g= color.g;
      layers[l].color.b= color.b;
      layers[l].off= color.index < 0;
      layers[l].frozen= (integers[70] & 1) != 0;
      layers[l].locked= (integers[70] & 4) != 0;
      ++l;
      }
     else
      readObject();
    }

  nLayers= l;
  return true;
}

/////////////////////////////////////////////////////////////////////////////

DXFReader::LAYER* DXFReader::getLayer( const char* layerName )
{
  if( strcmp( layerName, "0" ) == 0 )
    return defaultLayer;

  if( layers == 0 )
    {
    // no layer table previously read
    // create layer '0'
    layers= new LAYER;
    layers->index= 1;
    layers->name= strdup( "0" );
    layers->color.index= 7;
    layers->color.r= 1.;
    layers->color.g= 1.;
    layers->color.b= 1.;
    layers->off= false;
    layers->frozen= false;
    layers->locked= false;
    nLayers= 1;
    }

  int i;
  for( i= 0; i< nLayers; ++i )
    if( strcmp( layerName, layers[i].name ) == 0 )
      return layers+i;

//  if( fstderr != 0 )
    fprintf( stderr, "warning: DXFReader::getLayer(): post adding layer: %s\n", layerName );

  LAYER* newL= new LAYER [ nLayers + 1 ];
  for( i= 0; i< nLayers; ++i )
    newL[i]= layers[i];
  delete layers;
  layers= newL;

  newL[nLayers].index= nLayers+1;
  newL[nLayers].name= strdup(layerName);
  newL[nLayers].color.index= 7;
  newL[nLayers].color.r= 1.;
  newL[nLayers].color.g= 1.;
  newL[nLayers].color.b= 1.;
  newL[nLayers].off= false;
  newL[nLayers].frozen= false;
  newL[nLayers].locked= false;
  ++nLayers;

  return newL + nLayers - 1;
}

/////////////////////////////////////////////////////////////////////////////

DXFReader::LAYER* DXFReader::getLayer( int layerIndex )
{
  if( layerIndex == 0 )
    return defaultLayer;

  --layerIndex;
  assert( layers != 0 );
  assert( layerIndex < nLayers );

  return layers + layerIndex;
}


boolean DXFReader::skipObjectTest( const char* objectName )
{
  return strcmp( objectName, "ATTDEF" ) == 0 ||
         strcmp( objectName, "TEXT" ) == 0 ||
         strcmp( objectName, "SHAPE" ) == 0 ||
         strcmp( objectName, "ATTRIB" ) == 0 ||
         strcmp( objectName, "DIMENSION" ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

// read an object: 3DFACE, VIEW... from TABLES or ENTITIES or BLOCKS sections
boolean DXFReader::readObject( boolean waitForFirst /* = false */ )
{
  for(;;)
    {
    if( ! getLine() )
      {
      if( fstderr != 0 )
        {
        fprintf( fstderr, "need ENDSEC\n" );
        fflush(fstderr);
        }
      return false;
      }
    if( groupCode == 0 )
      {
      if( !waitForFirst && skipObjectTest( textLine ) )
        {
        if( fstderr != 0 )
          {
          fprintf( fstderr, "Warning: skipping %s object found line %ld\n",
                   textLine, lineN-1 );
          fflush(fstderr);
          }
        goto next;
        }
      break;
      }

    if( groupCode >= 1 && groupCode <= 9 ) // cardinal group
      {
      if( !readCardinalGroup() )
        return false;
      goto next;
      }
    if( groupCode >= 10 && groupCode <= 18 ) // X coordinates
      {
      if( !readXCoord( groupCode-10 ) )
        return false;
      goto next;
      }
    if( groupCode >= 20 && groupCode <= 28 ) // Y coordinates
      {
      if( !readYCoord( groupCode-20 ) )
        return false;
      goto next;
      }
    if( groupCode >= 30 && groupCode <= 38 ) // Z coordinates
      {
      if( !readZCoord( groupCode-30 ) )
        return false;
      goto next;
      }
    if( groupCode == 38 ) // entity elevation if nonzero: skipped
      {
      ++entities.evelation;
      goto next;
      }
    if( groupCode == 39 ) // entity thickness
      {
      sscanf( textLine, "%lf", & polyline.thickness );
      ++entities.thickness;
      goto next;
      }
    if( groupCode >= 40 && groupCode <= 48 ) // misc floats
      {
      ++entities.floats;
      sscanf( textLine, "%lf", & floats[ groupCode-40 ] );
      goto next;
      }
    if( groupCode == 49 ) // repeated value groups: skipped
      {
      ++entities.repeatedValue;
      goto next;
      }
    if( groupCode >= 50 && groupCode <= 58 ) // misc angles
      {
      ++entities.angles;
      sscanf( textLine, "%lf", & angles[ groupCode-50 ] );
      goto next;
      }
    if( groupCode == 62) // Color number
      {
      if( ! readColor() )
        return false;
      goto next;
      }
    if( groupCode == 66 ) // "entities follow" flag: skipped
      {
      ++entities.followFlag;
      goto next;
      }
    if( groupCode >= 70 && groupCode <= 78 ) // misc ints
      {
      ++entities.integers;
      sscanf( textLine, "%d", & integers[ groupCode-70 ] );
      goto next;
      }
    if( groupCode == 210 || groupCode == 220 || groupCode == 230 )
      { // X, Y, Z components of extrusion direction
      double d;
      sscanf( textLine, "%lf", & d );
      switch( groupCode )
        {
        case 210: polyline.extrusionDir.setx( d ); break;
        case 220: polyline.extrusionDir.sety( d ); break;
        case 230: polyline.extrusionDir.setz( d ); break;
        }
      ++entities.extrusion;
      goto next;
      }
    if( groupCode == 999 ) // Comment: skipped
      {
      ++entities.comments;
      goto next;
      }
    if( fstderr != 0 )
      {
      fprintf( fstderr, "Warning: unknown group code %d line %ld\n",
               groupCode, lineN-2 );
      fflush(fstderr);
      }
next:;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// we gonna read each vertex of the polyline, (ada==shit) is true
boolean DXFReader::readPolyLine()
{
  // wait for a vertex
  do{
    readObject();
    if( find( "SEQEND" ) ) //|| find( "ENDBLK" ) )
      {
      if( fstderr != 0 )
        {
        fprintf( fstderr, "hmmm, strange, polyline with no vertex\n" );
        fflush(fstderr);
        }
      return false;
      }
    }
  while( ! find( "VERTEX" ) );

  // we've now read the polyline header, let's dump it in a new polyline
  // structure
  polyline.nVertices= 0;
  polyline.flags= integers[0]; // group 70
  polyline.M= integers[1]; // group 71
  polyline.N= integers[2]; // group 72
  polyline.group75= integers[5];

  // we're exactly after a VERTEX command
  for(;;)
    {
    strncpy( curObject, textLine, 80 ); // store the current read object
    coords[0]= Point( 0., 0., 0. );
    integers[0]= 0;
    integers[1]= 0;
    integers[2]= 0;
    integers[3]= 0;
    integers[4]= 0;
    integers[5]= 0;
    floats[2]= 0.;
    readObject(); // read VERTEX object

    if( findObject( "VERTEX" ) )
      {
      // dump the vertex
      boolean outcome=
      polyline.addVertex( coords[0], // the point
                          integers[0], // flags = group 70
                          floats[2], // bulge = group 42
                          integers[1],
                          integers[2],
                          integers[3],
                          integers[4] // = indices[0..3]
                        );
      if( !outcome )
        return false;
      }

    if( //find( "ENDBLK" ) || // end of block
        find( "SEQEND" ) ) // end of polyline object
      break;
    }

  readObject(); // skip SEQEND and read until we find a 0 group

  strcpy( curObject, "POLYLINE" ); // we've read a POLYLINE object
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::skipBlockTest( const char* blockName )
{
/*
  if( blockName[0] == '*' ) // remove anonymous blocks
    return true;
*/
  return strcmp( blockName, "AVE_RENDER" ) == 0 ||
         strcmp( blockName, "AVE_GLOBAL" ) == 0 ||
         strcmp( blockName, "AME_NIL" ) == 0 ||
         strcmp( blockName, "AME_FRZ" ) == 0 ||
         strcmp( blockName, "AME_SOL" ) == 0 ||
         strcmp( blockName, "AME_MAT" ) == 0 ||
         strcmp( blockName, "ASHADE" ) == 0 ||
         strcmp( blockName, "ASURF_NIL" ) == 0 ||
         strcmp( blockName, "ASURF_FRZ" ) == 0 ||
         strcmp( blockName, "RM_SDB" ) == 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readEntitiesSection()
{
  // textLine contains the first entity name (eg. 3DFACE, POLYLINE...)

  for(;;)
    {
    // we get the entity name in textLine (eg 3DFACE, CIRCLE, POLYLINE...)
    if( find( "ENDBLK" ) ) // we were into the BLOCKS section
      break;               // and we've read a BLOCK

    if( find( "ENDSEC" ) ) // we were into the ENTITIES section
      break;               // and we've read the main block

    strncpy( curObject, textLine, 80 );
    if( findObject( "POLYLINE" ) ) // next object is a polyline: read it
      readPolyLine();
     else
      readObject();

    writeObject();
    if( groupCode != 0 )
      readObject(); // get next entity name
    }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readBlocksSection()
{
  for(;;)
    {
    readObject();
    if( find( "ENDSEC" ) )
      break;

    assert( find( "BLOCK" ) );
    *name= 0;
    readObject( true ); // read name, baseBlockPoint, ...
                        // -> and for first object (even we'll skip it)

    assert( *name != 0 ); // we've read a name (group 2) ?

    boolean skipThisBlock= skipBlockTest( name );
    if( ! skipThisBlock )
      {
      blockBasePoint= coords[0];
      dxfExport->start_BLOCK( name,
                              Point( blockBasePoint.x(),
                                     blockBasePoint.y(),
                                     blockBasePoint.z() ) );

      readEntitiesSection();
      assert( find( "ENDBLK" ) );

      dxfExport->end_BLOCK();
      }
     else
      {
      // skip block, ie wait for ENDBLK
      while( ! find( "ENDBLK" ) )
        readObject();
      }
    }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readCardinalGroup()
{
  ++entities.cardinals;
  switch( groupCode )
    {
    case 1: // primary text value for entity (?)
      break;
    case 2: // block name, attribute tag, etc
      if( findObject( "INSERT" ) )
        {
        strncpy( insert_name, textLine, 80 ); // get name
        }
       else
        {
        // get block first name (BLOCK)
        // or layer name (LAYER)
        // or view port name (VPORT)
        strncpy( name, textLine, 80 );
        }
      break;
    case 3:
      break; // second block name (in BLOCK definition)
    case 4:
      break;
    case 5: // entity handle (hex string)
      break;
    case 6: // line type name
      break;
    case 7: // text style name
      break;
    case 8:
      curLayer= getLayer( textLine ); // get layer name from layer name
      break;
    case 9: // variable name ID (only in header)
      break;
    }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readXCoord( int index )
{
  ++entities.xcoords;
  double d;
  sscanf( textLine, "%lf", &d );
  coords[index].setx( d );
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readYCoord( int index )
{
  ++entities.ycoords;
  double d;
  sscanf( textLine, "%lf", &d );
  coords[index].sety( d );
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::readZCoord( int index )
{
  ++entities.zcoords;
  double d;
  sscanf( textLine, "%lf", &d );
  coords[index].setz( d );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// read basic AutoCAD 9-color palette
// What is the color which is indexed by 15 ???
boolean DXFReader::readColor()
{
  ++entities.color;
  char* s= textLine;
  while( *s == ' ' ) ++s;
  sscanf( s, "%d", &color.index );
  switch( abs(color.index) )
    {
    case 0: // black
      color.r= color.g= color.b= 0.;
      break;
    case 1: // red
      color.r= 1.;
      color.b= color.g= 0.;
      break;
    case 2: // yellow
      color.r= color.g= 1.;
      color.b= 0.;
      break;
    case 3: // green
      color.g= 1.;
      color.r= color.b= 0.;
      break;
    case 4: // cyan
      color.b= color.g= 1.;
      color.r= 0.;
      break;
    case 5: // blue
      color.b= 1.;
      color.r= color.g= 0.;
      break;
    case 6: // magenta
      color.b= color.r= 1.;
      color.g= 0.;
      break;
    case 8: // dk. grey
      color.r= color.g= color.b= 0.5;
      break;
    case 9: // lt. grey
      color.r= color.g= color.b= 0.75;
      break;
    case 7: // white
    default: // make anything else white (?)
      color.r= color.g= color.b= 1.;
      if( abs(color.index) != 7 )
        if( fstderr != 0 )
          {
          fprintf( fstderr, "Warning: unknown color index %d line %ld\n",
                   color.index, lineN-1 );
          fflush(fstderr);
          }
    }
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::writePolylineObject()
{
  if( polyline.isPolyline3D() || // this is ok.
     (!polyline.isPolyMesh() && !polyline.isMeshSurface()) // but hmmmm...
    )
    return polyline.writePolyline3D( dxfExport );
  if( polyline.isPolyMesh() )
    {
/*
    if( polyline.N == 0 ) // flag 64 only... cf. archeo/surfaces.dxf
      return polyline.writePolyline3D( dxfExport );
*/

    return polyline.writePolyMesh( dxfExport );
    }

  if( polyline.isMeshSurface() )
    return polyline.writeMeshSurface( dxfExport );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::writeInsertObject()
{
  if( skipBlockTest( insert_name ) )
    return true; // easy! the fingers in the nose!

  Point& insertionPoint = coords[0]; // groups 10,20,30
  Vector scale = Vector( (floats[1]==0.) ? 1. : floats[1],   // group 41
                         (floats[2]==0.) ? 1. : floats[2],   // group 42
                         (floats[3]==0.) ? 1. : floats[3] ); // group 43
  double rotationAngle= angles [ 0 ]; // group 50
  dxfExport->insert_BLOCK( insert_name,
                           insertionPoint, scale, rotationAngle );
  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean DXFReader::writeObject()
{
  boolean outcome= true;

  if( *curObject == 0 ) // no object currently defined
    goto TheEnd;

  if( findObject( "INSERT" ) )
    {
    outcome= writeInsertObject();
    goto TheEnd;
    }

  if( findObject( "POLYLINE" ) )
    {
    outcome= writePolylineObject();
    goto TheEnd;
    }

  if( findObject( "3DLINE" ) )
    {
    dxfExport->polylineThickness( 2, coords, false,
                                  polyline.extrusionDir * polyline.thickness );
    goto TheEnd;
    }

  if( findObject( "LINE" ) )
    {
    dxfExport->polylineThickness( 2, coords, false,
                                  polyline.extrusionDir * polyline.thickness );
    goto TheEnd;
    }
  if( findObject( "POINT" ) )
    {
    if( polyline.thickness == 0. )
      dxfExport->POINT( coords[0] );
     else
      {
      Point p[2];
      p[0]= coords[0];
      p[1]= coords[0] + polyline.extrusionDir * polyline.thickness;
      dxfExport->POLYLINE( 2, p, false );
      }
    goto TheEnd;
    }
  if( findObject( "CIRCLE" ) )
    {
    if( polyline.thickness == 0. )
      dxfExport->CIRCLE( coords[0], floats[0] );
     else
      {
      dxfExport->CYLINDER( coords[0], floats[0],
                           polyline.extrusionDir * polyline.thickness );
      }
    goto TheEnd;
    }
  if( findObject( "ARC" ) )
    {
    dxfExport->arc( coords[0], floats[0], angles[0], angles[1],
                    polyline.extrusionDir * polyline.thickness );
    goto TheEnd;
    }
  if( findObject( "TRACE" ) )
    {
//    dxfExport->TRACE( coords[0], coords[1], coords[2], coords[3] );
    dxfExport->polylineThickness( 4, coords, true,
                                  polyline.extrusionDir * polyline.thickness );
    goto TheEnd;
    }

  if( findObject( "SOLID" ) )
    {
    if( coords[0] == coords[1] )
      {
      dxfExport->SOLID( coords[1], coords[2], coords[3] );
      goto TheEnd;
      }
    if( coords[1] == coords[2] )
      {
      dxfExport->SOLID( coords[0], coords[2], coords[3] );
      goto TheEnd;
      }
    if( coords[2] == coords[3] )
      {
      dxfExport->SOLID( coords[0], coords[1], coords[2] );
      goto TheEnd;
      }

    dxfExport->SOLID( coords[0], coords[1], coords[2], coords[3] );
    goto TheEnd;
    }

  if( findObject( "3DFACE" ) )
    {
    if( coords[0] == coords[1] )
      {
      dxfExport->FACE( coords[1], coords[2], coords[3] );
      goto TheEnd;
      }
    if( coords[1] == coords[2] )
      {
      dxfExport->FACE( coords[0], coords[2], coords[3] );
      goto TheEnd;
      }
    if( coords[2] == coords[3] )
      {
      dxfExport->FACE( coords[0], coords[1], coords[2] );
      goto TheEnd;
      }
    if( coords[3] == coords[0] )
      {
      dxfExport->FACE( coords[0], coords[1], coords[2] );
      goto TheEnd;
      }

    dxfExport->FACE( coords[0], coords[1], coords[2], coords[3] );
    goto TheEnd;
    }

  if( fstderr != 0 )
    {
    fprintf( fstderr, "Warning: ignore object %s defined before line %ld\n",
             curObject, lineN );
    fflush(fstderr);
    }
TheEnd:
  flushObject();
  return outcome;
}

/////////////////////////////////////////////////////////////////////////////
