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
  FormalText.C

  Stephane Rehel

  February 8 1998
*/

#include <stdio.h>

#include "tools/MLConfig.h"
#include "tools/MLPath.h"
#include "tools/MLPathList.h"
#include "tools/MLConfig.h"

#include "tools/file/MLRFileBlock.h"
#include "tools/file/MLWFileBlock.h"

#include "scene/MLScene.h"
#include "scene/MeshRep.h"

#include "FormalText.h"

#include "MLTextGenerate.h"
#include "gltt/FTEngine.h"
#include "gltt/FTFace.h"
#include "gltt/GLTTFont.h"

/////////////////////////////////////////////////////////////////////////////

FormalText::FormalText( MLScene* _scene ): FormalRep(_scene)
{
  text= "Moonlight";
  ttf_name= "congar.ttf";
  precision= 20.;

  front= true;
  side= true;
  back= true;

  extrude_height= 1. / 5.;

  reversed_sides= false;
}

/////////////////////////////////////////////////////////////////////////////

FormalText::~FormalText()
{}

/////////////////////////////////////////////////////////////////////////////

int FormalText::getSizeOf() const
{
  int s= FormalRep::getSizeOf();

  s += sizeof(text) + text.length();

  return s;
}

/////////////////////////////////////////////////////////////////////////////

FormalRep::TYPE FormalText::getType() const
{
  return FormalRep::TEXT;
}

/////////////////////////////////////////////////////////////////////////////

void FormalText::generate( MeshRep* mesh ) const
{
  if( mesh == 0 )
    return;

  FTEngine engine;
  if( ! engine.init() )
    return;

  MLConfig* config= scene->getConfig();
//  MLString ttf_path= config->getVariable( scene->getConfigGroup(),
  MLString ttf_path= config->getVariable( "Moonlight 3D",
                                         "TTFontPath" );
  MLPathList pl;
  pl.set(ttf_path);

  MLPath* path= pl.findFile(ttf_name);
  if( path == 0 )
    {
fprintf(stderr,"FormalText::generate(): file not found '%s/%s'\n",ttf_path.getBytes(),ttf_name.get());
return;
    }

  MLString filename= path->get() + ttf_name;

  FTFace face;
  if( ! face.open(filename.get()) )
    return;

  GLTTFont font(&face);

  font.setPrecision(precision);

  int point_size= 100;
  if( ! font.create(point_size) )
    return;

  MLTextGenerate g;

  boolean outcome= g.generate( mesh, font, this );

if( ! outcome )
  {
  fprintf(stderr,"FormalText::generate(): error generating mesh!\n");
  mesh->clear();
  return;
  }

  mesh->findEdges();

  mesh->finalizeRep();
}

/////////////////////////////////////////////////////////////////////////////

FormalRep* FormalText::duplicate()
{
  FormalText* f= new FormalText(scene);

  f->text= text;
  f->ttf_name= ttf_name;
  f->precision= precision;

  f->front= front;
  f->side= side;
  f->back= back;
  f->extrude_height= extrude_height;

  f->reversed_sides= reversed_sides;

  return f;
}

/////////////////////////////////////////////////////////////////////////////

void FormalText::_read( MLRFileBlock* block )
{
  *block >> text;
  *block >> ttf_name;
  precision= block->getDouble();

  front= block->getInteger() != 0;
  side= block->getInteger() != 0;
  back= block->getInteger() != 0;
  extrude_height= block->getDouble();
  reversed_sides= block->getInteger() != 0;
}

/////////////////////////////////////////////////////////////////////////////

void FormalText::_write( MLWFileBlock* block )
{
  *block << text;
  *block << ttf_name;
  *block << double(precision);

  *block << int(front);
  *block << int(side);
  *block << int(back);
  *block << double(extrude_height);
  *block << int(reversed_sides);
}

/////////////////////////////////////////////////////////////////////////////
