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
   Box.h

   Creation: SR, August 24th, 1995
   Revisions:
*/

#ifndef __Box_h
#define __Box_h

#ifndef __Vector_h
#include "Vector.h"
#endif

/////////////////////////////////////////////////////////////////////////////

class Box
{
protected:
  Point minP, maxP;

public:
  Box()
    {}

  Box( const Point& p1, const Point& p2 ):
    minP( min(p1,p2) ),
    maxP( max(p1,p2) )
    {}

  Box( const Box& b ):
    minP(b.minP),
    maxP(b.maxP)
    {}

  const Point& getMin() const
    {
    return minP;
    }
  const Point& getMax() const
    {
    return maxP;
    }
  void setMin( const Point& p )
    {
    minP= p;
    }
  void setMax( const Point& p )
    {
    maxP= p;
    }

  Box& operator = ( const Box& b )
    {
    minP= b.minP;
    maxP= b.maxP;
    return *this;
    }

  Vector getDiag() const
    {
    return maxP - minP;
    }

  DBL getXSize() const { return maxP.x() - minP.x(); }
  DBL getYSize() const { return maxP.y() - minP.y(); }
  DBL getZSize() const { return maxP.z() - minP.z(); }

  DBL getVolume() const
    {
    return getDiag().norm();
    }

  Point getCenter() const
    {
    return (minP + maxP) * DBL(0.5);
    }

  boolean into( const Point& p ) const
    {
    return p.x() >= minP.x() && p.x() <= maxP.x() &&
           p.y() >= minP.y() && p.y() <= maxP.y() &&
           p.z() >= minP.z() && p.z() <= maxP.z() ;
    }

  boolean intersect( const Box& b ) const
    {
/*
    return   into( b.minP ) ||
             into( Point(b.minP.x(),b.minP.y(),b.maxP.z()) ) ||
             into( Point(b.minP.x(),b.maxP.y(),b.minP.z()) ) ||
             into( Point(b.minP.x(),b.maxP.y(),b.maxP.z()) ) ||
             into( Point(b.maxP.x(),b.minP.y(),b.minP.z()) ) ||
             into( Point(b.maxP.x(),b.minP.y(),b.maxP.z()) ) ||
             into( Point(b.maxP.x(),b.maxP.y(),b.minP.z()) ) ||
             into( b.maxP ) ||
           b.into(   minP ) ||
           b.into( Point(  minP.x(),  minP.y(),  maxP.z()) ) ||
           b.into( Point(  minP.x(),  maxP.y(),  minP.z()) ) ||
           b.into( Point(  minP.x(),  maxP.y(),  maxP.z()) ) ||
           b.into( Point(  maxP.x(),  minP.y(),  minP.z()) ) ||
           b.into( Point(  maxP.x(),  minP.y(),  maxP.z()) ) ||
           b.into( Point(  maxP.x(),  maxP.y(),  minP.z()) ) ||
           b.into(   maxP ) ||
*/
    return !( b.maxP.x() < minP.x() ||
              b.minP.x() > maxP.x() ||
              b.maxP.y() < minP.y() ||
              b.minP.y() > maxP.y() ||
              b.maxP.z() < minP.z() ||
              b.minP.z() > maxP.z() );
    }

  void extend( const Point& p )
    {
    minP= min( minP, p );
    maxP= max( maxP, p );
    }

  boolean intersect_from_exterior( const Point& origin,
                                 const Vector& direction,
                                 double min_distance,
                                 double max_distance ) const;

  boolean intersect_from_interior( const Point& origin,
                                 const Vector& direction,
                                 double min_distance,
                                 double max_distance ) const;

  boolean segment_intersect( const Point& p1, const Point& p2 );

  boolean inHalfPlane( const Point& point,
                     const Vector& normal // not normalized
                   ) const;
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __Box_h
