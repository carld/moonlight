SUBDIRS=engines modes functions commands dialogs gdialogs render file
SRC= ModelEngine.C ModelCanvas.C ModelModule.C ModelFlags.C \
 ModelColors.C ModelPrim.C \
 createModelDialog.C \
 printRenderTime.C \
 MLStatusLightingSwitch.C ModelStatusLineEntry.C ModelLogListener.C \
 ModelStatusBars.C \
 ModelModes.C ModelFunctions.C \
 ModelCommand.C ModelScene.C \
 popupCommand.C ModelPopups.C \
 handleModelCommand.C \
 MLLightingProcess.C \
 MLTagVertexArray.C \
 MLModelVersion.C \
 ModelKernelModule.C \
 ModelCommandLine.C \
 $(call wild,*/*.C) $(call wild,*/*/*.C)

kmodel_sources=$(SRC)
kmodel_dlibs=koalagui scene
kmodel_slibs=
kmodel_install=$(prefix)/lib/moonlight
kmodel_depends=koalagui/libkoalagui.so 3D/scene/libscene.so

SHARED=kmodel
