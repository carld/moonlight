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
  KWDialogOp.C

  The Koala Widget API (kWAPI)

  Stephane Rehel
  August 4 1996
*/

#include <math.h>

#include "koala/VWindow.h"
#include "koala/VWidget.h"

#include "koala/VButton.h"
#include "koala/VRadioButton.h"
#include "koala/VLabel.h"
#include "koala/VCheckBox.h"
#include "koala/VEdit.h"
#include "koala/VValueEdit.h"
#include "koala/VVertBar.h"
#include "koala/VHorizBar.h"
#include "koala/VVertScrollBar.h"
#include "koala/VHorizScrollBar.h"
#include "koala/VList.h"
#include "koala/VTextList.h"
#include "koala/VFolderList.h"
#include "koala/VTextBox.h"
#include "koala/VVertLine.h"
#include "koala/VHorizLine.h"
#include "koala/VGroup.h"
#include "koala/VArea.h"
#include "koala/VProgressBar.h"
#include "koala/VRangeBar.h"
#include "koala/VValueBar.h"
#include "koala/VPicture.h"
#include "koala/VChooser.h"
#include "koala/VCard.h"

#include "koala/VDialog.h"

#include "KWDialog.h"
#include "KWDialogAllocator.h"

/////////////////////////////////////////////////////////////////////////////

