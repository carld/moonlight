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
  VPicture.h

  Stephane Rehel
  August 16 1996
*/

#ifndef __VPicture_h
#define __VPicture_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

class MLPicture;

/////////////////////////////////////////////////////////////////////////////

class VPicture: public VWidget
{
protected:
  MLPicture* picture; // not owner

public:
  VPicture( VWindow* _parent );

  virtual ~VPicture();

  void create( int x, int y, MLPicture* _picture );

  virtual VWidget::TYPE type() const
    {
    return VWidget::PICTURE;
    }

  virtual void drawWidget();

  MLPicture* getMLPicture() const
    {
    return picture;
    }

protected:
  void drawPicture();

public:
  virtual boolean handleEvent( MLEvent& event );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VPicture_h
