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
  MLDSelectTTFont.C

  Stephane Rehel

  March 8 1998
*/

#include <stdio.h>

#include "tools/MLPath.h"
#include "tools/MLPathList.h"
#include "tools/MLFilename.h"
#include "tools/MLConfig.h"
#include "tools/ListFiles.h"

#include "graphics/mlgl.h"

#include "koala/VWidget.h"
#include "kw/KWDialog.h"

#include "kmodel/ModelModule.h"

#include "MLDSelectTTFont.h"
#include "SelectTTFontDlg.h"

#include "gltt/FTEngine.h"
#include "gltt/FTFace.h"
#include "gltt/GLTTBitmapFont.h"

/////////////////////////////////////////////////////////////////////////////

static int current_font_path= 1;
static const char* error_string= "*ERROR";
static const char* config_path_name= "TTFontPath";

/////////////////////////////////////////////////////////////////////////////

MLDSelectTTFont::MLDSelectTTFont( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;

  engine= new FTEngine;
  engine->init();

  face= 0;
  glttFont= 0;
  drawn_fontname= "";
}

/////////////////////////////////////////////////////////////////////////////

MLDSelectTTFont::~MLDSelectTTFont()
{
  delete glttFont;
  glttFont= 0;

  delete face;
  face= 0;

  delete engine;
  engine= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the .ttf full filename
int MLDSelectTTFont::run( MLString& _font )
{
  if( ! MLGlobalDialog::create(SelectTTFontDlg::data) )
    return false;

  buildRepository();
  buildFontList();

// TODO
kwDialog->setAvailable(SelectTTFontDlg::remove,0);
kwDialog->setAvailable(SelectTTFontDlg::save,0);

  kwDialog->setAvailable(SelectTTFontDlg::add,false);
  kwDialog->focus(SelectTTFontDlg::fonts);

  int outcome= kwDialog->run();

  MLString font_name= getSelectedFont();

  MLGlobalDialog::close();

  if( outcome == KWDialog::VALIDATE )
    {
    FTFace f(engine);

    boolean ok= false;
    if( f.open(font_name.get()) )
      {
      GLTTBitmapFont pf(&f);
      if( pf.create(50) )
        ok= true;
      }

    if( ! ok )
      {
      KWErrorf( "Error", "Unable to load font:\n%s\n", font_name.get() );
      return false;
      }

    _font= font_name;

    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLDSelectTTFont::buildRepository()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(SelectTTFontDlg::reposit);

  MLConfig* config= model->getConfig();
  MLString ttf_path= config->getVariable( model->getConfigGroup(),
                                         ::config_path_name );
  MLPathList pl;
  pl.set(ttf_path);

  if( pl.getNPaths() == 0 )
    {
    buildFontList();
    return;
    }

  for( int i= 1; i <= pl.getNPaths(); ++i )
    {
    MLPath* p= pl.getPath(i);
    if( p == 0 )
      continue;
    kwDialog->appendListText( SelectTTFontDlg::reposit, p->get() );
    }

  if( ::current_font_path < 1 || ::current_font_path > pl.getNPaths() )
    ::current_font_path= 1;

  kwDialog->setInteger( SelectTTFontDlg::reposit, ::current_font_path );

  buildFontList();
}

/////////////////////////////////////////////////////////////////////////////

void MLDSelectTTFont::buildFontList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(SelectTTFontDlg::fonts);

  MLConfig* config= model->getConfig();
  MLString ttf_path= config->getVariable( model->getConfigGroup(),
                                         ::config_path_name );
  MLPathList pl;
  pl.set(ttf_path);

  if( ::current_font_path < 1 || ::current_font_path > pl.getNPaths() )
    ::current_font_path= 1;

  ListFiles lf;
  MLPath* the_path= 0;

  if( pl.getNPaths() == 0 )
    {
error:
    kwDialog->appendListText( SelectTTFontDlg::fonts, ::error_string );
    updatePreview();
    return;
    }

  the_path= pl.getPath(::current_font_path);
  if( the_path == 0 )
    goto error;

  MLString the_dir= the_path->get();

  if( lf.list( the_dir, "*.*", false ) != ListFiles::SUCCESS )
    goto error;

  IListIterator<ListFiles::FileItem> li(lf.getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* fi= li++;
    MLString name= fi->name;
    name= name.toUpperCase();
    MLFilename filename(name);
    if( filename.getExtension() == ".TTF" )
      kwDialog->appendListText( SelectTTFontDlg::fonts, fi->name );
    }

  updatePreview();
}

/////////////////////////////////////////////////////////////////////////////

MLString MLDSelectTTFont::getSelectedFont()
{
  if( kwDialog == 0 )
    return "";

  MLString name= kwDialog->getText(SelectTTFontDlg::fonts).get();

  if( name == ::error_string )
    return "";

  name= kwDialog->getText(SelectTTFontDlg::reposit) + name;
  if( ! ListFiles::exist(name) || ! ListFiles::canRead(name) )
    return "";

  return name;
}

/////////////////////////////////////////////////////////////////////////////

void MLDSelectTTFont::updatePreview()
{
  if( kwDialog == 0 )
    return;

  VWidget* widget= kwDialog->getWidget(SelectTTFontDlg::preview);
  if( widget == 0 )
    return;

  MLString font_name= getSelectedFont();

  if( drawn_fontname != font_name )
    {
    delete glttFont;
    glttFont= 0;

    drawn_fontname= font_name;
    delete face;
    face= new FTFace(engine);

    if( ! face->open(font_name.get()) )
      {
      delete face;
      face= 0;
      goto next;
      }

    glttFont= new GLTTBitmapFont(face);

    int point_size= 65;
    if( ! glttFont->create(point_size) )
      {
      delete face;
      face= 0;
      delete glttFont;
      glttFont= 0;
      goto next;
      }
    }

next:
  MLString text= kwDialog->getText(SelectTTFontDlg::preview_text);
  if( text.length() == 0 )
    text= "Moonlight";

  if( ! widget->beginDraw() )
    return;

  IVector size= widget->getSize();
  widget->scissor();

  mlWhite();
  mlRect( 0, 0, size.x()-1, size.y()-1 );
  if( face == 0 || glttFont == 0 )
    {}
   else
    {
    mlBlack();
    glttFont->output(0,-glttFont->getDescender(),text.get());
    }
  widget->unscissor();

  widget->endDraw();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDSelectTTFont::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::SELECT &&
      event.widget == SelectTTFontDlg::reposit )
    {
    ::current_font_path= kwDialog->getInteger(SelectTTFontDlg::reposit);
    buildFontList();
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SelectTTFontDlg::fonts )
    {
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::KEY_PROCESSED &&
      event.widget == SelectTTFontDlg::new_reposit )
    {
    MLString r= kwDialog->getText(SelectTTFontDlg::new_reposit);
    boolean ok= ListFiles::isAccessibleDirectory(r);
    kwDialog->setAvailable(SelectTTFontDlg::add,ok);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SelectTTFontDlg::add )
    {
    MLString r= kwDialog->getText(SelectTTFontDlg::new_reposit);
    boolean ok= ListFiles::isAccessibleDirectory(r);
    if( ! ok )
      return true;

    MLConfig* config= model->getConfig();
    MLString ttf_path= config->getVariable( model->getConfigGroup(),
                                           ::config_path_name );
    MLPathList pl;
    pl.set(ttf_path);

    kwDialog->setText(SelectTTFontDlg::new_reposit,"");
    kwDialog->setAvailable(SelectTTFontDlg::add,false);

    MLPath p(r);
    MLPath* found= pl.find(p);

    if( found == 0 )
      {
      pl.append(p);

      MLString new_path_list= pl.get();
      config->setVariable( model->getConfigGroup(),
                           ::config_path_name, new_path_list );

      found= &p;
      buildRepository();
      }

    kwDialog->setText(SelectTTFontDlg::reposit,found->get());
    ::current_font_path= kwDialog->getInteger(SelectTTFontDlg::reposit);
    buildFontList();
    return true;
    }

  if( event.type == KWEvent::DRAW &&
      event.widget == SelectTTFontDlg::preview )
    {
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == SelectTTFontDlg::remove )
    {
    MLString r= kwDialog->getText(SelectTTFontDlg::reposit);

    if( KWWarningOkCancelf( "Remove True Type Repository: %s ?",
                            r.get() ) )
      {
      MLConfig* config= model->getConfig();
      MLString ttf_path= config->getVariable( model->getConfigGroup(),
                                             ::config_path_name );
      MLPathList pl;
      pl.set(ttf_path);

      MLPathList pl2;

      for( int i= 1; i <= pl.getNPaths(); ++i )
        {
        MLPath* p= pl.getPath(i);
        if( p == 0 )
          continue;

        if( p->get() == r )
          continue;

        pl2.append(*p);
        }

      MLString new_path_list= pl2.get();
      config->setVariable( model->getConfigGroup(),
                           ::config_path_name, new_path_list );

      ::current_font_path= 1;
      kwDialog->setInteger(SelectTTFontDlg::reposit,::current_font_path);
      buildRepository();
      buildFontList();
      }

    return true;
    }

  if( event.type == KWEvent::KEY_PROCESSED &&
      event.widget == SelectTTFontDlg::preview_text )
    {
    updatePreview();

    return false;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////
