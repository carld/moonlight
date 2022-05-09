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
  CanvasDraw.C

  Stephane Rehel

  October 24 1997

  from Canvas.C

  *** CAREFUL: This is disgusting code ***
  ***          Sorry.                  ***
*/

#include <stdio.h>

#include "graphics/mlgl.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLFonts.h"
#include "graphics/MLFont.h"
#include "graphics/MLEventManager.h"

#include "koala/VWidget.h"
#include "koala/VCard.h"
#include "koala/VCartouche.h"

#include "GWidget.h"

#include "Canvas.h"
#include "CanvasXinfo.h"

#include "kbuilder/KBModule.h"

/////////////////////////////////////////////////////////////////////////////

void Canvas::drawInfos()
{
/*
  char s[512];
  glColor3f(1,1,1);
  if( ! selectedWidgets.empty() )
    {
    GWidget* gw= selectedWidgets.getLast();
    IRectangle rect;
    if( gw->isCard )
      rect= cardsRectangle;
     else
      rect= IRectangle( gw->position, gw->size );
    const char* card= 0;
    if( gw->card != 0 )
      card= gw->card->getMessage().get();
    sprintf( s,
             "Name: %s  id:%3d  linkID:%3d  pos=(%3d,%3d)  size=(%3d,%3d)"
             "  Available:%s  Card:%s",
             gw->name.get(),gw->id,gw->linkWidget,
             rect.p1().x(),rect.p1().y(),
             rect.getSize().x(), rect.getSize().y(),
             gw->available ? "yes" : "no ",
             (card!=0) ? card : "*none*"
             );

    normalFont->print( 20, 20, s );
    }

  sprintf( s, "Filename: %s  DialogName: %s  pos=(%3d,%3d)  size=(%3d,%3d)"
              "  nWidgets: %d  %s\n",
              filename.get(),
              (name.length()==0) ? "*None*" : (name+MLString(".[Ch]")).get(),
              dialogPosition.x(), dialogPosition.y(),
              dialogSize.x(), dialogSize.y(),
              widgets.getNElements(),
              modified ? "<Modified>" : "" );
  normalFont->print( 20, root->getSize().y()-25, s );
*/

  if( ! selectedWidgets.empty() )
    {
    GWidget* gw= selectedWidgets.getLast();
    IRectangle rect;
    if( gw->isCard )
      rect= cardsRectangle;
     else
      rect= IRectangle( gw->position, gw->size );
    const char* card= 0;
    if( gw->card != 0 )
      card= gw->card->getMessage().get();
    MLString s= MLString::printf(
              "Name: %s  id:%3d  linkID:%3d  pos=(%3d,%3d)  size=(%3d,%3d)"
              "  Available:%s  Card:%s",
              gw->name.get(),gw->id,gw->linkWidget,
              rect.p1().x(),rect.p1().y(),
              rect.getSize().x(), rect.getSize().y(),
              gw->available ? "yes" : "no ",
              (card!=0) ? card : "*none*"
              );

    kbModule->printMessage(s);
    }

  MLString s= MLString::printf(
               "Filename: %s  DialogName: %s  pos=(%3d,%3d)  size=(%3d,%3d)"
               "  nWidgets: %d  %s\n",
               filename.get(),
               (name.length()==0) ? "*None*" : (name+MLString(".[Ch]")).get(),
               dialogPosition.x(), dialogPosition.y(),
               dialogSize.x(), dialogSize.y(),
               widgets.getNElements(),
               modified ? "<Modified>" : "" );
  kbModule->printWidgetMessage(s);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::draw_now()
{
  if( systemWindow == 0 )
    return;

  root->currentPixel();
  mlFront();
  root->clear(0.3,0.3,0.3);

  drawInfos();

  XUnmapWindow(xinfo->display,xinfo->windowID);
  XSync(xinfo->display,False);

  cartouche->drawCartoucheAndDialog();

  MLEventManager* eventManager= MLEventManager::getEventManager();
  eventManager->removeRedraw(cartouche);

  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    VWidget* w= gw->widget;
    if( w == 0 ) continue;
    if( w->type() == VWidget::LIST ||
        w->type() == VWidget::TEXTLIST ||
        w->type() == VWidget::FOLDERLIST )
      {
      w->drawWidget();
      continue;
      }

    if( w->type() == VWidget::AREA )
      goto cross;
    if( w->type() == VWidget::PICTURE && gw->picture == 0 )
      goto cross;
    continue;
cross:
    w->beginDraw();
    w->fillWindow( w->getGray2() );
    w->gray0();
    w->drawRectangle( IPoint(0,0), IPoint(-1,-1)+w->getSize() );
    w->drawLine( IPoint(0,0), IPoint(-1,-1)+w->getSize() );
    w->drawLine( IPoint(w->getSize().x()-1,0), IPoint(0,w->getSize().y()-1) );
    w->endDraw();
    }
  XSync(xinfo->display,False);
  XMapWindow(xinfo->display,xinfo->windowID);
  XSync(xinfo->display,False);

  xorSelected();

  if( drawGroups && !groups.empty() )
    {
    XSetLineAttributes( xinfo->display, xinfo->gc,
                        1, // line_width
                        LineOnOffDash, // line_style
                        CapButt, // cap_style
                        JoinMiter // join_style
                      );
    SIListIterator<GGroup> gi(groups);
    while( ! gi.eol() )
      {
      GGroup* g= gi.next();
      IPoint minP(10000,10000);
      IPoint maxP(-10000,-10000);
      boolean dont= false;
      SIListIterator<GWidget> li(g);
      while( ! li.eol() )
        {
        VWidget* w= li.next()->widget;
        if( w == 0 ) continue;
        if( ! w->mapped() )
          {
          dont= true;
          break;
          }
        IRectangle rect= w->getWidgetRectangle();
        minP= min( minP, rect.p1() );
        maxP= max( maxP, rect.p2() );
        }

      if( ! dont )
        drawXORRectangle( IRectangle(minP-IVector(1,1),maxP+IVector(0,0)) );
      }

    XSetLineAttributes( xinfo->display, xinfo->gc,
                        1, // line_width
                        LineSolid, // line_style
                        CapButt, // cap_style
                        JoinMiter // join_style
                      );
    }
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::draw()
{
  kbModule->refreshScreen(KBModule::REFRESH_BUILDER);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::drawXORRectangle( const IRectangle& _rect,
                               boolean correct /* = true */ )
{
  XSetFunction( xinfo->display, xinfo->gc, GXxor );
  XSetForeground( xinfo->display, xinfo->gc,
                  WhitePixel(xinfo->display,DefaultScreen(xinfo->display)) );

  IRectangle rect;
  if( correct )
    rect= IRectangle( _rect.p1()+cartouche->getClientArea().p1(),
                      _rect.getSize() );
   else
    rect= _rect;

  rect= IRectangle( min( rect.p1(), rect.p2() ),
                    max( rect.p1(), rect.p2() ) );
  XDrawRectangle( xinfo->display, xinfo->windowID, xinfo->gc,
                  rect.p1().x(), rect.p1().y(),
                  rect.width(), rect.height() );
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::xorRectangle( GWidget* gw, const IRectangle& _rect )
{
  boolean horiz= true;
  boolean vert= true;
  if( gw != 0 )
    {
    horiz= gw->sizeable.x() != 0;
    vert= gw->sizeable.y() != 0;

    if( gw->isCard )
      {
      horiz= true;
      vert= true;
      }
    }

  XSetFunction( xinfo->display, xinfo->gc, GXxor );

  IRectangle rect= IRectangle( _rect.p1()+cartouche->getClientArea().p1(),
                               _rect.getSize() );

  IPoint center= (rect.p1() + rect.p2())/2;
  IPoint p1= IPoint( center.x(), rect.p1().y() );
  IPoint p2= IPoint( rect.p2().x(), center.y() );
  IPoint p3= IPoint( center.x(), rect.p2().y() );
  IPoint p4= IPoint( rect.p1().x(), center.y() );

  XSetForeground( xinfo->display, xinfo->gc,
                  WhitePixel(xinfo->display,DefaultScreen(xinfo->display)) );
  XDrawRectangle( xinfo->display, xinfo->windowID, xinfo->gc,
                  rect.p1().x(), rect.p1().y(),
                  rect.width(), rect.height() );
  XDrawRectangle( xinfo->display, xinfo->windowID, xinfo->gc,
                  rect.p1().x()-1, rect.p1().y()-1,
                  rect.width()+2, rect.height()+2 );

  const int h= xorRectangleSize;
  p1-= IVector(h,h)/2;
  p2-= IVector(h-1,h)/2;
  p3-= IVector(h,h-1)/2;
  p4-= IVector(h,h)/2;

  if( vert )
    {
    XFillRectangle( xinfo->display, xinfo->windowID, xinfo->gc, p1.x(), p1.y(), h, h );
    XFillRectangle( xinfo->display, xinfo->windowID, xinfo->gc, p3.x(), p3.y(), h, h );
    }

  if( horiz )
    {
    XFillRectangle( xinfo->display, xinfo->windowID, xinfo->gc, p2.x(), p2.y(), h, h );
    XFillRectangle( xinfo->display, xinfo->windowID, xinfo->gc, p4.x(), p4.y(), h, h );
    }

  if( gw->widget != 0 )
    {
    VWidget* link= gw->widget->getLinkWidget();
    if( link != 0 )
      {
      IRectangle rect= link->getWidgetRectangle();
      drawXORRectangle(rect);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::xorSelected( GWidget* gw )
{
  if( gw == 0 || gw->widget == 0 )
    return;
  IRectangle rect;
  if( gw->isCard )
    rect= cardsRectangle;
   else
    rect= gw->widget->getWidgetRectangle();

  xorRectangle(gw,rect);
//  xorRectangle( IRectangle( rect.p1()+drag_pos,
//                            IVector(rect.width(),rect.height()) ) );
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::xorSelected()
{
  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    xorSelected(gw);
    }
}

/////////////////////////////////////////////////////////////////////////////
