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
  CanvasMouse.C

  Stephane Rehel
  October 24 1997

  from Canvas.C

  *** CAREFUL: This is disgusting code ***
  ***          Sorry.                  ***
*/

#include <stdlib.h>

#include "graphics/MLEvent.h"

#include "koala/VDialog.h"
#include "koala/VCartouche.h"

#include "GWidget.h"
#include "Canvas.h"
#include "CanvasXinfo.h"

/////////////////////////////////////////////////////////////////////////////

GWidget* Canvas::getMousePosition( const IPoint& pos, int& mousePos )
{
  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( gw->widget == 0 || !gw->isCard )
      continue;
    IRectangle rect= gw->widget->getWidgetRectangle();

    if( IRectangle( rect.p1()-IVector(1,1),
                    rect.p2()+IVector(1,1) ).into(pos) )
      {
      mousePos= INTO;
      return gw;
      }
    }

  for( int i= dialog->getWidgets().getNElements(); i; --i )
    {
    VWidget* w= dialog->getWidgets().get(i);
    GWidget* gw= 0;
    SIListIterator<GWidget> li2(widgets);
    while( ! li2.eol() )
      {
      gw= li2.next();
      if( gw->widget == w )
        break;
      gw= 0;
      }
    if( gw == 0 )
      continue;

    if( ! w->mapped() )
      continue;
    IRectangle rect= w->getWidgetRectangle();

    if( gw->isCard )
      {
      if( selectedWidgets.exist(gw) )
        rect= cardsRectangle;
      }

    IPoint center= (rect.p1() + rect.p2())/2;
    IPoint p1= IPoint( center.x(), rect.p1().y() );
    IPoint p2= IPoint( rect.p2().x(), center.y() );
    IPoint p3= IPoint( center.x(), rect.p2().y() );
    IPoint p4= IPoint( rect.p1().x(), center.y() );

    const int h= xorRectangleSize;
    p1-= IVector(h,h)/2;
    p2-= IVector(h,h)/2;
    p3-= IVector(h,h)/2;
    p4-= IVector(h,h)/2;

    if( ! IRectangle( rect.p1()-IVector(h,h),
                      rect.p2()+IVector(h,h) ).into(pos) )
      continue;

    if( IRectangle( rect.p1()+IVector(1,1),
                    rect.p2()-IVector(1,1) ).into(pos) )
      {
      mousePos= INTO;
      return gw;
      }
    if( IRectangle(p1,IVector(h,h)).into(pos) )
      {
      mousePos= UP;
      return gw;
      }
    if( IRectangle(p2,IVector(h,h)).into(pos) )
      {
      mousePos= RIGHT;
      return gw;
      }
    if( IRectangle(p3,IVector(h,h)).into(pos) )
      {
      mousePos= DOWN;
      return gw;
      }
    if( IRectangle(p4,IVector(h,h)).into(pos) )
      {
      mousePos= LEFT;
      return gw;
      }
    }

  mousePos= NOT_INTO;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::mousePress( MLEvent& event )
{
  dragging= true;

  IPoint pos= event.position - IVector(cartouche->getClientArea().p1().x(),
                                       cartouche->getClientArea().p1().y());

  GWidget* gw= getMousePosition(pos,mousePos);

  if( mousePos == NOT_INTO && event.button == 1 )
    {
    if( ! event.shift_pressed() )
      {
      xorSelected();
      selectedWidgets.destroy();
      }
    tmp_drag_pos= IVector(0,0);
    drag_pos= IVector(0,0);
    drag_origin= event.position;
    XSetLineAttributes( xinfo->display, xinfo->gc,
                        1, // line_width
                        LineOnOffDash, // line_style
                        CapButt, // cap_style
                        JoinMiter // join_style
                      );
    return;
    }

  if( gw == 0 )
    return;
  VWidget* w= gw->widget;
  if( w == 0 )
    return;

  if( !event.shift_pressed() || mousePos != INTO )
    {
    xorSelected();
    selectedWidgets.destroy();
    }

  if( event.button == 3 && !event.control_pressed() )
    {
    if( gw->isCard )
      return;
    xorSelected();
    selectedWidgets.destroy();
    selectedWidgets.append(gw);
    xorSelected(gw);
    raiseWidgets(true);
    mousePos= NOT_INTO;
    modified= true;
    return;
    }
  if( event.button == 3 && event.control_pressed() )
    {
    if( gw->isCard )
      return;
    xorSelected();
    selectedWidgets.destroy();
    selectedWidgets.append(gw);
    xorSelected(gw);
    raiseWidgets(false);
    mousePos= NOT_INTO;
    modified= true;
    return;
    }

  GGroup* g= event.control_pressed() ? findGroup(gw) : 0;
  if( g == 0 )
    {
    if( selectedWidgets.exist(gw) )
      selectedWidgets.remove(gw);
     else
      {
      if( gw->isCard )
        {
        xorSelected();
        selectedWidgets.destroy();
        }
      if( !selectedWidgets.empty() )
        {
        if( selectedWidgets.getFirst()->card != gw->card )
          {
          xorSelected();
          selectedWidgets.destroy();
          }
        }
      selectedWidgets.append(gw);
      }
    xorSelected(gw);
    }
   else
    selectGroup(g,false);

  tmp_drag_pos= IVector(0,0);
  drag_pos= IVector(0,0);

  if( selectedWidgets.empty() )
    {
    drag_origin= IPoint(0,0);
    drag_origin2= IPoint(0,0);
    }
   else
    {
    if( gw->isCard )
      {
      drag_origin= cardsRectangle.p1();
      drag_origin2= cardsRectangle.p2();
      if( activeCard != (VCard*) gw->widget )
        {
        setActiveCard( (VCard*) gw->widget );
        draw();
        }
      return;
      }
     else
      {
      drag_origin= IPoint(10000,10000);
      drag_origin2= IPoint(-10000,-10000);
      SIListIterator<GWidget> li(selectedWidgets);
      while( ! li.eol() )
        {
        IRectangle rect= li.next()->widget->getWidgetRectangle();
        drag_origin= min( drag_origin, rect.p1() );
        drag_origin2= max( drag_origin2, rect.p2() );
        }
      }
    }

  drawInfos();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::mouseMotion( MLEvent& event )
{
  dragging= true;

  if( mousePos == NOT_INTO && event.left_pressed() )
    {
    drawXORRectangle( IRectangle( drag_origin, drag_pos ), false );
    drag_pos+= event.size;
    drawXORRectangle( IRectangle( drag_origin, drag_pos ), false );
    return;
    }

  if( selectedWidgets.empty() )
    return;

  if( mousePos == INTO && event.left_pressed() )
    {
    // just drag them
    SIListIterator<GWidget> li(selectedWidgets);
    while( ! li.eol() )
      {
      GWidget* gw= li.next();
      IRectangle rect;
      if( gw->isCard )
         rect= cardsRectangle;
        else
         rect= gw->widget->getWidgetRectangle();
      xorRectangle( gw,
                    IRectangle( rect.p1()+drag_pos,
                                IVector(rect.width(),rect.height()) ) );
      }

    tmp_drag_pos+= event.size;

    if( abs(tmp_drag_pos.x()) < drag_pos_gravity &&
        abs(tmp_drag_pos.y()) < drag_pos_gravity )
      drag_pos= IVector(0,0);
     else
      {
      int x= drag_origin.x() + tmp_drag_pos.x();
      int y= drag_origin.y() + tmp_drag_pos.y();
      x-= x % granularity;
      y-= y % granularity;
      drag_pos= IVector( x - drag_origin.x(), y - drag_origin.y() );

      int min_x= 0;
      int min_y= 0;
      if( selectedWidgets.getFirst()->card != 0 )
        {
        min_x= cardsRectangle.p1().x();
        min_y= cardsRectangle.p1().y();
        }
      if( drag_origin.x()+drag_pos[0] < min_x )
        drag_pos[0]= min_x-drag_origin.x();
      if( drag_origin.y()+drag_pos[1] < min_y )
        drag_pos[1]= min_y-drag_origin.y();

      int max_x= dialog->getSize().x();
      int max_y= dialog->getSize().y();
      if( selectedWidgets.getFirst()->card != 0 )
        {
        max_x= cardsRectangle.p2().x();
        max_y= cardsRectangle.p2().y();
        }
      if( drag_origin2.x()+drag_pos[0] >= max_x )
        drag_pos[0]= max_x-drag_origin2.x()-1;
      if( drag_origin2.y()+drag_pos[1] >= max_y )
        drag_pos[1]= max_y-drag_origin2.y()-1;
      }

    li.reset();
    while( ! li.eol() )
      {
      GWidget* gw= li.next();
      IRectangle rect;
      if( gw->isCard )
         rect= cardsRectangle;
        else
         rect= gw->widget->getWidgetRectangle();
      xorRectangle( gw,
                    IRectangle( rect.p1()+drag_pos,
                                IVector(rect.width(),rect.height()) ) );
      }
    return;
    }

  GWidget* gw= selectedWidgets.getFirst();
  VWidget* w= gw->widget;
  if( w == 0 )
    return;

  if( mousePos == RIGHT || mousePos == DOWN  && event.left_pressed() )
    {
    IRectangle rect;
    if( gw->isCard )
       rect= cardsRectangle;
      else
       rect= gw->widget->getWidgetRectangle();
    xorRectangle( gw, IRectangle( rect.p1(), rect.p2()+drag_pos ) );

    tmp_drag_pos+= event.size;

    if( abs(tmp_drag_pos.x()) < drag_pos_gravity &&
        abs(tmp_drag_pos.y()) < drag_pos_gravity )
      {
      drag_pos= IVector(0,0);
      xorRectangle( gw, IRectangle( rect.p1(), rect.p2()+drag_pos ) );
      return;
      }

    int x= rect.p2().x();
    int y= rect.p2().y();
    if( mousePos == RIGHT )
      {
      x+= tmp_drag_pos.x();
      x-= x % granularity;
      }
     else
      {
      y+= tmp_drag_pos.y();
      y-= y % granularity;
      }

    IVector size( x-rect.p1().x()+1, y-rect.p1().y()+1 );
    size= max( size, gw->minSize );
    if( gw->maxSize[0] != 0 )
      size[0]= min( size[0], gw->maxSize[0] );
    if( gw->maxSize[1] != 0 )
      size[1]= min( size[1], gw->maxSize[1] );
    x= rect.p1().x() + size.x() - 1;
    y= rect.p1().y() + size.y() - 1;

    int max_x= dialog->getSize().x()-1;
    int max_y= dialog->getSize().y()-1;
    if( gw->card != 0 )
      {
      max_x= cardsRectangle.p2().x()-1;
      max_y= cardsRectangle.p2().y()-1;
      }
    x= min( x, max_x );
    y= min( y, max_y );

    drag_pos= IVector( x - rect.p2().x(), y - rect.p2().y() );
    if( mousePos == DOWN ) drag_pos[0]= 0;
    if( mousePos == RIGHT ) drag_pos[1]= 0;
    if( gw->sizeable[0] == 0 ) drag_pos[0]= 0;
    if( gw->sizeable[1] == 0 ) drag_pos[1]= 0;

    xorRectangle( gw, IRectangle( rect.p1(), rect.p2()+drag_pos ) );
    return;
    }

  if( mousePos == LEFT || mousePos == UP  && event.left_pressed() )
    {
    IRectangle rect;
    if( gw->isCard )
       rect= cardsRectangle;
      else
       rect= gw->widget->getWidgetRectangle();
    xorRectangle( gw, IRectangle( rect.p1()+drag_pos, rect.p2() ) );

    tmp_drag_pos+= event.size;

    if( abs(tmp_drag_pos.x()) < drag_pos_gravity &&
        abs(tmp_drag_pos.y()) < drag_pos_gravity )
      {
      drag_pos= IVector(0,0);
      xorRectangle( gw, IRectangle( rect.p1()+drag_pos, rect.p2() ) );
      return;
      }

    int x= rect.p1().x();
    int y= rect.p1().y();
    if( mousePos == LEFT )
      {
      x+= tmp_drag_pos.x();
      x-= x % granularity;
      }
     else
      {
      y+= tmp_drag_pos.y();
      y-= y % granularity;
      }

    IVector size( rect.p2().x()-x+1, rect.p2().y()-y+1 );
    size= max( size, gw->minSize );
    if( gw->maxSize[0] != 0 )
      size[0]= min( size[0], gw->maxSize[0] );
    if( gw->maxSize[1] != 0 )
      size[1]= min( size[1], gw->maxSize[1] );
    x= -size.x() + rect.p2().x() - 1;
    y= -size.y() + rect.p2().y() - 1;

    int min_x= 0;
    int min_y= 0;
    if( gw->card != 0 )
      {
      min_x= cardsRectangle.p1().x();
      min_y= cardsRectangle.p1().y();
      }
    x= max( x, min_x );
    y= max( y, min_y );

    drag_pos= IVector( x-rect.p1().x(), y-rect.p1().y() );
    if( mousePos == UP ) drag_pos[0]= 0;
    if( mousePos == LEFT ) drag_pos[1]= 0;
    if( gw->sizeable[0] == 0 ) drag_pos[0]= 0;
    if( gw->sizeable[1] == 0 ) drag_pos[1]= 0;

    xorRectangle( gw, IRectangle( rect.p1()+drag_pos, rect.p2() ) );
    return;
    }
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::mouseRelease( MLEvent& event )
{
  dragging= false;

  XSetLineAttributes( xinfo->display, xinfo->gc,
                      1, // line_width
                      LineSolid, // line_style
                      CapButt, // cap_style
                      JoinMiter // join_style
                    );

  if( event.middle_pressed() || event.right_pressed() )
    return;

  int _mousePos= mousePos;
  mousePos= NOT_INTO;
  if( drag_pos == IVector(0,0) )
    return;

  if( _mousePos == NOT_INTO && event.button == 1 )
    {
    IRectangle drag= IRectangle( drag_origin, drag_pos );
    drag= IRectangle( min( drag.p1(), drag.p2() ),
                      max( drag.p1(), drag.p2() ) );
    drag= IRectangle( drag.p1()-IVector( cartouche->getClientArea().p1().x(),
                                         cartouche->getClientArea().p1().y()),
                      drag.getSize() );

    SIListIterator<GWidget> li(widgets);
    while( ! li.eol() )
      {
      GWidget* gw= li.next();
      if( gw->widget == 0 ) continue;
      if( gw->isCard ) continue;
      if( ! gw->widget->mapped() ) continue;
      if( ! selectedWidgets.empty() )
        {
        if( selectedWidgets.getFirst()->card != gw->card )
          continue;
        }
      IRectangle rect= IRectangle( gw->position, gw->size );
      if( rect.p1().x() > drag.p2().x() ||
          rect.p2().x() < drag.p1().x() ||
          rect.p1().y() > drag.p2().y() ||
          rect.p2().y() < drag.p1().y() )
        continue;
      if( ! selectedWidgets.exist(gw) )
        {
        if( gw->isCard )
          {
          selectedWidgets.destroy();
          selectedWidgets.append(gw);
          setActiveCard( (VCard*)gw->widget );
          break;
          }
        selectedWidgets.append(gw);
        }
      }

    goto end;
    }

  if( _mousePos == INTO )
    {
    SIListIterator<GWidget> li(selectedWidgets);
    while( ! li.eol() )
      {
      GWidget* gw= li.next();
      VWidget* w= gw->widget;
      if( gw->isCard )
        {
        SIListIterator<GWidget> ll(widgets);
        while( ! ll.eol() )
          {
          GWidget* g= ll.next();
          VWidget* ww= g->widget; if( ww == 0 ) continue;
          if( ! g->isCard )
            {
            if( g->card == 0 )
              continue;
            }
          ww->setWidgetPosition( ww->getWidgetRectangle().p1() + drag_pos );
          g->position= ww->getWidgetRectangle().p1();
          }

        cardsRectangle= IRectangle( cardsRectangle.p1() + drag_pos,
                                    cardsRectangle.getSize() );
        dialog->setCardsRectangle(cardsRectangle);
        }
       else
        {
        w->setWidgetPosition( w->getWidgetRectangle().p1() + drag_pos );
        gw->position= w->getWidgetRectangle().p1();
        }
      }

    modified= true;
    goto end;
    }

  if( _mousePos == RIGHT || _mousePos == LEFT ||
      _mousePos == UP || _mousePos == DOWN )
    {
    GWidget* gw= selectedWidgets.getFirst();
    VWidget* w= gw->widget;
    if( w == 0 )
      goto end;

    if( _mousePos == UP || _mousePos == LEFT )
      {
      if( _mousePos == UP ) drag_pos[0]= 0;
      if( _mousePos == LEFT ) drag_pos[1]= 0;
      if( gw->isCard )
        {
        cardsRectangle= IRectangle( cardsRectangle.p1() + drag_pos,
                                    cardsRectangle.getSize() - drag_pos );
        dialog->setCardsRectangle(cardsRectangle);
        }
       else
        {
        gw->position+= drag_pos;
        gw->size-= drag_pos;
        }
      }
     else
      {
      if( _mousePos == DOWN ) drag_pos[0]= 0;
      if( _mousePos == RIGHT ) drag_pos[1]= 0;
      if( gw->isCard )
        {
        cardsRectangle= IRectangle( cardsRectangle.p1(),
                                    cardsRectangle.getSize() + drag_pos );
        dialog->setCardsRectangle(cardsRectangle);
        }
       else
        {
        gw->size+= drag_pos;
        }
      }

    if( gw->isCard )
      dialog->setCardsRectangle(cardsRectangle);
     else
      gw->create();
    makeLinks();
    modified= true;
    goto end;
    }

end:
  drag_pos= IVector(0,0);
  tmp_drag_pos= IVector(0,0);
  draw();
}

/////////////////////////////////////////////////////////////////////////////
