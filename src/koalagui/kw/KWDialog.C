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
  KWDialog.C

  The Koala Widget API (kWAPI)

  Stephane Rehel
  August 4 1996
*/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "tools/Chrono.h"

#include "graphics/KeyCodes.h"
#include "graphics/MLEventManager.h"

#include "koala/VWindow.h"
#include "koala/VWidget.h"


#include "koala/VOptions.h"
#include "koala/VButton.h"
#include "koala/VRadioButton.h"

#include "koala/VDialog.h"
#include "koala/VCartouche.h"
#include "koala/VScrollingCartouche.h"

#include "KWDialog.h"
#include "KWDialogAllocator.h"

/////////////////////////////////////////////////////////////////////////////

KWDialog::KWDialog()
{
  dialogID= 0;
  cartouche= 0;
  scartouche= 0;
  scrolling= false;
  dialog= 0;
  callbackClass= 0;
  previousKH= 0;
  previousEG= 0;
  validateCancel= false;
  validateID= 0;
  cancelID= 0;
  groups= 0;
  nGroups= 0;
  name= "";
}

/////////////////////////////////////////////////////////////////////////////

KWDialog::~KWDialog()
{
  if( groups != 0 )
    {
    for( int i= 0; i < nGroups; ++i )
      {
      delete groups[i].widgets;
      groups[i].widgets= 0;
      }
    delete groups; groups= 0;
    }
  nGroups= 0;

  dialog= 0;

  delete cartouche; cartouche= 0;
  delete scartouche; scartouche= 0;

  KWDialogAllocator::removeDialog(this);
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setScrollingCartouche()
{
  scrolling= true;
}

/////////////////////////////////////////////////////////////////////////////

// private
VWidget* KWDialog::getWidget( KWidget widget )
{
  if( dialog == 0 )
    return 0;

  SIListIterator<VWidget> li(dialog->getWidgets());
  while( ! li.eol() )
    {
    register VWidget* w= li.next();
    if( w->id == int(widget) )
      return w;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::drawDialog()
{
  if( dialog == 0 )
    return;

  if( scrolling )
    {
    if( scartouche != 0 )
      scartouche->drawCartouche();
    }
   else
    {
    if( cartouche != 0 )
      {
      MLEventManager* eventManager= MLEventManager::getEventManager();
      eventManager->removeRedraw(cartouche);
      cartouche->drawCartouche();
      }
    }
  dialog->drawWidget();
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::run()
{
  if( dialog == 0 )
    {
    quitMe= true;
    exitCode= CANCEL;
    return exitCode;
    }

  exitCode= 0;
  quitMe= false;

//  MLKeyboardHandler* kh= eventManager->setKeyboardFocus(dialog);
//  if( kh != dialog )
//    previousKH= kh;

  MLEventManager* eventManager= MLEventManager::getEventManager();
  MLEventGrabber* eg= eventManager->setEventGrabber(dialog);
  if( eg != dialog )
    previousEG= eg;

  eventManager->dispatch();

  for(;;)
    {
    if( quitMe )
      break;
    eventManager->wait();
    eventManager->dispatch();
    }

//  eventManager->setKeyboardFocus(previousKH);
  eventManager->setEventGrabber(previousEG);

  return exitCode;
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::quit( int _exitCode /* = 0 */ )
{
  if( !scrolling )
    {
    exitCode= _exitCode;
    quitMe= true;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::handleValidateCancelKey( Key key )
{
  if( ! validateCancel || dialog == 0 )
    return false;

  if( key == KeyCodes::ESC && cancelID != 0 )
    {
cancel_dialog:
    VWidget* w= getWidget(cancelID);
    if( w == 0 )
      return false;
    if( ! w->isAvailable() || !w->mapped() )
      return false;

    w->setCurrent(true);
    w->drawWidget();
    if( w->type() == VWidget::BUTTON )
      {
      VButton* b= (VButton*) w;
      b->press();
      b->flush();
      Chrono::delay( vOptions->button.pressingDuration ); // sleeping
      b->unpress();
      }

    if( ! scrolling )
      quit(CANCEL);
    return true;
    }

  if( key == KeyCodes::ENTER )
    {
    if( dialog->active != 0 )
      {
      VWidget* active= dialog->active;
      int id= active->getWidgetID();
      if( id == cancelID && cancelID != 0 )
        goto cancel_dialog;

      if( active->type() == VWidget::BUTTON && id != validateID )
        return false; // ENTER should press this button
      }

    VWidget* w= getWidget(validateID);
    if( w == 0 )
      return false;
    if( ! w->isAvailable() || !w->mapped() )
      return false;

    w->setCurrent(true);
    w->drawWidget();
    if( w->type() == VWidget::BUTTON )
      {
      VButton* b= (VButton*) w;
      b->press();
      b->flush();
      Chrono::delay( vOptions->button.pressingDuration ); // sleeping
      b->unpress();
      }

    validateDialog();

    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// private
void KWDialog::validateDialog()
{
  if( scrolling )
    {
    KWEvent event;
    event.type= KWEvent::VALIDATE;
    event.widget= KWidget(0);
    event.key= Key::NULL_KEY;
    event.dialog= this;
    kwDialogCallback(event);
    }
   else
    quit(VALIDATE);
}

/////////////////////////////////////////////////////////////////////////////

// private
void KWDialog::removeWidget( VWidget* w )
{
  if( w == 0 )
    return;

  KWGroup* g= getRadioGroup( KWidget(w->id) );
  if( g != 0 )
    {
    int i= 0;
    while( g->widgets[i] != KWidget(w->id) )
      ++i;
    while( i+1 < g->size )
      {
      g->widgets[i]= g->widgets[i+1];
      ++i;
      }
    g->widgets[ g->size - 1 ]= KWidget(0);
    --(g->size);
    }
}

/////////////////////////////////////////////////////////////////////////////

// private
boolean KWDialog::kwDialogCallback( KWEvent& event )
{
  if( event.type == KWEvent::SELECT && groups != 0 )
    {
    KWGroup* g= getRadioGroup(event.widget);

    if( g != 0 )
      {
      // switch off all other buttons
      for( int j= 0; j < g->size; ++j )
        {
        VWidget* w= getWidget(g->widgets[j]);
        if( w != 0 )
          ((VRadioButton*)w)->check( g->widgets[j] == event.widget );
        }
      }
    }

  if( callbackClass != 0)
    return callbackClass->kwCallback(event);
   else
    return false;
}

/////////////////////////////////////////////////////////////////////////////

// private
KWDialog::KWGroup* KWDialog::getRadioGroup( KWidget widget )
{
  if( groups == 0 )
    return 0;

  for( int i= 0; i < nGroups; ++i )
    {
    KWGroup& g= groups[i];

    for( int j= 0; j < g.size; ++j )
      if( g.widgets[j] == widget )
        return &g;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

// static
MLString** KWDialog::splitString( MLFont* font,
                                 const MLString& string,
                                 int max_width,
                                 int nLines )
{
  nLines= max(1,nLines);
  if( nLines == 1 )
    return new MLString* ( new MLString(string) );

  SIList<MLString>* list= VWindow::splitString( font, string, max_width );
  MLString** lines= new MLString* [ nLines ];
  for( int i= 0; i < nLines; ++i )
    lines[i]= 0;

  int n= 1;
  SIListIterator<MLString> li(list);
  while( ! li.eol() )
    {
    MLString* s= li.next();

    if( n >= nLines )
      {
      ++n;
      if( lines[nLines-1] == 0 )
        {
        lines[nLines-1]= s;
        continue;
        }
      *(lines[nLines-1])+= *s;
      delete s;
      continue;
      }
    lines[n-1]= s;
    ++n;
    }

  while( n <= nLines )
    {
    lines[n-1]= new MLString("");
    ++n;
    }

  list->destroy();
  delete list;

  return lines;
}

/////////////////////////////////////////////////////////////////////////////
