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
  MLSocketServer.C

  Stephane Rehel
  April 2 1998
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "MLSocketServer.h"
#include "MLSocketConnection.h"

#include "config.h"

/////////////////////////////////////////////////////////////////////////////

MLSocketServer::MLSocketServer()
{
  errorMessage= "";

  sd= -1;

  _sa= new sockaddr_in;
}

/////////////////////////////////////////////////////////////////////////////

MLSocketServer::~MLSocketServer()
{
  close();

  delete _sa;
  _sa= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLSocketConnection* MLSocketServer::listen( int port )
{
  sd= socket( AF_INET, SOCK_STREAM, 0 );
  if( sd < 0 )
    {
    errorMessage= strerror(errno);
    return 0;
    }

  sockaddr_in* sa= (sockaddr_in*) _sa;

  sa->sin_family= AF_INET;
  sa->sin_addr.s_addr= INADDR_ANY;
  sa->sin_port= port;

  if( bind( sd, (sockaddr*) sa, sizeof(sockaddr_in) ) < 0 )
    {
    errorMessage= strerror(errno);
    return 0;
    }

  socklen_t len= sizeof(sockaddr_in);
  getsockname( sd, (sockaddr*) sa, &len );

//printf("Port number: %d\n", ntohs(sa->sin_port));

  if( ::listen( sd, 5 ) < 0 )
    {
    errorMessage= strerror(errno);
    return 0;
    }

  return new MLSocketConnection( this, sd );
}

/////////////////////////////////////////////////////////////////////////////

MLString MLSocketServer::getErrorMessage()
{
  MLString em= errorMessage;
  errorMessage= "";
  return em;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSocketServer::close()
{
  if( sd < 0 )
    return true;

  boolean ok= true;

  if( ::close(sd) < 0 )
    {
    errorMessage= strerror(errno);
    ok= false;
    }

  sd= -1;

  return ok;
}

/////////////////////////////////////////////////////////////////////////////
