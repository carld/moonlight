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
 *      MLExportRIB.C -- by David Whittington (4/4/1998)
 */

#include "MLExportRIB.h"

#include "tools/Color.h"
#include "tools/MLGammaCorrection.h"

#include "tools/3D/MLPolygon.h"
#include "tools/3D/MLVertex.h"

#include "scene/MLSceneVersion.h"

#include "scene/MLPolygonsIterator.h"
#include "scene/MeshRep.h"
#include "scene/MLMesh.h"
#include "scene/lighting/MLLight.h"
#include "scene/MLCamera.h"
#include "scene/MLObjectsIterator.h"
#include "scene/lighting/MLMeshLighting.h"
#include "scene/lighting/MLPoolTable.h"
#include "scene/MLScene.h"


/////////////////////////////////////////////////////////////////////////////

MLExportRIB::MLExportRIB(MLScene *_scene) :
  MLExport(_scene)
{
    only_triangles= false;
    only_one_pool= false;
}

/////////////////////////////////////////////////////////////////////////////

MLExportRIB::~MLExportRIB()
{
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportRIB::run()
{
    boolean       bRet;

    if ((file != 0) &&
        (scene != 0))
    {
        int                     nLight;
        int                     nMaxLights;
        int                     nObject;
        int                     nMaxObjects;
        MLCamera *              pCamera;
        MLObjectsIterator       oi(scene);

        nMaxLights = 0;
        nMaxObjects = 0;
        pCamera = (MLCamera *) NULL;
        while (!oi.eol())
        {
            MLObject *  o;

            o = oi++;
            if (o != 0)
            {
                if (o->getObjectType() == MLObject::MESH)
                {
                    MLMesh *    mlmesh = (MLMesh *) o;
                    MeshRep *   mesh = mlmesh->getMeshRep();

                    mlmesh = (MLMesh *) o;
                    mesh = mlmesh->getMeshRep();
                    if (mesh != 0)
                    {
                        if ((mesh->nPolygons != 0) &&
                            (mesh->nVertices != 0))
                            nMaxObjects++;
                    }
                }
                else if (o->getObjectType() == MLObject::LIGHT)
                {
                    nMaxLights++;
                }
                else if (o->getObjectType() == MLObject::CAMERA)
                {
                    if (pCamera == (MLCamera *) NULL)
                        pCamera = (MLCamera *) o;
                }
            }
        }

        fprintf(file,
                "# Exported from Moonlight Creator %s\n"
                "# Scene name: %s\n",
                MLSceneVersion::version(),
                scene->getName().name.get());

        if (pCamera)
        {
            fprintf(file, "Format %d %d 1\n",
                    pCamera->x_res, pCamera->y_res);
        }
        else
        {
            fprintf(file, "Format 320 240 1\n");
        }
        fprintf(file,
                "FrameBegin 0\n"
                "Display \"%s.tif\" \"file\" \"rgba\"\n",
                scene->getName().name.get());
        if (pCamera)
        {
            if (pCamera->ortho)
            {
                fprintf(file, "Projection \"orthographic\"\n");
            }
            else
            {
                fprintf(file,
                        "Projection \"perspective\" \"fov\" %g\n",
                        (pCamera->angle * 180.0) / M_PI);
            }
        }

        fprintf(file, "WorldBegin\n");

        if (pCamera)
        {
            fprintf(file,
                    "  Translate % 8.4g % 8.4g %8.4g\n",
                    -pCamera->location.x(),
                    -pCamera->location.z(),
                     pCamera->location.y());
            /* TODO: Compute the necessary rotation for the "lookat" point */
        }

        bRet = true;
        nLight = 1;
        oi.reset();
        while ((bRet) &&
               (!oi.eol()))
        {
            MLObject *  o;

            o = oi++;
            if (o != 0)
            {
                if (o->getObjectType() == MLObject::LIGHT)
                {
                    MLLight *   pLight;

                    pLight = (MLLight *) o;
                    if (exportLightRIB(nLight, nMaxLights, pLight))
                        nLight++;
                    else
                        bRet = false;
                }
            }
        }

        nObject = 1;
        oi.reset();
        while ((bRet) &&
               (!oi.eol()))
        {
            MLObject *  o;

            o = oi++;
            if (o != 0)
            {
                if (o->getObjectType() == MLObject::MESH)
                {
                    MLMesh *    mlmesh;
                    MeshRep *   mesh;

                    mlmesh = (MLMesh*) o;
                    mesh = mlmesh->getMeshRep();
                    if (mesh != 0)
                    {
                        if ((mesh->nPolygons != 0) &&
                            (mesh->nVertices != 0))
                        {
                            if (exportMeshRIB(nObject, nMaxObjects, mlmesh))
                                ++nObject;
                            else
                                bRet = false;
                        }
                    }
                }
            }
        }
        fprintf(file, "WorldEnd\nFrameEnd\n");
        close();
    }
    else
        bRet = false;

    return (bRet);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportRIB::exportMeshRIB(int nObject, int nMaxObjects,
                                 MLMesh *mlmesh)
{
    int                 i, count;
    int                 bFirst;
    boolean               bRet;
    MLPolygonsIterator  pi(mlmesh);
    const Matrix4x4 &   o2w = mlmesh->getGlobalXForm().get_o2w();
    MLVertexArray &     vertices  = mlmesh->getMeshRep()->vertices;
    MLSVertexArray &    svertices = mlmesh->getMeshRep()->svertices;

    bRet = true;
    if (fprintf(file,
                "# Object %d of %d: \"%s\"\n"
                "AttributeBegin\n"
                "  PointsPolygons [",
                nObject, nMaxObjects,
                mlmesh->getName().name.get()) <= 0)
        bRet = false;

    count = 0;
    while (!pi.eol())
    {
        MLPolygon *     p = pi++;

        if (bRet)
        {
            if (fprintf(file, " %d", p->nVertices) > 0)
            {
                count++;
                if (count > 20)
                {
                    if (fprintf(file, "\n\t\t  ") <= 0)
                        bRet = false;
                    count = 0;
                }
            }
            else
                bRet = false;
        }
    }

    if ((bRet) &&
        (fprintf(file, " ]\n\t\t [") <= 0))
        bRet = false;

    bFirst = true;
    pi.reset();
    while (!pi.eol())
    {
        MLPolygon *     p = pi++;

        if (bFirst)
            bFirst = false;
        else if ((bRet) &&
                 (fprintf(file, "\n\t\t  ") <= 0))
            bRet = false;
        for (i = 0; i < p->nVertices; i++)
        {
            if ((bRet) &&
                (fprintf(file, " %3d", (p->elements[i].svertex - 1)) <= 0))
                bRet = false;
        }
        if ((bRet) &&
            (fprintf(file, "  ") <= 0))
            bRet = false;
    }

    if ((bRet) &&
        (fprintf(file, " ]\n\t\t\"P\" [ ") <= 0))
        bRet = false;

    bFirst = true;
    for (i = 1; (i <= svertices.getSize()) && (bRet); i++)
    {
        Point           p(0,0,0);

        if ((svertices[i].allocated()) &&
            (svertices[i].vertex != 0))
        {
            p = o2w.apply(vertices[svertices[i].vertex].point);
            if (bFirst)
                bFirst = false;
            else
                fprintf(file, "\n\t\t      ");
            if ((bRet) &&
                (fprintf(file, "% 10.6g % 10.6g % 10.6g",
                         (fabs(p.x()) < 1e-10 ? 0.0 :  p.x()),
                         (fabs(p.z()) < 1e-10 ? 0.0 :  p.z()),
                         (fabs(p.y()) < 1e-10 ? 0.0 : -p.y())) <= 0))
                bRet = false;
        }
    }
    if (bRet)
    {
        if (fprintf(file, " ]\nAttributeEnd\n") <= 0)
            bRet = false;
    }

    return (bRet);
}

boolean MLExportRIB::exportLightRIB(int nLight, int nMaxLights, MLLight *pLight)
{
    boolean       bRet;

    (void) nMaxLights;          // currently unused...

    bRet = true;
    if (fprintf(file, "  LightSource ") <= 0)
        bRet = false;

    switch (pLight->getLightType())
    {
    case MLLight::POINT:
        if ((bRet) &&
            (fprintf(file, "\"pointlight\"") <= 0))
            bRet = false;
        break;
    case MLLight::DIRECTIONAL:
        /* TODO */
        break;
    case MLLight::SPOT:
        /* TODO */
        break;
    default:
        break;
    }
    if ((bRet) &&
        (fprintf(file, " %2d \"from\" % 7.4g % 7.4g % 7.4g \"intensity\" %g\n",
                  nLight,
                  pLight->worldPosition.x(),
                  pLight->worldPosition.z(),
                 -pLight->worldPosition.y(),
                 pLight->direct_intensity) <= 0))
        bRet = false;

    return (bRet);
}

/////////////////////////////////////////////////////////////////////////////

boolean MLExportRIB::exportPolygon(const MLPolygon &p) const
{
    boolean       bRet;

    bRet = false;
    if (p.allocated())
    {
        if (only_triangles)
            bRet = (p.nVertices == 3);
        else if ((p.flags & MLPolygon::BASE_POLYGON) != 0)
            bRet = true;
    }

    return (bRet);
}

/////////////////////////////////////////////////////////////////////////////
