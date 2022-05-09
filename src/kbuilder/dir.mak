SUBDIRS=

SRC= KBEngine.C KBCanvas.C KBModule.C \
 KBModuleKeyboardHandler.C \
 createKBDialog.C \
 KBStatusBars.C \
 KBModes.C KBFunctions.C \
 KBCommand.C \
 popupCommand.C KBPopups.C \
 handleKBCommand.C \
 KBKernelModule.C \
 $(call wild,*/*.C)

kbuilder_sources=$(SRC)
kbuilder_dlibs=koalagui
kbuilder_slibs=
kbuilder_install=$(prefix)/lib/moonlight
kbuilder_depends=koalagui/libkoalagui.so

SHARED=kbuilder
