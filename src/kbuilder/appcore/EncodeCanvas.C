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
  EncodeCanvas.C

  Stephane Rehel
  August 1 1996
*/

#include <time.h>
#include <stdio.h>

#include "tools/zlib/MLLibZ.h"

#include "graphics/MLPicture.h"

#include "koala/VCard.h"
#include "Canvas.h"
#include "GWidget.h"

/////////////////////////////////////////////////////////////////////////////

static unsigned char* bytes= 0;
static int length= 0;
static int bytes_index= 0;
static SIList<MLPicture> pictures;

static inline void _encode( unsigned char b )
{
  if( bytes != 0 )
    bytes[bytes_index]= b;
  ++bytes_index;
}

static void encode( int i )
{
  if( bytes == 0 )
    {
    bytes_index+= 4;
    return;
    }

  _encode( (((unsigned int)i) >> 24) & 0xff );
  _encode( (((unsigned int)i) >> 16) & 0xff );
  _encode( (((unsigned int)i) >>  8) & 0xff );
  _encode( (((unsigned int)i) >>  0) & 0xff );
}

static void encode( const MLString& s )
{
  encode(s.length());

  if( bytes == 0 )
    {
    bytes_index+= s.length();
    return;
    }

  for( int i= 0; i < s.length(); ++i )
    _encode( s.charAt(i) );
}

static void encode_double( double d )
{
  encode( MLString::printf( "%g", d ) );
}

static int getPictureIndex( MLPicture* p )
{
  int i;
  for( i= 1; i <= pictures.getNElements(); ++i )
    if( pictures.get(i) == p )
      return i;
  return 0;
}

