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
  MLSocketClient.C

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

#include "MLSocketClient.h"

/////////////////////////////////////////////////////////////////////////////

MLSocketClient::MLSocketClient()
{
  errorMessage= "";

  sd= -1;

  _he= new hostent;
  _sa= new sockaddr_in;

  file= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLSocketClient::~MLSocketClient()
{
  close();

  delete _he;
  _he= 0;
  delete _sa;
  _sa= 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSocketClient::open( const MLString& hostname, int port )
{
  close();

  hostent* he= (hostent*) _he;
  sockaddr_in* sa= (sockaddr_in*) _sa;

  sa->sin_family= AF_INET;

  hostent* he0= gethostbyname( hostname.get() );
  if( he0 == 0 )
    {
    errorMessage= strerror(errno);
    return false;
    }
  memcpy( _he, (const void*) he0, sizeof(hostent) );

  memcpy( (void*) &(sa->sin_addr), (void*) (he->h_addr), he->h_length );

  sa->sin_port= htons(port);

  sd= socket( AF_INET, SOCK_STREAM, 0 );
  if( sd < 0 )
    {
    errorMessage= strerror(errno);
    return false;
    }

  int outcome= connect( sd, (const sockaddr*) &sa, sizeof(sockaddr_in) );

  if( outcome < 0 )
    {
    errorMessage= strerror(errno);
    return false;
    }

/*
  printf("Connextion etablie. Parlez !\n");
  while ((n=read(0, buf, sizeof buf)) > 0)
    {
    if (write(sd, buf, n) < 0)
     exit(-1);
    }
*/

  file= fdopen(sd,"rw");

  if( file == 0 )
    {
    errorMessage= strerror(errno);
    return false;
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

MLString MLSocketClient::getErrorMessage()
{
  MLString em= errorMessage;
  errorMessage= "";
  return em;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLSocketClient::close()
{
  if( sd < 0 )
    return true;

  boolean ok= true;

  if( file != 0 )
    {
    if( ::fclose(file) < 0 )
      {
      errorMessage= strerror(errno);
      ok= false;
      }
    file= 0;
    }

  if( ::close(sd) < 0 )
    {
    errorMessage= strerror(errno);
    ok= false;
    }

  sd= -1;

  return ok;
}

/////////////////////////////////////////////////////////////////////////////
