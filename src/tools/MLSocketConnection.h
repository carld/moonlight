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
  MLSocketConnection.h

  Stephane Rehel
  April 2 1998
*/

#ifndef __MLSocketConnection_h
#define __MLSocketConnection_h

#include "boolean.h"
#include "MLString.h"

/////////////////////////////////////////////////////////////////////////////

class MLSocketConnection
{
  friend class MLSocketServer;

protected:
  MLString errorMessage;

  MLSocketServer* ss;
  int sd;

  int cd;

  void* _sa; // struct sockaddr_in*

protected:
  MLSocketConnection( MLSocketServer* _ss, int _sd );

public:
  virtual ~MLSocketConnection();

  boolean accept();

  MLString getErrorMessage();

  boolean close();
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLSocketConnection_h
