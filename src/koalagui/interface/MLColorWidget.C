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
  MLColorWidget.C

  Stephane Rehel

  March 15 1997
*/

#include "koala/VWidget.h"
#include "graphics/mlgl.h"

#include "MLColorWidget.h"
#include "kw/KWDialog.h"

#include "tools/HSVColor.h"

/////////////////////////////////////////////////////////////////////////////

MLColorWidget::MLColorWidget( KWDialog* _dialog,
                              unsigned int label_r,
                              unsigned int label_g,
                              unsigned int label_b,
                              unsigned int valuesBar_r,
                              unsigned int valuesBar_g,
                              unsigned int valuesBar_b,
                              unsigned int _radio_rgb,
                              unsigned int _radio_hsv,
                              unsigned int _area,
                              unsigned int _factor /* = 0 */ )
{
  dialog= _dialog;
  labels[R]= label_r;
  labels[G]= label_g;
  labels[B]= label_b;
  valuesBar[R]= valuesBar_r;
  valuesBar[G]= valuesBar_g;
  valuesBar[B]= valuesBar_b;
  radio_rgb= _radio_rgb;
  radio_hsv= _radio_hsv;
  area= _area;
  factor= _factor;

  color= Color(0,0,0);
  color_factor= 1.;

  setRGBColorModel();//dialog->setChecked( radio_rgb, true );
}

/////////////////////////////////////////////////////////////////////////////

MLColorWidget::~MLColorWidget()
{}

/////////////////////////////////////////////////////////////////////////////

void MLColorWidget::set( const Color& _color, double _factor /* =1. */ )
{
  if( color == _color && (factor==0 || _factor == color_factor) )
    return;

  color= _color;
  if( factor != 0 )
    color_factor= _factor;

  if( inRGBColorModel() )
    {
    dialog->setValue(valuesBar[R],color.r());
    dialog->setValue(valuesBar[G],color.g());
    dialog->setValue(valuesBar[B],color.b());
    }
   else
    {
    HSVColor hsvColor(color);
    dialog->setValue(valuesBar[H],hsvColor.h());
    dialog->setValue(valuesBar[S],hsvColor.s());
    dialog->setValue(valuesBar[V],hsvColor.v());
    }

  if( factor != 0 )
    dialog->setValue(factor,color_factor);

  dialog->repaintArea(area);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLColorWidget::inRGBColorModel()
{
  return (dialog->getRadioChecked(radio_rgb) == KWidget(radio_rgb));
}

/////////////////////////////////////////////////////////////////////////////

void MLColorWidget::setRGBColorModel()
{
  if( dialog == 0 )
    return;

  dialog->setChecked(radio_rgb,true);

  dialog->setText(labels[R],"R");
  dialog->setText(labels[G],"G");
  dialog->setText(labels[B],"B");
  dialog->setValue(valuesBar[R],color.r());
  dialog->setValue(valuesBar[G],color.g());
  dialog->setValue(valuesBar[B],color.b());

  if( factor != 0 )
    dialog->setValue( factor, color_factor );

  dialog->repaintArea(area);
}

/////////////////////////////////////////////////////////////////////////////

void MLColorWidget::setHSVColorModel()
{
  if( dialog == 0 )
    return;

  dialog->setChecked(radio_hsv,true);

  HSVColor hsvColor(color);
  dialog->setText(labels[R],"H");
  dialog->setText(labels[G],"S");
  dialog->setText(labels[B],"V");
  dialog->setValue(valuesBar[H],hsvColor.h());
  dialog->setValue(valuesBar[S],hsvColor.s());
  dialog->setValue(valuesBar[V],hsvColor.v());

  if( factor != 0 )
    dialog->setValue( factor, color_factor );

  dialog->repaintArea(area);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLColorWidget::colorCallback( KWEvent& event )
{
  if( event.dialog != dialog )
    return false;

  boolean rgbModel= inRGBColorModel();

  if( event.widget == valuesBar[R] ||
      event.widget == valuesBar[G] ||
      event.widget == valuesBar[B] )
    {
    int what= (event.widget == valuesBar[R]) ? 0 :
              (event.widget == valuesBar[G]) ? 1 : 2;
    if( event.type == KWEvent::VALUE )
      {
      if( rgbModel )
        color[what]= dialog->getValue(event.widget);
       else
        {
        HSVColor hsvColor(color);
        switch( what )
          {
          case 0: hsvColor.seth( dialog->getValue(event.widget) );
                  break;
          case 1: hsvColor.sets( dialog->getValue(event.widget) );
                  break;
          case 2: hsvColor.setv( dialog->getValue(event.widget) );
                  break;
          }
        color= Color(hsvColor);
        }

      dialog->repaintArea(area);
      return true;
      }

    return false;
    }

  if( event.widget == radio_rgb )
    {
    if( event.type == KWEvent::SELECT )
      {
      setRGBColorModel();

      dialog->repaintArea(area);

      return true;
      }
    return true;
    }

  if( event.widget == radio_hsv )
    {
    if( event.type == KWEvent::SELECT )
      {
      setHSVColorModel();

      dialog->repaintArea(area);

      return true;
      }
    return false;
    }

  if( event.widget == area )
    {
    if( event.type == KWEvent::DRAW )
      {
      VWidget* w= dialog->getWidget(area);
      if( w != 0 )
        {
        if( w->beginDraw() )
          {
          int margin= 4;
          IVector size= w->getSize();
//          glEnable(GL_DITHER);
          mlBlack();
          mlRect( 0, 0, size.x()-1, margin-1 );
          mlRect( 0, size.y()-margin, size.x()-1, size.y()-1 );
          mlRect( 0, 0, margin-1, size.y()-1 );
          mlRect( size.x()-margin, 0, size.x()-1, size.y()-1 );
          mlColor3f( color.r(), color.g(), color.b() );
          mlRect( margin, margin, size.x()-margin-1, size.y()-margin-1 );
//          glDisable(GL_DITHER);
          w->endDraw();
          }
        }
      return true;
      }

    return false;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////

void MLColorWidget::setAvailable( boolean yes )
{
  dialog->setAvailable( labels[R], yes );
  dialog->setAvailable( labels[G], yes );
  dialog->setAvailable( labels[B], yes );
  dialog->setAvailable( valuesBar[R], yes );
  dialog->setAvailable( valuesBar[G], yes );
  dialog->setAvailable( valuesBar[B], yes );
  dialog->setAvailable( radio_rgb, yes );
  dialog->setAvailable( radio_hsv, yes );

  if( factor != 0 )
    dialog->setAvailable( factor, yes );
}

/////////////////////////////////////////////////////////////////////////////
