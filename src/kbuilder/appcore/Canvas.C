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
  Canvas.C

  Stephane Rehel
  July 30 1996

  *** CAREFUL: This is disgusting code ***
  ***          Sorry.                  ***
*/

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>

#include "graphics/GraphicsSystem.h"
#include "graphics/X11/XGraphicsSystem.h"
#include "graphics/WindowDriver.h"
#include "graphics/X11/XWindowDriver.h"
#include "graphics/MLEventManager.h"
#include "graphics/MLWindow.h"
#include "graphics/MLEvent.h"
#include "graphics/SystemWindow.h"
#include "graphics/SoftWindow.h"

#include "koala/VWindow.h"
#include "koala/VDialog.h"
#include "koala/VCard.h"
#include "koala/VCartouche.h"

#include "kw/KWDialog.h"

#include "GWidget.h"

#include "Canvas.h"
#include "CanvasXinfo.h"

#include "kbuilder/KBModule.h"
#include "kbuilder/KBCanvas.h"

//static
const int Canvas::xorRectangleSize= 8;
const int Canvas::NOT_INTO= 1;
const int Canvas::INTO    = 2;
const int Canvas::UP      = 3;
const int Canvas::DOWN    = 4;
const int Canvas::RIGHT   = 5;
const int Canvas::LEFT    = 6;
MLString Canvas::canvasExtension="";

/////////////////////////////////////////////////////////////////////////////

Canvas::Canvas( SystemWindow* _root, KBCanvas* _kbCanvas )
{
  kbCanvas= _kbCanvas;
  kbModule= (KBModule*) kbCanvas->getModule();

  root= _root;

  Canvas::canvasExtension=".dlg";

  mousePos= NOT_INTO;
  drag_pos= IVector(0,0);
  tmp_drag_pos= IVector(0,0);
  drag_origin= IPoint(0,0);
  drag_origin2= IPoint(0,0);
  drag_pos_gravity= 3;

  cartouche= 0;
  dialog= 0;
  systemWindow= 0;
  xinfo= new Xinfo;
  xinfo->windowID= 0;
  xinfo->display= 0;
  xinfo->gc= 0;

  dialogPosition= IPoint(5,5); //100+100,50);
  dialogSize= IVector(400,300);

  frame= true;
  killButton= true;
  title= MLString("DialogTitle");
  name= MLString("");
  filename= MLString("noname") + canvasExtension;
  C_filename= "";
  h_filename= "";
  has_C_filename= false;
  has_h_filename= false;
  write_C_file= true;

  validateCancel= true;
  validateID= 0;
  cancelID= 0;

  defaultGranularity= 4;
  granularity= defaultGranularity;
  drawGroups= true;
  top_margin= bottom_margin= right_margin= left_margin= 2*granularity;

  popup= 0;
//  createPopup();

  cardsRectangle= IRectangle( IPoint(5,5), IVector(100,80) );
  activeCard= 0;

  dragging= false;
  modified= false;
  h_modified= false;
  create();
}

/////////////////////////////////////////////////////////////////////////////

