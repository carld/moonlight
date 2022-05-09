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
  ReadCanvas.C

  Stephane Rehel
  August 1 1996
*/

#include <stdio.h>

#include "graphics/KeyCodes.h"
#include "tools/ListFiles.h"

#include "Canvas.h"
#include "koala/VWindow.h"
#include "koala/VDialog.h"
#include "koala/VCard.h"

#include "GWidget.h"
#include "kw/KWDialog.h"
#include "LoadDlg.h"

/////////////////////////////////////////////////////////////////////////////

void Canvas::readCanvas()
{
  KWDialog* d= new KWDialog;

  d->create( root, LoadDlg::data, (KWCallbackClass*)this );
  d->setText( LoadDlg::edit, filename );
  d->focus(LoadDlg::edit);
  int code= d->run();

  if( code == KWDialog::VALIDATE )
    readCanvas( d->getText(LoadDlg::edit) );

  delete d;
  draw();
}

/////////////////////////////////////////////////////////////////////////////

static FILE* file= 0;

static MLString getLine()
{
  if( feof(file) )
    return MLString("");

  static char line[512];
  *line= '\0';
  fgets( line, 511, file );
  if( feof(file) )
    return MLString("");

  if( *line == '\0' )
    return MLString("");

  if( line[strlen(line)-1] == '\n' )
    line[strlen(line)-1]= 0;

  return MLString(line);
}

static int getInt()
{
  MLString line= getLine();
  int i= 0;
  sscanf( line.get(), "%d", &i );
  return i;
}

static double getDouble()
{
  MLString line= getLine();
  double d= 0;
  sscanf( line.get(), "%lf", &d );
  return d;
}

static void get2Int( int* x, int* y )
{
  MLString line= getLine();
  sscanf( line.get(), "%d %d", x, y );
}

static IVector getIVector()
{
  int x, y;
  get2Int(&x,&y);
  return IVector(x,y);
}

static IPoint getIPoint()
{
  int x, y;
  get2Int(&x,&y);
  return IPoint(x,y);
}

