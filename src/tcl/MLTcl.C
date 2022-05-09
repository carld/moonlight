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
  MLTcl.C

  Stephane Rehel

  April 13 1998
*/

#include <stdio.h>
#include <string.h>

#include "MLTcl.h"
#include "MLTclPlugin.h"

#include "kernel/MLProperty.h"
#include "kernel/MLNode.h"
#include "kernel/MLNodeType.h"
#include "kernel/MLAppRoot.h"
#include "kernel/MLKernel.h"

#include "kernel/MLPBoolean.h"
#include "kernel/MLPInteger.h"
#include "kernel/MLPReal.h"
#include "kernel/MLPString.h"
#include "kernel/MLPVector.h"
#include "kernel/MLPPoint.h"
#include "kernel/MLPColor.h"
#include "kernel/MLPNodeRef.h"
#include "kernel/MLPArray.h"

/////////////////////////////////////////////////////////////////////////////

MLTcl::MLTcl( MLTclPlugin* _tclplugin )
{
  tclplugin= _tclplugin;
  interp= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLTcl::~MLTcl()
{
  destroy();
}

/////////////////////////////////////////////////////////////////////////////

void MLTcl::destroy()
{
  if( interp != 0 )
    {
    Tcl_DeleteInterp( interp );
    interp= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

boolean MLTcl::create()
{
  if( interp != 0 )
    return true;

  interp= Tcl_CreateInterp();

  if( interp == 0 )
    {
    fprintf( stderr, "error creating tcl interp\n");
    return false;
    }

  Tcl_SetVar( interp, "tcl_interactive", "0", TCL_GLOBAL_ONLY );

  if( Tcl_Init(interp) != TCL_OK )
    {
    fprintf( stderr, "error initializing tcl interp\n");
    return false;
    }

  add_kernel_commands();

#ifdef ML_TCL_DEBUG_TEST
  MLString s= MLString("source ") + MLString(ML_TCL_DEBUG_TEST);
  int outcome= Tcl_Eval(interp,s.getBytes());
  if( outcome != TCL_OK )
    printf("%s\n",interp->result);
#endif

  return true;
}

/////////////////////////////////////////////////////////////////////////////

static
int ml_tcl_proc_entry( ClientData clientData,
                       Tcl_Interp*
#ifndef NDEBUG
                       interp
#endif
                       ,
                       int objc, Tcl_Obj* objv[] )
{
  if( clientData == 0 )
    return TCL_ERROR;

  MLTcl::MLTclProc* p= (MLTcl::MLTclProc*) clientData;
  assert( p->tcl->getInterp() == interp );

  MLTcl::MLTcl_CmdProc proc= p->proc;
  return (p->tcl->*proc)(objc,objv);
}

/////////////////////////////////////////////////////////////////////////////

static
void ml_tcl_proc_delete( ClientData clientData )
{
  if( clientData == 0 )
    return;

  MLTcl::MLTclProc* p= (MLTcl::MLTclProc*) clientData;

  p->tcl->procs.remove(p); // expensive
  delete p;
}

/////////////////////////////////////////////////////////////////////////////

void MLTcl::add_kernel_commands()
{
  if( interp == 0 )
    return;

  createCommand( "ml_root", &ml_root );
}

/////////////////////////////////////////////////////////////////////////////

void MLTcl::createCommand( char* name, MLTcl_CmdProc proc )
{
  MLTclProc* p= new MLTclProc;
  p->tcl= this;
  p->proc= proc;

  procs.append(p);

  Tcl_CreateObjCommand( interp,
                        name,
                        ml_tcl_proc_entry,
                        (ClientData) p,
                        ml_tcl_proc_delete );
}

/////////////////////////////////////////////////////////////////////////////

MLString MLTcl::createNodeCommand( MLNode* node )
{
  if( node == 0 )
    return "";

  char name[20];
  sprintf( name, "node%d", node->getID() );

  Tcl_CmdInfo info;
  if( Tcl_GetCommandInfo(interp,name,&info) == 0 )
    createCommand( name, &ml_node );

  return MLString(name);
}

/////////////////////////////////////////////////////////////////////////////

MLString MLTcl::createPropertyCommand( MLProperty* prop )
{
  if( prop == 0 )
    return "";

  char name[20];
  sprintf( name, "prop%d", prop->getID() );

  Tcl_CmdInfo info;
  if( Tcl_GetCommandInfo(interp,name,&info) == 0 )
    createCommand( name, &ml_prop );

  return MLString(name);
}

/////////////////////////////////////////////////////////////////////////////

int MLTcl::ml_root( int objc, Tcl_Obj* objv[] )
{
  return ml_node( tclplugin->getAppRoot(), objc, objv );
}

/////////////////////////////////////////////////////////////////////////////

int MLTcl::ml_node( int objc, Tcl_Obj* objv[] )
{
  if( objc < 2 )
    {
    Tcl_WrongNumArgs( interp, 1, objv, "command ?args?");
    return TCL_ERROR;
    }

  int id= 0;
  if( sscanf(Tcl_GetString(objv[0]),"node%d",&id) != 1 )
    id= 0;

  MLNode* node= (id==0) ? 0 : tclplugin->getKernel()->getNode(id);

  if( strcmp(Tcl_GetString(objv[1]),"exists") == 0 )
    {
    Tcl_SetIntObj( Tcl_GetObjResult(interp),
                   (node==0) ? 0 : 1 );
    return TCL_OK;
    }

  if( node == 0 )
    {
    Tcl_ResetResult(interp);
    Tcl_AppendResult( interp,
                      "unknown node: ", Tcl_GetString(objv[0]), 0 );
    return TCL_ERROR;
    }

  return ml_node(node,objc,objv);
}

/////////////////////////////////////////////////////////////////////////////

int MLTcl::ml_node( MLNode* node, int objc, Tcl_Obj* objv[] )
{
  if( objc < 2 )
    {
    Tcl_WrongNumArgs( interp, 0, objv,
                      MLString("command ?args?").getBytes() );
    return TCL_ERROR;
    }

  char* cmd= Tcl_GetString(objv[1]);

  Tcl_ResetResult(interp);
  if( strcmp(cmd,"name") == 0 )
    {
    Tcl_SetStringObj( Tcl_GetObjResult(interp),
                      node->getName().getBytes(), -1 );
    return TCL_OK;
    }

  if( strcmp(cmd,"nodeof") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("nodeof name").getBytes() );
      return TCL_ERROR;
      }

    MLNodeType* types= tclplugin->getAppRoot()->getTypes();
    MLNodeType::Type t= types->getType( Tcl_GetString(objv[2]) );
    if( int(t) == 0 )
      {
      Tcl_AppendResult( interp,
                        "unknown node type: ", Tcl_GetString(objv[1]), 0 );
      return TCL_ERROR;
      }

    Tcl_SetIntObj( Tcl_GetObjResult(interp),
                   node->nodeOf(t) ? 1 : 0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"nChildren") == 0 )
    {
    Tcl_SetIntObj( Tcl_GetObjResult(interp),
                   node->getNChildren() );
    return TCL_OK;
    }

  if( strcmp(cmd,"children") == 0 )
    {
    int n= node->getNChildren();
    for( int i= 0; i < n; ++i )
      {
      MLNode* child= node->getChild(i);
      MLString node_cmd= createNodeCommand(child);
      Tcl_AppendElement( interp, node_cmd.getBytes() );
      }
    return TCL_OK;
    }

  if( strcmp(cmd,"findChild") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("findChild child_name").getBytes() );
      return TCL_ERROR;
      }

    char* child_name= Tcl_GetString(objv[2]);
    MLNode* child= node->getChild(child_name);

    Tcl_AppendElement( interp, createNodeCommand(child).getBytes() );

    return TCL_OK;
    }

  if( strcmp(cmd,"newChild") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("newChild child_name").getBytes() );
      return TCL_ERROR;
      }

    char* child_name= Tcl_GetString(objv[2]);
    MLNode* child= node->newChild(child_name);

    Tcl_AppendElement( interp, createNodeCommand(child).getBytes() );

    return TCL_OK;
    }

  if( strcmp(cmd,"nProperties") == 0 )
    {
    Tcl_SetIntObj( Tcl_GetObjResult(interp),
                   node->getNProperties() );
    return TCL_OK;
    }

  if( strcmp(cmd,"properties") == 0 )
    {
    int n= node->getNProperties();
    for( int i= 0; i < n; ++i )
      {
      MLProperty* p= node->getProperty(i);
      MLString prop_cmd= createPropertyCommand(p);
      Tcl_AppendElement( interp, prop_cmd.getBytes() );
      }
    return TCL_OK;
    }

  if( strcmp(cmd,"findProperty") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("findProp prop_name").getBytes() );
      return TCL_ERROR;
      }

    char* prop_name= Tcl_GetString(objv[2]);
    MLProperty* prop= node->getProperty(prop_name);

    Tcl_AppendElement( interp, createPropertyCommand(prop).getBytes() );

    return TCL_OK;
    }