Canvas::~Canvas()
{
  destroy();
  delete popup; popup= 0;

  while( ! clipboard.empty() )
    {
    delete clipboard.getFirst();
    clipboard.removeFirst();
    }

  delete xinfo;
  xinfo= 0;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::destroy()
{
  selectedWidgets.destroy();
  widgets.destroy();
  SIList_destroyElements( GGroup, groups );
  activeCard= 0;

  delete cartouche; cartouche= 0;
  delete systemWindow; systemWindow= 0;
  xinfo->windowID= 0;
  xinfo->display= 0;
  xinfo->gc= 0;

  modified= false;
  h_modified= false;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::recreate()
{
  // save card widgets reference
  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( gw->isCard )
      gw->cardIndex= -1;
     else
      {
      VCard* card= gw->card;
      if( card == 0 )
        gw->cardIndex= 0; // root card
       else
        {
        // get card list index
        gw->cardIndex= 0;
        int index= 1;
        SIListIterator<GWidget> li2(widgets);
        while( ! li2.eol() )
          {
          GWidget* g= li2.next();
          if( g->widget == card )
            {
            gw->cardIndex= index;
            break;
            }
          ++index;
          }
        }
      }
    }

  selectedWidgets.destroy();

  li.reset();
  while( ! li.eol() )
    li.next()->destroyWidget();

  delete cartouche; cartouche= 0;
  delete systemWindow; systemWindow= 0;
  xinfo->windowID= 0;
  xinfo->display= 0;
  xinfo->gc= 0;
  activeCard= 0;

  cartouche= new VCartouche(root);
  cartouche->create( dialogPosition.x(), dialogPosition.y(),
                     dialogSize.x(), dialogSize.y(),
                     frame, killButton, title );
  cartouche->map();
  dialog= cartouche->getDialog();
  dialog->setCardsRectangle(cardsRectangle);

  systemWindow= new SystemWindow;
  systemWindow->create( root,
                        cartouche->getPosition().x(),
                        cartouche->getPosition().y(),
                        cartouche->getSize().x(),
                        cartouche->getSize().y() );

  systemWindow->map();
  systemWindow->setEventHandler(this);

  xinfo->windowID= ((XWindowDriver*)systemWindow->getDriver())->getWindowID();

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();
  xinfo->display= ((XGraphicsSystem*)graphicsSystem)->getDisplay();

  xinfo->gc= XCreateGC( xinfo->display, xinfo->windowID, 0, 0 );

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->setKeyboardFocus(this);

  // first create cards
  li.reset();
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    gw->dialog= dialog;
    if( gw->isCard )
      gw->create();
    }

  // then widgets
  li.reset();
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    gw->dialog= dialog;
    if( ! gw->isCard )
      {
      if( gw->cardIndex >= 1 )
        {
        GWidget* gcard= widgets.get(gw->cardIndex);
        assert( gcard->isCard );
        gw->card= (VCard*) gcard->widget;
        }

      gw->create();
      }
    }

  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::create()
{
  destroy();

  recreate();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newDialog()
{
  dialogPosition= IPoint(5,5);//100+100,50);
  dialogSize= IVector(400,300);

  frame= true;
  killButton= true;
  title= MLString("DialogTitle");
  C_filename= "";
  h_filename= "";
  name= "";
  filename= MLString("noname") + canvasExtension;

  validateCancel= true;
  validateID= 0;
  cancelID= 0;

  create();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::isRadioBoxGroup( GGroup* g )
{
  if( g == 0 )
    return false;
  SIListIterator<GWidget> li(g);
  while( ! li.eol() )
    if( li.next()->type != VWidget::RADIOBUTTON )
      return false;

  return true;
}

/////////////////////////////////////////////////////////////////////////////

Canvas::GGroup* Canvas::findGroup( GWidget* gw )
{
  if( gw == 0 )
    return 0;
  SIListIterator<GGroup> gi(groups);
  while( ! gi.eol() )
    {
    GGroup* g= gi.next();
    if( g->exist(gw) )
      return g;
    }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::removeFromGroup( GWidget* gw )
{
  GGroup* g= findGroup(gw);
  if( g == 0 )
    return;

  g->remove(gw);
  if( g->getNElements() <= 1 )
    {
    groups.remove(g);
    delete g;
    }

  modified= true;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::selectGroup( GGroup* g, boolean toggle )
{
  if( g == 0 )
    return;

  SIListIterator<GWidget> li(g);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();

    if( selectedWidgets.exist(gw) )
      {
      if( toggle )
        {
        selectedWidgets.remove(gw);
        xorSelected(gw);
        }
      }
     else
      {
      selectedWidgets.append(gw);
      xorSelected(gw);
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

Canvas::GGroup* Canvas::createGroup()
{
  if( selectedWidgets.getNElements() <= 1 )
    return 0;

  GGroup* g= new GGroup;

  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    removeFromGroup(gw);
    g->append(gw);
    }

  groups.append(g);

  modified= true;

  return g;
}

/////////////////////////////////////////////////////////////////////////////

GWidget* Canvas::findWidget( int id )
{
  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* w= li.next();
    if( w->id == id )
      return w;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::moveWidgets( const IVector& displ )
{
  if( displ == IVector(0,0) || selectedWidgets.empty() )
    return;

  IPoint minP( 10000, 10000),
         maxP(-10000,-10000);
  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    IRectangle rect= li.next()->widget->getWidgetRectangle();
    minP= min( minP, rect.p1() );
    maxP= max( maxP, rect.p2() );
    }

  boolean isCard= selectedWidgets.getFirst()->isCard;
  if( isCard )
    {
    minP= cardsRectangle.p1();
    maxP= cardsRectangle.p2();
    }
  minP+= displ;
  maxP+= displ;

  int min_x= 0;
  int min_y= 0;
  int max_x= dialog->getSize().x();
  int max_y= dialog->getSize().y();
  if( selectedWidgets.getFirst()->card != 0 )
    {
    max_x= cardsRectangle.p2().x();
    max_y= cardsRectangle.p2().y();
    min_x= cardsRectangle.p1().x();
    min_y= cardsRectangle.p1().y();
    }

  if( minP.x() < min_x || minP.y() < min_y ||
      maxP.x() >= max_x || maxP.y() >= max_y )
    return;

  if( isCard )
    li.init(&widgets);
   else
    li.reset();
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    VWidget* w= gw->widget;
    if( w == 0 ) continue;
    if( isCard )
      if( gw->card == 0 && !gw->isCard )
        continue;
    IPoint p= w->getWidgetRectangle().p1() + displ;
    w->setWidgetPosition(p);
    gw->position= w->getWidgetRectangle().p1();
    }

  if( isCard )
    {
    cardsRectangle= IRectangle( cardsRectangle.p1() + displ,
                                cardsRectangle.getSize() );
    dialog->setCardsRectangle(cardsRectangle);
    }

  modified= true;
  draw();
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::eventHandler( MLWindow* window, MLEvent& event )
{
  switch( event.type )
    {
/*
    case MLEvent::REDRAW:
      ++draw_loop;
      draw();
      --draw_loop;
      break;
*/

    case MLEvent::MOUSE_PRESS:
      mousePress(event);
      break;

    case MLEvent::MOUSE_MOTION:
      mouseMotion(event);
      break;

    case MLEvent::MOUSE_RELEASE:
      mouseRelease(event);
      break;

    default:
      return window->handleEvent(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::hasCards()
{
  return dialog->getActiveCard() != 0;
}

/////////////////////////////////////////////////////////////////////////////

IRectangle Canvas::getBounding()
{
  IPoint minP( 10000, 10000);
  IPoint maxP(-10000,-10000);

  SIListIterator <GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    minP= min( minP, gw->position );
    maxP= max( maxP, gw->position+gw->size+IVector(-1,-1) );
    }

  if( hasCards() )
    {
    minP= min( minP, cardsRectangle.p1() );
    maxP= max( maxP, cardsRectangle.p2() );
    }

  return IRectangle( minP, maxP );
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::changeDialogSize( const IVector& newSize )
{
  if( dialogSize == newSize )
    return;

  modified= true;
  h_modified= true;

  dialogSize= newSize;

  cartouche->setDialogSize(newSize);
  systemWindow->setSize( cartouche->getSize() );

  if( cardsRectangle.p2().x() >= newSize.x() ||
      cardsRectangle.p2().y() >= newSize.y() )
    cardsRectangle= IRectangle( cardsRectangle.p1(),
                                IPoint(newSize.x()-4,newSize.y()-4) );
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::makeLinks()
{
  sortCards();

  dialog->makeIndex();

  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( gw->linkWidget == 0 )
      continue;
    GWidget* link= findWidget(gw->linkWidget);
    if( link != 0 )
      {
      if( link->card != gw->card )
        {
        gw->widget->setLinkWidget(0);
        gw->linkWidget= 0;
        continue;
        }
      }

    if( gw->widget == 0 )
      continue;

    if( link != 0 && link->widget != 0 )
      gw->widget->setLinkWidget(link->widget);
     else
      {
      gw->widget->setLinkWidget(0);
      gw->linkWidget= 0;
      }
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean Canvas::kwCallback( KWEvent& e )
{
  const MLString& name= e.dialog->getName();

  if( name == "ButtonDlg" ) return editButtonCallback(e);
  if( name == "DialogDlg" ) return editDialogCallback(e);
  if( name == "EditDlg" )   return editEditCallback(e);
  if( name == "ListDlg" )   return editListCallback(e);
  if( name == "FolderListDlg" ) return editFolderListCallback(e);
//  if( name == "PopupDlg" )  return popupCallback(e);
  if( name == "AlignDlg" )  return editAlignCallback(e);
  if( name == "ValueEditDlg" ) return editValueEditCallback(e);
  if( name == "ChooserDlg" )return editChooserCallback(e);
  if( name == "ValueBarDlg" )return editValueBarCallback(e);

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString Canvas::getCardName( VCard* card )
{
  if( card == 0 )
    return MLString("Root Card");
   else
    return card->getMessage() + MLString(" (")
          + MLString::valueOf(card->getWidgetID()) + MLString(")");
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::setGranularity( int g )
{
  granularity= g;

  kbModule->refreshScreen(KBModule::REFRESH_DIALOGS);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::setActiveCard( VCard* card )
{
  if( activeCard == card )
    return;

  activeCard= card;

  if( dialog != 0 && card != 0 )
    dialog->setActiveCard(card);

  kbModule->refreshScreen(KBModule::REFRESH_DIALOGS);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::sortCards()
{
  int x= cardsRectangle.p1().x()+1;

  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( gw->widget == 0 || !gw->isCard )
      continue;
    VCard* c= (VCard*) gw->widget;

    // ooooh disguuusting:-;
    VCard* dummy= new VCard(dialog);
    dummy->create( 0, 0, 0, 0, gw->message );
    IVector size= dummy->getSize();
    delete dummy; dummy= 0;

    c->setWidgetPosition( IPoint(x,cardsRectangle.p1().y()) );
    c->setSize(size);
    c->changeMessage(c->getMessage());
    gw->position= c->getPosition();
    gw->size= c->getSize();
    x+= size.x();
    }

  setActiveCard(activeCard);

  kbModule->refreshScreen(KBModule::REFRESH_DIALOGS);
}

/////////////////////////////////////////////////////////////////////////////
