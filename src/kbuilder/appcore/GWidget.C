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
  GWidget.C

  Stephane Rehel
  July 31 1996
*/

#include "graphics/MLPictures.h"
#include "graphics/MLPicture.h"

#include "image/MLImageFormat.h"
#include "image/MLImageReader.h"

#include "Canvas.h"
#include "GWidget.h"
#include "koala/VDialog.h"
#include "koala/VButton.h"
#include "koala/VLabel.h"
#include "koala/VCheckBox.h"
#include "koala/VEdit.h"
#include "koala/VValueEdit.h"
#include "koala/VRadioButton.h"
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

/////////////////////////////////////////////////////////////////////////////

GWidget::GWidget( Canvas* _canvas )
{
  canvas= _canvas;
  dialog= canvas->dialog;
  widget= 0;

  type= VWidget::BUTTON;
  position= IPoint(0,0);
  size= IVector(0,0);
  minSize= maxSize= size;
  sizeable= IVector(0,0);
  message= MLString("");
  margin= 0;
  justify= LEFT;
  vertical= true;
  scrolling= true;
  linkWidget= 0;
  available= true;
  fixedFont= false;
  bigFont= false;
  textList= true;
  enterPXM= false;
  minValue= 0.;
  maxValue= 1.;
  nDigits= 3;
  minValueBar= 0.;
  maxValueBar= 1.;
  editWidth= 30;
  folderPlusMinus= true;
  picture= 0;
  pictureFilename= "";
  isCard= false;
  card= canvas->activeCard;

  // choose id
  id= 1;
  for(;;)
    {
    boolean ok= true;
    SIListIterator<GWidget> li(canvas->widgets);
    while( ! li.eol() )
      if( li.next()->id == id )
        {
        ok= false;
        break;
        }
    if( ok )
      break;
    ++id;
    }

  name= MLString("Widget_")+MLString::valueOf(id);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::set( GWidget* gw )
{
  dialog= gw->dialog;
  type= gw->type;
  isCard= type == VWidget::CARD;
  card= gw->card;
  widget= 0;
  position= gw->position;
  size= gw->size;
  minSize= gw->minSize;
  maxSize= gw->maxSize;
  sizeable= gw->sizeable;

  message= gw->message;

  for( int i= 1; i <= gw->list.getNElements(); ++i )
    list.append( new MLString(*gw->list.get(i)) );

  margin= gw->margin;
  justify= gw->justify;
  vertical= gw->vertical;
  scrolling= gw->scrolling;
  linkWidget= 0;
  available= gw->available;
  fixedFont= gw->fixedFont;
  bigFont= gw->bigFont;
  textList= gw->textList;
  enterPXM= gw->enterPXM;
  minValue= gw->minValue;
  maxValue= gw->maxValue;
  nDigits= gw->nDigits;
  minValueBar= gw->minValueBar;
  maxValueBar= gw->maxValueBar;
  editWidth= gw->editWidth;
  folderPlusMinus= gw->folderPlusMinus;
  pictureFilename= gw->pictureFilename;
  picture= gw->picture;
  MLPictures::ref(picture);
}

/////////////////////////////////////////////////////////////////////////////

GWidget::~GWidget()
{
  destroyWidget();

  while( ! list.empty() )
    {
    MLString* s= list.getFirst();
    list.removeFirst();
    delete s;
    }

  MLPictures::unref(picture);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::destroyWidget()
{
  if( dialog != 0 )
    dialog->removeWidget(widget);
  delete widget; widget= 0;
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newCard( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::CARD;
  isCard= true;
  size= IVector(0,0);
  minSize= IVector(100,50);
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= _message;
  card= 0;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newPicture( const MLString& filename )
{
  assert( widget == 0 );

  type= VWidget::PICTURE;
  size= IVector(0,0);
  minSize= IVector( 10, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(0,0);
  picture= 0;
  pictureFilename= filename;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newButton( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::BUTTON;
  size= IVector(0,0);
  minSize= IVector( 10, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= _message;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newChooser()
{
  assert( widget == 0 );

  type= VWidget::CHOOSER;
  size= IVector(80,20);
  minSize= IVector( 10, 20 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newLabel( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::LABEL;
  size= IVector(0,0);
  minSize= IVector( 10, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= _message;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newCheckBox( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::CHECKBOX;
  size= IVector(0,0);
  minSize= IVector( 20, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= _message;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newEdit()
{
  assert( widget == 0 );

  type= VWidget::EDIT;
  size= IVector(50,0);
  minSize= IVector( 20, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= MLString("");

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newValueEdit()
{
  assert( widget == 0 );

  type= VWidget::VALUEEDIT;
  size= IVector(50,0);
  minSize= IVector( 20, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= MLString("");

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newRadioButton( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::RADIOBUTTON;
  message= _message;
  size= IVector(0,0);
  minSize= IVector( 30, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newVertBar()
{
  assert( widget == 0 );

  type= VWidget::BAR;
  size= IVector(0,100);
  minSize= IVector( 20, 50 );
  maxSize= IVector(0,0);
  sizeable= IVector(0,1);
  message= MLString("");
  vertical= true;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newHorizBar()
{
  assert( widget == 0 );

  type= VWidget::BAR;
  size= IVector(100,0);
  minSize= IVector( 50, 20);
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= MLString("");
  vertical= false;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newVertScrollBar()
{
  assert( widget == 0 );

  type= VWidget::SCROLLBAR;
  size= IVector(0,100);
  minSize= IVector( 20, 50 );
  maxSize= IVector(0,0);
  sizeable= IVector(0,1);
  message= MLString("");
  vertical= true;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newHorizScrollBar()
{
  assert( widget == 0 );

  type= VWidget::SCROLLBAR;
  size= IVector(100,0);
  minSize= IVector( 50, 20);
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= MLString("");
  vertical= false;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newTextBox( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::TEXTBOX;
  size= IVector(80,0);
  minSize= IVector(20,0);
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= _message;
  justify= CENTER;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newScrollList()
{
  assert( widget == 0 );

  type= VWidget::TEXTLIST;
  size= IVector(144,144);
  minSize= IVector(50,50);
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= MLString("");
  scrolling= true;
  textList= true;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newFolderList()
{
  assert( widget == 0 );

  type= VWidget::FOLDERLIST;
  size= IVector(144,144);
  minSize= IVector(50,50);
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= MLString("");
  scrolling= true;
  textList= true;
  folderPlusMinus= true;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newHorizLine()
{
  assert( widget == 0 );

  type= VWidget::LINE;
  size= IVector(100,12);
  minSize= IVector(10,10);
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= MLString("");
  vertical= false;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newVertLine()
{
  assert( widget == 0 );

  type= VWidget::LINE;
  size= IVector(12,100);
  minSize= IVector(10,10);
  maxSize= IVector(0,0);
  sizeable= IVector(0,1);
  message= MLString("");
  vertical= true;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newGroup( const MLString& _message )
{
  assert( widget == 0 );

  type= VWidget::GROUP;
  size= IVector(100,100);
  minSize= IVector(10,10);
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= _message;

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newArea()
{
  assert( widget == 0 );

  type= VWidget::AREA;
  size= IVector(100,100);
  minSize= IVector(5,5);
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newProgress()
{
  assert( widget == 0 );

  type= VWidget::PROGRESSBAR;
  size= IVector(200,21);
  minSize= IVector(30,10);
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newRangeBar()
{
  assert( widget == 0 );

  type= VWidget::RANGEBAR;
  size= IVector(50,0);
  minSize= IVector( 20, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,1);
  message= MLString("");

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::newValueBar()
{
  assert( widget == 0 );

  type= VWidget::VALUEBAR;
  size= IVector(80,0);
  minSize= IVector( 30, 10 );
  maxSize= IVector(0,0);
  sizeable= IVector(1,0);
  message= MLString("");

  create(true);
}

/////////////////////////////////////////////////////////////////////////////

void GWidget::create( boolean first /* = false */,
                      boolean alignIt /* = true */ )
{
  VWidget* w= 0;

  if( name.length() == 0 )
    name= MLString("Widget_")+MLString::valueOf(id);

  boolean st= false;
  switch( type )
    {
    case VWidget::CARD:
      {
      if( ! first )
        return;

      VCard* c= new VCard(dialog);
      c->create( 0, 0, 0, 0, message );
      w= c;
      card= 0;
      isCard= true;
      break;
      }

    case VWidget::PICTURE:
      {
      updatePicture();
      VPicture* p= new VPicture(dialog);
      p->create( position.x(), position.y(), picture );
      IVector size= p->getSize();
      int x2= position.x() + size.x() - 1;
      int y2= position.y() + size.y() - 1;
      if( x2 >= dialog->getSize().x() )
        position[0]= dialog->getSize().x()-1 - size.x();
      if( y2 >= dialog->getSize().y() )
        position[1]= dialog->getSize().y()-1 - size.y();
      if( p->getPosition() != position )
        p->setPosition(position);
      w= p;
      st= true;
      break;
      }

    case VWidget::BUTTON:
      {
      VButton* button= new VButton(dialog);
      if( enterPXM )
        button->setEnterPixmap(true);
      button->create( position.x(), position.y(), message );

      if( size == IVector(0,0) )
        size= button->getWidgetRectangle().getSize();
       else
        button->setSize(size);
      w= button;
      break;
      }

    case VWidget::CHOOSER:
      {
      VChooser* c= new VChooser(dialog);
      c->create( position.x(), position.y(), size.x(), size.y() );
      SIListIterator<MLString> li(list);
      while( ! li.eol() )
        c->appendMessage( *li.next() );
      if( ! list.empty() )
        c->changeMessage( *list.getFirst() );
      w= c;
      break;
      }

    case VWidget::LABEL:
      {
      VLabel* label= new VLabel(dialog);
      if( fixedFont )
        label->setFixedFont();
      if( bigFont )
        label->setBigFont();
      label->setMargin(margin);
      label->create( position.x(), position.y(), size.x(), size.y(),
                     message.get(),
                     (justify==RIGHT) ? VLabel::RIGHT :
                     (justify==CENTER) ? VLabel::CENTER : VLabel::LEFT );
      size= label->getWidgetRectangle().getSize();
      w= label;
      st= true;
      break;
      }

    case VWidget::CHECKBOX:
      {
      VCheckBox* cb= new VCheckBox(dialog);
      cb->setMargin(margin);
      cb->create( position.x(), position.y(), size.x(), size.y(),
                  message.get(),
                  (justify==RIGHT) ? VLabel::RIGHT :
                  (justify==CENTER) ? VLabel::CENTER : VLabel::LEFT );
      size= cb->getWidgetRectangle().getSize();
      w= cb;
      break;
      }

    case VWidget::EDIT:
      {
      VEdit* e= new VEdit(dialog);
      if( fixedFont )
        e->setFixedFont();
      e->create( position.x(), position.y(), size.x(), "Edit" );

      size= e->getWidgetRectangle().getSize();
      w= e;
      break;
      }

    case VWidget::VALUEEDIT:
      {
      VValueEdit* e= new VValueEdit(dialog);
      if( fixedFont )
        e->setFixedFont();
      e->create( position.x(), position.y(), size.x(),
                 minValue, maxValue, nDigits, minValue );
      size= e->getWidgetRectangle().getSize();
      w= e;
      break;
      }

    case VWidget::RADIOBUTTON:
      {
      VRadioButton* r= new VRadioButton(dialog);
      r->setMargin(margin);
      r->create( position.x(), position.y(), size.x(), size.y(),
                 message,
                 (justify==RIGHT) ? VLabel::RIGHT :
                 (justify==CENTER) ? VLabel::CENTER : VLabel::LEFT );
      size= r->getWidgetRectangle().getSize();
      w= r;
      break;
      }

    case VWidget::BAR:
      {
      VBar* b;
      int height;
      if( vertical )
        {
        b= new VVertBar(dialog);
        height= size.y();
        }
       else
        {
        b= new VHorizBar(dialog);
        height= size.x();
        }

      b->create( position.x(), position.y(), height, 8, 1 );

      size= b->getWidgetRectangle().getSize();
      w= b;
      st= true;
      break;
      }

    case VWidget::SCROLLBAR:
      {
      VScrollBar* b;
      int height;
      if( vertical )
        {
        b= new VVertScrollBar(dialog);
        height= size.y();
        }
       else
        {
        b= new VHorizScrollBar(dialog);
        height= size.x();
        }

      b->create( position.x(), position.y(), height, 8, 1 );

      size= b->getWidgetRectangle().getSize();
      w= b;
      st= true;
      break;
      }

    case VWidget::TEXTBOX:
      {
      VTextBox* tb= new VTextBox(dialog);
      if( fixedFont )
        tb->setFixedFont();
      tb->create( position.x(), position.y(), size.x(),
                  message.get(),
                  (justify==RIGHT) ? VTextBox::RIGHT :
                  (justify==LEFT) ? VTextBox::LEFT : VTextBox::CENTER );

      size= tb->getWidgetRectangle().getSize();
      w= tb;
      st= true;
      break;
      }

    case VWidget::TEXTLIST:
    case VWidget::LIST:
      {
      VList* l= 0;
      if( !textList )
        {
        l= new VList(dialog);
        if( justify == RIGHT )
          l->setRightBar();
        l->create( position.x(), position.y(), size.x(), size.y(),
                   scrolling, 1 );
        type= l->type();
        }
       else
        {
        VTextList* tl= new VTextList(dialog);
        l= tl;
        if( fixedFont )
          tl->setFixedFont();
        if( justify == RIGHT )
          tl->setRightBar();
        tl->create( position.x(), position.y(), size.x(), size.y(),
                    scrolling );
        tl->appendMessage( "First item" );
        tl->appendMessage( "Reports" );
        tl->appendMessage( "Groups" );
        tl->appendMessage( "Append position" );
tl->appendMessage( "Actions.C");
tl->appendMessage( "Actions.o");
tl->appendMessage( "Align.C");
tl->appendMessage( "Align.dlg");
tl->appendMessage( "Align.o");
tl->appendMessage( "AlignDlg.C");
tl->appendMessage( "AlignDlg.h");
tl->appendMessage( "AlignDlg.o");
tl->appendMessage( "Area.C");
tl->appendMessage( "Area.dlg");
tl->appendMessage( "Area.o");
tl->appendMessage( "AreaDlg.C");
tl->appendMessage( "AreaDlg.h");
tl->appendMessage( "AreaDlg.o");
tl->appendMessage( "Bar.C");
tl->appendMessage( "Bar.dlg");
tl->appendMessage( "Bar.o");
tl->appendMessage( "BarDlg.C");
tl->appendMessage( "BarDlg.h");
tl->appendMessage( "BarDlg.o");
tl->appendMessage( "Button.C");
tl->appendMessage( "Button.dlg");
tl->appendMessage( "Button.o");
tl->appendMessage( "ButtonDlg.C");
tl->appendMessage( "ButtonDlg.h");
tl->appendMessage( "ButtonDlg.o");
tl->appendMessage( "Canvas.C");
tl->appendMessage( "Canvas.h");
tl->appendMessage( "Canvas.o");
tl->appendMessage( "CheckBox.C");
tl->appendMessage( "CheckBox.dlg");
tl->appendMessage( "CheckBox.o");
tl->appendMessage( "CheckBoxDlg.C");
tl->appendMessage( "CheckBoxDlg.h");
tl->appendMessage( "CheckBoxDlg.o");
tl->appendMessage( "Dialog.C");
tl->appendMessage( "Dialog.dlg");
tl->appendMessage( "Dialog.o");
tl->appendMessage( "DialogDlg.C");
tl->appendMessage( "DialogDlg.h");
tl->appendMessage( "DialogDlg.o");
tl->appendMessage( "Edit.C");
tl->appendMessage( "Edit.dlg");
tl->appendMessage( "Edit.o");
tl->appendMessage( "EditButton.o");
tl->appendMessage( "EditDlg.C");
tl->appendMessage( "EditDlg.h");
tl->appendMessage( "Progress.dlg");
tl->appendMessage( "Progress.o");
tl->appendMessage( "ProgressDlg.o");
tl->appendMessage( "RadioButton.C");
tl->appendMessage( "RadioButton.dlg");
tl->appendMessage( "RadioButton.o");
tl->appendMessage( "RadioButtonDlg.o");
tl->appendMessage( "ReadCanvas.C");
tl->appendMessage( "ReadCanvas.o");
tl->appendMessage( "SaveAs.dlg");
tl->appendMessage( "ValueEditDlg.C");
tl->appendMessage( "ValueEditDlg.h");
tl->appendMessage( "ValueEditDlg.o");
tl->appendMessage( "WriteCanvas.C");
tl->appendMessage( "WriteCanvas.o");
tl->appendMessage( "builder.C");
tl->appendMessage( "go");

        type= l->type();
        }
      size= l->getWidgetRectangle().getSize();
      w= l;
      break;
      }

    case VWidget::FOLDERLIST:
      {
      VFolderList* fl= new VFolderList(dialog);
      if( fixedFont )
        fl->setFixedFont();
      if( justify == RIGHT )
        fl->setRightBar();
      if( justify == RIGHT )
        fl->setPlusMinusFolder(folderPlusMinus);
      fl->create( position.x(), position.y(), size.x(), size.y(),
                  scrolling );

      fl->appendItem( 0, "groups" );
      fl->appendItem( 0, "locations" );
      fl->appendItem( 0, "localconfyg" );
      int fid1= fl->appendFolder( 0, "Folder 1" );
      fl->appendItem( fid1, "item 1 /F1" );
      fl->appendItem( fid1, "item 2 /F1" );
      fl->appendItem( fid1, "item 3 /F1" );
      fl->appendItem( 0, "protocols" );
      fl->appendItem( 0, "rpcs" );
      int fid2= fl->appendFolder( fid1, "Folder 2" );
      fl->appendItem( fid2, "item 1 /F2" );
      fl->appendItem( fid2, "item 2 /F2" );
      fl->appendItem( fid2, "item 3 /F2" );
      fl->appendItem( fid2, "item 4 /F2" );
      fl->appendItem( 0, "machines" );
      fl->appendItem( 0, "mounts" );
      fl->appendItem( 0, "networks" );
      fl->appendItem( 0, "printers" );
      fl->appendItem( 0, "services" );
      fl->appendItem( 0, "users" );
      fl->appendItem( 0, "ThisIsAHighlyVeryLongItemYouKnow" );
      fl->appendItem( fid2, "item 5 /F2" );
      fl->openFolder(fid1);

      type= fl->type();

      size= fl->getWidgetRectangle().getSize();
      w= fl;
      break;
      }

    case VWidget::LINE:
      {
      VLine* l;
      IPoint p2;
      if( vertical )
        {
        l= new VVertLine(dialog);
        p2= position + IVector(0,size.y()-1);
        }
       else
        {
        l= new VHorizLine(dialog);
        p2= position + IVector(size.x()-1,0);
        }

      l->create( position, p2 );

      size= l->getWidgetRectangle().getSize();
      w= l;
      st= true;
      break;
      }

    case VWidget::GROUP:
      {
      VGroup* g= new VGroup(dialog);
      g->create( position.x(), position.y(), size.x(), size.y(),
                 message );
      size= g->getWidgetRectangle().getSize();
      w= g;
      st= true;
      break;
      }

    case VWidget::AREA:
      {
      VArea* a= new VArea(dialog);
      a->create( position.x(), position.y(), size.x(), size.y() );
      size= a->getWidgetRectangle().getSize();
      w= a;
      st= true;
      break;
      }

    case VWidget::PROGRESSBAR:
      {
      VProgressBar* p= new VProgressBar(dialog);
      p->create( position.x(), position.y(), size.x(), size.y() );
      p->changeValue(0.23);
      size= p->getWidgetRectangle().getSize();
      w= p;
      st= true;
      break;
      }

    case VWidget::RANGEBAR:
      {
      VRangeBar* rb= new VRangeBar(dialog);
      rb->create( position.x(), position.y(), size.x(), size.y(),
                  minValue, minValue, maxValue );
      size= rb->getWidgetRectangle().getSize();
      w= rb;
      st= true;
      break;
      }

    case VWidget::VALUEBAR:
      {
      VValueBar* vb= new VValueBar(dialog);
      if( fixedFont )
        vb->setFixedFont();

      int _editWidth= min( size.x()-10, editWidth );

      vb->create( position.x(), position.y(), size.x(),
                  _editWidth,
                  minValueBar, maxValueBar,
                  minValue, maxValue, nDigits, minValue );
      size= vb->getWidgetRectangle().getSize();
      w= vb;
      break;
      }
    default:
      break;
    }

  if( w == 0 )
    return;

  if( widget != 0 )
    {
    dialog->removeWidget(widget);
    delete widget;
    widget= 0;
    }

  widget= w;
  if( isCard )
    dialog->addCard( (VCard*)widget );
   else
    dialog->addWidget(widget,st,card);

  widget->setWidgetID(id);

  size= widget->getWidgetRectangle().getSize();
  if( first && alignIt && ! isCard )
    {
    IRectangle rect;
    if( card == 0 )
      rect= IRectangle( IPoint(0,0), dialog->getSize() );
     else
      rect= canvas->cardsRectangle;

    IPoint p= rect.p1() + (rect.getSize() - size) / 2;
    p= max( p, rect.p1() );
    p[0]-= p[0] % canvas->granularity;
    p[1]-= p[1] % canvas->granularity;
    widget->setWidgetPosition(p);
    }

  position= widget->getWidgetRectangle().p1();

  if( sizeable[0] == 0 )
    minSize[0]= maxSize[0]= size[0];
  if( sizeable[1] == 0 )
    minSize[1]= maxSize[1]= size[1];

  if( ! available )
    widget->setAvailable(false);
}

/////////////////////////////////////////////////////////////////////////////

static MLPicture* readImage( const MLString& filename );

void GWidget::updatePicture()
{
  if( type != VWidget::PICTURE )
    return;

  MLString old_filename= MLPictures::getName(picture);
  MLString new_filename= pictureFilename;

  if( old_filename == new_filename )
    return;

  MLPicture* p= MLPictures::find(new_filename);
  if( p == 0 )
    {
    p= readImage(new_filename);
    if( p == 0 )
      {
      MLPictures::unref(picture);
      picture= 0;
      return;
      }
    }

  MLPictures::ref(p);
  if( p->getWidth() > dialog->getSize().x() ||
      p->getHeight() > dialog->getSize().y() )
    {
    MLPictures::unref(p);
    picture= 0;
    return;
    }

  MLPictures::add( p, new_filename );
  MLPictures::unref(picture);
  picture= p;
}

/////////////////////////////////////////////////////////////////////////////

static MLPicture* readImage( const MLString& filename )
{
  MLImageReader* r= MLImageFormat::getInstance()->guessReader(filename.get());
  if( r == 0 )
    {
//    KWError( "Failed", "Unable to guess image format from extension" );
    return 0;
    }

  if( ! r->open(filename) )
    {
//    KWErrorf( "Failed",
//              "Unable to open file '%s'", gw->pictureFilename.get() );
    return 0;
    }

  MLPicture* p= new MLPicture;
  if( ! r->read(p) )
    {
//    KWErrorf( "Failed",
//              "Unable to read file '%s'", gw->pictureFilename.get() );
    delete p;
    return 0;
    }

  delete r;
  r= 0;

  p->convertToRGB();

  p->upsidedown();

  return p;
}

/////////////////////////////////////////////////////////////////////////////
