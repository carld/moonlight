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
   MLCamera.C

   Stephane Rehel

   Creation: SR, August 18th, 1995
   Revised: January 19 1997
*/

#include "tools/file/MLWFileBlock.h"
#include "tools/file/MLRFileBlock.h"

#include "MLCamera.h"

/////////////////////////////////////////////////////////////////////////////

static const double defaultAngle= DEGtoRAD(60.); // radians
static const double defaultNearClippingPlane= 0.01;
static const double defaultFarClippingPlane= 1e6;

/////////////////////////////////////////////////////////////////////////////

MLCamera::MLCamera()
{
  ortho= false;
  orthoZoom= 1.;
  angle= ::defaultAngle;
  nearClippingPlane= ::defaultNearClippingPlane;
  farClippingPlane= ::defaultFarClippingPlane;

  location= Point(1.,0.,0.);
  direction= Vector(-1.,0.,0.);
  sky= Vector(0.,0.,1.);
  x_res= y_res= 1;
  lookat= location + direction;
  phi= theta= 0.;

  projection.setIdentity();
  inv_projection.setIdentity();
}

/////////////////////////////////////////////////////////////////////////////

MLCamera::MLCamera( const MLCamera& c )
{
  location= c.location;
  direction= c.direction;
  sky= c.sky;
  lookat= c.lookat;

  ortho= c.ortho;
  orthoZoom= c.orthoZoom;

  right= c.right;
  up= c.up;

  x_res= c.x_res;
  y_res= c.y_res;

  angle= c.angle;
  nearClippingPlane= c.nearClippingPlane;
  farClippingPlane= c.farClippingPlane;

  projection= c.projection;
  inv_projection= c.inv_projection;

  phi= c.phi;
  theta= c.theta;
}

/////////////////////////////////////////////////////////////////////////////

