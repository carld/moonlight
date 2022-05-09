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
  MLLoadGeometry.C

  Stephane Rehel

  April 20 1997
*/

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "tools/MLPath.h"
#include "tools/ListFiles.h"
#include "tools/MLGammaCorrection.h"

#include "graphics/mlgl.h"
#include "graphics/MLEvent.h"
#include "graphics/MLWindow.h"
#include "graphics/SystemWindow.h"

#include "MLLoadGeometry.h"
#include "LoadGeometryDlg.h"
#include "ReadingProgressDlg.h"

#include "koala/VWidget.h"
#include "kw/KWDialog.h"

#include "interface/MLIOProgressDialog.h"

#include "kmodel/ModelModule.h"

#include "scene/file/MLDatabasesList.h"
#include "scene/file/MLDatabase.h"
#include "scene/file/MLQuickReadGeometry.h"
#include "scene/file/MLAbstractMeshIO.h"
#include "scene/MLCamera.h"

/////////////////////////////////////////////////////////////////////////////

static const char* errorString= "*ERROR*";
static int preview_switch= true;

/////////////////////////////////////////////////////////////////////////////

MLLoadGeometry::MLLoadGeometry( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;
  dblist= 0;
  qrg= 0;
  camera= new MLCamera;
  pWindow= 0;
  files= new ListFiles;
}

/////////////////////////////////////////////////////////////////////////////

MLLoadGeometry::~MLLoadGeometry()
{
  delete dblist;
  dblist= 0;

  delete files;
  files= 0;

  delete qrg;
  qrg= 0;

  delete camera;
  camera= 0;

  pWindow= 0;
}

/////////////////////////////////////////////////////////////////////////////

// return the abstract object read
// return 0 if cancelled or error
MLAbstractMesh* MLLoadGeometry::run()
{
  delete dblist;
  dblist= new MLDatabasesList;
  dblist->build();

  delete qrg;
  qrg= 0;

  if( dblist->get().empty() )
    {
/*
    KWError( "Error",
               MLString("No database available!\nCheck: ")
             + MLDatabasesList::getDatabasesResourceFilename() );
*/
    KWError( "Error",
             MLString("No database available.\nCheck your resource file!") );
    return 0;
    }

  if( ! MLGlobalDialog::create(LoadGeometryDlg::data) )
    return 0;

  VWidget* preview= kwDialog->getWidget(LoadGeometryDlg::preview);
  if( preview == 0 )
    pWindow= 0;
   else
    {
    MLWindow* mlWindow= preview->getSystemParent();
    SystemWindow* parent= mlWindow->getSystemWindow();
    IVector pos=  mlWindow->getAbsolutePosition() + preview->getPosition()
                 -parent->getAbsolutePosition();

    IVector size= preview->getSize();

    pWindow= new SystemWindow;
    pWindow->create( parent, pos.x(), pos.y(), size.x(), size.y() );

    pWindow->setEventHandler(this);
    pWindow->map();
    pWindow->postRedraw();
    }

  SIListIterator<MLDatabase> li(dblist->get());
  while( ! li.eol() )
    {
    MLDatabase* db= li++;
    kwDialog->appendListText( LoadGeometryDlg::databases,
                              db->getBasePath() );
    }
  kwDialog->setInteger( LoadGeometryDlg::databases,
                        dblist->getCurrentDatabasePosition());

  MLString dbpath= kwDialog->getText(LoadGeometryDlg::databases);
  currentDatabase= dblist->getDatabase(dbpath);

  updateFilesList();

  kwDialog->setChecked( LoadGeometryDlg::preview_switch, ::preview_switch );
  kwDialog->focus( LoadGeometryDlg::list );

  int outcome= kwDialog->run();

  MLString filename= kwDialog->getText(LoadGeometryDlg::filename);

  ::preview_switch= kwDialog->getChecked( LoadGeometryDlg::preview_switch );

  pWindow= 0;

  if( outcome == KWDialog::CANCEL || currentDatabase == 0 )
    {
    MLGlobalDialog::close();
    return 0;
    }

  filename= currentDatabase->getBasePath() + filename;

  MLAbstractMesh* ao= load(filename);

  MLGlobalDialog::close();

  return ao;
}

/////////////////////////////////////////////////////////////////////////////

