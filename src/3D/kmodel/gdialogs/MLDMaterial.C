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
  MLDMaterial.C

  Stephane Rehel

  August 27 1997
*/

#include <stdio.h>

#include "tools/MLPath.h"

#include "kw/KWDialog.h"

#include "scene/MLScene.h"
#include "scene/MLObject.h"
#include "scene/MLMesh.h"

#include "scene/file/MLDatabase.h"

#include "scene/material/MLMaterial.h"
#include "scene/material/MLMaterialsIterator.h"

#include "interface/MLColorWidget.h"

#include "kmodel/commands/MLCSetMaterial.h"
#include "kmodel/gdialogs/file/MLDLoadMaterial.h"

#include "kmodel/ModelModule.h"

#include "MLDMaterial.h"
#include "MaterialDlg.h"

/////////////////////////////////////////////////////////////////////////////

struct ComponentWidget
  {
  unsigned int card;
  unsigned int label_r;
  unsigned int label_g;
  unsigned int label_b;
  unsigned int valuesBar_r;
  unsigned int valuesBar_g;
  unsigned int valuesBar_b;
  unsigned int radio_rgb;
  unsigned int radio_hsv;
  unsigned int area;
  unsigned int factor;
  MLColorWidget* colorWidget;

  unsigned int use_texture;
  unsigned int texture_name;
  unsigned int browse_texture;
  unsigned int view_texture;
  unsigned int use_alpha;
  unsigned int xoffset;
  unsigned int yoffset;
  unsigned int xscale;
  unsigned int yscale;

  void setAvailable( KWDialog* dialog, boolean yes );
  void setTextureWidgetsAvailable( KWDialog* dialog, boolean yes );
  };

/////////////////////////////////////////////////////////////////////////////

void ComponentWidget::setAvailable( KWDialog* dialog, boolean yes )
{
  if( dialog == 0 )
    return;
  if( colorWidget == 0 )
    return;

  dialog->setAvailable(card,yes);

  colorWidget->setAvailable(yes);

  setTextureWidgetsAvailable(dialog,yes);
}

/////////////////////////////////////////////////////////////////////////////

void ComponentWidget::setTextureWidgetsAvailable( KWDialog* dialog,
                                                  boolean yes )
{
  if( dialog == 0 )
    return;

  dialog->setAvailable( texture_name,   yes );
  dialog->setAvailable( browse_texture, yes );
  dialog->setAvailable( view_texture,   yes );
  dialog->setAvailable( use_alpha,      yes );
  dialog->setAvailable( xoffset,        yes );
  dialog->setAvailable( yoffset,        yes );
  dialog->setAvailable( xscale,         yes );
  dialog->setAvailable( yscale,         yes );
}

/////////////////////////////////////////////////////////////////////////////

static const int nComponentWidgets= 5;
static ComponentWidget componentWidgets[nComponentWidgets];

enum {
     EMITTANCE= 0,
     DIFFUSE,
     SPECULAR,
     REFLECTION,
     TRANSMISSION
     };

/////////////////////////////////////////////////////////////////////////////

