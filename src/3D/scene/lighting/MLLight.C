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
  MLLight.C

  Stephane Rehel
  January 12 1997
*/

#include <assert.h>
#include <math.h>

#include "MLLight.h"
#include "MLLightPool.h"
#include "MLLightTable.h"
#include "MLPoolTable.h"

#include "scene/MLMesh.h"
#include "scene/MeshRep.h"
#include "scene/MLScene.h"
#include "scene/MLObjectsIterator.h"

/////////////////////////////////////////////////////////////////////////////

// static
const double MLLight::DEFAULT_MAX_EXTENT= 100.;

/////////////////////////////////////////////////////////////////////////////

MLLight::MLLight( MLObject* _father,
                  MLLightPool* pool,
                  boolean _directLight, /* = true */
                  boolean _globalLight  /* = true */ ):
  MLObject(_father)
{
//  name.name= "light-" + MLString::itoa(MLObject::getID()-1);

  int light_name_index= 1;
  MLString light_name= "";
  for(;;)
    {
    light_name= MLString("light-") + MLString::valueOf(light_name_index);
    boolean used= false;
    MLObjectsIterator oi(scene);
    while( ! oi.eol() )
      {
      MLObject* o= oi++;
      if( o->getName().name == light_name )
        {
        used= true;
        break;
        }
      }
    if( ! used )
      break;
    ++light_name_index;
    }
  name.name= light_name;

  assert( father != 0 );

  light_index= 0;
  pool_index= 0;

  // Light Default Parameters
  lightType= POINT;

  globalLight= _globalLight;
  directLight= _directLight;

  color_model= RGB;
  color= Color(1,1,1);
  direct_intensity= 100.;
  global_intensity= 100.;

  unshotEnergy= Spectrum(color,global_intensity);
  energy_has_been_shot= false;

  has_extent= false;
  extent= MLLight::DEFAULT_MAX_EXTENT;

  generate_shadows= true;

  spot_cutoff= DEGtoRAD(60.);
  spot_exponent= 10.;

  radiosity_attenuation= true;
  att_constant.yes= false;
  att_constant.value= 0.;
  att_linear.yes= false;
  att_linear.value= 0.;
  att_quadratic.yes= true;
  att_quadratic.value= 4.*M_PI;

  cone= BoundingCone( Point(0,0,0), Vector(0,0,-1), cos(spot_cutoff) );
  worldPosition= Point(0,0,0);
  worldDirection= Vector(0,0,-1);

  compile();

  // set light_index
  scene->getLightTable()->addLight(this);

  // set pool/default pool, and pool_index
  scene->getPoolTable()->addLightToPool(this,pool);
}

/////////////////////////////////////////////////////////////////////////////

