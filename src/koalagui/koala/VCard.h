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
  VCard.h

  Stephane Rehel
  August 19 1996
*/

#ifndef __VCard_h
#define __VCard_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

class VDialog;

/////////////////////////////////////////////////////////////////////////////

class VCard: public VWidget
{
private:
  MLString message;
  HotCharMessage hcm;
  MLFont* cardFont;
  VDialog* dialog;
  IPoint textpos;

public:
  VCard( VDialog* _dialog );

  virtual ~VCard();

  virtual void create( int x, int y, int width, int height,
                       const MLString& _message );

  const MLString& getMessage() const
    {
    return message;
    }

  void changeMessage( const MLString& _message );

  virtual VWidget::TYPE type() const
    {
    return VWidget::CARD;
    }

  virtual char getHotChar() const;

  virtual void drawWidget();

protected:
  virtual void drawCard();
  virtual boolean handleEvent( MLEvent& event );
  virtual boolean handleKey( Key key );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VCard_h

