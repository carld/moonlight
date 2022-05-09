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
  GLUT_RawRenderer.C

  Stephane Rehel

  December 14 1997
  June 1 1998
*/

#include <stdio.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include "MLRawReader.h"

/////////////////////////////////////////////////////////////////////////////

static MLRawReader* raw_scene= 0;

// size of the rendering window
static int width= 500, height= 500;

// pool factor
static int nPools= 0;
static float* pool_factors= 0;

static struct Camera
  {
  double x, y, z; // center of rotation
  double phi, theta; // degrees
  double radius;
  } camera;

/////////////////////////////////////////////////////////////////////////////

static void spheric_gl_camera( Camera& camera )
{
  double phi=   camera.phi  / 180. * M_PI;
  double theta= camera.theta/ 180. * M_PI;

  float x= camera.x + cos(phi) * cos(theta) * camera.radius;
  float y= camera.y + sin(phi) * cos(theta) * camera.radius;
  float z= camera.z +            sin(theta) * camera.radius;

  float vx= -cos(phi) * sin(theta) * camera.radius;
  float vy= -sin(phi) * sin(theta) * camera.radius;
  float vz=             cos(theta) * camera.radius;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 50, GLfloat(width)/GLfloat(height), 0.1, 1e6 );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt( x, y, z, camera.x, camera.y, camera.z, vx, vy, vz );
}

/////////////////////////////////////////////////////////////////////////////

static void draw_it( MLRawReader::Object* o )
{
  for( int i= 0; i < o->nPolygons; ++i )
    {
    MLRawReader::Polygon& p= o->polygons[i];

    glBegin(GL_POLYGON);
    for( int j= 0; j < p.nVertices; ++j )
      {
      MLRawReader::Vertex& v = o->vertices[ p.vertices[j] ];

      if( nPools > 0 )
        {
        MLRawReader::Spectrum s;
        s.r= s.g= s.b= 0.;
        for( int k= 0; k < nPools; ++k )
          {
          register float pf= pool_factors[k];
          register MLRawReader::Spectrum& vl= v.lighting[k];
          s.r+= vl.r * pf;
          s.g+= vl.g * pf;
          s.b+= vl.b * pf;
          }
        glColor3f( s.r, s.g, s.b );
        }

      glNormal3f( v.nx, v.ny, v.nz );
      glVertex3f( v.x, v.y, v.z );
      }
    glEnd();
    }
}

/////////////////////////////////////////////////////////////////////////////

static void draw_it()
{
  for( int i= 0; i < raw_scene->getNObjects(); ++i )
    {
    MLRawReader::Object* o= raw_scene->getObject(i);
    if( o == 0 )
      continue;

    draw_it(o);
    }
}

/////////////////////////////////////////////////////////////////////////////

static void redraw()
{
  spheric_gl_camera(camera);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glShadeModel(GL_SMOOTH);
  glDisable(GL_LIGHTING); // disable opengllighting
  glEnable(GL_DEPTH_TEST); // enable the zbuffer test...

  // tell open not to render reversed faces (back culling faces)
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  draw_it();

  glutSwapBuffers();
}

/////////////////////////////////////////////////////////////////////////////

static void setup_good_camera()
{
  camera.phi= -110.;
  camera.theta= 20.;

  // compute bounding box
  double min_x, min_y, min_z;
  double max_x, max_y, max_z;
  min_x= min_y= min_z= 1e30;
  max_x= max_y= max_z=-1e30;
  for( int i= 0; i < raw_scene->getNObjects(); ++i )
    {
    MLRawReader::Object* o= raw_scene->getObject(i);
    if( o == 0 )
      continue;

    for( int j= 0; j < o->nVertices; ++j )
      {
      register MLRawReader::Vertex& v= o->vertices[j];
      min_x= v.x <? min_x; min_y= v.y <? min_y; min_z= v.z <? min_z;
      max_x= v.x >? max_x; max_y= v.y >? max_y; max_z= v.z >? max_z;
      }
    }

  camera.x= (min_x+max_x) / 2.;
  camera.y= (min_y+max_y) / 2.;
  camera.z= (min_z+max_z) / 2.;
  camera.radius= sqrt( (max_x-min_x)*(max_x-min_x)/4.
                      +(max_y-min_y)*(max_y-min_y)/4.
                      +(max_z-min_z)*(max_z-min_z)/4. ) * 1.8;
}

/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
  if( argc != 2 )
    {
    fprintf( stderr, "Usage:\n%s <file.mlraw>\n", argv[0] );
    return 1;
    }

  const char* filename= argv[1];

  boolean verbose= true;

  FILE* file= fopen( filename, "r" );
  if( file == 0 )
    {
    fprintf( stderr, "unable to open file %s\n", filename );
    return 1;
    }

  raw_scene= new MLRawReader( file, verbose );

  if( !raw_scene->read() )
    {
    fprintf( stderr, "error reading file %s\n", filename );
    return 1;
    }

  fprintf( stderr, "successfully read\n" );

  fclose(file);
  file= 0;

  // let's initialize pools factors: we assign a float to each pool.
  // The lighting information for each pool will be multiply by this
  // factor. You can use it to switch off/on a light or a set of lights
  // of one pool for instance.
  nPools= raw_scene->getNPools();
  pool_factors= new float [ nPools ];
  for( int i= 0; i < nPools; ++i )
    pool_factors[i]= 1.;

  setup_good_camera();

  // Initialize GLUT package
  glutInit(&argc, argv);

  // Initialize a window
  // let's enable depth buffer...
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(width,height);

  // Create a window
  glutCreateWindow(argv[0]);

  // Register callbacks
  glutDisplayFunc(redraw);

  // Setup the created window
  glClearColor(0.0,0.0,0.0,0.0);

  // Go to main event loop
  glutMainLoop();

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
