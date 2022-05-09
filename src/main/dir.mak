SUBDIRS=
SRC=main.C

moonlight-bin_sources=$(SRC)
moonlight-bin_slibs= 
moonlight-bin_dlibs=stdc++ dl pthread tools kernel
moonlight-bin_install=$(prefix)/bin
moonlight-bin_depends=tools/libtools.so

EXE=moonlight-bin