void Canvas::_encodeCanvas()
{
  int i;

  pictures.destroy();

  // make MLPicture list
  for( i= 1; i <= widgets.getNElements(); ++i )
    {
    GWidget* gw= widgets.get(i);
    if( gw->type == VWidget::PICTURE && gw->picture != 0 )
      {
      if( gw->picture->getData() == 0 )
        continue;
      if( getPictureIndex(gw->picture) == 0 )
        pictures.append(gw->picture);
      }
    }

  int nPictures= pictures.getNElements();

  encode(1); // version

  unsigned int flags= 0;
  if( validateCancel ) flags |= ((unsigned int)1) << 0;
  if( frame )          flags |= ((unsigned int)1) << 1;
  if( killButton )     flags |= ((unsigned int)1) << 2;
  if( nPictures != 0 ) flags |= ((unsigned int)1) << 3;
  if( hasCards() )     flags |= ((unsigned int)1) << 4;
  encode(flags);

  if( hasCards() )
    {
    encode(cardsRectangle.p1().x());
    encode(cardsRectangle.p1().y());
    encode(cardsRectangle.getSize().x());
    encode(cardsRectangle.getSize().y());
    }

  if( nPictures != 0 )
    {
    boolean compressed= true;

    unsigned int flags= 0;

    if( compressed ) flags |= ((unsigned int)1) << 0;

    // encode pictures
    encode(flags); // flags
    encode(nPictures);
    for( int j= 1; j <= nPictures; ++j )
      {
      MLPicture* p= pictures.get(j);
      encode(p->getWidth());
      encode(p->getHeight());
      encode(p->hasAlpha());
      int image_size= p->getWidth()*p->getHeight() * (p->hasAlpha() ? 4 : 3);

      if( compressed )
        {
        int compression_level= 6;
        unsigned char* out_compr= new unsigned char [ image_size ];
        int compressed_size= 0;

        MLLibZ::deflate( (void*) p->getData(), image_size,
                         (void*) out_compr, image_size,
                         &compressed_size,
                         compression_level );

        int size= compressed_size;
        encode(size);
        if( bytes != 0 )
          {
          unsigned char* data= out_compr;
          while( size-- )
            _encode( *(data++) );
          }
         else
          bytes_index+= size;

        delete out_compr;
        out_compr= 0;
        }
       else
        {
        int size= image_size;
        encode(size);
        if( bytes != 0 )
          {
          unsigned char* data= p->getData();
          while( size-- )
            _encode( *(data++) );
          }
         else
          bytes_index+= size;
        }
      }
    }

  if( validateCancel )
    {
    encode(validateID);
    encode(cancelID);
    }

  if( frame )
    encode(title);
  encode(name);

  encode(dialogSize.x());
  encode(dialogSize.y());
  encode(widgets.getNElements());

  for( i= 1; i <= widgets.getNElements(); ++i )
    {
    GWidget* gw= widgets.get(i);
    VWidget* w= gw->widget;
    assert( w != 0 );

    encode( int(gw->type) );
    encode( int(w->getWidgetID()) );
    encode( int(gw->position.x()) );
    encode( int(gw->position.y()) );
    encode( int(gw->size.x()) );
    encode( int(gw->size.y()) );

    unsigned int flags= 0;
    if( w->getLinkWidget()!=0 ) flags |= ((unsigned int)1) << 0;
    if( w->isStaticWidget() )   flags |= ((unsigned int)1) << 1;
    if( gw->vertical )          flags |= ((unsigned int)1) << 2;
    if( gw->scrolling )         flags |= ((unsigned int)1) << 3;
    if( gw->available )         flags |= ((unsigned int)1) << 4;
    if( gw->fixedFont )         flags |= ((unsigned int)1) << 5;
    if( gw->enterPXM )          flags |= ((unsigned int)1) << 6;
    if( gw->textList )          flags |= ((unsigned int)1) << 7;
    if( gw->justify != GWidget::LEFT )  flags |= ((unsigned int)1) << 8;
    if( gw->justify == GWidget::RIGHT ) flags |= ((unsigned int)1) << 9;
    if( gw->margin != 0 )       flags |= ((unsigned int)1) << 10;
    if( gw->bigFont )           flags |= ((unsigned int)1) << 11;
    if( gw->card != 0 )         flags |= ((unsigned int)1) << 12;
    if( gw->folderPlusMinus )   flags |= ((unsigned int)1) << 13;

    encode(flags);

    if( gw->margin != 0 )
      encode(gw->margin);

    if( w->getLinkWidget() != 0 )
      encode( int(w->getLinkWidget()->getWidgetID()) );

    if( gw->card != 0 )
      encode(gw->card->getWidgetID());

    switch(w->type())
      {
      case VWidget::BUTTON:
      case VWidget::LABEL:
      case VWidget::CHECKBOX:
      case VWidget::TEXTBOX:
      case VWidget::GROUP:
      case VWidget::RADIOBUTTON:
      case VWidget::CARD:
        encode(gw->message);
        break;

      case VWidget::VALUEEDIT:
        {
        encode_double(gw->minValue);
        encode_double(gw->maxValue);
        encode( gw->nDigits );
        break;
        }

      case VWidget::RANGEBAR:
        {
        encode_double(gw->minValue);
        encode_double(gw->maxValue);
        break;
        }

      case VWidget::VALUEBAR:
        {
        encode_double(gw->maxValue);
        encode_double(gw->minValue);

        encode( gw->nDigits );
        encode_double(gw->minValueBar);
        encode_double(gw->maxValueBar);
        encode( gw->editWidth );
        break;
        }

      case VWidget::PICTURE:
        {
        int index= getPictureIndex( gw->picture );
        encode(index); // may be NULL
        break;
        }

      case VWidget::CHOOSER:
        {
        encode( gw->list.getNElements() );
        SIListIterator<MLString> li(gw->list);
        while( ! li.eol() )
          encode(*li.next());
        break;
        }

      default:
        break;
      }
    }

  pictures.destroy();

  int nGroups= 0;
  for( i= 1; i <= groups.getNElements(); ++i )
    if( isRadioBoxGroup(groups.get(i)) )
      ++nGroups;
  encode(nGroups);

  for( i= 1; i <= groups.getNElements(); ++i )
    {
    GGroup* g= groups.get(i);
    if( !isRadioBoxGroup(g) )
      continue;
    encode(g->getNElements());

    SIListIterator<GWidget> li(g);
    while( ! li.eol() )
      encode( int(li.next()->id) );
    }
}

/////////////////////////////////////////////////////////////////////////////