MLLight::~MLLight()
{
  MLLightPool* pool= getPool();
  if( pool != 0 )
    pool->removeLight(this);

  scene->getLightTable()->removeLight(this);

  pool_index= 0;
  light_index= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLLight::setLightType( LightType _lightType )
{
  if( int(_lightType) <= FIRST_TYPE ||
      int(_lightType) >= LAST_TYPE )
    _lightType= POINT; // ???

  if( lightType == _lightType )
    return;

  lightType= _lightType;

  cone= BoundingCone( worldPosition, worldDirection, cos(spot_cutoff) );
}

/////////////////////////////////////////////////////////////////////////////

void MLLight::copy( MLLight* l ) const
{
  if( l == 0 )
    return;

  MLObject::copy( (MLObject*) l );

  l->globalLight= globalLight;
  l->directLight= directLight;

  l->color_model= color_model;
  l->color= color;
  l->direct_intensity= direct_intensity;
  l->global_intensity= global_intensity;
  l->lightType= lightType;
  l->has_extent= has_extent;
  l->extent= extent;
  l->generate_shadows= generate_shadows;

  l->spot_cutoff= spot_cutoff;
  l->spot_exponent= spot_exponent;

  l->cone= cone;
  l->worldPosition= worldPosition;
  l->worldDirection= worldDirection;

  l->att_constant= att_constant;
  l->att_linear= att_linear;
  l->att_quadratic= att_quadratic;
}

/////////////////////////////////////////////////////////////////////////////

MLObject* MLLight::duplicate() const
{
  MLLight* l= new MLLight( getFather(), getPool() );

  ::ref(l);

  copy(l);

  return l;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLight::getLocalBBox( Box& /*box*/ ) const
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLLight::resetUnshotEnergy()
{
  unshotEnergy= Spectrum( color, global_intensity );
  if( unshotEnergy.power() == 0. )
    {
    energy_has_been_shot= true;
    return;
    }

  energy_has_been_shot= false;
}

/////////////////////////////////////////////////////////////////////////////

void MLLight::clearUnshotEnergy()
{
  unshotEnergy.zero();
  energy_has_been_shot= true;
}

/////////////////////////////////////////////////////////////////////////////

MLLightPool* MLLight::getPool() const
{
  return scene->getPoolTable()->getPool(pool_index);
}

/////////////////////////////////////////////////////////////////////////////

void MLLight::updateTransformations()
{
  MLObject::updateTransformations();

  const Matrix4x4& o2w= MLObject::global_xform.get_o2w();

  worldPosition= o2w.apply( Point(0,0,0) );

  worldDirection= o2w.apply( Vector(0,0,-1) );
  worldDirection.setNormalized();

  cone= BoundingCone( worldPosition, worldDirection, cos(spot_cutoff) );
}

/////////////////////////////////////////////////////////////////////////////

// This object *and* its children
// (using global bounding sphere)
boolean MLLight::objectIsUnreachable( MLObject* o ) const
{
  if( o == 0 )
    return true;

  if( ! o->isBounded() )
    return true;

  if( has_extent )
    {
    const BoundingSphere& gbs= o->getGlobalBoundingSphere();

    double distance2= (gbs.getCenter() - worldPosition).norm2();
    double max_dist2= sqr( extent + gbs.getRadius() );

    if( distance2 > max_dist2 )
      return false;
    }

  switch(lightType)
    {
    case POINT:
      return false;

    case DIRECTIONAL:
      {
      const BoundingSphere& gbs= o->getGlobalBoundingSphere();

      return ! gbs.inHalfPlane( worldPosition, worldDirection );
      }

    case SPOT:
      {
      const BoundingSphere& gbs= o->getGlobalBoundingSphere();

      return ! cone.intersectSphere(gbs);
      }

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// Only this local object (mesh)
// (using bounding box)
boolean MLLight::meshIsUnreachable( MLObject* o ) const
{
  if( o == 0 )
    return true;

  if( o->getNChildren() == 0 )
    return objectIsUnreachable(o);

  if( o->getObjectType() != MLObject::MESH )
    return true;

  MLMesh* m= (MLMesh*) o;
  MeshRep* mesh= m->getMeshRep();

  if( mesh == 0 )
    return true;

  if( mesh->nVertices == 0 )
    return true;

  if( has_extent )
    {
    // Transform local bounding box in world coordinates
    const Matrix4x4& o2w= o->getGlobalXForm().get_o2w();
    const Box& bbox= mesh->bbox;
    Point minP= o2w.apply(bbox.getMin());
    Point maxP= o2w.apply(bbox.getMax());
    Vector nx= o2w.apply( Vector(1,0,0) ); nx.setNormalized();
    Vector ny= o2w.apply( Vector(1,0,0) ); ny.setNormalized();
    Vector nz= o2w.apply( Vector(1,0,0) ); nz.setNormalized();

    // and try to exclude bounding sphere of light from this
    // transformed box
//*** COULD BE OPTIMIZED!
    BoundingSphere bs( worldPosition, extent );
    if( bs.inHalfPlane( minP, -nx ) ||
        bs.inHalfPlane( minP, -ny ) ||
        bs.inHalfPlane( minP, -nz ) ||
        bs.inHalfPlane( maxP,  nx ) ||
        bs.inHalfPlane( maxP,  ny ) ||
        bs.inHalfPlane( maxP,  nz ) )
      return true;
    }

  switch(lightType)
    {
    case POINT:
      return false;

    case DIRECTIONAL:
      {
      // Transform directional light in object space
      const Matrix4x4& w2o= o->getGlobalXForm().get_w2o();
      Point objectPosition= w2o.apply(worldPosition);
      Vector objectDirection= w2o.apply(worldDirection);

      return ! mesh->bbox.inHalfPlane( objectPosition, objectDirection );
      }

    case SPOT:
      {
      return ! o->meshPossiblyIntersect(cone);
      }

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLLight::compile()
{
  compiled.cos_cutoff= max( 1e-6, cos(spot_cutoff) );
  compiled.direct_color= color * direct_intensity;
  compiled.global_color= color * global_intensity;

  MLLightPool* pool= getPool();
  if( pool != 0 )
    {
    Spectrum pe= pool->getEnergy();
    Color c= Color(pe.r(),pe.g(),pe.b());
    compiled.direct_color &= c;
    compiled.global_color &= c;
    }

  compiled.has_direct_power= fabs(compiled.direct_color.cabs()) > 1e-20;
  compiled.has_global_power= fabs(compiled.global_color.cabs()) > 1e-20;

  compiled.att_constant= (att_constant .yes ? att_constant .value : 0.);
  compiled.att_linear=   (att_linear   .yes ? att_linear   .value : 0.);
  compiled.att_quadratic=(att_quadratic.yes ? att_quadratic.value : 0.);
  if( fabs(compiled.att_constant)  < 1e-30 &&
      fabs(compiled.att_linear)    < 1e-30 &&
      fabs(compiled.att_quadratic) < 1e-30 )
    compiled.att_constant= 1.;

  compiled.extent2= sqr(extent);

  if( lightType == MLLight::DIRECTIONAL )
    {
    compiled.extent2= 1e30;
    compiled.att_constant= 1.;
    compiled.att_linear= 0.;
    compiled.att_quadratic= 0.;
    }
   else
    {
    if( radiosity_attenuation )
      {
      compiled.att_constant= 0.;
      compiled.att_linear= 0.;
      compiled.att_quadratic= 4. * M_PI;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////
