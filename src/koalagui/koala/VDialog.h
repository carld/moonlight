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
  VDialog.h

  Stephane Rehel
  July 24 1996
*/

#ifndef __VDialog_h
#define __VDialog_h

#ifndef __MLKeyboardHandler_h
#include "graphics/MLKeyboardHandler.h"
#endif

#ifndef __MLEventGrabber_h
#include "graphics/MLEventGrabber.h"
#endif

#ifndef __VWidgetsSet_h
#include "VWidgetsSet.h"
#endif


class KWDialog;

/////////////////////////////////////////////////////////////////////////////

class VDialog: public VWidgetsSet, public MLKeyboardHandler,
                                   public MLEventGrabber

{
  friend class KWDialog;
  friend class VCartouche;
  friend class VScrollingCartouche;
  friend class VCard;

private:
  VCartouche* cartouche;
  KWDialog* kwDialog;

protected:
  VWidget* active;
  SIList<VWidget> dialogWidgets;

  VCard* activeCard;
  int nCards;
  VCard** cards;
  IRectangle cardsRectangle;

public:
  VDialog( VWindow* parent );
  virtual ~VDialog();

  virtual void create( int x, int y, int width, int height );
  void focus();
  void getFocus();
  void ungetFocus();

  virtual VWidget::TYPE type() const
    {
    return VWidget::DIALOG;
    }

  virtual void addWidget( VWidget* widget,
                          boolean staticWidget = false,
                          VCard* card = 0 );
  void addCard( VCard* card );

  // remove widget but dont delete it
  // return true if removed
  boolean removeWidget( VWidget* widget );

  void makeIndex();
private:
  void _makeIndex( VWidget* w );

protected:
  VWidget* getNextActive();

  void _drawCardsRectangle();

public:
  virtual void drawWidget();
  void setCardsRectangle( const IRectangle& r );
  const IRectangle& getCardsRectangle() const { return cardsRectangle; }
  VCard* getActiveCard() const
    {
    return activeCard;
    }
  void setActiveCard( VCard* card ); // called by VCard & KWDialog

  void setCurrentWidget( VWidget* widget );

  virtual boolean handleMLKeyboard( MLEvent& event );
  virtual boolean handleEvent( MLEvent& event );
protected:
  virtual boolean eventGrabber( MLWindow*, MLEvent& );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VDialog_h
