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
  ModelLogListener.C

  Stephane Rehel

  June 1 1998
*/

#include "tools/MLString.h"

#include "interface/MLStatusLineEntry.h"
#include "interface/MLStatusLineEntryLogging.h"

#include "ModelModule.h"
#include "ModelLogListener.h"

/////////////////////////////////////////////////////////////////////////////

ModelLogListener::ModelLogListener( ModelModule* _model,
                                    MLStatusLineEntry* _entry )
{
  model= _model;
  entry= _entry;
}

/////////////////////////////////////////////////////////////////////////////

ModelLogListener::~ModelLogListener()
{
  entry= 0;
}

/////////////////////////////////////////////////////////////////////////////

void ModelLogListener::newLog( const MLString& msg )
{
  if( entry != 0 )
    entry->setLastLog(msg);

  if( model != 0 )
    {
    MLStatusLineEntryLogging* entryLogging= model->getEntryLogging();
    if( entryLogging != 0 )
      entryLogging->addEntry(msg);
    }
}

/////////////////////////////////////////////////////////////////////////////

void ModelLogListener::flush()
{
  if( entry != 0 )
    entry->draw();
}

/////////////////////////////////////////////////////////////////////////////
