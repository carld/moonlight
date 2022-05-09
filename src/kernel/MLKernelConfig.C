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
  main.C

  Stephane Rehel

  March 31 1998 (the mozilla day)
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "tools/MLCommandLine.h"
#include "tools/MLFilename.h"
#include "tools/MLConfig.h"
#include "tools/MLConfigReader.h"
#include "tools/MLConfigWriter.h"

#include "MLKernelVersion.h"
#include "MLKernel.h"

#include "config.h"

/////////////////////////////////////////////////////////////////////////////

static void usage();
static const char* ml_install_prefix= ML_INSTALL_PREFIX;
static const char* ml_install_exec_prefix= ML_INSTALL_EXEC_PREFIX;
static boolean ml_verbose= false;

/////////////////////////////////////////////////////////////////////////////

static void read_config( MLConfigReader& r, MLConfig* c )
{
  if( ! r.read(c) )
    {
    fprintf( stderr, "syntax error line %d of file %s\n",
             r.getLineN(), r.getFilename().get() );
    exit(1);
    }
}

/////////////////////////////////////////////////////////////////////////////

static MLConfig* create_config( MLCommandLine& command )
{
  MLConfig* config= new MLConfig;

  MLConfigReader r;

  MLString install_config= MLString(ml_install_prefix) + "/etc/moonlightrc";
  if( r.open(install_config) )
    {
    if( ml_verbose )
      fprintf( stderr, "Reading %s\n", install_config.get() );
    config->setFilename(install_config);
    read_config(r,config);
    r.close();
    }

  MLString user_default_config= MLFilename::expand("~/.moonlightrc");
  if( r.open(user_default_config) )
    {
    if( ml_verbose )
      fprintf( stderr, "Reading %s\n", user_default_config.get() );
    config->setFilename(user_default_config);
    read_config(r,config);
    r.close();
    }

  char* env_conf_filename= getenv("MOONLIGHT_RC_FILE");
  if( env_conf_filename != 0 )
    {
    MLString resourceFilename= MLFilename::expand(env_conf_filename);

    if( r.open(resourceFilename) )
      {
      if( ml_verbose )
        fprintf( stderr, "Reading %s\n", resourceFilename.get() );
      config->setFilename(resourceFilename);
      read_config(r,config);
      r.close();
      }
    }

  if( command.findOption( "-mlrc" ) )
    {
    MLString resourceFilename= command.getString();
    if( resourceFilename.length() == 0 )
      usage();
    if( ml_verbose )
      fprintf( stderr, "Reading %s\n", resourceFilename.get() );
    if( ! r.open(resourceFilename) )
      {
      perror(resourceFilename.get());
      exit(1);
      }

    read_config(r,config);
    r.close();

    config->setFilename(resourceFilename);
    }

  if( config->getFilename().length() == 0 )
    {
    fprintf( stderr, "\nUnable to find resource file!\n"
                     "(Warning: %s/etc/moonlightrc doesn't exist)\n"
                     "You can:\n"
                     "   put it into ~/.moonlightrc\n"
                     "or define a MOONLIGHT_RC_FILE environment variable\n"
                     "or specify a -mlrc command line option\n\n",
                     ml_install_prefix );
    exit(1);
    }

  if( ml_verbose )
    {
    fprintf( stderr, "\nHere's the config i have read:\n\n" );
    MLConfigWriter w;
    w.write( stderr, config );
    }

  return config;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLKernel::make_config()
{
  MLCommandLine& command= getCommandLine();

/*
  if( command.findOption("-h") ||
      command.findOption("-help") ||
      command.findOption("--h") ||
      command.findOption("--help") ||
      command.findOption("-version") ||
      command.findOption("--version") )
    usage();
*/

  _verbose= command.findOption("-v") ||
            command.findOption("--verbose");

  install_prefix= MLPath(ml_install_prefix).get();
  install_exec_prefix= MLPath(ml_install_exec_prefix).get();

  assert( ml_config == 0 );
  ml_config= create_config(command);

  if( _verbose )
    ml_config->setVariable( "Moonlight", "verbose", "1" );

  ml_config->setVariable( "Moonlight", "prefix", install_prefix );
  ml_config->setVariable( "Moonlight", "exec-prefix", install_exec_prefix );

  return true;
}

/////////////////////////////////////////////////////////////////////////////

static void usage()
{
//  char* program= "moonlight";
#define fp fprintf
#define f stdout
  fp( f, "Moonlight Creator %s for %s-%s-%s\n",
         MLKernelVersion::version(),
         MLKernelVersion::build_cpu(),
         MLKernelVersion::build_vendor(),
         MLKernelVersion::build_os() );

  fp( f, "Moonlight %s (C) 1996-1998 By Stephane C.F. Rehel\n",
         MLKernelVersion::version() );
  fp( f, "Moonlight comes with ABSOLUTELY NO WARRANTY. This is free software, and\n" );
  fp( f, "you are welcome to redistribute it under certain conditions.\n" );
  fp( f, "See the LICENSE file for details.\n" );
  fp( f, "Email: rehel@kluster.com\n" );
  fp( f, "  WWW: http://www.cybersociety.com/moonlight/\n" );
  fp( f, "Build #%d, %s\n", MLKernelVersion::build_number(),
                            MLKernelVersion::build_time() );
#ifndef NDEBUG
  fp( f, "*** DEBUG VERSION ***\n" );
#endif
  fp( f, "\n" );

  exit(1);

#undef fp
#undef f
}

/////////////////////////////////////////////////////////////////////////////
