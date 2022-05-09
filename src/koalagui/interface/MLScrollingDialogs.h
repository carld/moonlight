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
  MLScrollingDialogs.h

  Stephane Rehel

  January 1st 1998
          ^^^
          woo!
*/

#ifndef __MLScrollingDialogs_h
#define __MLScrollingDialogs_h

class MLModule;
class MLString;
class VScrollingDialogs;
class MLScrollingDialog;

/////////////////////////////////////////////////////////////////////////////

class MLScrollingDialogs
{
protected:
  MLModule* module;

  MLScrollingDialog** dialogs;
  int nScrollingDialogs;

public:
  MLScrollingDialogs( MLModule* _module );

  virtual ~MLScrollingDialogs();

  int getNScrollingDialogs() const
    {
    return nScrollingDialogs;
    }

  MLScrollingDialog* getScrollingDialog( int i ) const;

  void postRefreshDialog( int dialogID );
  void postRefreshDialog( const MLString& name );
  void refreshScreen( unsigned int what );

  MLScrollingDialog* getDialog( const MLString& name ) const;
  void createDialog( MLScrollingDialog* dialog );
  void removeDialog( const MLString& name );
  void removeAllDialogs();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLScrollingDialogs_h
