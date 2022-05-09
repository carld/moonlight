SUBDIRS=material lighting formal nurbs file renderer convert

SRC= \
 MLCamera.C \
 MLVerticesIterator.C \
 MLPolygonsIterator.C \
 MeshRep.C MeshRepPurge.C MeshRepPolygonTesselate.C \
 MLXForm.C MLName.C \
 MLAbstractMeshAllocator.C MLAbstractMeshesIterator.C \
 MLAbstractObject.C \
 MLAbstractMesh.C \
 MLCurveVertex.C \
 MLAbstractCurveAllocator.C MLAbstractCurve.C \
 CurveRep.C MLCurve.C \
 MLObjectAllocator.C MLObjectsIterator.C MLMeshesIterator.C \
 MLObject.C MLChildrenIterator.C \
 MLMesh.C MLVertexID.C \
 MLNullObject.C \
 MLScene.C \
 MLRay.C MLRayTracer.C \
 MLHashBox.C \
 MLSceneVersion.C \
 MLControlPointID.C \
 $(call wild,*/*.C) $(call wild,*/*/*.C) $(call wild,*/*/*/*.C)

scene_sources=$(SRC)
scene_dlibs=gltt ttf GLU GL
scene_slibs=
scene_install=$(prefix)/lib/moonlight

SHARED=scene