#define newProperty(NAME,NEW_METHOD,DEFAULT_VALUE) \
  if( strcmp(cmd,NAME) == 0 )                                            \
    {                                                                    \
    if( objc < 3 )                                                       \
      {                                                                  \
      Tcl_WrongNumArgs( interp, 0, objv,                                 \
                        MLString("newProperty prop_name").getBytes() );  \
      return TCL_ERROR;                                                  \
      }                                                                  \
                                                                         \
    char* prop_name= Tcl_GetString(objv[2]);                             \
    MLProperty* prop= node->NEW_METHOD(prop_name,DEFAULT_VALUE);         \
                                                                         \
    Tcl_AppendElement( interp, createPropertyCommand(prop).getBytes() ); \
                                                                         \
    return TCL_OK;                                                       \
    }
newProperty("newBoolean",newBoolean,false)
newProperty("newInteger",newInteger,0)
newProperty("newReal",newReal,0.)
newProperty("newString",newString,"")
newProperty("newVector",newVector,Vector(0,0,0))
newProperty("newColor",newColor,Color(0,0,0))
#undef newProperty
  Tcl_AppendResult( interp, "unknown node command: ", cmd, 0 );

  return TCL_ERROR;
}

/////////////////////////////////////////////////////////////////////////////

int MLTcl::ml_prop( int objc, Tcl_Obj* objv[] )
{
  if( objc < 2 )
    {
    Tcl_WrongNumArgs( interp, 1, objv, "command ?args?");
    return TCL_ERROR;
    }

  char* propn= Tcl_GetString(objv[0]);

  int id= 0;
  if( sscanf(propn,"prop%d",&id) != 1 )
    id= 0;

  MLProperty* prop= (id==0) ? 0 : tclplugin->getKernel()->getProperty(id);

  if( strcmp(Tcl_GetString(objv[1]),"exists") == 0 )
    {
    Tcl_SetIntObj( Tcl_GetObjResult(interp),
                   (prop==0) ? 0 : 1 );
    return TCL_OK;
    }

  if( prop == 0 )
    {
    Tcl_ResetResult(interp);
    Tcl_AppendResult( interp, "unknown property: ", propn, 0 );
    return TCL_ERROR;
    }

  return ml_prop(prop,objc,objv);
}