int Canvas::findEncodingLength()
{
  length= 0;
  bytes_index= 0;
  bytes= 0;

  _encodeCanvas();

  return bytes_index;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::encodeCanvas_C()
{
  if( C_filename.length() == 0 )
    return true;
  if( ! write_C_file )
    return true;

  length= findEncodingLength();

  bytes= new unsigned char [ length ];
  bytes_index= 0;

  _encodeCanvas();

  FILE* C_file= fopen( C_filename.get(), "wt" );
  if( C_file == 0 )
    return false;

  int line_index= 0;

  #define writeb(b,end) { \
    fprintf( C_file, "0x%02x%s", (b), (end) ? "" : "," ); \
    if( line_index++ == 13 ) { line_index= 0; fprintf( C_file, "\n " ); } }

  time_t _time; time(&_time);
  char* theDate= ctime(&_time);

  fprintf( C_file,
           "// This file is part of Moonlight Creator\n"
           "//   Copyright (C) 1996-1998  Stephane Rehel\n"
           "//\n"
           "// This program is free software; you can redistribute it and/or\n"
           "// modify it under the terms of the GNU Library General Public\n"
           "// License as published by the Free Software Foundation; either\n"
           "// version 2 of the License, or (at your option) any later version.\n"
           "//\n"
           "// This program is distributed in the hope that it will be useful,\n"
           "// but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
           "// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
           "// Library General Public License for more details.\n"
           "//\n"
           "// You should have received a copy of the GNU Library General Public\n"
           "// License along with this library; if not, write to the Free\n"
           "// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"
           "\n" );

  fprintf( C_file, "/*\n"
                   "  %s\n"
                   "  %s\n"
                   "  Automatically generated by kBuilder\n"
                   "  Stephane Rehel, August 1996\n"
                   "*/\n\n"
                   "#include \"%s.h\"\n"
                   "\n",
                   C_filename.get(), theDate, name.get() );


  fprintf( C_file, "const int %s::width = %d;\n"
                   "const int %s::height= %d;\n\n",
                   name.get(), dialogSize.x(),
                   name.get(), dialogSize.y() );
  fprintf( C_file, "const unsigned char %s::data[%d] = {\n ",
           name.get(), length+4 );

  writeb( ((((unsigned int)length)>>24) & 0xff), 0 );
  writeb( ((((unsigned int)length)>>16) & 0xff), 0 );
  writeb( ((((unsigned int)length)>> 8) & 0xff), 0 );
  writeb( ((((unsigned int)length)>> 0) & 0xff), 0 );

  for( int j= 0; j < length; ++j )
    writeb( bytes[j], j==length-1 );

  fprintf( C_file, " };\n" );
  fclose(C_file);

  #undef writeb

  delete bytes;
  bytes= 0;
  bytes_index= 0;
  length= 0;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::encodeCanvas_h()
{
  if( h_filename.length() == 0 )
    return true;

  length= findEncodingLength();

  time_t _time; time(&_time);
  char* theDate= ctime(&_time);

  FILE* file= fopen( h_filename.get(), "wt" );
  if( file == 0 )
    return false;

  fprintf( file,
           "// This file is part of Moonlight Creator\n"
           "//   Copyright (C) 1996-1998  Stephane Rehel\n"
           "//\n"
           "// This program is free software; you can redistribute it and/or\n"
           "// modify it under the terms of the GNU Library General Public\n"
           "// License as published by the Free Software Foundation; either\n"
           "// version 2 of the License, or (at your option) any later version.\n"
           "//\n"
           "// This program is distributed in the hope that it will be useful,\n"
           "// but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
           "// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
           "// Library General Public License for more details.\n"
           "//\n"
           "// You should have received a copy of the GNU Library General Public\n"
           "// License along with this library; if not, write to the Free\n"
           "// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"
           "\n" );

  fprintf( file, "/*\n"
                 "  %s\n"
                 "  %s\n"
                 "  Automatically generated by kBuilder\n"
                 "  Stephane Rehel, August 1996\n"
                 "*/\n\n", h_filename.get(), theDate );

  fprintf( file,
           "#ifndef __%s_h\n"
           "#define __%s_h\n"
           "\n"
           "class %s\n"
           "{\n"
           "public:\n"
           "  static const int width;\n"
           "  static const int height;\n"
           "  static const unsigned char data[];\n",
           name.get(), name.get(), name.get()/*, length+4*/ );

  if( ! widgets.empty() )
    {
    fprintf( file,
             "\n"
             "  enum\n"
             "    {\n" );

    int max_len= 0;
    int i;
    for( i= 1; i <= widgets.getNElements(); ++i )
      max_len= max( (unsigned int) max_len, (unsigned int) widgets.get(i)->name.length() );

    for( i= 1; i <= widgets.getNElements(); ++i )
      {
      GWidget* gw= widgets.get(i);
      fprintf( file,
               "    %*s = %d%s\n",
               max_len, gw->name.get(), gw->id,
               (i==widgets.getNElements()) ? "" : "," );
      }

    fprintf( file,
             "    };\n" );
    }

  fprintf( file,
           "};\n"
           "\n"
           "#endif // ifndef __%s_h\n", name.get() );

  fclose(file);

  return true;
}

/////////////////////////////////////////////////////////////////////////////
