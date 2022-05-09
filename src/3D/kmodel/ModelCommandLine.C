// This file is part of Moonlight Creator
//   Copyright (C) 1996-1998  Stephane Rehel
//                 2002       Patrick Mauritz
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
  ModelCommandLine.h

  Stephane Rehel

  June 4 1998
*/

#include <stdio.h>

#include "tools/MLString.h"
#include "tools/MLLog.h"

#include "interface/MLStatusLineEntryLogging.h"
#include "ModelModule.h"
#include "ModelCommandLine.h"

/////////////////////////////////////////////////////////////////////////////

ModelCommandLine::ModelCommandLine( ModelModule* _model )
{
  model= _model;
}

/////////////////////////////////////////////////////////////////////////////

ModelCommandLine::~ModelCommandLine()
{
  model= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelCommandLine::handleEntryCallback( MLStatusLineEntry* entry,
                                            const MLString& text )
{
  if( entry == 0 || text.length() == 0 )
    return;

  if( model != 0 )
    {
    MLStatusLineEntryLogging* entryLogging= model->getEntryLogging();
    if( entryLogging != 0 )
      entryLogging->addEntry( text, true ); // user entry
    }

  model->getLog()->printf("No command available yet.\n");
printf("No command available yet.\n");
/*
if( text == "show" )
  {
  IList<MLStatusLineEntryLogging::Entry>& entries=model->entryLogging->getEntries();
  IListIterator<MLStatusLineEntryLogging::Entry> li(entries);
  while( ! li.eol() )
    {
    MLStatusLineEntryLogging::Entry* e= li++;
    printf("%d : '%s' (%d)\n", e->the_time,e->msg.get(),e->n_times);
    }
  }
*/
}

/////////////////////////////////////////////////////////////////////////////
