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
  WriteCanvas.C

  Stephane Rehel
  August 1 1996
*/

#include <stdio.h>

#include "tools/ListFiles.h"
#include "graphics/KeyCodes.h"

#include "Canvas.h"
#include "koala/VDialog.h"
#include "koala/VCard.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "SaveAsDlg.h"

static FILE* file= 0;

#define header(name,nb_lines) fprintf( file, "%s %d\n", name, nb_lines );

/////////////////////////////////////////////////////////////////////////////

void Canvas::writeCanvas()
{
  writeCanvas(filename);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::writeCanvasAs()
{
  KWDialog* d= new KWDialog;

  d->create( root, SaveAsDlg::data, (KWCallbackClass*)this );
  d->setText( SaveAsDlg::edit, filename );
  d->focus(SaveAsDlg::edit);
  int code= d->run();

  if( code == KWDialog::VALIDATE )
    writeCanvas( d->getText(SaveAsDlg::edit) );

  delete d;
  draw();
}

/////////////////////////////////////////////////////////////////////////////

static void writeGWidget( GWidget* gw )
{
  fprintf( file, "\n" );

  header( "Type", 1 ); fprintf( file, "%d\n", int(gw->type) );
  header( "Position", 1 ); fprintf( file, "%d %d\n",
                                    gw->position.x(), gw->position.y() );
  header( "Size", 1 ); fprintf( file, "%d %d\n",
                                gw->size.x(), gw->size.y() );
  header( "MinSize", 1 ); fprintf( file, "%d %d\n",
                                   gw->minSize.x(), gw->minSize.y() );
  header( "MaxSize", 1 ); fprintf( file, "%d %d\n",
                                   gw->maxSize.x(), gw->maxSize.y() );
  header( "Sizeable", 1 ); fprintf( file, "%d %d\n",
                                   gw->sizeable.x(), gw->sizeable.y() );
  header( "Message", 1 ); fprintf( file, "%s\n", gw->message.get() );
  header( "ID", 1 ); fprintf( file, "%d\n", gw->id );
  header( "LinkID", 1 ); fprintf( file, "%d\n", gw->linkWidget );
  header( "CardID", 1 ); fprintf( file, "%d\n",
                            (gw->card==0) ? 0 : (gw->card->getWidgetID()) );
  header( "Name", 1 ); fprintf( file, "%s\n", gw->name.get() );

  header( "List", gw->list.getNElements() );
  for( int j= 1; j <= gw->list.getNElements(); ++j )
    fprintf( file, "%s\n", gw->list.get(j)->get() );

  header( "Margin", 1 ); fprintf( file, "%d\n", gw->margin );
  header( "Justify", 1 ); fprintf( file, "%d\n", int(gw->justify) );
  header( "Vertical", 1 ); fprintf( file, "%d\n", gw->vertical );
  header( "Scrolling", 1 ); fprintf( file, "%d\n", gw->scrolling );
  header( "Available", 1 ); fprintf( file, "%d\n", gw->available );
  header( "FixedFont", 1 ); fprintf( file, "%d\n", gw->fixedFont );
  header( "FolderPlusMinus", 1 ); fprintf( file, "%d\n", gw->folderPlusMinus );
  header( "BigFont", 1 ); fprintf( file, "%d\n", gw->bigFont );
  header( "TextList", 1 ); fprintf( file, "%d\n", gw->textList );
  header( "EnterPXM", 1 ); fprintf( file, "%d\n", gw->enterPXM );
  header( "MinValue", 1 ); fprintf( file, "%g\n", gw->minValue );
  header( "MaxValue", 1 ); fprintf( file, "%g\n", gw->maxValue );
  header( "NDigits", 1 ); fprintf( file, "%d\n", gw->nDigits );
  header( "MinValueBar", 1 ); fprintf( file, "%g\n", gw->minValueBar );
  header( "MaxValueBar", 1 ); fprintf( file, "%g\n", gw->maxValueBar );
  header( "EditWidth", 1 ); fprintf( file, "%d\n", gw->editWidth );
  header( "PictureFilename", 1 ); fprintf( file, "%s\n", gw->pictureFilename.get() );

  header( "EndWidget", 0 );
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::writeCanvas( const MLString& _filename )
{
  MLString fn= _filename;
  if( ListFiles::getExtension(fn).length() == 0 )
    fn += canvasExtension;
  file= fopen( fn.get(), "wt" );
  if( file == 0 )
    return false;

  header("DialogPosition",1); fprintf( file, "%d %d\n",
                                       dialogPosition.x(),
                                       dialogPosition.y() );
  header("DialogSize",1 ); fprintf( file, "%d %d\n",
                                    dialogSize.x(), dialogSize.y() );
  header( "nWidgets", 1 ); fprintf( file, "%d\n", widgets.getNElements() );
  header( "DialogTitle", 1 ); fprintf( file, "%s\n", title.get() );
  header( "DialogFrame", 1 ); fprintf( file, "%d\n", frame );
  header( "DialogKillIcon", 1 ); fprintf( file, "%d\n", killButton );
  header( "Name", 1 ); fprintf( file, "%s\n", name.get() );
  header( "C_filename", 1 ); fprintf( file, "%s\n", C_filename.get() );
  header( "h_filename", 1 ); fprintf( file, "%s\n", h_filename.get() );
  header( "has_C_filename", 1 ); fprintf( file, "%d\n", has_C_filename );
  header( "has_h_filename", 1 ); fprintf( file, "%d\n", has_h_filename );
  header( "write_C_file", 1 ); fprintf( file, "%d\n", write_C_file );
  header( "ValidateCancel", 1 ); fprintf( file, "%d\n", validateCancel );
  header( "ValidateID", 1 ); fprintf( file, "%d\n", validateID );
  header( "CancelID", 1 ); fprintf( file, "%d\n", cancelID );
  header( "DefaultGranularity", 1 ); fprintf( file, "%d\n", defaultGranularity );
  header( "Granularity", 1 ); fprintf( file, "%d\n", granularity );
  header( "Margins", 4 ); fprintf( file, "%d\n%d\n%d\n%d\n",
                                   top_margin, bottom_margin,
                                   right_margin, left_margin );
  header( "CardsRectangle", 2 );
  fprintf( file, "%d %d\n%d %d\n",
           cardsRectangle.p1().x(), cardsRectangle.p1().y(),
           cardsRectangle.getSize().x(), cardsRectangle.getSize().y() );

  header( "EndHeader", 0 );

  // first write cards
  int i;
  for( i= 1; i <= widgets.getNElements(); ++i )
    {
    GWidget* gw= widgets.get(i);
    if( ! gw->isCard )
      continue;
    writeGWidget(gw);
    }
  for( i= 1; i <= widgets.getNElements(); ++i )
    {
    GWidget* gw= widgets.get(i);
    if( gw->isCard )
      continue;
    writeGWidget(gw);
    }

  fprintf( file, "nGroups %d\n", groups.getNElements() );
  SIListIterator<GGroup> gi(groups);
  while( ! gi.eol() )
    {
    GGroup* g= gi.next();
    fprintf( file, "Group %d\n", g->getNElements() );
    SIListIterator<GWidget> li(g);
    while( ! li.eol() )
      fprintf( file, "%d\n", li.next()->id );
    }

  fclose(file);
  file= 0;

  filename= fn;

  encodeCanvas_C();

  if( h_modified )
    encodeCanvas_h();

  modified= false;
  h_modified= false;

  return true;

}

/////////////////////////////////////////////////////////////////////////////