MLDMaterial::MLDMaterial( ModelModule* _model ):
  MLGlobalDialog(_model)
{
  model= _model;

  componentWidgets[0].card=        MaterialDlg::emittance;
  componentWidgets[0].label_r=     MaterialDlg::e_r_label;
  componentWidgets[0].label_g=     MaterialDlg::e_g_label;
  componentWidgets[0].label_b=     MaterialDlg::e_b_label;
  componentWidgets[0].valuesBar_r= MaterialDlg::e_r;
  componentWidgets[0].valuesBar_g= MaterialDlg::e_g;
  componentWidgets[0].valuesBar_b= MaterialDlg::e_b;
  componentWidgets[0].radio_rgb=   MaterialDlg::e_rgb;
  componentWidgets[0].radio_hsv=   MaterialDlg::e_hsv;
  componentWidgets[0].area=        MaterialDlg::e_area;
  componentWidgets[0].factor=      MaterialDlg::e_color_factor;
  componentWidgets[0].colorWidget= 0;
  componentWidgets[0].use_texture=    MaterialDlg::e_use_texture;
  componentWidgets[0].texture_name=   MaterialDlg::e_texture_name;
  componentWidgets[0].browse_texture= MaterialDlg::e_browse_texture;
  componentWidgets[0].view_texture=   MaterialDlg::e_view_texture;
  componentWidgets[0].use_alpha=      MaterialDlg::e_use_alpha;
  componentWidgets[0].xoffset=        MaterialDlg::e_xoffset;
  componentWidgets[0].yoffset=        MaterialDlg::e_yoffset;
  componentWidgets[0].xscale=         MaterialDlg::e_xscale;
  componentWidgets[0].yscale=         MaterialDlg::e_yscale;

  componentWidgets[1].card=        MaterialDlg::diffuse;
  componentWidgets[1].label_r=     MaterialDlg::d_r_label;
  componentWidgets[1].label_g=     MaterialDlg::d_g_label;
  componentWidgets[1].label_b=     MaterialDlg::d_b_label;
  componentWidgets[1].valuesBar_r= MaterialDlg::d_r;
  componentWidgets[1].valuesBar_g= MaterialDlg::d_g;
  componentWidgets[1].valuesBar_b= MaterialDlg::d_b;
  componentWidgets[1].radio_rgb=   MaterialDlg::d_rgb;
  componentWidgets[1].radio_hsv=   MaterialDlg::d_hsv;
  componentWidgets[1].area=        MaterialDlg::d_area;
  componentWidgets[1].factor=      MaterialDlg::d_color_factor;
  componentWidgets[1].colorWidget= 0;
  componentWidgets[1].use_texture=    MaterialDlg::d_use_texture;
  componentWidgets[1].texture_name=   MaterialDlg::d_texture_name;
  componentWidgets[1].browse_texture= MaterialDlg::d_browse_texture;
  componentWidgets[1].view_texture=   MaterialDlg::d_view_texture;
  componentWidgets[1].use_alpha=      MaterialDlg::d_use_alpha;
  componentWidgets[1].xoffset=        MaterialDlg::d_xoffset;
  componentWidgets[1].yoffset=        MaterialDlg::d_yoffset;
  componentWidgets[1].xscale=         MaterialDlg::d_xscale;
  componentWidgets[1].yscale=         MaterialDlg::d_yscale;

  componentWidgets[2].card=        MaterialDlg::specular;
  componentWidgets[2].label_r=     MaterialDlg::s_r_label;
  componentWidgets[2].label_g=     MaterialDlg::s_g_label;
  componentWidgets[2].label_b=     MaterialDlg::s_b_label;
  componentWidgets[2].valuesBar_r= MaterialDlg::s_r;
  componentWidgets[2].valuesBar_g= MaterialDlg::s_g;
  componentWidgets[2].valuesBar_b= MaterialDlg::s_b;
  componentWidgets[2].radio_rgb=   MaterialDlg::s_rgb;
  componentWidgets[2].radio_hsv=   MaterialDlg::s_hsv;
  componentWidgets[2].area=        MaterialDlg::s_area;
  componentWidgets[2].factor=      MaterialDlg::s_color_factor;
  componentWidgets[2].colorWidget= 0;
  componentWidgets[2].use_texture=    MaterialDlg::s_use_texture;
  componentWidgets[2].texture_name=   MaterialDlg::s_texture_name;
  componentWidgets[2].browse_texture= MaterialDlg::s_browse_texture;
  componentWidgets[2].view_texture=   MaterialDlg::s_view_texture;
  componentWidgets[2].use_alpha=      MaterialDlg::s_use_alpha;
  componentWidgets[2].xoffset=        MaterialDlg::s_xoffset;
  componentWidgets[2].yoffset=        MaterialDlg::s_yoffset;
  componentWidgets[2].xscale=         MaterialDlg::s_xscale;
  componentWidgets[2].yscale=         MaterialDlg::s_yscale;

  componentWidgets[3].card=        MaterialDlg::reflection;
  componentWidgets[3].label_r=     MaterialDlg::r_r_label;
  componentWidgets[3].label_g=     MaterialDlg::r_g_label;
  componentWidgets[3].label_b=     MaterialDlg::r_b_label;
  componentWidgets[3].valuesBar_r= MaterialDlg::r_r;
  componentWidgets[3].valuesBar_g= MaterialDlg::r_g;
  componentWidgets[3].valuesBar_b= MaterialDlg::r_b;
  componentWidgets[3].radio_rgb=   MaterialDlg::r_rgb;
  componentWidgets[3].radio_hsv=   MaterialDlg::r_hsv;
  componentWidgets[3].area=        MaterialDlg::r_area;
  componentWidgets[3].factor=      MaterialDlg::r_color_factor;
  componentWidgets[3].colorWidget= 0;
  componentWidgets[3].use_texture=    MaterialDlg::r_use_texture;
  componentWidgets[3].texture_name=   MaterialDlg::r_texture_name;
  componentWidgets[3].browse_texture= MaterialDlg::r_browse_texture;
  componentWidgets[3].view_texture=   MaterialDlg::r_view_texture;
  componentWidgets[3].use_alpha=      MaterialDlg::r_use_alpha;
  componentWidgets[3].xoffset=        MaterialDlg::r_xoffset;
  componentWidgets[3].yoffset=        MaterialDlg::r_yoffset;
  componentWidgets[3].xscale=         MaterialDlg::r_xscale;
  componentWidgets[3].yscale=         MaterialDlg::r_yscale;

  componentWidgets[4].card=        MaterialDlg::transmission;
  componentWidgets[4].label_r=     MaterialDlg::t_r_label;
  componentWidgets[4].label_g=     MaterialDlg::t_g_label;
  componentWidgets[4].label_b=     MaterialDlg::t_b_label;
  componentWidgets[4].valuesBar_r= MaterialDlg::t_r;
  componentWidgets[4].valuesBar_g= MaterialDlg::t_g;
  componentWidgets[4].valuesBar_b= MaterialDlg::t_b;
  componentWidgets[4].radio_rgb=   MaterialDlg::t_rgb;
  componentWidgets[4].radio_hsv=   MaterialDlg::t_hsv;
  componentWidgets[4].area=        MaterialDlg::t_area;
  componentWidgets[4].factor=      MaterialDlg::t_color_factor;
  componentWidgets[4].colorWidget= 0;
  componentWidgets[4].use_texture=    MaterialDlg::t_use_texture;
  componentWidgets[4].texture_name=   MaterialDlg::t_texture_name;
  componentWidgets[4].browse_texture= MaterialDlg::t_browse_texture;
  componentWidgets[4].view_texture=   MaterialDlg::t_view_texture;
  componentWidgets[4].use_alpha=      MaterialDlg::t_use_alpha;
  componentWidgets[4].xoffset=        MaterialDlg::t_xoffset;
  componentWidgets[4].yoffset=        MaterialDlg::t_yoffset;
  componentWidgets[4].xscale=         MaterialDlg::t_xscale;
  componentWidgets[4].yscale=         MaterialDlg::t_yscale;
}