boolean Canvas::readCanvas( const MLString& _filename )
{
  MLString fn= _filename;
  if( ListFiles::getExtension(fn).length() == 0 )
    fn += canvasExtension;

  file= fopen( fn.get(), "rt" );
  if( file == 0 )
    return false;

  int n= 0;

  for(;;)
    {
    char _h[100]; *_h= '\0';
    int hSize= 0;
    MLString line= getLine();
    sscanf( line.get(), "%s %d", _h, &hSize );
    MLString h= MLString(_h);

    if( h == "EndHeader" )
      break;
    if( h == "DialogSize" )
      {
      dialogSize= getIVector();
      continue;
      }
    if( h == "DialogPosition" )
      {
      dialogPosition= getIPoint();
      continue;
      }
    if( h == "Name" )
      {
      name= getLine();
      C_filename= name + MLString(".C");
      h_filename= name + MLString(".h");
      write_C_file= true;
      continue;
      }
    if( h == "C_filename" )
      {
      C_filename= getLine();
      continue;
      }
    if( h == "h_filename" )
      {
      h_filename= getLine();
      continue;
      }
    if( h == "has_C_filename" )
      {
      has_C_filename= getInt();
      continue;
      }
    if( h == "has_h_filename" )
      {
      has_h_filename= getInt();
      continue;
      }
    if( h == "write_C_file" )
      {
      write_C_file= getInt();
      continue;
      }
    if( h == "DialogTitle" )
      {
      title= getLine();
      continue;
      }
    if( h == "DialogFrame" )
      {
      frame= getInt();
      continue;
      }
    if( h == "DialogKillIcon" )
      {
      killButton= getInt();
      continue;
      }
    if( h == "nWidgets" )
      {
      n= getInt();
      continue;
      }
    if( h == "ValidateCancel" )
      {
      validateCancel= getInt();
      continue;
      }
    if( h == "ValidateID" )
      {
      validateID= getInt();
      continue;
      }
    if( h == "CancelID" )
      {
      cancelID= getInt();
      continue;
      }
    if( h == "DefaultGranularity" )
      {
      defaultGranularity= getInt();
      continue;
      }
    if( h == "Granularity" )
      {
      granularity= getInt();
      setGranularity(granularity);
      continue;
      }
    if( h == "Margins" )
      {
      top_margin= getInt();
      bottom_margin= getInt();
      right_margin= getInt();
      left_margin= getInt();
      continue;
      }
    if( h == "CardsRectangle" )
      {
      IPoint p= getIPoint();
      IVector v= getIVector();
      cardsRectangle= IRectangle( p, v );
      continue;
      }

    // unknown variable!
    while( hSize-- )
      getLine();
    }

  create();
  modified= false;
  h_modified= false;

  for( int i= 1; i <= n; ++i )
    {
    GWidget* gw= new GWidget(this);

    getLine();

    for(;;)
      {
      char _h[100]; *_h= '\0';
      int hSize= 0;
      MLString line= getLine();
      sscanf( line.get(), "%s %d", _h, &hSize );
      MLString h= MLString(_h);

      if( h == "EndWidget" )
        {
        widgets.append(gw);
        gw->create(true,false);
        break;
        }

      if( h == "Type" )
        {
        gw->type= VWidget::TYPE(getInt());
        continue;
        }

      if( h == "Position" )
        {
        gw->position= getIPoint();
        continue;
        }
      if( h == "Size" )
        {
        gw->size= getIVector();
        continue;
        }
      if( h == "MinSize" )
        {
        gw->minSize= getIVector();
        continue;
        }
      if( h == "MaxSize" )
        {
        gw->maxSize= getIVector();
        continue;
        }
      if( h == "Sizeable" )
        {
        gw->sizeable= getIVector();
        continue;
        }
      if( h == "Message" )
        {
        gw->message= getLine();
        continue;
        }
      if( h == "Name" )
        {
        gw->name= getLine();
        continue;
        }
      if( h == "List" )
        {
        for( int k= 1; k <= hSize; ++k )
          gw->list.append( new MLString(getLine()) );
        continue;
        }
      if( h == "Available" )
        {
        gw->available= getInt();
        continue;
        }
      if( h == "FixedFont" )
        {
        gw->fixedFont= getInt();
        continue;
        }
      if( h == "FolderPlusMinus" )
        {
        gw->folderPlusMinus= getInt();
        continue;
        }
      if( h == "BigFont" )
        {
        gw->bigFont= getInt();
        continue;
        }
      if( h == "TextList" )
        {
        gw->textList= getInt();
        continue;
        }
      if( h == "EnterPXM" )
        {
        gw->enterPXM= getInt();
        continue;
        }
      if( h == "ID" )
        {
        gw->id= getInt();
        continue;
        }
      if( h == "LinkID" )
        {
        gw->linkWidget= getInt();
        continue;
        }
      if( h == "CardID" )
        {
        int cardID= getInt();
        VCard* c= 0;
        if( cardID == 0 )
          c= 0;
         else
          {
          GWidget* gw= findWidget(cardID);
          if( gw != 0 )
            {
            VWidget* w= gw->widget;
            if( w != 0 )
              if( w->type() == VWidget::CARD )
                c= (VCard*) w;
            }
          }
        gw->card= c;
        continue;
        }
      if( h == "Margin" )
        {
        gw->margin= getInt();
        continue;
        }
      if( h == "Justify" )
        {
        gw->justify= GWidget::Justify(getInt());
        if( gw->justify != GWidget::LEFT &&
            gw->justify != GWidget::CENTER &&
            gw->justify != GWidget::RIGHT )
          gw->justify= GWidget::LEFT;
        continue;
        }
      if( h == "Vertical" )
        {
        gw->vertical= getInt();
        continue;
        }
      if( h == "Scrolling" )
        {
        gw->scrolling= getInt();
        continue;
        }
      if( h == "MinValue" )
        {
        gw->minValue= getDouble();
        continue;
        }
      if( h == "MaxValue" )
        {
        gw->maxValue= getDouble();
        continue;
        }
      if( h == "NDigits" )
        {
        gw->nDigits= getInt();
        continue;
        }
      if( h == "MinValueBar" )
        {
        gw->minValueBar= getDouble();
        continue;
        }
      if( h == "MaxValueBar" )
        {
        gw->maxValueBar= getDouble();
        continue;
        }
      if( h == "EditWidth" )
        {
        gw->editWidth= getInt();
        continue;
        }
      if( h == "PictureFilename" )
        {
        gw->pictureFilename= getLine();
        continue;
        }

      // unknown variable!
      while( hSize-- )
        getLine();
      }
    }

  int nGroups= 0;
  MLString line= getLine();
  sscanf( line.get(), "nGroups %d", &nGroups );
  while( nGroups-- > 0 )
    {
    GGroup* g= new GGroup;
    int n= 0;
    MLString line= getLine();
    sscanf( line.get(), "Group %d", &n );
    while( n-- > 0 )
      {
      int id;
      line= getLine();
      sscanf( line.get(), "%d", &id );
      GWidget* gw= findWidget(id);
      if( gw != 0 )
        g->append(gw);
      }
    if( g->getNElements() <= 1 )
      delete g;
     else
      groups.append(g);
    }

  fclose(file);

  filename= fn;

  makeLinks();
  draw();

  return true;
}

/////////////////////////////////////////////////////////////////////////////
