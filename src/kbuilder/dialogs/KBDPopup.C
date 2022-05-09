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
  KBDPopup.C

  Stephane Rehel

  October 24 1997
*/

#include <stdlib.h>

#include "kw/KWDialog.h"

#include "kbuilder/KBModule.h"

#include "KBDPopup.h"
#include "KBPopupDlg.h"

#include "koala/VDialog.h"
#include "koala/VCard.h"
#include "kw/KWDialog.h"

#include "kbuilder/appcore/Canvas.h"
#include "kbuilder/appcore/GWidget.h"

/////////////////////////////////////////////////////////////////////////////

KBDPopup::KBDPopup( KBModule* _kb ):
  MLScrollingDialog(_kb)
{
  kb= _kb;
  data= KBPopupDlg::data;
  name= "kBuilder";

  MLScrollingDialog::updateFlags |= KBModule::REFRESH_DIALOGS;
}

/////////////////////////////////////////////////////////////////////////////

KBDPopup::~KBDPopup()
{}

/////////////////////////////////////////////////////////////////////////////

void KBDPopup::update()
{
  Canvas* c= kb->getBuilderCanvas();
  if( c == 0 )
    return;

  int g= c->getGranularity();
  if( g == 1 )
    kwDialog->setChecked( KBPopupDlg::grid, false );
   else
    kwDialog->setChecked( KBPopupDlg::grid, true );

  VCard* card= c->getActiveCard();
  if( kwDialog != 0 )
    kwDialog->setText( KBPopupDlg::card, Canvas::getCardName(card) );
  kwDialog->setAvailable( KBPopupDlg::insertCard, card!=0 );
  kwDialog->setAvailable( KBPopupDlg::deleteCard, card!=0 );
  kwDialog->setAvailable( KBPopupDlg::editCard, card!=0 );

  sortCards();
}

/////////////////////////////////////////////////////////////////////////////

boolean KBDPopup::kwCallback( KWEvent& e )
{
  Canvas* c= kb->getBuilderCanvas();
  if( c == 0 )
    return MLScrollingDialog::kwCallback(e);

  switch( e.type )
    {
    case KWEvent::SELECT:
      {
      switch( e.widget )
        {
        case KBPopupDlg::create:
          {
          const MLString& widgetName= kwDialog->getText(KBPopupDlg::widgetsList);
          if( widgetName == "Label" )      { c->newLabel(); break; }
          if( widgetName == "Button" )     { c->newButton(); break; }
          if( widgetName == "CheckBox" )   { c->newCheckBox(); break; }
          if( widgetName == "Edit" )       { c->newEdit(); break; }
          if( widgetName == "ValueEdit" )  { c->newValueEdit(); break; }
          if( widgetName == "RadioButton" ){ c->newRadioButton(); break; }
          if( widgetName == "Vert Bar" )   { c->newVertBar(); break; }
          if( widgetName == "Horiz Bar" )  { c->newHorizBar(); break; }
          if( widgetName == "TextBox" )    { c->newTextBox(); break; }
          if( widgetName == "List" )       { c->newList(); break; }
          if( widgetName == "Folder List" ){ c->newFolderList(); break; }
          if( widgetName == "Horiz Line" ) { c->newHorizLine(); break; }
          if( widgetName == "Vert Line" )  { c->newVertLine(); break; }
          if( widgetName == "Group" )      { c->newGroup(); break; }
          if( widgetName == "Area" )       { c->newArea(); break; }
          if( widgetName == "Progress Bar"){ c->newProgress(); break; }
          if( widgetName == "Range Bar" )  { c->newRangeBar(); break; }
          if( widgetName == "Value Bar" )  { c->newValueBar(); break; }
          if( widgetName == "Picture" )    { c->newPicture(); break; }
          if( widgetName == "Chooser" )    { c->newChooser(); break; }
          break;
          }

        case KBPopupDlg::dialog:        c->editDialogAction(); break;
        case KBPopupDlg::widget:        c->editWidget(); break;
        case KBPopupDlg::centerAll:     c->centerAllWidgets(); break;
        case KBPopupDlg::prefs:         c->editPrefs(); break;
        case KBPopupDlg::alignWidget:   c->editAlign(); break;
        case KBPopupDlg::available:     c->availableWidgets(); break;
        case KBPopupDlg::duplicate:     c->duplicateWidgets(); break;
        case KBPopupDlg::deleteWidget:  c->deleteWidgets(); break;
        case KBPopupDlg::raiseWidget:   c->raiseWidgets(true); break;
        case KBPopupDlg::lowerWidget:   c->raiseWidgets(false); break;
        case KBPopupDlg::groupWidgets:  c->groupWidgets(); c->draw(); break;
        case KBPopupDlg::ungroupWidgets:c->ungroupWidgets(); c->draw(); break;

//        case KBPopupDlg::quit: exit(0); break;

        case KBPopupDlg::grid:
          {
          int g= kwDialog->getChecked(KBPopupDlg::grid)
                  ? c->getDefaultGranularity()
                  : 1;
          kb->getBuilderCanvas()->setGranularity(g);
          break;
          }

        case KBPopupDlg::card:
          {
          const MLString& text= kwDialog->getText(KBPopupDlg::card);

          if( text == Canvas::getCardName(0) )
            {
            c->setActiveCard(0);
            c->draw();
            break;
            }

          SIListIterator<GWidget> li(c->getWidgets());
          while( ! li.eol() )
            {
            GWidget* gw= li.next();
            if( gw->widget == 0 || !gw->isCard )
              continue;
            VCard* card= (VCard*) gw->widget;
            if( text == Canvas::getCardName(card) )
              {
              c->setActiveCard(card);
              c->draw();
              break;
              }
            }
          break;
          }

        case KBPopupDlg::newCard:
          {
          c->newCard();
          break;
          }

        case KBPopupDlg::insertCard:
          {
          if( c->getActiveCard() != 0 )
            c->newCard(true);
          break;
          }

        case KBPopupDlg::editCard:
          {
          c->editCard();
          break;
          }

        case KBPopupDlg::deleteCard:
          {
          c->deleteCard();
          break;
          }
        };
      break;
      }

    default:
      return MLScrollingDialog::kwCallback(e);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void KBDPopup::sortCards()
{
  Canvas* c= kb->getBuilderCanvas();
  if( c == 0 )
    return;

  KWDialog* popup= kwDialog;

  popup->clearList(KBPopupDlg::card);

  popup->appendListText( KBPopupDlg::card, Canvas::getCardName(0) );

  SIListIterator<GWidget> li(c->getWidgets());
  while( ! li.eol() )
    {
    GWidget* gw= li.next();
    if( gw->widget == 0 || !gw->isCard )
      continue;
    VCard* card= (VCard*) gw->widget;

    popup->appendListText( KBPopupDlg::card, Canvas::getCardName(card) );
    }

  popup->setText( KBPopupDlg::card, Canvas::getCardName(c->getActiveCard()) );
}

/////////////////////////////////////////////////////////////////////////////

void KBDPopup::justCreated()
{
  kwDialog->setInteger( KBPopupDlg::widgetsList, 1 );
  update();
}

/////////////////////////////////////////////////////////////////////////////
