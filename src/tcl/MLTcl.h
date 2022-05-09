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
  MLTcl.h

  Stephane Rehel

  April 13 1998
*/

#ifndef __MLTcl_h
#define __MLTcl_h

#include "tools/SIList.h"
#include "tools/MLString.h"
#include "tcl.h"

class MLTclPlugin;
class MLNode;
class MLProperty;

/////////////////////////////////////////////////////////////////////////////

class MLTcl
{
protected:
  MLTclPlugin* tclplugin;
  Tcl_Interp* interp;

public:
  typedef int (MLTcl::*MLTcl_CmdProc)(int,Tcl_Obj*[]);
  struct MLTclProc
    {
    MLTcl* tcl;
    MLTcl_CmdProc proc;
    };

  SIList<MLTclProc> procs;

public:
  MLTcl( MLTclPlugin* _tclplugin );

  virtual ~MLTcl();

  Tcl_Interp* getInterp() const
    {
    return interp;
    }

protected:
  void destroy();

public:
  boolean create();

protected:
  void add_kernel_commands();

  void createCommand( char* name, MLTcl_CmdProc proc );
  MLString createNodeCommand( MLNode* node );
  MLString createPropertyCommand( MLProperty* prop );

public:
  int ml_root( int objc, Tcl_Obj* objv[] );

  int ml_node( int objc, Tcl_Obj* objv[] );

  int ml_node( MLNode* node, int objc, Tcl_Obj* objv[] );

  int ml_prop( int objc, Tcl_Obj* objv[] );

  int ml_prop( MLProperty* p, int objc, Tcl_Obj* objv[] );
};

/////////////////////////////////////////////////////////////////////////////

#endif // ifndef __MLTcl_h