MLAbstractMesh* MLLoadGeometry::load( const MLString& filename )
{
  if( currentDatabase == 0 || filename.length() == 0 )
    return 0;

  model->stopLightingProcess();

  MLIOProgressDialog* progress= new MLIOProgressDialog(model);
  progress->create( ReadingProgressDlg::data,
                    ReadingProgressDlg::progress,
                    ReadingProgressDlg::label );
  progress->flush();

  model->pushWaitCursor();

  MLAbstractMesh* ao= MLAbstractMeshIO::read( currentDatabase,
                                                  model->getScene(),
                                                  filename,
                                                  progress );

  delete progress;
  progress= 0;

  model->popWaitCursor();

  if( ao == 0 )
    {
    KWError( "Error",
               MLString("Unable to load:\n")
             + filename + MLString("\n")
             + MLString(strerror(errno)) );
    return 0;
    }

  return ao;
}

/////////////////////////////////////////////////////////////////////////////

void MLLoadGeometry::updateFilesList()
{
  if( kwDialog == 0 )
    return;

  kwDialog->clearList(LoadGeometryDlg::list);

  MLDatabase* database= currentDatabase;
  if( database == 0 )
    return;

  MLDatabasesList::setCurrent( currentDatabase->getBasePath() );

  MLString path= database->getGeomPath();
  MLString pattern= MLString("*") + MLString(MLDatabase::geometryExtension);

  if( files->list(path,pattern,false) != ListFiles::SUCCESS )
    {
    kwDialog->appendListText( LoadGeometryDlg::list, errorString );
    kwDialog->setText(LoadGeometryDlg::filename,"");
    kwDialog->setText(LoadGeometryDlg::size,"");
    kwDialog->setText(LoadGeometryDlg::date,"");
    kwDialog->setText(LoadGeometryDlg::user,"");
    return;
    }

  IListIterator<ListFiles::FileItem> li(files->getFiles());
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    kwDialog->appendListText( LoadGeometryDlg::list, item->name );
    }

  kwDialog->setText(LoadGeometryDlg::filename,"");
  kwDialog->setText(LoadGeometryDlg::size,"");
  kwDialog->setText(LoadGeometryDlg::date,"");
  kwDialog->setText(LoadGeometryDlg::user,"");
}

/////////////////////////////////////////////////////////////////////////////

