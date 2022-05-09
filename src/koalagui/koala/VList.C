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
  VList.C

  Stephane Rehel
  Creation: SR, July 29th, 1995
  Revisions:
    + SR July 5 1996
    + rewrote
*/

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/MLGLPixmap.h"
#include "graphics/KeyCodes.h"

#include "kw/kw.h"
#include "VOptions.h"
#include "VList.h"
#include "VVertBar.h"

/////////////////////////////////////////////////////////////////////////////

// static
int VList::labelsCounter= 1;
static int margin= 30; // image margin

/////////////////////////////////////////////////////////////////////////////

VList::VList( VWindow* parent ): VWidget(parent)
{
  image= 0;
  scrollBar= 0;

  imageWidth= 0;
  imageHeight= 0;
  virtualHeight= 0;
  preciseVirtualHeight= 0;
  imagePos= 0;
  gimagePos= 0;
  bar= Label(0);
  needRedraw= false;
  needImageRedraw= false;
  hasBar= true;
  updatePositionFlag= true;
  scrollingGranularity= 1;
  drawImageOrigin= IPoint(0,0);
  rightBar= false;
}

/////////////////////////////////////////////////////////////////////////////

VList::~VList()
{
  while( ! items.empty() )
    {
    Item* i= items.getLast();
    items.removeLast();

    VListItemData* data= i->data;
    i->data= 0;
    delete data;
    delete i;
    }

  delete image;
  image= 0;

  if( scrollBar != 0 )
    {
    delete scrollBar;
    scrollBar= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void VList::setRightBar()
{
  rightBar= true;
}

/////////////////////////////////////////////////////////////////////////////

void VList::create( int x, int y, int width, int height, boolean scrolling,
                    int _scrollingGranularity )
{
  VWindow::create( x, y, width, height );

  scrollingGranularity= max( 1, _scrollingGranularity );

  imageHeight= height-4;
  int barWidth= vOptions->scrollBar.width-5;
  if( scrolling )
    {
    scrollBar= new VVertBar(this);
    scrollBar->scrollBar_width= barWidth;
    int x= 3;
    int y= 3;
    if( rightBar )
      {
      x= width-1 - (barWidth+1)-1;
      }
    scrollBar->create( x, y, imageHeight-1,
                       1, imageHeight );

    scrollBar->setWidgetEventHandler(this);

    scrollBar->changeTotalLength( max( 0, virtualHeight-imageHeight ) );
    }

  drawImageOrigin= IPoint(2,2);
  imageWidth= width-4;
  if( scrolling )
    {
    imageWidth-= 1+barWidth+1+1;
    if( !rightBar )
      drawImageOrigin[0]+= 1+barWidth+1+1;
    }
  image= MLGLPixmap::newMLGLPixmap();
  image->create( imageWidth, imageHeight+2*margin );

  redrawImage();
}

/////////////////////////////////////////////////////////////////////////////

void VList::enableBar( boolean yes )
{
  hasBar= yes;
  if( ! yes )
    bar= 0;
}

/////////////////////////////////////////////////////////////////////////////

void VList::drawWidget()
{
  GLint drawBuffer;
  glGetIntegerv( GL_DRAW_BUFFER, &drawBuffer );
  if( drawBuffer != int(GL_FRONT) )
    {
    postDrawWidget();
    return;
    }

  removeRedraw();

redrawImage();
  drawList();
  if( scrollBar != 0 )
    scrollBar->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

VWidget** VList::getWidgetsList( int* nWidgets )
{
  static VWidget* w[2];

  w[0]= this;
  w[1]= scrollBar;
  *nWidgets= 2;

  return w;
}

/////////////////////////////////////////////////////////////////////////////

void VList::drawList()
{
  int width= getSize().x();
  int height= getSize().y();

  if( ! beginDraw() )
    return;

  gray1();
  drawHorizLine( 0, width-1, 0 );
  drawVertLine( 0, 0, height-2 );
  gray2();
  drawHorizLine( 1, width-2, height-2 );
  drawVertLine( width-2, 1, height-2 );
  if( scrollBar != 0 && !rightBar )
    drawRectangle( IPoint(2,2), IPoint(drawImageOrigin.x()-2,height-2) );
  gray0();
  drawHorizLine( 1, width-3, 1 );
  drawVertLine( 1, 1, height-3 );
  if( scrollBar != 0 )
    {
    if( rightBar )
      {
      drawVertLine( ((VWindow*)scrollBar)->getPosition().x()-2, 2, height-2 );
      drawVertLine( width-2, 1, height-3 );
      gray2();
      drawVertLine( ((VWindow*)scrollBar)->getPosition().x()-1, 2, height-2 );
      }
     else
      drawVertLine( drawImageOrigin.x()-1, 2, height-2 );
    }
  gray3();
  drawHorizLine( 0, width-1, height-1 );
  drawVertLine( width-1, 1, height-1 );

  _drawListImage();

  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VList::drawListImage()
{
  if( ! beginDraw() )
    return;
  _drawListImage();
  endDraw();
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VList::_drawListImage()
{
  drawImage( image, 0, margin, imageWidth, imageHeight,
             drawImageOrigin.x(), drawImageOrigin.y() );
  needRedraw= false;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VList::redrawItem( VList::Item* i )
{
  if( i == 0 )
    return;

  if( i->hidden )
    return;

  int y1= i->p1.y() - gimagePos;
  int y2= i->p2.y() - gimagePos;

  if( y1 >= imageHeight )
    return;
  if( y2 >= 0 && i->handler != 0 )
    {
    y2+= margin;
    y2= (imageHeight+2*margin)-1-y2;
    i->handler->handleVListItem( this, i, y2, bar==i->label );

    if( isCurrent() && bar == i->label )
      {
      vOptions->gray0();

      glEnable(GL_LINE_STIPPLE);
      glLineStipple( 1, 0x5555 );

      glBegin(GL_LINE_LOOP);
      glVertex2i( 0, y2 );
      glVertex2i( i->getWidth()-1, y2 );
      glVertex2i( i->getWidth()-1, y2+i->getHeight()-1 );
      glVertex2i( 0, y2+i->getHeight()-1 );
      glEnd();

      glDisable(GL_LINE_STIPPLE);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void VList::redrawImage()
{
  image->currentPixel();

  vOptions->gray2();
  mlRect( 0, 0, imageWidth-1, imageHeight-1+2*margin );

  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* i= li.next();
    if( i->hidden )
      continue;
    if( (i->p1.y() - gimagePos) >= imageHeight )
      break;
    redrawItem(i);
    }

  needImageRedraw= false;
}

/////////////////////////////////////////////////////////////////////////////

void VList::refresh()
{
  if( needImageRedraw )
    {
    redrawImage();
    needRedraw= true;
    }
  if( needRedraw )
//    drawListImage();
    postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

// protected:
VList::Item* VList::newItem( int height,
                             VListItemHandler* handler,
                             VListItemData* data,
                             boolean hidden, /* = false */
                             void* user_data /* = 0 */ )
{
  Item* i= new Item;
  i->handler= handler;
  i->data= data;
  i->height= height;
  i->p1= IPoint( 0, 0 );
  i->p2= IPoint( imageWidth-1, height-1 );
  i->pressArea= IRectangle( i->p1, i->p2 );
  i->hidden= hidden;

  i->label= Label(VList::labelsCounter++);

  i->user_data= user_data;

  return i;
}

/////////////////////////////////////////////////////////////////////////////

VList::Item* VList::appendItem( int height,
                                VListItemHandler* handler,
                                VListItemData* data,
                                boolean hidden, /* = false */
                                void* user_data /* = 0 */ )
{
  Item* i= newItem( height, handler, data, hidden, user_data );

  items.append(i);

  if( ! hidden )
    {
    i->p1+= IVector( 0, preciseVirtualHeight );
    i->p2+= IVector( 0, preciseVirtualHeight );

    preciseVirtualHeight+= i->getHeight();
    }

  updateItemsList();

  return i;
}

/////////////////////////////////////////////////////////////////////////////

// 1 <= i <= nElement+1
VList::Item* VList::insertItem( int i,
                                int height,
                                VListItemHandler* handler,
                                VListItemData* data,
                                boolean hidden, /* = false */
                                void* user_data /* = 0 */ )
{
  if( i < 1 || i > items.getNElements()+1 )
    return 0;
  if( i == items.getNElements()+1 )
    return appendItem( height, handler, data, hidden, user_data );

  Item* it= newItem( height, handler, data, hidden, user_data );

  if( ! hidden )
    {
    int h= 0;
    IListIterator<Item> li(items);
    int j;
    for( j= 1; j < i; ++j )
      {
      Item* item= li.next();
      if( item->hidden )
        continue;
      h+= item->getHeight();
      }

    it->p1+= IVector( 0, h );
    it->p2+= IVector( 0, h );

    items.insert(i,it);

    preciseVirtualHeight+= height;

    li.reset();
    for( j= 1; j <= i; ++j )
      li.next();
    while( ! li.eol() )
      {
      Item* item= li.next();
      if( item->hidden )
        continue;
      item->p1[1]+= height;
      item->p2[1]+= height;
      }
    }
   else
    items.insert(i,it);

  updateItemsList();

  return it;
}

/////////////////////////////////////////////////////////////////////////////

boolean VList::deleteItem( Label label )
{
  if( label == 0 )
    return false;

  IListElement<Item>* e= 0;
  IListIterator<Item> li(items);
  int j= 0;
  while( ! li.eol() )
    {
    e= li.nextElement();
    ++j;
    if( e->element->label == label )
      break;
    }

  if( e == 0 )
    return false;

  if( e->element->label != label )
    return false; // not found

  int height= e->element->getHeight();
  boolean hidden= e->element->hidden;

  items.remove(e);

  if( ! hidden )
    {
    li.reset();
    for( int k= 1; k < j; ++k )
      li.next();
    while( ! li.eol() )
      {
      Item* it= li.next();
      if( it->hidden )
        continue;
      it->p1[1]-= height;
      it->p2[1]-= height;
      }

    preciseVirtualHeight-= height;
    }

  if( bar == label )
    bar= 0;

  if( items.getNElements() == 0 )
    {
    gimagePos= 0;
    imagePos= 0;
    }

  updateItemsList();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

// protected
void VList::updateItemsPositions()
{
  preciseVirtualHeight= 0;

  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* item= li.next();
    if( item->hidden )
      continue;

    item->p1[1]= preciseVirtualHeight;
    item->p2[1]= item->p1[1] + item->getHeight() - 1;

    preciseVirtualHeight+= item->getHeight();
    }

  updateItemsList();
}

/////////////////////////////////////////////////////////////////////////////

// private
void VList::updateItemsList()
{
  // bored with %'s :-))
  virtualHeight= max( 0, preciseVirtualHeight - imageHeight );

//  while( (virtualHeight % scrollingGranularity) != 0 )
//    ++virtualHeight;
  int j= virtualHeight % scrollingGranularity;
  if( j != 0 )
    virtualHeight += scrollingGranularity - j;

  virtualHeight+= imageHeight;

  needImageRedraw= true;

  if( scrollBar != 0 )
    scrollBar->changeTotalLength( max( 0, virtualHeight-imageHeight ) );

  postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

VList::Item* VList::findItem( VList::Label label )
{
  if( label == 0 )
    return 0;
  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* i= li.next();
    if( i->label == label )
      return i;
    }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

VList::Item* VList::findItem( void* user_data )
{
  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* i= li.next();
    if( i->user_data == user_data )
      return i;
    }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean VList::setBar( VList::Label label )
{
  if( ! hasBar )
    return false;

  Item* i= findItem(label);
  if( i == 0 )
    return false;
  if( bar == label )
    return false;

  if( i->hidden )
    return false;

  Item* old_i= findItem(bar);
  bar= label;

  if( ! needImageRedraw )
    {
    image->currentPixel();
    if( old_i != 0 )
      redrawItem(old_i);

    redrawItem(i);
    }

  needRedraw= true;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void VList::setInteger( int i )
{
  if( i < 1 || i > items.getNElements() )
    return;

  Item* item= items.get(i);

  if( setBar(item->label) )
    postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

// may return 0
int VList::getInteger()
{
  if( ! hasBar || bar == 0 )
    return 0;

  int index= 1;
  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    if( li.next()->label == bar )
      return index;
    ++index;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void* VList::getUserData( int i )
{
  if( i < 1 || i > items.getNElements() )
    return 0;

  Item* item= items.get(i);

  if( item == 0 )
    return 0;

  return item->user_data;
}

/////////////////////////////////////////////////////////////////////////////

// may return 0
int VList::getUserDataIndex( void* user_data )
{
  int index= 1;
  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* item= li.next();
    if( item->user_data == user_data )
      return index;
    ++index;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int VList::getLength() const
{
  return items.getNElements();
}

/////////////////////////////////////////////////////////////////////////////

void VList::setImagePos( int newImagePos )
{
  newImagePos= max( 0, newImagePos );
  newImagePos= min( max(0,virtualHeight-imageHeight), newImagePos );
  if( newImagePos != imagePos )
    {
    imagePos= newImagePos;

    if( updatePositionFlag && scrollBar != 0 )
      scrollBar->changePosition(imagePos+1);

    int new_gimagePos= imagePos + scrollingGranularity/2;
    new_gimagePos-= new_gimagePos % scrollingGranularity;

    if( new_gimagePos != gimagePos )
      {
      gimagePos= new_gimagePos;
      postRedraw();
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean VList::handleEvent( MLEvent& event )
{
  if( event.type == MLEvent::REDRAW )
    {
redrawImage();
    drawList();
    return true;
    }

  if( event.type == MLEvent::MOUSE_PRESS )
    {
    static unsigned long last_time= 0;

    if( ! isCurrent() )
      {
      setCurrent(true);
      redrawImage();
      drawList();
      }

    if( event.button == 1 && !event.control_pressed() )
      {
      int x= event.position.x()-drawImageOrigin.x();
      int y= event.position.y()-drawImageOrigin.y();
      if( x < 0 || x >= imageWidth || y < 0 || y >= imageHeight )
        return true;

      y+= imagePos;
      Item* i= 0;
      IListIterator<Item> li(items);
      while( ! li.eol() )
        {
        Item* it= li.next();
        if( it->hidden )
          continue;
        if( it->p1.y() <= y && y <= it->p2.y() )
          {
          i= it;
          break;
          }
        }
      if( i == 0 )
        return true;

//      boolean ok= (i->label != bar) && hasBar;
      boolean ok= hasBar;
      int prev_bar= bar;
      setBar(i->label);
      if( i->label != prev_bar )
        drawListImage();

      double delta_time= double(event.time - last_time) * 1e-3;
      last_time= event.time;

      if( ok )
        {
        if( delta_time < vOptions->doubleClickDelay )
          {
          if( ! handleDoubleClick(i->label) )
            kwCall(KWEvent::DOUBLE_SELECT);
          }
         else
          {
          if( ! handleClick( i->label, x - i->p1.x(), y - i->p1.y() ) )
            kwCall(KWEvent::SELECT);
          }
        }
      }
    return true;
    }

  if( event.type == MLEvent::MOUSE_MOTION )
    {
    if( ! event.middle_pressed() )
      {
      if( ! (event.left_pressed() && event.control_pressed()) )
        return true;
      }
    int dy= event.size.y();

    setImagePos( imagePos - dy );
    return true;
    }

  return VWindow::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

// handles private widget events from VScrollBar
boolean VList::handlePrivateWidgetEvent( const VWidgetEvent& event )
{
  if( scrollBar == 0 )
    return true;

  if( event.getWidget() == scrollBar )
    {
    if( event.type() == VWidgetEvent::SETBARPOS )
      {
      scrollBar->handlePrivateWidgetEvent(event);
      updatePositionFlag= false; // prevent from infinite loop
      setImagePos( scrollBar->getPosition()-1 );
      updatePositionFlag= true;
      return true;
      }
    }

  return scrollBar->handlePrivateWidgetEvent(event);
}

/////////////////////////////////////////////////////////////////////////////

void VList::visibleItem( Label label )
{
  Item* i= findItem(label);
  if( i == 0 )
    return;

  int np= gimagePos;

  if( i->getHeight() >= imageHeight )
    np= i->p1.y();
   else
    {
    int y1= i->p1.y() - gimagePos;
    int y2= i->p2.y() - gimagePos;
    if( y1 >= 0 && y2 < imageHeight )
      return; // already visible

    if( y1 < 0 )
      np-= -y1;

    if( y2 >= imageHeight )
      np+= y2-imageHeight+1 + scrollingGranularity-1;
    }

  setImagePos(np);
}

/////////////////////////////////////////////////////////////////////////////

boolean VList::handleKey( Key key )
{
  int index= 0;
  int _index= 1;
  IListIterator<Item> li(items);
  while( ! li.eol() )
    {
    Item* it= li.next();
    if( it->label == bar )
      {
      index= _index;
      break;
      }
    ++_index;
    }

  int nItems= items.getNElements();
  int delta= 0;

  if( key == KeyCodes::DOWN )
    {
    delta= +1;
    goto end;
    }

  if( key == KeyCodes::UP )
    {
    delta= -1;
    goto end;
    }
  if( key == KeyCodes::PAGEUP )
    {
//    delta= -imageHeight / scrollingGranularity;
    delta= -nItems * imageHeight / virtualHeight;
    if( delta > -1 )
      delta= -1;
    goto end;
    }
  if( key == KeyCodes::PAGEDOWN )
    {
    delta= nItems * imageHeight / virtualHeight;
    if( delta < 1 )
      delta= 1;
    goto end;
    }
  if( key == KeyCodes::HOME )
    {
    delta= -nItems;
    goto end;
    }
  if( key == KeyCodes::END )
    {
    delta= nItems;
    goto end;
    }
  if( key == KeyCodes::SPACE || key == KeyCodes::ENTER )
    {
    if( bar != 0 && hasBar )
      kwCall(KWEvent::SELECT);
    }
  return false;

end:
  if( index == 0 )
    index= 1;
   else
    {
    int save_index= index;

    for(;;)
      {
      index+= delta;
      if( index < 1 ) index= 1;
      if( index > nItems ) index= nItems;

      Item* new_item= items.get(index);
      if( ! new_item->hidden )
        break;
      if( index == nItems || index == 1 )
        {
        save_index= index;
        if( delta == -1 || delta == 1 )
          break;
        delta= (delta>=0) ? 1 : -1;
        }
      }
    }

  Item* new_item= items.get(index);

  if( bar == new_item->label )
    return true; // no move

  bar= new_item->label;
  visibleItem(bar);
  postRedraw();

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean VList::handleClick( int /*label*/, int /*x*/, int /*y*/ )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean VList::handleDoubleClick( int /* label */ )
{
  return false;
}

/////////////////////////////////////////////////////////////////////////////