void MLCamera::init( const Point& _location,
                     const Vector& _direction,
                     const Vector& _sky )
{
  location= _location;

  double n;

  n= _direction.norm();
  if( n < 1e-10 )
    direction= Vector( (location.x()>=0.) ? -1 : 1, 0, 0 );
   else
    direction= _direction / n;

  n= _sky.norm();
  if( n < 1e-10 )
    sky= Vector(0,0,1);
   else
    sky= _sky / n;

  Vector v= location - lookat;
  double vnorm= v.norm();
  if( vnorm < 1e-20 )
    {
    phi= theta= 0.;
    }
   else
    {
    // x= r*cos(phi)*cos(theta)
    // y= r*sin(phi)*cos(theta)
    // z= r*sin(theta)
    if( fabs(v.x()) < 1e-20 && fabs(v.y()) < 1e-20 )
      phi= 0;
     else
      phi= atan2( v.y(), v.x() );

    theta= asin( v.z() / vnorm );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCamera::initByLookat( const Point& _location,
                             const Point& _lookat,
                             const Vector& _sky )
{
  lookat= _lookat;
  init( _location, lookat - _location, _sky );
}


/////////////////////////////////////////////////////////////////////////////

// phi and theta = radians
void MLCamera::initSpheric( double _phi,
                            double _theta,
                            double _radius )
{
  phi= fmod( _phi, 2.*M_PI );
  if( phi > M_PI )
    phi -= 2.*M_PI;

  theta= fmod( _theta, 2.*M_PI );
  if( theta > M_PI )
    theta -= 2.*M_PI;

  _radius= fabs(_radius);
  if( _radius < 1e-10 )
    _radius= 1e-10;

  double cos_theta= cos(theta);
  double sin_theta= sin(theta);
  double cos_phi= cos(phi);
  double sin_phi= sin(phi);

  Vector v( cos_phi*cos_theta,
            sin_phi*cos_theta,
            sin_theta );
  sky= Vector( -cos_phi*sin_theta,
               -sin_phi*sin_theta,
               cos_theta );
  location= lookat + v * _radius;
  direction= -v;
}

/////////////////////////////////////////////////////////////////////////////

void MLCamera::getOrthoSize( DBL& sx, DBL& sy ) const
{
  if( x_res < y_res )
    {
    sx= orthoZoom;
    sy= sx * double(y_res) / double(x_res);
    }
   else
    {
    sy= orthoZoom;
    sx= sy * double(x_res) / double(y_res);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLCamera::compute()
{
  right= direction ^ sky;
  up= right ^ direction;

  if( ortho )
    {
    double sx, sy;
    getOrthoSize(sx,sy);
    double left= -sx;
    double right= +sx;
    double bottom= -sy;
    double top= +sy;
    double nearval= -farClippingPlane;
    double  farval= +farClippingPlane;

    double x= 2. / (right-left);
    double y= 2. / (top-bottom);
    double z=-2. / (farval-nearval);
    double tx= -(right+left) / (right-left);
    double ty= -(top+bottom) / (top-bottom);
    double tz= -(farval+nearval) / (farval-nearval);

#define M(i,j) projection.aij(i,j)
    M(0,0)= x;  M(0,1)= 0.; M(0,2)= 0.; M(0,3)= tx;
    M(1,0)= 0.; M(1,1)= y;  M(1,2)= 0.; M(1,3)= ty;
    M(2,0)= 0.; M(2,1)= 0.; M(2,2)= z;  M(2,3)= tz;
    M(3,0)= 0.; M(3,1)= 0.; M(3,2)= 0.; M(3,3)= 1.;
#undef M
    }
   else
    {
    double aspect= double(x_res) / double(y_res);

    double zNear= nearClippingPlane;
    double zFar=  farClippingPlane;

    double top= zNear * tan(angle*0.5);
    double bottom= -top;

    double left= bottom * aspect;
    double right=   top * aspect;

    double x= (2.0*zNear) / (right-left);
    double y= (2.0*zNear) / (top-bottom);
    double a= (right+left) / (right-left);
    double b= (top+bottom) / (top-bottom);
    double c= -(zFar+zNear) / (zFar-zNear);
    double d= -(2.0*zFar*zNear) / (zFar-zNear);

#define M(i,j) projection.aij(i,j)
    M(0,0)= x;  M(0,1)= 0.; M(0,2)= a;  M(0,3)= 0.;
    M(1,0)= 0.; M(1,1)= y;  M(1,2)= b;  M(1,3)= 0.;
    M(2,0)= 0.; M(2,1)= 0.; M(2,2)= c;  M(2,3)= d;
    M(3,0)= 0.; M(3,1)= 0.; M(3,2)=-1.; M(3,3)= 0.;
#undef M
    }

   Matrix4x4 P;
#define M(i,j) P.aij(i,j)
#define z direction
#define y up
#define x right
   M(0,0)= x[0]; M(0,1)= x[1]; M(0,2)= x[2]; M(0,3)= 0.0;
   M(1,0)= y[0]; M(1,1)= y[1]; M(1,2)= y[2]; M(1,3)= 0.0;
   M(2,0)=-z[0]; M(2,1)=-z[1]; M(2,2)=-z[2]; M(2,3)= 0.0;
   M(3,0)= 0.0;  M(3,1)= 0.0;  M(3,2)= 0.0;  M(3,3)= 1.0;
#undef x
#undef y
#undef z
#undef M
   projection *= P;

   // Translate Eye to Origin
   double* m= & projection.aij(0,0);
#define tx location.x()
#define ty location.y()
#define tz location.z()
   m[12]-= m[0]*tx + m[4]*ty + m[8] *tz;
   m[13]-= m[1]*tx + m[5]*ty + m[9] *tz;
   m[14]-= m[2]*tx + m[6]*ty + m[10]*tz;
   m[15]-= m[3]*tx + m[7]*ty + m[11]*tz;
#undef tx
#undef ty
#undef tz

  inv_projection= projection.computeInverse();
}

/////////////////////////////////////////////////////////////////////////////

// static
void MLCamera::makePickMatrix( double x, double y,
                               double width, double height,
                               int viewport[4],
                               Matrix4x4& pick )
{
  double sx= double(viewport[2]) / width;
  double sy= double(viewport[3]) / height;
  double tx= (double(viewport[2]) + 2.*(double(viewport[0])-x)) / width;
  double ty= (double(viewport[3]) + 2.*(double(viewport[1])-y)) / height;

#define M(i,j) pick.aij(i,j)
  M(0,0)= sx; M(0,1)= 0.; M(0,2)= 0.; M(0,3)= tx;
  M(1,0)= 0.; M(1,1)= sy; M(1,2)= 0.; M(1,3)= ty;
  M(2,0)= 0.; M(2,1)= 0.; M(2,2)= 1.; M(2,3)= 0.;
  M(3,0)= 0.; M(3,1)= 0.; M(3,2)= 0.; M(3,3)= 1.;
#undef M
}

/////////////////////////////////////////////////////////////////////////////

// from screen to space
Point MLCamera::unproject( const Point& win ) const
{
  double viewport[4];
  viewport[0]= 0.;
  viewport[1]= 0.;
  viewport[2]= double(x_res);
  viewport[3]= double(y_res);

  Point in;
  in[0]=(win.x()-viewport[0])*2./viewport[2] - 1.;
  in[1]=(win.y()-viewport[1])*2./viewport[3] - 1.;
  in[2]=2*win.z() - 1.0;

  return inv_projection.apply(in);
}

/////////////////////////////////////////////////////////////////////////////

void MLCamera::write( MLWFileBlock& block ) const
{
  block << location
        << direction
        << sky;

  block << lookat;

  block << int(ortho);
  block << double(orthoZoom);

  block << right << up;

  block << int(x_res) << int(y_res);

  block << double(angle);
  block << double(nearClippingPlane)
        << double(farClippingPlane);

  block << projection;
  block << inv_projection;

  block << double(phi)
        << double(theta);

  block << int(0);
}

/////////////////////////////////////////////////////////////////////////////

void MLCamera::read( MLRFileBlock& block )
{
  block >> location
        >> direction
        >> sky;

  block >> lookat;

  ortho= block.getInteger();
  orthoZoom= block.getDouble();

  block >> right >> up;

  x_res= block.getInteger();
  y_res= block.getInteger();

  angle= block.getDouble();
  nearClippingPlane= block.getDouble();
  farClippingPlane= block.getDouble();

  block >> projection;
  block >> inv_projection;

  phi= block.getDouble();
  theta= block.getDouble();

  if( block.getInteger() == 0 )
    return;
}

/////////////////////////////////////////////////////////////////////////////

MLWFileBlock& operator << ( MLWFileBlock& block, const MLCamera& c )
{
  c.write(block);
  return block;
}

/////////////////////////////////////////////////////////////////////////////

MLRFileBlock& operator >> ( MLRFileBlock& block, MLCamera& c )
{
  c.read(block);
  return block;
}

/////////////////////////////////////////////////////////////////////////////

Matrix4x4 MLCamera::getPixelProjection() const
{
  double px= double(x_res);
  double py= double(y_res);
  double ox= px * 0.5;
  double oy= py * 0.5;
  Matrix4x4 toViewport( px*0.5, 0., 0., ox,
                        0., py*0.5, 0., oy,
                        0.,     0., 1., 0.,
                        0.,     0., 0., 1. );

  return toViewport * projection;
}

/////////////////////////////////////////////////////////////////////////////