/////////////////////////////////////////////////////////////////////////////

MLDMaterial::~MLDMaterial()
{
  for( int i= 0; i < nComponentWidgets; ++i )
    {
    delete componentWidgets[i].colorWidget;
    componentWidgets[i].colorWidget= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLDMaterial::run( int object_id /* = 0 */ )
{
  MLScene* scene= model->getScene();

  MLObject* o= scene->getObject( (object_id != 0)
                                  ? object_id
                                  : model->getSingleSelected() );
  if( o == 0 )
    {
    model->printMessage( "Select one single object for material edition", 2 );
    return;
    }

  if( o->getObjectType() != MLObject::MESH )
    {
    model->printMessage( "Select an object for material edition", 2 );
    return;
    }

  MLMesh* mesh= (MLMesh*) o;
  MLMaterial* m= mesh->getMaterial();

  if( ! MLGlobalDialog::create(MaterialDlg::data) )
    return;

  kwDialog->setText( MaterialDlg::object_name, o->getName().name );
  kwDialog->setChecked( MaterialDlg::inherit_mat, mesh->inheritMaterial() );

  int i;

  buildMaterialsList();

  kwDialog->setText( MaterialDlg::mat_list, m->getName().name );
  kwDialog->setText( MaterialDlg::name_edit, m->getName().name );

kwDialog->setAvailable(MaterialDlg::preview_type,false);
kwDialog->setAvailable(MaterialDlg::update_preview,false);

  for( i= 0; i < nComponentWidgets; ++i )
    {
    ComponentWidget& cw= componentWidgets[i];
    componentWidgets[i].colorWidget= new MLColorWidget( kwDialog,
                                                        cw.label_r,
                                                        cw.label_g,
                                                        cw.label_b,
                                                        cw.valuesBar_r,
                                                        cw.valuesBar_g,
                                                        cw.valuesBar_b,
                                                        cw.radio_rgb,
                                                        cw.radio_hsv,
                                                        cw.area,
                                                        cw.factor );
    }

  selectMaterial();

  if( kwDialog->getChecked(MaterialDlg::inherit_mat) )
    kwDialog->focus(MaterialDlg::inherit_mat);
   else
    kwDialog->focus(MaterialDlg::name_edit);

  int outcome= kwDialog->run();

  MLMaterial new_m(scene);
  boolean new_inherit_material;
  dialogToMaterial(new_inherit_material,new_m);

  MLGlobalDialog::close();

  if( outcome == KWDialog::VALIDATE )
    {
    MLCSetMaterial* sm= new MLCSetMaterial(scene);
    MLCSetMaterial* undo= new MLCSetMaterial(scene);

    sm->object_id= o->getID();
    sm->inherit_material= new_inherit_material;
    *(sm->material)= new_m;

    sm->createUndo(undo);
    undo->object_id= o->getID();
    undo->inherit_material= mesh->inheritMaterial();
    *(undo->material)= *(mesh->getMaterial());

    model->post(sm);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLDMaterial::buildMaterialsList()
{
  MLScene* scene= model->getScene();

  kwDialog->clearList(MaterialDlg::mat_list);

  MLMaterialsIterator mi(scene);
  while( ! mi.eol() )
    {
    MLMaterial* m= mi++;
    if( m == 0 )
      continue;

    kwDialog->appendListText( MaterialDlg::mat_list, m->getName().name );
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLDMaterial::checkInheritAvailability()
{
  boolean yes= !kwDialog->getChecked(MaterialDlg::inherit_mat);

  kwDialog->setAvailable( MaterialDlg::mat_list, yes );
  kwDialog->setAvailable( MaterialDlg::name_edit, yes );

  checkNameButtonsAvailability();
/*
  kwDialog->setAvailable( MaterialDlg::direct_shaded, yes );
  kwDialog->setAvailable( MaterialDlg::global_contributor, yes );

  kwDialog->setAvailable( MaterialDlg::generate_shadows, yes );
  kwDialog->setAvailable( MaterialDlg::get_shadows, yes );

  kwDialog->setAvailable( MaterialDlg::lighting, yes );

  for( int i= 0; i < nComponentWidgets; ++i )
    {
    ComponentWidget& cw= componentWidgets[i];
    cw.setAvailable(kwDialog,yes);
    kwDialog->setAvailable( cw.use_texture, yes );
kwDialog->setAvailable( cw.use_texture, false );
    checkTexturingAvailability(i);
    }

  kwDialog->setAvailable( MaterialDlg::shininess, yes );
  kwDialog->setAvailable( MaterialDlg::trans_n, yes );
*/
  for( int i= 0; i < nComponentWidgets; ++i )
    {
    ComponentWidget& cw= componentWidgets[i];
kwDialog->setAvailable( cw.use_texture, false );
    checkTexturingAvailability(i);
    }
}

/////////////////////////////////////////////////////////////////////////////

void MLDMaterial::checkNameButtonsAvailability()
{
  boolean yes= !kwDialog->getChecked(MaterialDlg::inherit_mat);

  kwDialog->setAvailable( MaterialDlg::browse_mat, yes );

  boolean can_create= true;

  MLString name= kwDialog->getText(MaterialDlg::name_edit).trim();

  MLMaterial* m= model->getScene()->getMaterial(name);
  if( m != 0 )
    {
    yes= false;
    can_create= false;
    }

  m= getSelectedMaterial();
  if( m != 0 )
    {
    if( m->getID() == 0 )
      yes= false;
    }

  if( can_create )
    kwDialog->setText( MaterialDlg::change, "Create" );
   else
    kwDialog->setText( MaterialDlg::change, "Change" );

  if( name.length() == 0 )
    yes= false;

  kwDialog->setAvailable( MaterialDlg::rename_mat, yes );

  kwDialog->setAvailable( MaterialDlg::change, name.length() != 0 );
}

/////////////////////////////////////////////////////////////////////////////

// wc_index = component widget index
void MLDMaterial::checkTexturingAvailability( int wc_index )
{
  if( wc_index < 0 || wc_index >= nComponentWidgets )
    return;

  ComponentWidget& cw= componentWidgets[wc_index];

  boolean yes= kwDialog->getChecked( cw.use_texture );

  if( kwDialog->getChecked(MaterialDlg::inherit_mat) )
    yes= false;

  kwDialog->setAvailable( cw.texture_name, yes );
  kwDialog->setAvailable( cw.browse_texture, yes );
  kwDialog->setAvailable( cw.view_texture, yes );
  kwDialog->setAvailable( cw.use_alpha, yes );
  kwDialog->setAvailable( cw.xoffset, yes );
  kwDialog->setAvailable( cw.yoffset, yes );
  kwDialog->setAvailable( cw.xscale, yes );
  kwDialog->setAvailable( cw.yscale, yes );
}

/////////////////////////////////////////////////////////////////////////////

MLMaterial* MLDMaterial::getSelectedMaterial()
{
  if( kwDialog == 0 )
    return 0;

  MLString name= kwDialog->getText(MaterialDlg::mat_list);

  MLMaterial* m= model->getScene()->getMaterial(name);

  return m;
}

/////////////////////////////////////////////////////////////////////////////

void MLDMaterial::selectMaterial()
{
  MLMaterial* m= getSelectedMaterial();
  if( m == 0 )
    return;

  MLString name= kwDialog->getText(MaterialDlg::mat_list);
  kwDialog->setText(MaterialDlg::name_edit,name);

  MLMaterialComponent* mc[5];
  mc[EMITTANCE   ]= &(m->emittance);
  mc[DIFFUSE     ]= &(m->diffuse);
  mc[SPECULAR    ]= &(m->specular);
  mc[REFLECTION  ]= &(m->reflection);
  mc[TRANSMISSION]= &(m->transmission);

  int i;

  for( i= 0; i < nComponentWidgets; ++i )
    {
    ComponentWidget& cw= componentWidgets[i];
    cw.colorWidget->set( mc[i]->color, mc[i]->factor );

    if( mc[i]->color_model == MLMaterialComponent::RGB )
      cw.colorWidget->setRGBColorModel();
     else
      cw.colorWidget->setHSVColorModel();

    kwDialog->setText( cw.texture_name, mc[i]->image_name );
    kwDialog->setChecked( cw.use_alpha, mc[i]->use_alpha );
    kwDialog->setValue( cw.xoffset, double(mc[i]->x_offset) );
    kwDialog->setValue( cw.yoffset, double(mc[i]->y_offset) );
    kwDialog->setValue( cw.xscale, mc[i]->x_scale );
    kwDialog->setValue( cw.yscale, mc[i]->y_scale );
    }

  kwDialog->setValue( MaterialDlg::shininess, m->specular.shininess );
  kwDialog->setValue( MaterialDlg::trans_n, m->transmission.trans_n );

  kwDialog->setChecked( MaterialDlg::direct_shaded,
                        m->direct_shaded );
  kwDialog->setChecked( MaterialDlg::global_contributor,
                        m->rad_contributor );
  kwDialog->setChecked( MaterialDlg::generate_shadows,
                        m->generate_shadows );
  kwDialog->setChecked( MaterialDlg::get_shadows,
                        m->get_shadows );

  checkInheritAvailability();

  if( m->getID() == 0 )
    kwDialog->setAvailable(MaterialDlg::rename_mat,false);

  kwDialog->setText(MaterialDlg::change,"Change");
  kwDialog->setAvailable(MaterialDlg::change,true);
}

/////////////////////////////////////////////////////////////////////////////

void MLDMaterial::dialogToMaterial( boolean& inherit_material,
                                    MLMaterial& m )
{
  inherit_material= kwDialog->getChecked(MaterialDlg::inherit_mat);

  if( inherit_material )
    return;

  m.name.name= kwDialog->getText(MaterialDlg::name_edit).trim();

  m.direct_shaded= kwDialog->getChecked(MaterialDlg::direct_shaded );
  m.rad_contributor= kwDialog->getChecked(MaterialDlg::global_contributor);

  m.generate_shadows= kwDialog->getChecked(MaterialDlg::generate_shadows);
  m.get_shadows= kwDialog->getChecked(MaterialDlg::get_shadows);

  MLMaterialComponent* mcs[5];
  mcs[EMITTANCE   ]= &m.emittance;
  mcs[DIFFUSE     ]= &m.diffuse;
  mcs[SPECULAR    ]= &m.specular;
  mcs[REFLECTION  ]= &m.reflection;
  mcs[TRANSMISSION]= &m.transmission;

  int i;

  for( i= 0; i < nComponentWidgets; ++i )
    {
    ComponentWidget& cw= componentWidgets[i];
    MLMaterialComponent& mc= *(mcs[i]);

    mc.color= cw.colorWidget->get();
    mc.color_model= cw.colorWidget->inRGBColorModel()
                                    ? MLMaterialComponent::RGB
                                    : MLMaterialComponent::HSV;
    mc.factor= kwDialog->getValue(cw.factor);

    mc.use_texture= kwDialog->getChecked(cw.use_texture);
    mc.image_name= kwDialog->getText(cw.texture_name);
    mc.use_alpha=  kwDialog->getChecked(cw.use_alpha);
    mc.x_offset= int(kwDialog->getValue(cw.xoffset));
    mc.y_offset= int(kwDialog->getValue(cw.yoffset));
    mc.x_scale= kwDialog->getValue(cw.xscale);
    mc.y_scale= kwDialog->getValue(cw.yscale);
    }

  m.specular.shininess= kwDialog->getValue(MaterialDlg::shininess);
  m.transmission.trans_n= kwDialog->getValue(MaterialDlg::trans_n);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLDMaterial::kwCallback( KWEvent& event )
{
  for( int i= 0; i < nComponentWidgets; ++i )
    {
    ComponentWidget& cw= componentWidgets[i];
    if( cw.colorWidget == 0 )
      continue;
    if( cw.colorWidget->colorCallback(event) )
      return true;
    }

  if( event.type == KWEvent::KEY &&
      event.widget == MaterialDlg::name_edit )
    {
    if( event.key == Key('.') ||
        event.key == Key(MLDatabase::sepChar) ||
        event.key == Key(MLPath::separator) )
      return true;
    return false;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == MaterialDlg::inherit_mat )
    {
    checkInheritAvailability();
    return true;
    }

  if( event.type == KWEvent::SELECT )
    {
    for( int i= 0; i < nComponentWidgets; ++i )
      if( event.widget == componentWidgets[i].use_texture )
        {
        checkTexturingAvailability(i);
        return true;
        }
    }

  if( event.type == KWEvent::KEY_PROCESSED &&
      event.widget == MaterialDlg::name_edit )
    {
    checkNameButtonsAvailability();

    return false;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == MaterialDlg::mat_list )
    {
    selectMaterial();
    kwDialog->focus(MaterialDlg::name_edit);
    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == MaterialDlg::rename_mat )
    {
    MLMaterial* m= getSelectedMaterial();
    if( m == 0 )
      return true;

    MLString name= kwDialog->getText(MaterialDlg::name_edit).trim();

    m->name.name= name;

    buildMaterialsList();

    kwDialog->setText(MaterialDlg::mat_list,name);

    checkNameButtonsAvailability();

    kwDialog->focus(MaterialDlg::name_edit);

    return true;
    }

  if( event.type == KWEvent::SELECT &&
      event.widget == MaterialDlg::browse_mat )
    {
    MLDLoadMaterial* loadMaterial= new MLDLoadMaterial(model);
    MLMaterial* m= loadMaterial->run();
    delete loadMaterial;

    if( m == 0 )
      return false; // failed

    MLString name= m->getName().name;

    MLMaterial* find_mat= model->getScene()->getMaterial(name);
    if( find_mat == 0 )
      {
      // this a new one, register it
      m->registerID();

      buildMaterialsList();
      kwDialog->setText( MaterialDlg::mat_list, name );
      selectMaterial();
      }
     else
      {
      if( ! KWWarningOkCancelf( "Material \"%s\" already exists in this scene.\n"
                                "Overwrite it?", name.get() ) )
        {
        delete m;
        m= 0;

        return true;
        }

      *find_mat= *m;
      find_mat->compile();

      delete m;
      m= 0;

      kwDialog->setText( MaterialDlg::mat_list, name );
      selectMaterial();
      }

    return true;
    }

  return MLGlobalDialog::kwCallback(event);
}

/////////////////////////////////////////////////////////////////////////////

