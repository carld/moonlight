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
  KWcreateDialog.C

  The Koala Widget API (kWAPI)

  Stephane Rehel

  August 4 1996

  boolean KWDialog::create(...);
*/

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "tools/zlib/MLLibZ.h"

#include "graphics/GraphicsSystem.h"
#include "graphics/MLEventManager.h"
#include "graphics/SystemWindow.h"
#include "graphics/MLPicture.h"

#include "koala/VWindow.h"
#include "koala/VWidget.h"

#include "koala/VOptions.h"
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
#include "koala/VCartouche.h"
#include "koala/VScrollingCartouche.h"

#include "KWDialog.h"
#include "KWDialogAllocator.h"

/////////////////////////////////////////////////////////////////////////////

static unsigned int decode( register const unsigned char*& dm )
{
  unsigned int i= 0;
  i |= ((unsigned int)*(dm++)) << 24;
  i |= ((unsigned int)*(dm++)) << 16;
  i |= ((unsigned int)*(dm++)) <<  8;
  i |= ((unsigned int)*(dm++)) <<  0;

  return i;
}

static MLString decodeString( register const unsigned char*& dm )
{
  int length= decode(dm);
  MLString s(length);

  for( register int i= 0; i < length; ++i )
    s[i]= (char)*(dm++);
  return s;
}

#define CENTER_ME -31547

