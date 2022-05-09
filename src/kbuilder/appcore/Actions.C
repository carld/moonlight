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
  Actions.C

  Stephane Rehel
  August 12 1996
*/

#include <stdlib.h>
#include <stdio.h>

#include "koala/VWidget.h"
#include "koala/VDialog.h"
#include "koala/VCard.h"
#include "kw/KWDialog.h"

#include "Canvas.h"
#include "GWidget.h"

/////////////////////////////////////////////////////////////////////////////

void Canvas::deleteWidgets()
{
  if( selectedWidgets.empty() )
    return;

  if( selectedWidgets.getFirst()->isCard )
    return;

  while( ! selectedWidgets.empty() )
    {
    GWidget* gw= selectedWidgets.getFirst();
    if( gw->id == validateID )
      validateID= 0;
    if( gw->id == cancelID )
      cancelID= 0;

    selectedWidgets.removeFirst();
    widgets.remove(gw);
    removeFromGroup(gw);
    delete gw;
    }

  modified= true;
  h_modified= true;

  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::copy_to_clipboard()
{
  if( selectedWidgets.empty() )
    return;

  if( selectedWidgets.getFirst()->isCard )
    return;

  while( ! clipboard.empty() )
    {
    delete clipboard.getFirst();
    clipboard.removeFirst();
    }

  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();

    GWidget* new_gw= new GWidget(this);
    new_gw->set(gw);
    if( gw->name == (MLString("Widget_")+MLString::valueOf(gw->id)) )
      new_gw->name= "";
     else
      new_gw->name= gw->name;
    new_gw->dialog= 0;
    new_gw->card= 0;

    clipboard.append(new_gw);
    }
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::copy_from_clipboard()
{
  if( clipboard.empty() )
    return;

  modified= true;
  h_modified= true;

  selectedWidgets.destroy();

  SIListIterator<GWidget> li(clipboard);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();

    GWidget* new_gw= new GWidget(this);
    new_gw->set(gw);
    new_gw->name= gw->name;
    new_gw->dialog= dialog;
    new_gw->card= activeCard;

    new_gw->position+= IVector(1,1)*granularity;
    new_gw->create(true,false);
    widgets.append(new_gw);
    selectedWidgets.append(new_gw);
    }

  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::deleteCard()
{
  if( activeCard == 0 )
    return;

  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( !gw->isCard && gw->card == activeCard )
      {
      selectedWidgets.destroy();
      selectedWidgets.append(gw);
      draw();
      return; // failed
      }
    }

  GWidget* gw= findWidget(activeCard->getWidgetID());
  if( gw == 0 )
    return; // strange...

  selectedWidgets.destroy();
  widgets.remove(gw);
  delete gw;

  modified= true;
  h_modified= true;

  setActiveCard(0);
  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::end_newWidget( GWidget* gw )
{
  if( gw == 0 )
    return;

  xorSelected();
  selectedWidgets.destroy();

  widgets.append(gw);
  makeLinks();

  selectedWidgets.append(gw);

  VWidget* w= gw->widget;
  if( w != 0 && !gw->isCard )
    {
    IPoint p= w->getWidgetRectangle().p1();
    p= IPoint( p.x() - (p.x() % granularity),
               p.y() - (p.y() % granularity) );
    w->setWidgetPosition(p);
    gw->position= w->getWidgetRectangle().p1();
    }

  modified= true;
  h_modified= true;
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newCard( boolean insertIt /* = false */ )
{
  GWidget* gw= new GWidget(this);
  gw->newCard("Card");

  xorSelected();
  selectedWidgets.destroy();

  if( ! insertIt )
    widgets.append(gw);
   else
    {
    boolean ok= false;
    SIList<GWidget> cards;
    SIListIterator<GWidget> li(widgets);

    for(;;)
      {
      boolean found= false;
      li.reset();
      while( ! li.eol() )
        {
        GWidget* g= li.next();
        if( ! g->isCard )
          continue;
        if( activeCard == (VCard*) g->widget )
          {
          cards.append(gw);
          ok= true;
          }
        cards.append(g);
        widgets.remove(g);
        found= true;
        break;
        }
      if( ! found )
        break;
      }

    if( ! ok )
      cards.append(gw);
    li.init(&cards);
    while( ! li.eol() )
      widgets.append( li.next() );
    cards.destroy();
    }

  selectedWidgets.append(gw);
  makeLinks();

  setActiveCard( (VCard*) gw->widget );

  modified= true;
  h_modified= true;

  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newPicture()
{
  GWidget* w= new GWidget(this);
  w->newPicture("");
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newButton()
{
  GWidget* w= new GWidget(this);
  w->newButton("&Button");
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newChooser()
{
  GWidget* w= new GWidget(this);
  w->newChooser();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newLabel()
{
  GWidget* w= new GWidget(this);
  w->newLabel("&Label");
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newCheckBox()
{
  GWidget* w= new GWidget(this);
  w->newCheckBox("&CheckBox");
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newEdit()
{
  GWidget* w= new GWidget(this);
  w->newEdit();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newValueEdit()
{
  GWidget* w= new GWidget(this);
  w->newValueEdit();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newRadioButton()
{
  GWidget* w= new GWidget(this);
  w->newRadioButton("RadioButton");
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newVertBar()
{
  GWidget* w= new GWidget(this);
  w->newVertScrollBar();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newHorizBar()
{
  GWidget* w= new GWidget(this);
  w->newHorizScrollBar();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newTextBox()
{
  GWidget* w= new GWidget(this);
  w->newTextBox( "&TextBox" );
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newList()
{
  GWidget* w= new GWidget(this);
  w->newScrollList();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newFolderList()
{
  GWidget* w= new GWidget(this);
  w->newFolderList();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newHorizLine()
{
  GWidget* w= new GWidget(this);
  w->newHorizLine();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newVertLine()
{
  GWidget* w= new GWidget(this);
  w->newVertLine();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newGroup()
{
  GWidget* w= new GWidget(this);
  w->newGroup( "&Group" );
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newArea()
{
  GWidget* w= new GWidget(this);
  w->newArea();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newProgress()
{
  GWidget* w= new GWidget(this);
  w->newProgress();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newValueBar()
{
  GWidget* w= new GWidget(this);
  w->newValueBar();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::newRangeBar()
{
  GWidget* w= new GWidget(this);
  w->newRangeBar();
  end_newWidget(w);
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::expandCanvas()
{
  int expand= 3*granularity;
  changeDialogSize( dialogSize + IVector(1,1)*expand );
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::shrinkCanvas()
{
  int expand= 3*granularity;
  IVector newSize= dialogSize - IVector(1,1)*expand;
  IVector minSize(20,20);
  if( ! widgets.empty() )
    minSize= getBounding().p2()+IVector(1,1);

  changeDialogSize( IVector( max(minSize.x(),newSize.x()),
                             max(minSize.y(),newSize.y()) ) );
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::duplicateWidgets()
{
  if( selectedWidgets.empty() )
    return;

  if( selectedWidgets.getFirst()->isCard )
    return;

  SIList<GWidget> newWidgets;

  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    GWidget* new_gw= new GWidget(this);
    new_gw->set(gw);
    new_gw->position+= IVector(1,1)*granularity;
    new_gw->create(true,false);
    newWidgets.append(new_gw);
    widgets.append(new_gw);
    }

  selectedWidgets.destroy();
  while( ! newWidgets.empty() )
    {
    selectedWidgets.append( newWidgets.getFirst() );
    newWidgets.removeFirst();
    }

  modified= true;
  h_modified= true;
  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::availableWidgets()
{
  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    gw->available= ! gw->available;
    if( gw->widget != 0 )
      {
      gw->widget->setAvailable(gw->available);
      GWidget* lk= findWidget(gw->linkWidget);
      if( lk != 0 )
        {
        lk->available= gw->available;
        if( lk->widget != 0 )
          lk->widget->setAvailable(lk->available);
        }
      }
    }

  modified= true;

  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::editWidget()
{
  if( selectedWidgets.empty() )
    return;

  GWidget* gw= selectedWidgets.getLast();

  MLString old_name= gw->name;

  boolean ok= false;
  switch( gw->type )
    {
    case VWidget::PICTURE:
      ok= editPicture(gw);
      break;
    case VWidget::BUTTON:
      ok= editButton(gw);
      break;
    case VWidget::CHOOSER:
      ok= editChooser(gw);
      break;
    case VWidget::LABEL:
      ok= editLabel(gw);
      break;
    case VWidget::CHECKBOX:
      ok= editCheckBox(gw);
      break;
    case VWidget::GROUP:
      ok= editGroup(gw);
      break;
    case VWidget::EDIT:
      ok= editEdit(gw);
      break;
    case VWidget::VALUEEDIT:
      ok= editValueEdit(gw);
      break;
    case VWidget::RADIOBUTTON:
      ok= editRadioButton(gw);
      break;
    case VWidget::LINE:
      ok= editLine(gw);
      break;
    case VWidget::TEXTBOX:
      ok= editTextBox(gw);
      break;
    case VWidget::FOLDERLIST:
      ok= editFolderList(gw);
      break;
    case VWidget::TEXTLIST:
    case VWidget::LIST:
      ok= editList(gw);
      break;
    case VWidget::AREA:
      ok= editArea(gw);
      break;
    case VWidget::PROGRESSBAR:
      ok= editProgress(gw);
      break;
    case VWidget::RANGEBAR:
      ok= editRangeBar(gw);
      break;
    case VWidget::VALUEBAR:
      ok= editValueBar(gw);
      break;
    case VWidget::BAR:
    case VWidget::SCROLLBAR:
      ok= editBar(gw);
      break;
    case VWidget::CARD:
      ok= editCard();
      return;
    default:
      return;
    }

  if( ok )
    {
    modified= true;
    if( old_name != gw->name )
      h_modified= true;

    if( ! gw->isCard )
      gw->create();
    makeLinks();
    }
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::editDialogAction()
{
  boolean ok= editDialog();

  if( ok )
    {
    modified= true;
    h_modified= true;
    makeLinks();
    }
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::centerAllWidgets()
{
  if( widgets.empty() )
    return;

  // center widgets
  IRectangle bound= getBounding();
  IVector new_size= bound.getSize() + IVector( right_margin+left_margin,
                                               top_margin+bottom_margin );
  IVector new_pos= IVector(left_margin,top_margin);
  IVector displ= new_pos - bound.p1();

  if( displ == IVector(0,0) && dialogSize == new_size )
    return; // no change to be done

  changeDialogSize(new_size);

  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    VWidget* w= gw->widget;
    w->setWidgetPosition( w->getWidgetRectangle().p1() + displ );
    gw->position= w->getWidgetRectangle().p1();
    }

  cardsRectangle= IRectangle( cardsRectangle.p1() + displ,
                              cardsRectangle.getSize() );
  dialog->setCardsRectangle(cardsRectangle);

  modified= true;
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::raiseWidgets( boolean yes )
{
  if( selectedWidgets.empty() )
    return;

  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    raiseWidget( yes, li.next() );

  modified= true;
  makeLinks();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::raiseWidget( boolean yes, GWidget* gw )
{
  if( gw == 0 )
    return;
  if( gw->isCard )
    return;

  SIList<GWidget> newlist;

  boolean front= yes;
  if( ! front )
    {
    gw->create();
    newlist.append(gw);
    }

  SIListIterator<GWidget> li(widgets);
  while( ! li.eol() )
    {
    GWidget* g= li.next();
    if( g != gw )
      {
      g->create();
      newlist.append(g);
      }
    }
  if( front )
    {
    gw->create();
    newlist.append(gw);
    }

  widgets.destroy();
  while( ! newlist.empty() )
    {
    widgets.append( newlist.getFirst() );
    newlist.removeFirst();
    }

  modified= true;
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::groupWidgets()
{
  createGroup();
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::ungroupWidgets()
{
  if( selectedWidgets.empty() )
    return;

  SIListIterator<GWidget> li(selectedWidgets);
  while( ! li.eol() )
    removeFromGroup( li.next() );
  modified= true;
  draw();
}

/////////////////////////////////////////////////////////////////////////////

void Canvas::alignWidgets( int horiz, int vert )
{
  if( horiz == 0 && vert == 0 )
    return;
  if( selectedWidgets.getNElements() <= 1 )
    return;

  if( selectedWidgets.getFirst()->isCard )
    return;

  IPoint minP( 10000, 10000);
  IPoint maxP(-10000,-10000);
  SIListIterator <GWidget> li(selectedWidgets);
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    minP= min( minP, gw->position );
    maxP= max( maxP, gw->position+gw->size+IVector(-1,-1) );
    }

  IRectangle bound(minP,maxP);

  li.reset();
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( gw->widget == 0 )
      continue;

    IPoint pos= gw->position;
    switch( horiz )
      {
      case 1: pos[0]= bound.p1().x(); break;
      case 2: pos[0]= bound.p1().x() + (bound.getSize().x()-gw->size.x())/2;
                      break;
      case 3: pos[0]= bound.p2().x() - (gw->size.x()-1); break;
      default: break;
      }
    switch( vert )
      {
      case 1: pos[1]= bound.p1().y(); break;
      case 2: pos[1]= bound.p1().y() + (bound.getSize().y()-gw->size.y())/2;
                      break;
      case 3: pos[1]= bound.p2().y() - (gw->size.y()-1); break;
      default: break;
      }
    gw->widget->setWidgetPosition(pos);
    gw->position= gw->widget->getWidgetRectangle().p1();
    }

  modified= true;
}

/////////////////////////////////////////////////////////////////////////////