static ListFiles::FileItem* findListItem( IList<ListFiles::FileItem>& list,
                                          const MLString& filename )
{
  IListIterator<ListFiles::FileItem> li(list);
  while( ! li.eol() )
    {
    ListFiles::FileItem* item= li.next();
    if( item->name == filename )
      return item;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLLoadGeometry::kwCallback( KWEvent& event )
{
  if( event.type == KWEvent::DOUBLE_SELECT &&
      event.widget == LoadGeometryDlg::list )
    {
    kwDialog->quit(KWDialog::VALIDATE);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadGeometryDlg::list )
    {
    MLString filename= kwDialog->getText(LoadGeometryDlg::list);
    ListFiles::FileItem* item= findListItem(files->getFiles(),filename);
    if( item == 0 || currentDatabase == 0 )
      {
      kwDialog->setText(LoadGeometryDlg::filename,"");
      kwDialog->setText(LoadGeometryDlg::size,"");
      kwDialog->setText(LoadGeometryDlg::date,"");
      kwDialog->setText(LoadGeometryDlg::user,"");
      updatePreview();
      return true;
      }

    filename= (MLString(MLDatabase::GEOM_DIR_NAME) + MLPath::separator)
              + filename;

    kwDialog->setText( LoadGeometryDlg::filename,
                       filename );
    kwDialog->setText( LoadGeometryDlg::size,
                       MLString::valueOf(int(item->size)) );
    kwDialog->setText( LoadGeometryDlg::date,
                       item->getDateString() );
    kwDialog->setText( LoadGeometryDlg::user,
                       item->getUser() );

    updatePreview();

    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadGeometryDlg::databases )
    {
    MLString dbpath= kwDialog->getText(LoadGeometryDlg::databases);
    currentDatabase= dblist->getDatabase(dbpath);
    updateFilesList();
    updatePreview();
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == LoadGeometryDlg::preview_switch )
    {
    updatePreview();
    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

void MLLoadGeometry::updatePreview()
{
  if( kwDialog == 0 || pWindow == 0 )
    return;

  delete qrg;
  qrg= 0;

  if( kwDialog->getChecked(LoadGeometryDlg::preview_switch) )
    {
    MLString filename= kwDialog->getText(LoadGeometryDlg::filename);
    filename= currentDatabase->getBasePath() + filename;
    model->pushWaitCursor();
    model->pushWaitCursor();
    qrg= MLQuickReadGeometry::read(filename);
    model->popWaitCursor();
    model->popWaitCursor();
    }

  if( qrg != 0 )
    {
    Point center= qrg->bbox.getCenter();
    double radius= qrg->bbox.getVolume() / 2.;
    radius *= 2.; //1.5;

    camera->lookat= center;
    camera->angle= DEGtoRAD(50);
    camera->initByLookat( Point(1,1,1), center, Vector(0,0,1) );
    camera->initSpheric( DEGtoRAD(-40), DEGtoRAD(30), radius );

    MLString ps= MLString::printf( "%d vertices, %d polygons",
                                   qrg->nVertices, qrg->nPolygons );
    kwDialog->setText( LoadGeometryDlg::preview_stats, ps );
    }
   else
    {
    kwDialog->setText( LoadGeometryDlg::preview_stats, "" );
    }

  pWindow->postRedraw();
}

/////////////////////////////////////////////////////////////////////////////

void MLLoadGeometry::drawPreview()
{
  if( kwDialog == 0 || pWindow == 0 )
    return;

  if( qrg == 0 )
    {
    pWindow->currentPixel();
    IVector size= pWindow->getSize();
    mlFront();
    mlLightGray();
    mlRect( 0, 0, size.x()-1, size.y()-1 );
//    pWindow->getParent()->currentPixel();
    }
   else
    {
    pWindow->current();

    mlBack();

    MLGammaCorrection* gc= model->getGammaCorrection();
    double clearColor= gc->do_f(128.5/255.);
    glClearColor(clearColor,clearColor,clearColor,0);
    glClear( GL_COLOR_BUFFER_BIT );

    glDisable( GL_DEPTH_TEST );
    glDepthMask(GL_FALSE);
    glDisable( GL_LIGHTING );
    glShadeModel(GL_FLAT);

    IVector size= pWindow->getSize();
    camera->x_res= size.x();
    camera->y_res= size.y();
    camera->compute();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixd(camera->projection.dump());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mlBlack();
    glBegin(GL_LINES);

    MLQuickReadGeometry::Edge* edges= qrg->edges;
    int nEdges= qrg->nEdges;
    Point* vertices= qrg->vertices;
    for( int i= 1; i <= nEdges; ++i, ++edges )
      {
      if( edges->v1 == 0 )
        continue;
      Point& p1= vertices[edges->v1];
      Point& p2= vertices[edges->v2];
      glVertex3f( p1.x(), p1.y(), p1.z() );
      glVertex3f( p2.x(), p2.y(), p2.z() );
      }
    glEnd();

    pWindow->swapBuffers();
    }

}

/////////////////////////////////////////////////////////////////////////////

boolean MLLoadGeometry::eventHandler( MLWindow* window, MLEvent& event )
{
  if( window != pWindow )
    return false;

  if( event.type == MLEvent::REDRAW )
    {
    drawPreview();
    return true;
    }

  if( event.type == MLEvent::MOUSE_MOTION &&
      (event.state & MLEvent::LEFT_BUTTON) != 0 )
    {
    IVector size= window->getSize();
    double ms= double(max( size.x(), size.y() ));
    double dx= double(event.size.x()) / ms * M_PI;
    double dy= double(event.size.y()) / ms * M_PI;

    camera->initSpheric( camera->phi - dx,
                         camera->theta + dy,
                         (camera->location - camera->lookat).norm() );
    window->postRedraw();
    return true;
    }

  if( event.type == MLEvent::MOUSE_MOTION &&
      (event.state & MLEvent::MIDDLE_BUTTON) != 0 )
    {
    double radius= (camera->location - camera->lookat).norm();
    IVector size= window->getSize();
    double ms= double(max( size.x(), size.y() ));
    double delta= double(-event.size.y()) / ms;
    radius += radius*delta;
    if( radius < 1e-20 ) radius= 1e-20;

    camera->initSpheric( camera->phi, camera->theta, radius );

    window->postRedraw();
    return true;
    }

  if( event.type == MLEvent::MOUSE_MOTION &&
      (event.state & MLEvent::RIGHT_BUTTON) != 0 )
    {
    IVector size= window->getSize();
    double dx= double(event.size.x()) / double(size.x()) * 2.;
    double dy= double(event.size.y()) / double(size.y()) * 2.;

    const Point& fp= camera->lookat;
    double linear_speed= (fp - camera->location).norm()
                                          * tan(camera->angle*0.5);
    double d_right= dx * linear_speed;
    double d_up=   -dy * linear_speed;

    Vector translate= (camera->right * d_right) + (camera->up * d_up);

    camera->initByLookat( camera->location - translate,
                          camera->lookat - translate,
                          camera->sky );

    window->postRedraw();
    return true;
    }

  return false;
}

/////////////////////////////////////////////////////////////////////////////