boolean KWDialog::create( SystemWindow* parent, int x, int y,
                        const unsigned char* dialogMap,
                        KWCallbackClass* _callbackClass,
                        boolean map_it /* = true */ )
{
  if( dialogMap == 0 )
    return false;
  if( dialog != 0 )
    return false;

  GraphicsSystem* graphicsSystem= GraphicsSystem::getGraphicsSystem();

  if( parent == 0 )
    parent= graphicsSystem->getRootWindow();

  callbackClass= _callbackClass;

  const unsigned char* dm= dialogMap + 4;
/*
  unsigned int size= 0;
  size= (size<<8) | (unsigned int)(*(dm++));
  size= (size<<8) | (unsigned int)(*(dm++));
  size= (size<<8) | (unsigned int)(*(dm++));
  size= (size<<8) | (unsigned int)(*(dm++));
*/

  int version= decode(dm);
  if( version != 1 )
    return false;

  unsigned int flags= decode(dm);
  boolean canval=     (flags & (((unsigned int)1) << 0)) != 0;
  boolean frame=      (flags & (((unsigned int)1) << 1)) != 0;
  boolean killButton= (flags & (((unsigned int)1) << 2)) != 0;
  boolean hasPictures=(flags & (((unsigned int)1) << 3)) != 0;
  boolean hasCards=   (flags & (((unsigned int)1) << 4)) != 0;

  IRectangle cardsRectangle;
  if( hasCards )
    {
    int x= decode(dm);
    int y= decode(dm);
    int sx= decode(dm);
    int sy= decode(dm);
    cardsRectangle= IRectangle( IPoint(x,y), IVector(sx,sy) );
    }

  MLPicture** pictures= 0;
  int nPictures= 0;

  if( hasPictures )
    {
    unsigned int flags;
    flags= decode(dm);
    boolean compressed= (flags & (((unsigned int)1) << 0)) != 0;

    nPictures= decode(dm);
    pictures= new MLPicture* [ nPictures ];
    for( int j= 1; j <= nPictures; ++j )
      {
      MLPicture* p= new MLPicture;
      pictures[j-1]= p;
      int width= decode(dm);
      int height= decode(dm);
      int hasAlpha= decode(dm);
      int size= decode(dm);

      if( compressed )
        {
        p->create( width, height, 0, hasAlpha != 0 );
        int image_size= width * height * (hasAlpha ? 4 : 3);

        MLLibZ::inflate( (void*) dm, size,
                         p->getData(), image_size );

        if( graphicsSystem->hasGammaCorrection() )
          p->gammaCorrect();
        }
       else
        {
        if( ! graphicsSystem->hasGammaCorrection() )
          p->create( width, height, (void*) dm, hasAlpha != 0 );
         else
          {
          p->create( width, height, (void*) 0, hasAlpha != 0 );
          int image_size= width * height * (hasAlpha ? 4 : 3);
          memcpy( (void*) p->getData(), (const void*) dm, image_size );
          p->gammaCorrect();
          }
        }

      dm+= size;
      }
    }

  if( canval )
    {
    validateCancel= canval;
    validateID= decode(dm);
    cancelID= decode(dm);
    }
   else
    {
    validateCancel= false;
    cancelID= validateID= 0;
    }

  MLString title("");
  if( frame )
    title= decodeString(dm);

  name= decodeString(dm);

  int a= decode(dm),
      b= decode(dm);
  IVector dialogSize(a,b);

  if( scrolling )
    {
    scartouche= new VScrollingCartouche(parent,this);
    if( x == CENTER_ME || y == CENTER_ME )
      {
      IVector size= scartouche->probeCartoucheSize(dialogSize,frame);
      if( x == CENTER_ME )
        x= (parent->getSize().x()-size.x()) / 2;
      if( y == CENTER_ME )
        y= (parent->getSize().y()-size.y()) / 2;
      }
    scartouche->create( x, y, dialogSize.x(), dialogSize.y(),
                        frame, killButton, title );

    dialog= scartouche->getDialog();
    }
   else
    {
    cartouche= new VCartouche(parent,this);
    if( x == CENTER_ME || y == CENTER_ME )
      {
      IVector size= cartouche->probeCartoucheSize(dialogSize,frame);
      if( x == CENTER_ME )
        x= (parent->getSize().x()-size.x()) / 2;
      if( y == CENTER_ME )
        y= (parent->getSize().y()-size.y()) / 2;
      }
    cartouche->create( x, y, dialogSize.x(), dialogSize.y(),
                       frame, killButton, title );

    dialog= cartouche->getDialog();
    }

  dialog->kwDialog= this;

  if( hasCards )
    dialog->setCardsRectangle(cardsRectangle);

  int nWidgets= decode(dm);

  int* links= new int [nWidgets+1];
  VWidget** theWidgets= new VWidget* [nWidgets+1];
  int* ids= new int [nWidgets+1];
  int i;

  for( i= 0; i < nWidgets; ++i )
    {
    int type= decode(dm);
    int id= decode(dm),
        x= decode(dm),
        y= decode(dm),
        width= decode(dm),
        height= decode(dm);
    unsigned int flags= decode(dm);

    boolean hasLink=   (flags & (((unsigned int)1) << 0)) != 0;
    boolean isStatic=  (flags & (((unsigned int)1) << 1)) != 0;
    boolean vertical=  (flags & (((unsigned int)1) << 2)) != 0;
    boolean scrolling= (flags & (((unsigned int)1) << 3)) != 0;
    boolean available= (flags & (((unsigned int)1) << 4)) != 0;
    boolean fixedFont= (flags & (((unsigned int)1) << 5)) != 0;
    boolean enterPXM=  (flags & (((unsigned int)1) << 6)) != 0;
    boolean textList=  (flags & (((unsigned int)1) << 7)) != 0;
    boolean hasjust=   (flags & (((unsigned int)1) << 8)) != 0;
    boolean just=      (flags & (((unsigned int)1) << 9)) != 0;
    boolean hasMargin= (flags & (((unsigned int)1) << 10)) != 0;
    boolean bigFont=   (flags & (((unsigned int)1) << 11)) != 0;
    boolean hasCard=   (flags & (((unsigned int)1) << 12)) != 0;
    boolean folderPlusMinus= (flags & (((unsigned int)1) << 13)) != 0;

    VLabel::Justify justify;
    if( hasjust )
      justify= just ? VLabel::RIGHT : VLabel::CENTER;
     else
      justify= VLabel::LEFT;

    int margin= 0;
    if( hasMargin )
      margin= decode(dm);

    if( hasLink )
      links[i]= decode(dm);
     else
      links[i]= 0;
    ids[i]= id;

    int cardID= 0;
    if( hasCard )
      cardID= decode(dm);

    VWidget* w= 0;
    switch(type)
      {
      case VWidget::CARD:
        {
        MLString msg= decodeString(dm);
        VCard* c= new VCard(dialog);
        c->create( x, y, width, height, msg );
        w= c;
        break;
        }
      case VWidget::BUTTON:
        {
        MLString msg= decodeString(dm);
        VButton* b= new VButton(dialog);
        if( enterPXM )
          b->setEnterPixmap(true);
        b->create( x, y, width, height, msg );
        w= b;
        break;
        }
      case VWidget::LABEL:
        {
        MLString msg= decodeString(dm);
        VLabel* l= new VLabel(dialog);
        if( fixedFont )
          l->setFixedFont();
        if( bigFont )
          l->setBigFont();
        l->setMargin(margin);
        l->create( x, y, width, height, msg, justify );
        w= l;
        break;
        }
      case VWidget::EDIT:
        {
        VEdit* e= new VEdit(dialog);
        if( fixedFont )
          e->setFixedFont();
        e->create( x, y, width );
        w= e;
        break;
        }
      case VWidget::VALUEEDIT:
        {
        double minValue= 0., maxValue= 0.;
        MLString s;
        s= decodeString(dm); sscanf( s.get(), "%lf", &minValue );
        s= decodeString(dm); sscanf( s.get(), "%lf", &maxValue );
        int nDigits= decode(dm);

        VValueEdit* e= new VValueEdit(dialog);
        if( fixedFont )
          e->setFixedFont();
        e->create( x, y, width, minValue, maxValue, nDigits, minValue );
        w= e;
        break;
        }
      case VWidget::RADIOBUTTON:
        {
        MLString msg= decodeString(dm);
        VRadioButton* r= new VRadioButton(dialog);
        r->setMargin(margin);
        r->create( x, y, width, height, msg, justify );
        w= r;
        break;
        }
      case VWidget::CHECKBOX:
        {
        MLString msg= decodeString(dm);
        VCheckBox* cb= new VCheckBox(dialog);
        cb->setMargin(margin);
        cb->create( x, y, width, height, msg, justify );
        w= cb;
        break;
        }
      case VWidget::BAR:
        {
        VBar* b;
        int h;
        if( vertical )
          {
          b= new VVertBar(dialog);
          h= height;
          }
         else
          {
          b= new VHorizBar(dialog);
          h= width;
          }
        b->create( x, y, h, 8, 1 );
        w= b;
        break;
        }

      case VWidget::SCROLLBAR:
        {
        VScrollBar* b;
        int h;
        if( vertical )
          {
          b= new VVertScrollBar(dialog);
          h= height;
          }
         else
          {
          b= new VHorizScrollBar(dialog);
          h= width;
          }
        b->create( x, y, h, 8, 1 );
        w= b;
        break;
        }

      case VWidget::TEXTBOX:
        {
        MLString msg= decodeString(dm);
        VTextBox* tb= new VTextBox(dialog);

        if( fixedFont )
          tb->setFixedFont();
        tb->create( x, y, width,
                    msg.get(),
                    (justify==VLabel::LEFT)  ? VTextBox::LEFT :
                    (justify==VLabel::RIGHT) ? VTextBox::RIGHT :
                                               VTextBox::CENTER );
        w= tb;
        break;
        }

      case VWidget::LIST:
      case VWidget::TEXTLIST:
        {
        VList* l= 0;
        if( !textList )
          {
          l= new VList(dialog);
          if( justify == VLabel::RIGHT )
            l->setRightBar();
          l->create( x, y, width, height, scrolling, 1 );
          }
         else
          {
          VTextList* tl= new VTextList(dialog);
          l= tl;
          if( fixedFont )
            tl->setFixedFont();
          if( justify == VLabel::RIGHT )
            tl->setRightBar();
          tl->create( x, y, width, height, scrolling );
          }

        w= l;
        break;
        }

      case VWidget::FOLDERLIST:
        {
        VFolderList* fl= new VFolderList(dialog);
        if( fixedFont )
          fl->setFixedFont();
        if( justify == VLabel::RIGHT )
          fl->setRightBar();
        if( folderPlusMinus )
          fl->setPlusMinusFolder(folderPlusMinus);
        fl->create( x, y, width, height, scrolling );

        w= fl;
        break;
        }

      case VWidget::LINE:
        {
        VLine* l;
        IPoint position(x,y);
        IPoint p2;
        if( vertical )
          {
          l= new VVertLine(dialog);
          p2= position + IVector(0,height-1);
          }
         else
          {
          l= new VHorizLine(dialog);
          p2= position + IVector(width-1,0);
          }

        l->create( position, p2 );
        w= l;
        break;
        }

      case VWidget::GROUP:
        {
        MLString msg= decodeString(dm);
        VGroup* g= new VGroup(dialog);
        g->create( x, y, width, height, msg );
        w= g;
        break;
        }

      case VWidget::AREA:
        {
        VArea* a= new VArea(dialog);
        a->create( x, y, width, height );
        w= a;
        break;
        }

      case VWidget::PROGRESSBAR:
        {
        VProgressBar* p= new VProgressBar(dialog);
        p->create( x, y, width, height );
        w= p;
        break;
        }

      case VWidget::RANGEBAR:
        {
        double minValue= 0., maxValue= 0.;
        MLString s;
        s= decodeString(dm); sscanf( s.get(), "%lf", &minValue );
        s= decodeString(dm); sscanf( s.get(), "%lf", &maxValue );

        VRangeBar* rb= new VRangeBar(dialog);
        rb->create( x, y, width, height, minValue, minValue, maxValue );
        w= rb;
        break;
        }

      case VWidget::VALUEBAR:
        {
        MLString s;

        double minValue= 0., maxValue= 0.;
        s= decodeString(dm); sscanf( s.get(), "%lf", &minValue );
        s= decodeString(dm); sscanf( s.get(), "%lf", &maxValue );
        int nDigits= decode(dm);

        double minValueBar= 0., maxValueBar= 0.;
        s= decodeString(dm); sscanf( s.get(), "%lf", &minValueBar );
        s= decodeString(dm); sscanf( s.get(), "%lf", &maxValueBar );
        int editWidth= decode(dm);

        VValueBar* vb= new VValueBar(dialog);
        if( fixedFont )
          vb->setFixedFont();
        vb->create( x, y, width,
                    editWidth,
                    minValueBar, maxValueBar,
                    minValue, maxValue, nDigits, minValue );
        w= vb;
        break;
        }

      case VWidget::PICTURE:
        {
        int index= decode(dm);
        MLPicture* picture= (index>=1 && index<=nPictures)
                            ? pictures[index-1] : 0;
        VPicture* p= new VPicture(dialog);
        p->create( x, y, picture );
        w= p;
        break;
        }

      case VWidget::CHOOSER:
        {
        VChooser* c= new VChooser(dialog);
        c->create( x, y, width, height );
        w= c;

        int n= decode(dm);
        for( int k= 1; k <= n; ++k )
          c->appendMessage( decodeString(dm) );
        break;
        }

      default:
        break;
      }

    theWidgets[i]= w;
    if( w != 0 )
      {
      w->id= id;
      w->kwDialog= this;
      VWidget* cw= 0;
      if( cardID != 0 && w->type() != VWidget::CARD )
        {
        cw= getWidget(cardID);
        if( cw != 0 )
          if( cw->type() != VWidget::CARD )
            cw= 0;
        }
      if( w->type() == VWidget::CARD )
        dialog->addWidget( (VCard*) w );
       else
        dialog->addWidget(w,isStatic, (VCard*) cw );

      if( ! available )
        w->setAvailable(false);
      }
    }

  delete pictures;
  pictures= 0;

  nGroups= decode(dm);
  groups= 0;
  if( nGroups > 0 )
    {
    groups= new KWGroup [ nGroups ];
    int k;
    for( k= 0; k < nGroups; ++k )
      {
      groups[k].size= 0;
      groups[k].widgets= 0;
      }
    for( k= 0; k < nGroups; ++k )
      {
      int n= decode(dm);

      groups[k].size= n;
      groups[k].widgets= new KWidget [ n ];
      for( int j= 0; j < n; ++j )
        {
        groups[k].widgets[j]= (KWidget) decode(dm);
        if( j == 0 ) // check the first button
          {
          VWidget* w= getWidget(groups[k].widgets[j]);
          if( w != 0 )
            ((VRadioButton*)w)->check(true);
          }
        }
      }
    }

  // link widgets
  for( i= 0; i < nWidgets; ++i )
    {
    int linkID= links[i];
    if( linkID == 0 || linkID == ids[i] || theWidgets[i] == 0 )
      continue;
    VWidget* linkWidget= 0;
    for( int j= 0; j < nWidgets; ++j )
      if( ids[j] == linkID )
        {
        linkWidget= theWidgets[j];
        break;
        }
    if( linkWidget != 0 )
      theWidgets[i]->setLinkWidget( linkWidget );
    }

  delete ids;
  delete theWidgets;
  delete links;

  dialog->makeIndex();

  KWDialogAllocator::addDialog(this); // this sets dialogID

  if( scrolling )
    {
    scartouche->drawImage();
    }
   else
    {
    if( map_it )
      cartouche->map();

    MLEventManager* eventManager= MLEventManager::getEventManager();
    eventManager->removeRedraw(cartouche);
    if( map_it )
      cartouche->postRedraw();
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

boolean KWDialog::create( SystemWindow* parent,
                        const unsigned char* dialogMap,
                        KWCallbackClass* _callbackClass,
                        boolean map_it /* = true */ )
{
  return create( parent,
                 CENTER_ME, CENTER_ME,
                 dialogMap, _callbackClass, map_it );
}

/////////////////////////////////////////////////////////////////////////////

