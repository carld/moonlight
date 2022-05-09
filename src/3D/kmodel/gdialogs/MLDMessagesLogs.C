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
  MLDMessagesLogs.C

  Stephane Rehel

  June 7 1997
*/

#include <string.h>

#include "tools/Chrono.h"
#include "graphics/KeyCodes.h"

#include "kw/KWDialog.h"

#include "kmodel/ModelModule.h"

#include "MLDMessagesLogs.h"
#include "MessagesLogsDlg.h"

#include "interface/MLStatusLineEntryLogging.h"

/////////////////////////////////////////////////////////////////////////////

MLDMessagesLogs::MLDMessagesLogs( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
}

/////////////////////////////////////////////////////////////////////////////

MLDMessagesLogs::~MLDMessagesLogs()
{
  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

void MLDMessagesLogs::run()
{
  if( ! MLGlobalDialog::create(MessagesLogsDlg::data) )
    return;

  MLStatusLineEntryLogging* entryLogging= model->getEntryLogging();
  IList<MLStatusLineEntryLogging::Entry>& entries= entryLogging->getEntries();
  IListIterator<MLStatusLineEntryLogging::Entry> li(entries);
  while( ! li.eol() )
    {
    MLStatusLineEntryLogging::Entry* e= li++;
//    printf("%d : '%s' (%d)\n", e->the_time,e->msg.get(),e->n_times);

    char user_char= e->user_entry ? '$' : ':';
    MLString line;
    if( e->n_times > 1 )
      {
      line= MLString::printf( "%s%c %s (last message repeated %d times)",
                     Chrono::getHoursMinutesSeconds(e->n_times_time).get(),
                     user_char,
                     e->msg.get(), e->n_times );
      }
     else
      {
      line= MLString::printf( "%s%c %s",
                     Chrono::getHoursMinutesSeconds(e->the_time).get(),
                     user_char,
                     e->msg.get() );
      }

    kwDialog->appendListText( MessagesLogsDlg::list, line );
    }

/*
  const char** s= text;
  while( *s != 0 )
    {
    const char* line= *s;
    kwDialog->appendListText( MessagesLogsDlg::text, line );
    ++s;
    }
*/

  kwDialog->focus(MessagesLogsDlg::list);

  kwDialog->run();

  MLGlobalDialog::close();
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDMessagesLogs::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::KEY &&
      event.key == KeyCodes::ESC )
    {
    kwDialog->quit();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

