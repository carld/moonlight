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


// Shoot a test ray along +X axis.  The strategy, from MacMartin, is to
// compare vertex Y values to the testing point's Y and quickly discard
// edges which are entirely to one side of the test ray.
//
// Input 2D polygon _pgon_ with _numverts_ number of vertices and test point
// _point_, returns 1 if inside, 0 if outside.  WINDING and CONVEX can be
// defined for this test.

  register double tx= point[X];
  register double ty= point[Y];

  Point& p0= vertex(LAST_VERTEX);
  double v0x= p0[X];
  double v0y= p0[Y];
  // get test bit for above/below X axis
  int yflag0= (v0y >= ty);
  Point& p1= vertex(FIRST_VERTEX);
  register double v1x= p1[X];
  register double v1y= p1[Y];

#ifdef WINDING
  int crossings= 0;
#else
  int inside_flag= 0;
#endif
#ifdef CONVEX
  int line_flag= 0;
#endif

  FOR_ITER
    {
    int yflag1= (v1y >= ty);
    // check if endpoints straddle (are on opposite sides) of X axis
    // (i.e. the Y's differ); if so, +X ray could intersect this edge.
    if( yflag0 != yflag1 )
      {
      register int xflag0= (v0x >= tx);
      // check if endpoints are on same side of the Y axis (i.e. X's
      // are the same); if so, it's easy to test if edge hits or misses.
      if( xflag0 == (v1x >= tx) )
        {
        // if edge's X values both right of the point, must hit
#ifdef WINDING
        if( xflag0 )
          crossings+= ( yflag0 ? -1 : 1 );
#else
        if( xflag0 )
          inside_flag= !inside_flag;
#endif
        }
       else
        {
        // compute intersection of pgon segment with +X ray, note
        // if >= point's X; if so, the ray hits it.
        if( (v1x - (v1y-ty)*(v0x-v1x)/(v0y-v1y)) >= tx )
          {
#ifdef WINDING
          crossings+= ( yflag0 ? -1 : 1 );
#else
          inside_flag= !inside_flag;
#endif
          }
        }
#ifdef CONVEX
      // if this is second edge hit, then done testing
      if( line_flag )
        break;

      // note that one edge has been hit by the ray's line
      line_flag= 1;
#endif
      }

    // move to next pair of vertices, retaining info as possible
    yflag0= yflag1;
    v0x= v1x;
    v0y= v1y;

    register Point& p1= vertex(j);
    v1x= p1[X];
    v1y= p1[Y];
    }

#ifdef WINDING
  // test if crossings is not zero
  return (crossings != 0);
#else
  return inside_flag;
#endif

