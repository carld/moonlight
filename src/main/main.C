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
  main.C

  Stephane Rehel

  March 31 1998 (the mozilla day!)
*/

#include <stdlib.h>
#include <stdio.h>

#include "tools/MLCommandLine.h"

#include "kernel/MLKernelVersion.h"
#include "kernel/MLKernel.h"
#include "kernel/MLKernelModule.h"

#ifdef __CYGWIN32__
extern "C" MLAppRoot* kmodel_main(MLKernel*,MLNode*);
#endif

/////////////////////////////////////////////////////////////////////////////

static void usage();
static char* program= 0;

/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{

/*
  MLFilename argv0(argv[0]);
  MLString basename= argv0.getFilename(); // without any extension!
*/
  ::program= argv[0];
  MLCommandLine command(argc,(const char**)argv);

  if( command.findOption("-h") ||
      command.findOption("-help") ||
      command.findOption("--h") ||
      command.findOption("--help") ||
      command.findOption("-version") ||
      command.findOption("--version") )
    usage();

  MLKernel* kernel= new MLKernel;

  if( ! kernel->init(argc,argv) )
    {
    fprintf( stderr, "error in kernel initialization, aborting\n" );
    return 1;
    }

/*
  if( ! InitGraphicsSystem(config,command) )
    {
    fprintf( stderr, "Error: unable to open graphics system\n" );
    return 1;
    }

  interface= new Interface(config);

  interface->init(command);

  MLModule* module= 0;
  MLModule* defaultModule= 0;

  interface->activateModule(defaultModule->getID());

  MLEventManager* eventManager= MLEventManager::getEventManager();

  eventManager->removeAnyRedraw();
  interface->refreshInterface();

  interface->mainloop();
*/

MLString errormsg;
/*
MLAppRoot* approot= new MLAppRoot(kernel);
approot->setName("Moonlight-3D");
approot->init();
approot->build_resources(config);

//approot->queryPlugin("koalagui",0,&errormsg);
//printf("end foo\n");
//printf( "ok='%s'\n", errormsg.get() );

//approot->queryPlugin("tcl",0,&errormsg);
//printf( "tcl ok='%s'\n", errormsg.get() );
*/

  {
#ifndef __CYGWIN32__
  LOAD_KERNEL_MODULE(koalagui,kernel,0,&errormsg);
#endif
  if( errormsg.length() != 0 )
    printf("%s\n",errormsg.get());
  }

//ml_module_main( kernel, 0 );
//LOAD_KERNEL_MODULE(kmodel,kernel,0,&errormsg);

//printf( "kmodel module='%s'\n", errormsg.get() );

  return 0;
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

  fp( f, "General options:\n" );
  fp( f, "  -h                This message\n" );
  fp( f, "  -v, --verbose     Verbose mode\n" );
  fp( f, "  -convert          Run the converter (command line)\n" );
  fp( f, "  -mlrc <file>      Specify ML resource file\n" );
  fp( f, "\n" );

  fp( f, "Interface options:\n" );
  fp( f, "  -gamma <gamma>    Specify gamma correction (default is 1)\n" );
#ifndef __WINDOWS__
  fp( f, "  -display <server> Run on this X server (default is :0)\n" );
#endif
  fp( f, "  -fs               Run full-screen (default)\n" );
  fp( f, "  -debug            Don't run full-screen\n" );
  fp( f, "\n" );

  fp( f, "Converter: %s -convert [options] <infile>\n",program);
  fp( f, "  Options:\n");
  fp( f, "  -outname <out_scene_name> Specify output scene name\n" );
  fp( f, "  -db <database_path>       Specify a custom database (optional)\n" );
  fp( f, "  -v                        Verbose mode\n" );
  fp( f, "  -rv                       Reverse polygons\n" );
  fp( f, "  -dxfl2o                   Convert DXF layers to objects\n" );
  fp( f, "  -nw                       Don't write. For testing.\n" );
  fp( f, "\n");

  exit(1);

#undef fp
#undef f
}

/////////////////////////////////////////////////////////////////////////////
