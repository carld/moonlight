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
  DXFExport.h

  Revision:
    Stephane Rehel
    April 25 1997
*/

#ifndef __DXFExport_h
#define __DXFExport_h

#ifndef __boolean_h
#include "tools/boolean.h"
#endif

#ifndef __Vector_h
#include "tools/Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

// DXFExport abstract class
// Abstract base class for converting DXF files to other formats
// Converter class inherits DXFExport and implements pure virtual methods
class DXFExport
{
public:
  DXFExport()
    {}
  virtual ~DXFExport()
    {}

  // number of lines in a circle
  // by default: 0, i.e. dont draw circles...
  virtual int nCircleSubdivide() const
    {
    return 0;
    }
  Point* doCircle( int n, const Point& center, double radius );
  void arc( const Point& center, double radius,
            double startAngle, double endAngle,
            const Vector& extrusionDir );
  virtual void SOLID( const Point& p1, const Point& p2,
                      const Point& p3, const Point& p4 );
  virtual void SOLID( const Point& p1, const Point& p2, const Point& p3 );
  void polylineThickness( int nVertices, const Point* points,
                          boolean closed,
                          const Vector& extrusionDir );
/*
  virtual void TRACE( const Point& p1, const Point& p2,
                      const Point& p3, const Point& p4 );
*/
  virtual void CIRCLE( const Point& center, double radius );
  // cylinder is closed both on top and bottom
  virtual void CYLINDER( const Point& center, double radius,
                         const Vector& length );

// OBJECTS begin
// You should at least derive these functions:
  virtual void POINT( const Point& /* p */ );
  virtual void FACE( const Point& /* p1 */ , const Point& /* p2 */,
                     const Point& /* p3 */ );
  virtual void FACE( const Point& /* p1 */, const Point& /* p2 */,
                     const Point& /* p3 */, const Point& /* p4 */ );
  virtual void POLYLINE( int /* nVertices */,
                         const Point* /* points */,
                         boolean /* closed */ );
  virtual void MESHSURFACE( const Point* vertices,
                            int M, int N,
                            boolean closed_M, boolean closed_N );
// OBJECTS end

  // note: included blocks don't exist in dxf files
  virtual boolean supportBlocks() const
    {
    return true;
    }

  virtual void start_BLOCK( const char* /* name */,
                            const Point& /* basePoint */ )
    {}
  virtual void end_BLOCK()
    {}

  virtual void insert_BLOCK( const char* /* name */,
                             const Point& /* insertionPoint */,
                             const Vector& /* scale */,
                             double /* rotationAngle */ )
    {}
  virtual void start_exploding_BLOCK( const char* /* name */, int /* id */ )
    {}
  virtual void end_exploding_BLOCK( const char* /* name */, int /* id */ )
    {}

  // when we've read all the blocks, we start reading main object
  virtual void start_MAIN() =0;
  virtual void end_MAIN() =0;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __DXFExport_h
