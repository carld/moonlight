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
  MLSocketClient.h

  Stephane Rehel
  April 2 1998
*/

#ifndef __MLSocketClient_h
#define __MLSocketClient_h

#include <stdio.h>

#include "boolean.h"
#include "MLString.h"

/////////////////////////////////////////////////////////////////////////////

class MLSocketClient
{
protected:
  MLString errorMessage;

  int sd;
  struct hostent* _he;
  struct sockaddr_in* _sa;

  FILE* file;

public:
  MLSocketClient();

  virtual ~MLSocketClient();

  boolean open( const MLString& hostname, int port );

  FILE* getFile() const
    {
    return file;
    }

  MLString getErrorMessage();

  int getSocketDescriptor() const
    {
    return sd;
    }

  boolean close();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSocketClient_h
