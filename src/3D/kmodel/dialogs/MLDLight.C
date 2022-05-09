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
  MLDLight.C

  Stephane Rehel

  March 5 1997
*/

#include <math.h>

#include "tools/Sorts.h"

#include "koala/VWidget.h"
#include "graphics/mlgl.h"

#include "kmodel/ModelModule.h"
#include "kmodel/ModelCommand.h"
#include "kmodel/ModelCommands.h"

#include "kmodel/commands/MLCEditLight.h"

#include "MLDLight.h"
#include "LightDlg.h"

#include "interface/MLColorWidget.h"

#include "kw/KWDialog.h"
#include "scene/MLObject.h"
#include "scene/lighting/MLLight.h"
#include "scene/lighting/MLLightPool.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/lighting/MLLightTable.h"
#include "scene/lighting/MLGlobalLighting.h"

#include "scene/MLScene.h"

/////////////////////////////////////////////////////////////////////////////

MLDLight::MLDLight( ModelModule* _model ):
  MLScrollingDialog(_model)
{
  model= _model;
  data= LightDlg::data;
  name= "Light";

  MLScrollingDialog::updateFlags= ModelModule::REFRESH_DIALOGS   |
                                  ModelModule::REFRESH_NAMES     |
                                  ModelModule::REFRESH_SELECTION |
                                  ModelModule::REFRESH_GEOMETRY;
  colorWidget= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLDLight::~MLDLight()
{
  delete colorWidget;
  colorWidget= 0;
}

/////////////////////////////////////////////////////////////////////////////

MLLight* MLDLight::getSelectedLight()
{
  MLObject* o= model->getScene()->getObject(model->getSingleSelected());

  if( o == 0 )
    return 0;

  if( o->getObjectType() != MLObject::LIGHT )
    return 0;

  return (MLLight*) o;
}

/////////////////////////////////////////////////////////////////////////////

void MLDLight::update()
{
  if( kwDialog == 0 || colorWidget == 0 )
    return;

  MLLight* light= getSelectedLight();

  boolean avail= (light!=0);

  if( ! avail )
    {
    kwDialog->setText(LightDlg::name,"");
    kwDialog->clearList(LightDlg::pool);

    updateList();

    updateAvailabilities();
    return;
    }

  kwDialog->setText( LightDlg::name, light->getName().name );
  kwDialog->setAvailable(LightDlg::apply,true);
  kwDialog->setAvailable(LightDlg::update,true);

  colorWidget->set( light->color );
  if( light->color_model == MLLight::HSV )
    colorWidget->setHSVColorModel();
   else
    colorWidget->setRGBColorModel();
  kwDialog->setChecked(LightDlg::direct_shading,light->directLight);
  kwDialog->setValue(LightDlg::d_factor,light->direct_intensity);
  kwDialog->setChecked(LightDlg::global_contributor,light->globalLight);
  kwDialog->setValue(LightDlg::g_factor,light->global_intensity);

  kwDialog->setChecked(LightDlg::extent_check,light->has_extent);
  kwDialog->setValue(LightDlg::extent,light->extent);

  kwDialog->setChecked(LightDlg::generate_shadows,light->generate_shadows);

  if( light->radiosity_attenuation )
    kwDialog->setChecked( LightDlg::radiosity_attenuation );
   else
    kwDialog->setChecked( LightDlg::custom_attenuation );

  kwDialog->setChecked( LightDlg::att_check_constant,
                        light->att_constant.yes);
  kwDialog->setValue( LightDlg::att_constant, light->att_constant.value );
  kwDialog->setChecked( LightDlg::att_check_linear,
                        light->att_linear.yes);
  kwDialog->setValue( LightDlg::att_linear, light->att_linear.value );
  kwDialog->setChecked( LightDlg::att_check_quadratic,
                        light->att_quadratic.yes);
  kwDialog->setValue( LightDlg::att_quadratic, light->att_quadratic.value );

  kwDialog->setChecked( LightDlg::rd_point,
                        light->getLightType() == MLLight::POINT );
  kwDialog->setChecked( LightDlg::rd_directional,
                        light->getLightType() == MLLight::DIRECTIONAL );
  kwDialog->setChecked( LightDlg::rd_spotlight,
                        light->getLightType() == MLLight::SPOT );

  kwDialog->setValue( LightDlg::cutoff, RADtoDEG(light->spot_cutoff) );
  kwDialog->setValue( LightDlg::exponent, light->spot_exponent );
  kwDialog->repaintArea(LightDlg::coneArea);

  // update pools list
  kwDialog->clearList(LightDlg::pool);
  MLPoolTable* poolTable= model->getScene()->getPoolTable();
  int i;
  for( i= 1; i <= poolTable->getNPools(); ++i )
    {
    MLLightPool* pool= poolTable->getPool(i);

    MLString name= pool->getName()
                  + " [" + MLString::valueOf(pool->getNLights()) + "]";

    kwDialog->appendListText(LightDlg::pool,name);
    }

  kwDialog->setInteger( LightDlg::pool, light->getPoolIndex() );

  updateList();

  kwDialog->setText(LightDlg::name,light->getName().name);
  kwDialog->setText(LightDlg::list,light->getName().name);

  updateAvailabilities();
}

/////////////////////////////////////////////////////////////////////////////

void MLDLight::updateAvailabilities()
{
  MLLight* light= getSelectedLight();

  boolean avail= (light!=0);

  kwDialog->setAvailable(LightDlg::apply,avail);
  kwDialog->setAvailable(LightDlg::update,avail);

  kwDialog->setAvailable(LightDlg::name,avail);
  kwDialog->setAvailable(LightDlg::pool,avail);

  boolean ok;
  colorWidget->setAvailable(avail);

  boolean direct= kwDialog->getChecked(LightDlg::direct_shading);
  boolean global= kwDialog->getChecked(LightDlg::global_contributor);

  kwDialog->setAvailable(LightDlg::direct_shading,avail);
  ok= avail;
  if( light != 0 )
    ok= direct;
  kwDialog->setAvailable(LightDlg::d_factor,ok);

  kwDialog->setAvailable(LightDlg::global_contributor,avail);
  ok= avail;
  if( light != 0 )
    ok= global;
  kwDialog->setAvailable(LightDlg::g_factor,ok);

  KWidget type_w= kwDialog->getRadioChecked(LightDlg::rd_point);
  ok= avail;
  if( type_w == LightDlg::rd_directional )
    ok= false;
  kwDialog->setAvailable(LightDlg::extent_check,ok);
  kwDialog->setAvailable(LightDlg::extent,ok);

  kwDialog->setAvailable(LightDlg::generate_shadows,avail);

  kwDialog->setAvailable(LightDlg::rd_point,avail);
  kwDialog->setAvailable(LightDlg::rd_directional,avail);
  kwDialog->setAvailable(LightDlg::rd_spotlight,avail);

  ok= avail;
  if( light != 0 )
    ok= direct;
  if( ok )
    if( type_w == LightDlg::rd_directional )
      ok= false;
  kwDialog->setAvailable(LightDlg::radiosity_attenuation,ok);
  kwDialog->setAvailable(LightDlg::custom_attenuation,ok);
  if( kwDialog->getRadioChecked(LightDlg::radiosity_attenuation)
                             == LightDlg::radiosity_attenuation )
    ok= false;

  kwDialog->setAvailable(LightDlg::att_check_constant,ok);
  kwDialog->setAvailable(LightDlg::att_constant,ok);
  kwDialog->setAvailable(LightDlg::att_check_linear,ok);
  kwDialog->setAvailable(LightDlg::att_linear,ok);
  kwDialog->setAvailable(LightDlg::att_check_quadratic,ok);
  kwDialog->setAvailable(LightDlg::att_quadratic,ok);

  ok= avail;
  if( type_w != LightDlg::rd_spotlight )
    ok= false;
  kwDialog->setAvailable(LightDlg::cutoff,ok);
  kwDialog->setAvailable(LightDlg::exponent,ok);
  kwDialog->setAvailable(LightDlg::adjust_cutoff,ok);
  kwDialog->setAvailable(LightDlg::coneArea,avail);

//  kwDialog->setAvailable(LightDlg::list,avail);
//  kwDialog->setAvailable(LightDlg::name_edit,avail);

  kwDialog->setAvailable(LightDlg::rename,avail);

  ok= avail;
  if( ok )
    ok= global;
  kwDialog->setAvailable(LightDlg::update,ok);
}

/////////////////////////////////////////////////////////////////////////////

void MLDLight::updateList()
{
  int i;

  // Now, build the lights list
  MLLightTable* lightTable= model->getScene()->getLightTable();
  MLString* lightsNames= new MLString [ lightTable->getNLights() + 1 ];
  int j= 0;
  for( i= 1; i <= lightTable->getNLights(); ++i )
    {
    MLLight* l= lightTable->getLight(i);
    if( l == 0 )
      continue;
    lightsNames[++j]= l->getName().name;
    }

  if( j == 0 )
    {
    if( kwDialog->getListLength(LightDlg::list) > 0 )
      kwDialog->clearList(LightDlg::list);
    delete [] lightsNames;
    return;
    }

  Sorts<MLString> sort;
  sort.quickSort( lightsNames + 1, j );
  // check if we need to update the list
  boolean update_list= kwDialog->getListLength(LightDlg::list) != j;

  if( ! update_list )
    {
    for( i= 1; i <= j; ++i )
      {
      if( lightsNames[i] != kwDialog->getListText(LightDlg::list,i) )
        {
        update_list= true;
        break;
        }
      }
    }

  if( update_list )
    {
    // ok, update lights names list
    kwDialog->clearList(LightDlg::list);
    for( i= 1; i <= j; ++i )
      kwDialog->appendListText(LightDlg::list,lightsNames[i]);
    kwDialog->setText(LightDlg::name_edit,"");
    }

  delete [] lightsNames;
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDLight::kwCallback( KWEvent& event )
{
  if( colorWidget != 0 )
    {
    if( colorWidget->colorCallback(event) )
      return true;
    }

  switch( event.type )
    {
    case KWEvent::SELECT:
      {
      if( event.widget == LightDlg::radiosity_attenuation ||
          event.widget == LightDlg::custom_attenuation )
        {
        updateAvailabilities();
        return true;
        }

      if( event.widget == LightDlg::adjust_cutoff )
        {
        double exponent= kwDialog->getValue(LightDlg::exponent);
        if( exponent > 1e-10 )
          {
          double epsilon= 1e-3; // precision = 1 to 1000
          double cutoff= acos( pow( epsilon, 1. / exponent ) );
          kwDialog->setValue(LightDlg::cutoff,RADtoDEG(cutoff));
          kwDialog->repaintArea(LightDlg::coneArea);
          }
        return true;
        }

      if( event.widget == LightDlg::direct_shading )
        {
        updateAvailabilities();
        boolean yes= kwDialog->getChecked(LightDlg::direct_shading);
        kwDialog->setAvailable(LightDlg::d_factor,yes);
        if( yes )
          kwDialog->focus(LightDlg::d_factor);
        return true;
        }

      if( event.widget == LightDlg::global_contributor )
        {
        updateAvailabilities();
        boolean yes= kwDialog->getChecked(LightDlg::global_contributor);
        kwDialog->setAvailable(LightDlg::g_factor,yes);
        if( yes )
          kwDialog->focus(LightDlg::g_factor);
        return true;
        }

      if( event.widget == LightDlg::rd_point ||
          event.widget == LightDlg::rd_directional ||
          event.widget == LightDlg::rd_spotlight )
        {
        updateAvailabilities();
        kwDialog->repaintArea(LightDlg::coneArea);
        return true;
        }

      if( event.widget == LightDlg::update )
        {
        MLLight* light= getSelectedLight();

        if( light != 0 )
          {
          model->stopLightingProcess();
          light->getScene()->getGlobalLighting()->update_light(light);
          model->runLightingProcess();
          }

        model->refreshScreen(ModelModule::REFRESH_DIALOGS);
        return true;
        }

      if( event.widget == LightDlg::rename )
        {
        MLString name= kwDialog->getText(LightDlg::name_edit).trim();
        if( name.length() == 0 )
          break;
        const MLString& sname= kwDialog->getText(LightDlg::list);
//*** NOT CORRECT!!
MLName nn; nn.name= sname;
        MLLight* light= model->getScene()->getLight(nn);
        if( light == 0 )
          break;
        if( name == sname )
          break;
//*** NOT CORRECT!!
nn.name= name;
        if( model->getScene()->getLight(nn) != 0 )
          break; // already exists

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::RENAME_OBJECT;
        cmd->index= light->getID();
        cmd->string= name;

        model->post(cmd);

        return true;
        }

      if( event.widget == LightDlg::list )
        {
        const MLString& name= kwDialog->getText(LightDlg::list);

//*** NOT CORRECT!!
MLName nn; nn.name= name;
        MLLight* light= model->getScene()->getLight(nn);
        if( light == 0 )
          break; // unable to find it !??

        kwDialog->setText(LightDlg::name_edit,name);
        kwDialog->focus(LightDlg::name_edit);

        // select this light
        if( getSelectedLight() == light )
          break; // already selected

        ModelCommand* cmd= new ModelCommand;
        cmd->type= ModelCommands::SET_SELECTED;
        cmd->indices.alloc(1);
        cmd->indices[0]= light->getID();

        model->post(cmd);

        return true;
        }

      break;
      }

    case KWEvent::VALIDATE:
      {
      apply_button();
      break;
      }

    case KWEvent::DRAW:
      {
      if( event.widget == LightDlg::coneArea )
        {
        drawLightCone();
        return true;
        }
      break;
      }

    case KWEvent::VALUE:
      {
      if( event.widget == LightDlg::cutoff ||
          event.widget == LightDlg::exponent )
        {
/*
        double cone= kwDialog->getValue(LightDlg::cone);
        double spread= kwDialog->getValue(LightDlg::spread);

        if( spread > (360.-cone) / 2. + 1e-6 )
          {
          spread= (360.-cone) / 2.;
          kwDialog->setValue(LightDlg::spread,spread);
          return true;
          }
*/
        kwDialog->repaintArea(LightDlg::coneArea);
        return true;
        }
      break;
      }

    default:
      return MLScrollingDialog::kwCallback(event);
    }

  return true;
}

/////////////////////////////////////////////////////////////////////////////

void MLDLight::justCreated()
{
  delete colorWidget;
  colorWidget= new MLColorWidget( kwDialog,
                                  LightDlg::label_r,
                                  LightDlg::label_g,
                                  LightDlg::label_b,
                                  LightDlg::value_r,
                                  LightDlg::value_g,
                                  LightDlg::value_b,
                                  LightDlg::radio_rgb,
                                  LightDlg::radio_hsv,
                                  LightDlg::colorArea );
}

/////////////////////////////////////////////////////////////////////////////

void MLDLight::drawLightCone()
{
  if( kwDialog == 0 )
    return;

  VWidget* w= kwDialog->getWidget(LightDlg::coneArea);
  if( w == 0 )
    return;

  double cutoff= DEGtoRAD(kwDialog->getValue(LightDlg::cutoff));
  double exponent= kwDialog->getValue(LightDlg::exponent);

  if( ! w->beginDraw() )
    return;

  IVector size= w->getSize();
  IPoint center= IPoint(0,0) + size/2;

  mlBlack();
  mlRect( 0, 0, size.x()-1, size.y()-1 );

  if( kwDialog->getRadioChecked(LightDlg::rd_point) !=
                                                    LightDlg::rd_spotlight )
    {
    w->endDraw();
    return;
    }

  int radius= min( size.x(), size.y() ) / 2 -2;
  int n= 32; // n points on half disk
  struct DiskPoint
    {
    int x, y;
    Color color;
    };
  DiskPoint* points= new DiskPoint [ n ];
  Color color= (colorWidget==0) ? Color(0,0,0) : colorWidget->get();

  int i;
  for( i= 0; i < n; ++i )
    {
    double angle= double(i) / double(n-1) * M_PI;
    double cos_a= cos(angle);
    double sin_a= sin(angle);
    points[i].x= int( double(radius) * sin_a + .5 );
    points[i].y= center.y() - int( double(radius) * cos_a + .5 );

    if( angle > cutoff )
      {
      points[i].color= Color(0,0,0);
      continue;
      }

    points[i].color= color * pow( cos_a, exponent );
    }

  glShadeModel(GL_SMOOTH);
  glBegin(GL_TRIANGLES);
  for( i= 0; i < n-1; ++i )
    {
    DiskPoint& p1= points[i];
    DiskPoint& p2= points[i+1];
    Color c1= p1.color;
    Color c2= p2.color;
    Color c0= (c1+c2) * 0.5;

    mlColor(c0);
    glVertex2i( center.x(), center.y() );
    mlColor(c1);
    glVertex2i( center.x()+p1.x, p1.y );
    mlColor(c2);
    glVertex2i( center.x()+p2.x, p2.y );

    mlColor(c0);
    glVertex2i( center.x(), center.y() );
    mlColor(c1);
    glVertex2i( center.x()-p1.x, p1.y );
    mlColor(c2);
    glVertex2i( center.x()-p2.x, p2.y );
    }
  glEnd();

  mlColor(color);
//  glLineStipple( 1, 0x5555 );
//  glEnable( GL_LINE_STIPPLE );

  int max_x= int( double(radius) * sin(cutoff) + .5 );
  int max_y= center.y() - int( double(radius) * cos(cutoff) + .5 );

  glBegin(GL_LINES);
    glVertex2i( center.x(), center.y() );
    glVertex2i( center.x() + max_x, max_y );
    glVertex2i( center.x(), center.y() );
    glVertex2i( center.x() - max_x, max_y );
  glEnd();
//  glDisable( GL_LINE_STIPPLE );

  delete points;

  w->endDraw();
}

/////////////////////////////////////////////////////////////////////////////

// protected
void MLDLight::apply_button()
{
  MLLight* light= getSelectedLight();

  if( light == 0 || kwDialog == 0 || colorWidget == 0 )
    return;

  int color_model= colorWidget->inRGBColorModel() ? MLLight::RGB
                                                  : MLLight::HSV;
  Color color= colorWidget->get();
  boolean  directLight= kwDialog->getChecked(LightDlg::direct_shading);
  double d_factor=    kwDialog->getValue(LightDlg::d_factor);
  boolean  globalLight= kwDialog->getChecked(LightDlg::global_contributor);
  double g_factor=    kwDialog->getValue(LightDlg::g_factor);

  boolean  has_extent= kwDialog->getChecked(LightDlg::extent_check);
  double extent    = kwDialog->getValue(LightDlg::extent);

  boolean  generate_shadows= kwDialog->getChecked(LightDlg::generate_shadows);

  boolean  rad_att= kwDialog->getRadioChecked(LightDlg::radiosity_attenuation)
                  == LightDlg::radiosity_attenuation;

  boolean  att_constant_yes= kwDialog->getChecked(LightDlg::att_check_constant);
  double att_constant_val= kwDialog->getValue(LightDlg::att_constant);
  boolean  att_linear_yes= kwDialog->getChecked(LightDlg::att_check_linear);
  double att_linear_val= kwDialog->getValue(LightDlg::att_linear);
  boolean  att_quadratic_yes= kwDialog->getChecked(LightDlg::att_check_quadratic);
  double att_quadratic_val= kwDialog->getValue(LightDlg::att_quadratic);

  double spot_cutoff= DEGtoRAD(kwDialog->getValue(LightDlg::cutoff));
  double spot_exponent= kwDialog->getValue(LightDlg::exponent);

  KWidget wltype= kwDialog->getRadioChecked(LightDlg::rd_point);
  MLLight::LightType lt= (wltype == LightDlg::rd_spotlight)
                         ? MLLight::SPOT :
                         (wltype == LightDlg::rd_directional)
                         ? MLLight::DIRECTIONAL :
                           MLLight::POINT;

  int poolIndex= kwDialog->getInteger(LightDlg::pool);
  MLLightPool* pool= model->getScene()->getPoolTable()->getPool(poolIndex);
  if( pool == 0 )
    return; // unknown pool

  boolean c_type= (lt != light->getLightType());
  boolean c_global= (globalLight?1:0) != (light->globalLight?1:0) ;
  boolean c_direct= (directLight?1:0) != (light->directLight?1:0) ;
  boolean c_parameters= fabs(spot_cutoff - light->spot_cutoff) > DEGtoRAD(0.01) ||
                      fabs(spot_exponent-light->spot_exponent) > 1e-4 ||
                      ((has_extent?1:0) != (light->has_extent?1:0)) ||
                      fabs(extent - light->extent) > 0.001 ||
                      ((generate_shadows?1:0) != (light->generate_shadows?1:0)) ||
                      fabs(d_factor - light->direct_intensity) > 1e-5;
  boolean c_att= ((rad_att?1:0) != (light->radiosity_attenuation?1:0)) ||
               ((att_constant_yes?1:0) != (light->att_constant.yes?1:0)) ||
               ( att_constant_val      !=  light->att_constant.value) ||
               ((att_linear_yes?1:0) != (light->att_linear.yes?1:0)) ||
               ( att_linear_val      !=  light->att_linear.value) ||
               ((att_quadratic_yes?1:0) != (light->att_quadratic.yes?1:0)) ||
               ( att_quadratic_val      !=  light->att_quadratic.value) ;
  boolean c_energy= fabs(g_factor - light->global_intensity) > 1e-6 ||
                  fabs((color - light->color).getGray()) > 0.005;

  if( pool == light->getPool() &&
      !c_type &&
      !c_global && !c_direct &&
      !c_parameters && !c_att && !c_energy )
     return; // no change

  MLCEditLight* cmd= new MLCEditLight;
  MLCEditLight* undo= new MLCEditLight;

  cmd->createUndo(undo);

  cmd->lightIndex= light->getLightIndex();
  cmd->light= light;
  cmd->poolIndex= poolIndex;
  cmd->pool= pool;

  cmd->lightType= lt;
  cmd->directLight= directLight;
  cmd->globalLight= globalLight;

  cmd->color_model= color_model;
  cmd->color= color;
  cmd->direct_intensity= d_factor;
  cmd->global_intensity= g_factor;
  cmd->has_extent= has_extent;
  cmd->extent= extent;
  cmd->generate_shadows= generate_shadows;

  cmd->spot_cutoff= spot_cutoff;
  cmd->spot_exponent= spot_exponent;

  cmd->radiosity_attenuation= rad_att;
  cmd->att_constant.yes  = att_constant_yes;
  cmd->att_constant.value= att_constant_val;
  cmd->att_linear.yes  = att_linear_yes;
  cmd->att_linear.value= att_linear_val;
  cmd->att_quadratic.yes  = att_quadratic_yes;
  cmd->att_quadratic.value= att_quadratic_val;

  undo->lightIndex= light->getLightIndex();
  undo->light= light;
  undo->poolIndex= light->getPoolIndex();
  undo->pool= light->getPool();

  undo->lightType= int(light->getLightType());
  undo->directLight= light->directLight;
  undo->globalLight= light->globalLight;
  undo->color_model= light->color_model;
  undo->color= light->color;
  undo->direct_intensity= light->direct_intensity;
  undo->global_intensity= light->global_intensity;
  undo->has_extent= light->has_extent;
  undo->extent= light->extent;
  undo->generate_shadows= light->generate_shadows;
  undo->spot_cutoff= light->spot_cutoff;
  undo->spot_exponent= light->spot_exponent;
  undo->radiosity_attenuation= light->radiosity_attenuation;
  undo->att_constant.yes=   light->att_constant.yes;
  undo->att_constant.value= light->att_constant.value;
  undo->att_linear.yes=   light->att_linear.yes;
  undo->att_linear.value= light->att_linear.value;
  undo->att_quadratic.yes=   light->att_quadratic.yes;
  undo->att_quadratic.value= light->att_quadratic.value;

  model->post(cmd);
}

/////////////////////////////////////////////////////////////////////////////
