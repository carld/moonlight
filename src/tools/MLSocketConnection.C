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
  MLSocketConnection.C

  Stephane Rehel
  April 2 1998
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "MLSocketServer.h"
#include "MLSocketConnection.h"

#include "config.h"

/////////////////////////////////////////////////////////////////////////////

MLSocketConnection::MLSocketConnection( MLSocketServer* _ss, int _sd )
{
  errorMessage= "";

  ss= _ss;
  sd= _sd;

  cd= -1;

  _sa= new sockaddr_in;
}

/////////////////////////////////////////////////////////////////////////////

MLSocketConnection::~MLSocketConnection()
{
  close();

  delete _sa;
  _sa= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSocketConnection::accept()
{
  close();

  socklen_t len= sizeof(sockaddr_in);

  cd= ::accept( sd, (sockaddr*) _sa, &len );
  if( cd < 0 )
    {
    errorMessage= strerror(errno);
    return false;
    }

  return true;
}

/*
  for (;;)
    {
    len = sizeof(sa2);
    cd = accept(sd, &sa2, &len);
    if (fork() == 0)
      {
      while ((n = read(cd, buf, sizeof buf)) >0)
        write(1, buf, n);
      exit(0);
      }
    close(cd);
    }
*/

/////////////////////////////////////////////////////////////////////////////

MLString MLSocketConnection::getErrorMessage()
{
  MLString em= errorMessage;
  errorMessage= "";
  return em;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSocketConnection::close()
{
  if( cd < 0 )
    return true;

  boolean ok= true;

  if( ::close(cd) < 0 )
    {
    errorMessage= strerror(errno);
    ok= false;
    }

  cd= -1;

  return ok;
}

/////////////////////////////////////////////////////////////////////////////
