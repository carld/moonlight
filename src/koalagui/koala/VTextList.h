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
  VTextList.h

  Stephane Rehel
  Creation: SR, July 23 1996
*/

#ifndef __VTextList_h
#define __VTextList_h

#ifndef __VWidget_h
#include "VWidget.h"
#endif

#ifndef __VList_h
#include "VList.h"
#endif

class MLFont;

/////////////////////////////////////////////////////////////////////////////

class VTextList: public VList, public VListItemHandler
{
protected:
  MLFont* font;
  int margin;

  class VTextListItemData: public VListItemData
    {
    public:
      MLString message;
      MLString cutMessage;
      VTextListItemData(): message(""), cutMessage("") {}
      virtual ~VTextListItemData() {}
    };

public:
  VTextList( VWindow* parent );
  virtual ~VTextList();

  void setFixedFont();

  void create( int x, int y, int width, int height, boolean scrolling );

  virtual VWidget::TYPE type() const
    {
    return VWidget::TEXTLIST;
    }

  int getInteger(); // may return 0

  boolean appendMessage( const MLString& text );
  // 1 <= i <= nElements
  boolean setMessage( int i, const MLString& text );
  // 1 <= i <= nElements+1
  boolean insertMessage( int i, const MLString& text );
  // 1 <= i <= nElements
  boolean deleteMessage( int i );

  // 1 <= i <= nElements
  const MLString& getMessage( int i );

  int getMessagePosition( const MLString& message );

protected:
  void handleVListItem( VList*, VList::Item*,
                        int yBase, boolean selected );

};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __VTextList_h