void KWDialog::map( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  w->map(true);
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::unmap( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  w->map(false);
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::mapped( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  return w->mapped();
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setText( KWidget widget, const MLString& text )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::BUTTON:
      ((VButton*)w)->changeMessage(text);
      break;
    case VWidget::LABEL:
      ((VLabel*)w)->changeMessage(text);
      break;
    case VWidget::CHECKBOX:
      ((VCheckBox*)w)->changeMessage(text);
      break;
    case VWidget::EDIT:
      ((VEdit*)w)->changeEdit(text);
      break;
    case VWidget::RADIOBUTTON:
      ((VRadioButton*)w)->changeMessage(text);
      break;
    case VWidget::GROUP:
      ((VGroup*)w)->changeMessage(text);
      break;
    case VWidget::TEXTBOX:
      ((VTextBox*)w)->changeMessage(text);
      break;
    case VWidget::CHOOSER:
      ((VChooser*)w)->changeMessage(text);
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setChecked( KWidget widget, boolean yes /* = true */ )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::CHECKBOX:
      ((VCheckBox*)w)->check(yes);
      break;

    case VWidget::RADIOBUTTON:
      {
      KWGroup* g= getRadioGroup(widget);
      if( g != 0 )
        {
        if( !yes )
          return;
        // switch off all other buttons
        for( int j= 0; j < g->size; ++j )
          {
          VWidget* w= getWidget(g->widgets[j]);
          if( w != 0 )
            ((VRadioButton*)w)->check( g->widgets[j] == widget );
          }
        return;
        }

      // lonely radio button (same as a check box)
      ((VRadioButton*)w)->check(yes);

      break;
      }

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setAvailable( KWidget widget, boolean yes /* = true */ )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  w->setAvailable(yes);
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::focus( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 || dialog == 0 )
    return;
  if( ! w->mapped() )
    return;

  w->setCurrent(true);
  w->postDrawWidget();
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setValue( KWidget widget, double value )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::VALUEEDIT:
      ((VValueEdit*)w)->changeValue(value);
      break;

    case VWidget::VALUEBAR:
      ((VValueBar*)w)->changeValue(value);
      break;

    case VWidget::PROGRESSBAR:
      ((VProgressBar*)w)->changeValue(value);
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setInteger( KWidget widget, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      ((VTextList*)w)->setInteger(i);
      break;

    case VWidget::FOLDERLIST:
      ((VFolderList*)w)->setInteger(i);
      break;

    case VWidget::LIST:
      ((VList*)w)->setInteger(i);
      break;

    case VWidget::CHOOSER:
      ((VChooser*)w)->setInteger(i);
      break;

    case VWidget::VALUEEDIT:
      ((VValueEdit*)w)->changeValue( double(i) );
      break;

    case VWidget::VALUEBAR:
      ((VValueBar*)w)->changeValue( double(i) );
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::setListText( KWidget widget, int i, const MLString& text )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      return ((VTextList*)w)->setMessage(i,text);

    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->setItem(0,i,text);

    case VWidget::CHOOSER:
      return ((VChooser*)w)->setMessage(i,text);

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::appendListText( KWidget widget, const MLString& text )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      return ((VTextList*)w)->appendMessage(text);

    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->appendItem(0,text);

    case VWidget::CHOOSER:
      return ((VChooser*)w)->appendMessage(text);

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::insertListText( KWidget widget, int i, const MLString& text )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      return ((VTextList*)w)->insertMessage(i,text);

    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->insertItem(0,i,text);

    case VWidget::CHOOSER:
      return ((VChooser*)w)->insertMessage(i,text);

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::deleteListText( KWidget widget, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      return ((VTextList*)w)->deleteMessage(i);
      break;

    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->deleteItem(0,i);
      break;

    case VWidget::CHOOSER:
      return ((VChooser*)w)->deleteMessage(i);
      break;

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::clearList( KWidget widget )
{
  setInteger( widget, 1 );

  while( getListLength(widget) > 0 )
    if( ! deleteListText( widget, 1 ) )
      break; // error?
}

/////////////////////////////////////////////////////////////////////////////

void* KWDialog::getListUserData( KWidget widget, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::LIST:
    case VWidget::TEXTLIST:
      return ((VList*)w)->getUserData(i);

    case VWidget::FOLDERLIST:
      return ((VList*)(VFolderList*)w)->getUserData(i);

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getListUserDataIndex( KWidget widget, void* user_data )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::LIST:
    case VWidget::TEXTLIST:
      return ((VList*)w)->getUserDataIndex(user_data);

    case VWidget::FOLDERLIST:
      return ((VList*)(VFolderList*)w)->getUserDataIndex(user_data);

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::appendFolder( KWidget widget,
                            int folder_id,
                            const MLString& text,
                            void* user_data)
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->appendFolder( folder_id, text, user_data );

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::appendFolderItem( KWidget widget,
                                 int folder_id,
                                 const MLString& text,
                                 void* user_data)
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->appendItem( folder_id,
                                            text,
                                            user_data );
    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::insertFolder( KWidget widget,
                            int folder_id,
                            int i,
                            const MLString& text,
                            void* user_data)
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->insertFolder( folder_id, i, text, user_data );

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::insertFolderItem( KWidget widget,
                                  int folder_id,
                                  int i,
                                  const MLString& text,
                                  void* user_data)
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->insertItem( folder_id,
                                            i,
                                            text,
                                            user_data );

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

// i==0 for folder text
boolean KWDialog::setFolderData( KWidget widget,
                               int folder_id,
                               int i,
                               const MLString& text,
                               void* user_data /* = 0 */ )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->setItem( folder_id, i, text, user_data );

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void* KWDialog::getFolderUserData( KWidget widget, int folder_id, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getUserData( folder_id, i );

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getFolderUserDataIndex( KWidget widget,
                                      int folder_id,
                                      void* user_data )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getUserDataIndex( folder_id, user_data );

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getFolderID( KWidget widget, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getFolderID(i);

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getFolderID( KWidget widget, int folder_id, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getFolderID(folder_id,i);

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getFolderLength( KWidget widget, int folder_id )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getFolderLength(folder_id);

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::deleteFolder( KWidget widget, int folder_id )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->deleteFolder(folder_id);

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::openFolder( KWidget widget,
                           int folder_id,
                           boolean yes)
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      ((VFolderList*)w)->openFolder( folder_id, yes );
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::closeFolder( KWidget widget, int folder_id )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::FOLDERLIST:
      ((VFolderList*)w)->closeFolder( folder_id );
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setValueEditRanges( KWidget widget,
                                   double minValue,
                                   double maxValue,
                                   int nDigits )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::VALUEEDIT:
      ((VValueEdit*)w)->setRanges(minValue,maxValue,nDigits);
      break;

    case VWidget::VALUEBAR:
      ((VValueEdit*)(VValueBar*)w)->setRanges(minValue,maxValue,nDigits);
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::setRangeBarRanges( KWidget widget,
                                  double minValue, double maxValue )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::RANGEBAR:
      ((VRangeBar*)w)->setRanges(minValue,maxValue);
      break;

    case VWidget::VALUEBAR:
      {
      int nWidgets;
      VWidget** list= ((VValueBar*)w)->getWidgetsList(&nWidgets);
      if( nWidgets >= 2 )
        {
        VWidget* rb= list[1];
        if( rb->type() == VWidget::RANGEBAR )
          ((VRangeBar*)rb)->setRanges(minValue,maxValue);
        }
      break;
      }

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::repaintArea( KWidget widget )
{
  VWidget* w= getWidget(widget);

  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::AREA:
      ((VArea*)w)->postDrawWidget();
      break;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

IRectangle KWDialog::getRectangle( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return IRectangle( IPoint(0,0), IVector(0,0) );

  return w->getWidgetRectangle();
}

/////////////////////////////////////////////////////////////////////////////

const MLString& KWDialog::getText( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return MLString::empty();

  switch( w->type() )
    {
    case VWidget::BUTTON:
      return ((VButton*)w)->getMessage();

    case VWidget::LABEL:
      return ((VLabel*)w)->getMessage();

    case VWidget::CHECKBOX:
      return ((VCheckBox*)w)->getMessage();

    case VWidget::EDIT:
      return ((VEdit*)w)->getEdit();

    case VWidget::VALUEEDIT:
      return ((VValueEdit*)w)->getEdit();

    case VWidget::RADIOBUTTON:
      return ((VRadioButton*)w)->getMessage();

    case VWidget::GROUP:
      return ((VGroup*)w)->getMessage();

    case VWidget::TEXTBOX:
      return ((VTextBox*)w)->getMessage();

    case VWidget::CHOOSER:
      return ((VChooser*)w)->getMessage();

    case VWidget::TEXTLIST: // return the selected item
      {
      int i= getInteger(widget);
      if( i == 0 )
        return MLString::empty();
      return getListText(widget,i);
      }

    case VWidget::FOLDERLIST: // return the selected item
      {
      int i= getInteger(widget);
      if( i == 0 )
        return MLString::empty();
      return getListText(widget,i);
      }

    default:
      break;
    }

  return MLString::empty();
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::getChecked( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  switch( w->type() )
    {
    case VWidget::CHECKBOX:
      return ((VCheckBox*)w)->checked();

    case VWidget::RADIOBUTTON:
      return ((VRadioButton*)w)->checked();

    default:
      break;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::getAvailable( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return false;

  return w->isAvailable();
}

/////////////////////////////////////////////////////////////////////////////

// may return KWidget(0)
KWidget KWDialog::getRadioChecked( KWidget radioFromGroup )
{
  VWidget* w= getWidget(radioFromGroup);
  if( w == 0 )
    return KWidget(0);

  KWGroup* g= getRadioGroup(radioFromGroup);
  if( g == 0 )
    return KWidget(0);

  for( int j= 0; j < g->size; ++j )
    {
    VWidget* w= getWidget(g->widgets[j]);
    if( w == 0 )
      continue;
    if( ((VRadioButton*)w)->checked() )
      return g->widgets[j];
    }

  return KWidget(0);
}

/////////////////////////////////////////////////////////////////////////////

double KWDialog::getValue( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0.;

  switch( w->type() )
    {
    case VWidget::VALUEEDIT:
      return ((VValueEdit*)w)->getValue();

    case VWidget::VALUEBAR:
      return ((VValueBar*)w)->getValue();

    default:
      break;
    }

  return 0.;
}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getInteger( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::LIST:
      return ((VList*)w)->getInteger();

    case VWidget::TEXTLIST:
      return ((VList*)(VTextList*)w)->getInteger();

    case VWidget::FOLDERLIST:
      return ((VList*)(VFolderList*)w)->getInteger();

    case VWidget::CHOOSER:
      return ((VChooser*)w)->getInteger();

    case VWidget::VALUEEDIT:
      {
      double d= ((VValueEdit*)w)->getValue();
      if( fabs(d) > double(1<<30) )
        return 0;
      return int(d);
      }

    case VWidget::VALUEBAR:
      {
      double d= ((VValueBar*)w)->getValue();
      if( fabs(d) > double(1<<30) )
        return 0;
      return int(d);
      }

    default:
      break;
    }

  return 0;

}

/////////////////////////////////////////////////////////////////////////////

int KWDialog::getListLength( KWidget widget )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::LIST:
      return ((VList*)w)->getLength();

    case VWidget::TEXTLIST:
      return ((VTextList*)w)->getLength();

    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getLength();

    case VWidget::CHOOSER:
      return ((VChooser*)w)->getLength();

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

const MLString& KWDialog::getListText( KWidget widget, int i )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return MLString::empty();

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      return ((VTextList*)w)->getMessage(i);

    case VWidget::FOLDERLIST:
      return ((VFolderList*)w)->getItem(0,i);

    case VWidget::CHOOSER:
      return ((VChooser*)w)->getMessage(i);

    default:
      break;
    }

  return MLString::empty();
}


/////////////////////////////////////////////////////////////////////////////

int KWDialog::getListTextIndex( KWidget widget, const MLString& text )
{
  VWidget* w= getWidget(widget);
  if( w == 0 )
    return 0;

  switch( w->type() )
    {
    case VWidget::TEXTLIST:
      return ((VTextList*)w)->getMessagePosition(text);

    case VWidget::CHOOSER:
      return ((VChooser*)w)->getMessagePosition(text);

    default:
      break;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

void KWDialog::activateCard( KWidget widget )
{
  if( dialog == 0 )
    return;

  VWidget* w= getWidget(widget);
  if( w == 0 )
    return;

  switch( w->type() )
    {
    case VWidget::CARD:
      dialog->setActiveCard( (VCard*) w );
      return;

    default:
      break;
    }
}

/////////////////////////////////////////////////////////////////////////////

KWidget KWDialog::getActiveCard()
{
  if( dialog == 0 )
    return KWidget(0);

  VCard* card= dialog->getActiveCard();

  if( card == 0 )
    return KWidget(0);

  return card->id;
}

/////////////////////////////////////////////////////////////////////////////

// static
KWDialog* KWDialog::find( KWDialogID id )
{
  return KWDialogAllocator::findDialog(id);
}

/////////////////////////////////////////////////////////////////////////////
