SUBDIRS=graphics koala kw interface

SRC=InterfaceKernelModule.C $(call wild,*/*.C) $(call wild,interface/*/*.C) $(call wild,graphics/fonts/*1024.C)
ifeq ($(WINDOWS),yes)
  SRC+=$(call wild,graphics/Win/*.C)
else
  SRC+=$(call wild,graphics/X11/*.C)
endif

koalagui_sources=$(SRC)
koalagui_dlibs=GL image
koalagui_slibs=
koalagui_install=$(prefix)/lib/moonlight
koalagui_depends=image/libimage.so

SHARED=koalagui
