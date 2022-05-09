SUBDIRS=

SRC= MLPropertyType.C \
 MLPBoolean.C MLPInteger.C MLPReal.C \
 MLPVector.C MLPPoint.C MLPColor.C MLPString.C \
 MLPNodeRef.C MLPNodeStruct.C \
 MLPByteArray.C MLPIntegerArray.C MLPRealArray.C \
 MLPVectorArray.C MLPPointArray.C MLPColorArray.C \
 MLPVertexArray.C MLPEdgeArray.C \
 MLPPolygonArray.C MLPSVertexArray.C \
 MLProperty.C \
 MLNodeType.C MLNode.C MLOperator.C \
 MLProcedure.C MLProcArgs.C MLProcProfile.C \
 MLRunnableProc.C MLProcQueue.C MLUndoRedoProcStack.C MLProcManager.C \
 MLAppRoot.C \
 MLKernelVersion.C MLKernel.C MLKernelConfig.C \
 MLDynamicLoader.C \
 MLPluginLoader.C MLPlugin.C \
 MLModuleLoader.C MLKernelModule.C


kernel_sources=$(SRC)
kernel_dlibs=
kernel_slibs=
kernel_install=$(prefix)/lib/moonlight

SHARED=kernel