/////////////////////////////////////////////////////////////////////////////

int MLTcl::ml_prop( MLProperty* prop, int objc, Tcl_Obj* objv[] )
{
  if( objc < 2 )
    {
    Tcl_WrongNumArgs( interp, 0, objv,
                      MLString("command ?args?").getBytes() );
    return TCL_ERROR;
    }

  char* cmd= Tcl_GetString(objv[1]);

  Tcl_ResetResult(interp);
  if( cmd == 0 )
    {
    Tcl_AppendElement( interp, "error in prop command" );
    return TCL_ERROR;
    }

  if( strcmp(cmd,"name") == 0 )
    {
    MLString n= prop->getName();
    Tcl_SetStringObj( Tcl_GetObjResult(interp),
                      n.getBytes(), -1 );
    return TCL_OK;
    }

  if( strcmp(cmd,"node") == 0 )
    {
    MLNode* node= prop->getNode();
    Tcl_SetStringObj( Tcl_GetObjResult(interp),
                      (node==0) ? "" : createNodeCommand(node).getBytes(),
                      -1 );
    return TCL_OK;
    }

  if( strcmp(cmd,"type") == 0 )
    {
    MLString n= MLPropertyType::getName(prop->getType());

    Tcl_SetStringObj( Tcl_GetObjResult(interp),
                      n.getBytes(), -1 );
    return TCL_OK;
    }

  if( strcmp(cmd,"toString") == 0 )
    {
    MLString s= prop->toString();

    Tcl_SetStringObj( Tcl_GetObjResult(interp),
                      s.getBytes(), -1 );
    return TCL_OK;
    }

  if( strcmp(cmd,"getBoolean") == 0 )
    {
    if( prop->getType() != MLPropertyType::BOOLEAN )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a boolean", 0 );
      return TCL_ERROR;
      }
    Tcl_AppendResult( interp,
                      ((MLPBoolean*)prop)->get() ? "true" : "false", 0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"getInteger") == 0 )
    {
    if( prop->getType() != MLPropertyType::INTEGER )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not an integer", 0 );
      return TCL_ERROR;
      }
    Tcl_AppendElement( interp,
                       MLString::valueOf(((MLPInteger*)prop)->get()).getBytes() );
    return TCL_OK;
    }

  if( strcmp(cmd,"getReal") == 0 )
    {
    if( prop->getType() != MLPropertyType::REAL )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a real", 0 );
      return TCL_ERROR;
      }
    Tcl_AppendElement( interp,
                       MLString::valueOf(((MLPReal*)prop)->get()).getBytes() );
    return TCL_OK;
    }

  if( strcmp(cmd,"getVector") == 0 )
    {
    if( prop->getType() != MLPropertyType::VECTOR )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a vector", 0 );
      return TCL_ERROR;
      }

    Vector v= ((MLPVector*)prop)->get();
    Tcl_AppendResult( interp,
                      MLString::valueOf(v.x()).getBytes(),
                      " ",
                      MLString::valueOf(v.y()).getBytes(),
                      " ",
                      MLString::valueOf(v.z()).getBytes(),
                      0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"getPoint") == 0 )
    {
    if( prop->getType() != MLPropertyType::POINT )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a point", 0 );
      return TCL_ERROR;
      }

    Point v= ((MLPPoint*)prop)->get();
    Tcl_AppendResult( interp,
                      MLString::valueOf(v.x()).getBytes(),
                      " ",
                      MLString::valueOf(v.y()).getBytes(),
                      " ",
                      MLString::valueOf(v.z()).getBytes(),
                      0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"getColor") == 0 )
    {
    if( prop->getType() != MLPropertyType::COLOR )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a color", 0 );
      return TCL_ERROR;
      }

    Color c= ((MLPColor*)prop)->get();
    Tcl_AppendResult( interp,
                      MLString::valueOf(c.r()).getBytes(),
                      " ",
                      MLString::valueOf(c.g()).getBytes(),
                      " ",
                      MLString::valueOf(c.b()).getBytes(),
                      0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"getString") == 0 )
    {
    if( prop->getType() != MLPropertyType::STRING )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a string", 0 );
      return TCL_ERROR;
      }

    Tcl_AppendElement( interp,
                       MLString(((MLPString*)prop)->get()).getBytes() );

    return TCL_OK;
    }

  if( strcmp(cmd,"getNodeRef") == 0 )
    {
    if( prop->getType() != MLPropertyType::NODE_REF )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not a node ref", 0 );
      return TCL_ERROR;
      }

    Tcl_AppendElement( interp,
                       createNodeCommand(((MLPNodeRef*)prop)->get()).getBytes() );

    return TCL_OK;
    }

  if( strcmp(cmd,"getArrayLength") == 0 )
    {
    if( ! prop->isArray() )
      {
      Tcl_AppendResult( interp, "property '", cmd, "' is not an array", 0 );
      return TCL_ERROR;
      }
    Tcl_AppendElement( interp,
                       MLString::valueOf(prop->getArrayLength()).getBytes() );
    return TCL_OK;
    }

  if( strcmp(cmd,"setBoolean") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setBoolean boolean").getBytes() );
      return TCL_ERROR;
      }

    int b;
    if( Tcl_GetBooleanFromObj(interp,objv[2],&b) != TCL_OK )
      {
      Tcl_AppendElement( interp, "error in setBoolean" );
      return TCL_ERROR;
      }
    ((MLPBoolean*)prop)->set(b);
    Tcl_AppendElement( interp, b ? "true" : "false" );
    return TCL_OK;
    }

  if( strcmp(cmd,"setInteger") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setInteger integer").getBytes() );
      return TCL_ERROR;
      }

    int i;
    if( Tcl_GetIntFromObj(interp,objv[2],&i) != TCL_OK )
      {
      Tcl_AppendElement( interp, "error in setInteger" );
      return TCL_ERROR;
      }
    ((MLPInteger*)prop)->set(i);
    Tcl_AppendElement( interp, MLString::valueOf(i).getBytes() );
    return TCL_OK;
    }

  if( strcmp(cmd,"setReal") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setReal real").getBytes() );
      return TCL_ERROR;
      }

    double d;
    if( Tcl_GetDoubleFromObj(interp,objv[2],&d) != TCL_OK )
      {
      Tcl_AppendElement( interp, "error in setReal" );
      return TCL_ERROR;
      }
    ((MLPReal*)prop)->set(d);
    Tcl_AppendElement( interp, MLString::valueOf(d).getBytes() );
    return TCL_OK;
    }

  if( strcmp(cmd,"setString") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setString string").getBytes() );
      return TCL_ERROR;
      }

    char* s= Tcl_GetString(objv[2]);
    ((MLPString*)prop)->set(s);
    Tcl_AppendElement( interp, s );
    return TCL_OK;
    }

  if( strcmp(cmd,"setVector") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setVector [x y z]").getBytes() );
      return TCL_ERROR;
      }

    int length;
    Tcl_Obj** objs;
    if( Tcl_ListObjGetElements( interp, objv[2],
                                &length, &objs ) != TCL_OK ||
        length != 3 )
      {
      Tcl_AppendElement( interp, "error in setVector" );
      return TCL_ERROR;
      }

    double x, y, z;
    if( Tcl_GetDoubleFromObj(interp,objs[0],&x) != TCL_OK ||
        Tcl_GetDoubleFromObj(interp,objs[1],&y) != TCL_OK ||
        Tcl_GetDoubleFromObj(interp,objs[2],&z) != TCL_OK )
      {
      Tcl_AppendElement( interp, "error in setVector" );
      return TCL_ERROR;
      }
    ((MLPVector*)prop)->set(Vector(x,y,z));
    Tcl_AppendResult( interp,
                      MLString::valueOf(x).getBytes(),
                      " ",
                      MLString::valueOf(y).getBytes(),
                      " ",
                      MLString::valueOf(z).getBytes(),
                      0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"setPoint") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setPoint [x y z]").getBytes() );
      return TCL_ERROR;
      }

    int length;
    Tcl_Obj** objs;
    if( Tcl_ListObjGetElements( interp, objv[2],
                                &length, &objs ) != TCL_OK ||
        length != 3 )
      {
      Tcl_AppendElement( interp, "error in setPoint" );
      return TCL_ERROR;
      }

    double x, y, z;
    if( Tcl_GetDoubleFromObj(interp,objs[0],&x) != TCL_OK ||
        Tcl_GetDoubleFromObj(interp,objs[1],&y) != TCL_OK ||
        Tcl_GetDoubleFromObj(interp,objs[2],&z) != TCL_OK )
      {
      Tcl_AppendElement( interp, "error in setPoint" );
      return TCL_ERROR;
      }
    ((MLPPoint*)prop)->set(Point(x,y,z));
    Tcl_AppendResult( interp,
                      MLString::valueOf(x).getBytes(),
                      " ",
                      MLString::valueOf(y).getBytes(),
                      " ",
                      MLString::valueOf(z).getBytes(),
                      0 );
    return TCL_OK;
    }

  if( strcmp(cmd,"setColor") == 0 )
    {
    if( objc < 3 )
      {
      Tcl_WrongNumArgs( interp, 0, objv,
                        MLString("setColor [r g b]").getBytes() );
      return TCL_ERROR;
      }

    int length;
    Tcl_Obj** objs;
    if( Tcl_ListObjGetElements( interp, objv[2],
                                &length, &objs ) != TCL_OK ||
        length != 3 )
      {
      Tcl_AppendElement( interp, "error in setVector" );
      return TCL_ERROR;
      }

    double r, g, b;
    if( Tcl_GetDoubleFromObj(interp,objs[0],&r) != TCL_OK ||
        Tcl_GetDoubleFromObj(interp,objs[1],&g) != TCL_OK ||
        Tcl_GetDoubleFromObj(interp,objs[2],&b) != TCL_OK )
      {
      Tcl_AppendElement( interp, "error in setColor" );
      return TCL_ERROR;
      }
    ((MLPColor*)prop)->set(Color(r,g,b));
    Tcl_AppendResult( interp,
                      MLString::valueOf(r).getBytes(),
                      " ",
                      MLString::valueOf(g).getBytes(),
                      " ",
                      MLString::valueOf(b).getBytes(),
                      0 );
    return TCL_OK;
    }

  Tcl_AppendResult( interp, "unknown property command: ", cmd, 0 );

  return TCL_ERROR;
}

/////////////////////////////////////////////////////////////////////////////
