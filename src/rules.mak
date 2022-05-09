ifndef ORIGTOP
DIR=.
TOP=.
ORIGTOP:=$(TOP)
SOURCES=

makeobj = $(addsuffix .o,$(basename $(1)))
makedep = $(addsuffix .d,$(basename $(1)))
adddir = $(1:%=$(DIR)/%)

wild=$(patsubst $(VPATH)/$(DIR)/%,%,$(wildcard $(VPATH)/$(DIR)/$(1)))

buildcommand =$(shell if [ -e $(DIR)/makext.touch ]; then echo "$(1)" >>$(EXTMAK); echo "	$(2)" >>$(EXTMAK); fi)
buildexec   = $(call buildcommand,"$(1): $(2) $(4) $(call adddir,dir.mak)","$(CC) $(LDFLAGS) -o $(1) $(2) $(3)")
buildshared = $(call buildcommand,"$(1): $(2) $(4) $(call adddir,dir.mak)","$(CC) -shared $(LDFLAGS) -o $(1) $(2) $(3)") 
buildstatic = $(call buildcommand,"$(1): $(2) $(3) $(call adddir,dir.mak)","$(AR) r $(1) $(2);ranlib $(1)")
installfile = $(call buildcommand,"$(1)/$(2): $(3) $(1)","cp $(3) $$$$\(DESTDIR\)$(1)/$(2)")
installdir  = $(call buildcommand,"$(1)::","@mkdir -p $$$$\(DESTDIR\)$(1)")

.PHONY: all compile install
all: compile

%.d: %.C
	$(CXX) $(CXXFLAGS) -M $< | sed 's,\([^\.]*\)\.o[ :]*,$(dir $@)/\1.o $@ : ,g' >$@

%.d: %.cc
	$(CXX) $(CXXFLAGS) -M $< >$@
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -M $< >$@

%.d: %.c
	$(CC) $(CFLAGS) -M $< >$@

%.o: %.C %.d
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc %.d
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cpp %.d
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.c %.d
	$(CC) $(CFLAGS) -c -o $@ $<

%.makbuilt: %.mak
	@mkdir -p $(dir $@)
	@echo DIR:=$(@D) >$@
	@echo SRC:= >>$@
	@echo SUBDIRS:= >>$@
	@echo EXE:= >>$@
	@echo SHARED:= >>$@
	@echo STATIC:= >>$@
	@cat $< >>$@
	@echo include $(VPATH)/rules.mak >>$@

%.makext: %.mak

endif

FULLSUBDIRS:=$(SUBDIRS:%=$(DIR)/%)
EXTMAK:=$(DIR)/dir.makext
DIRMAKS:=$(strip $(FULLSUBDIRS:%=%/dir.makbuilt) $(FULLSUBDIRS:%=%/dir.makext))
ALLDIRS:=$(ALLDIRS) $(dir $(foreach so,$(SRC),$(call adddir,$(so))))
ALLMAKS:=$(ALLMAKS) $(DIRMAKS)

tmp:=$(shell if [ $(VPATH)/$(DIR)/dir.mak -nt $(DIR)/dir.makext ]; then touch $(DIR)/makext.touch; rm -f $(DIR)/dir.makext; fi)

INSTALLTARGETS:=$(INSTALLTARGETS) $(foreach exe,$(EXE),$($(exe)_install)/$(exe)) $(foreach so,$(SHARED),$($(so)_install)/lib$(so).so) $(foreach so,$(STATIC),$($(so)_install)/$(so).a)
$(foreach exe,$(EXE),$(call installfile,"$($(exe)_install)","$(exe)","$(call adddir,$(exe))")$(call installdir,"$($(exe)_install)"))
$(foreach so,$(SHARED),$(call installfile,"$($(so)_install)","lib$(so).so","$(call adddir,lib$(so)).so")$(call installdir,"$($(so)_install)"))
$(foreach so,$(STATIC),$(call installfile,"$($(so)_install)","$(so).a","$(call adddir,$(so)).a")$(call installdir,"$($(so)_install)"))

EXE:=$(EXE) $(EXENOINST)
SHARED:=$(SHARED) $(SHAREDNOINST)
STATIC:=$(STATIC) $(STATICNOINST)

EXECUTABLES:=$(EXECUTABLES) $(EXE:%=$(DIR)/%)
$(foreach exe,$(EXE), \
$(call buildexec,"$(call adddir,$(exe))","$(call adddir,$(call makeobj,$($(exe)_sources))) $($(exe)_slibs)","$(addprefix -l,$($(exe)_dlibs))","$($(exe)_depends)")\
)
SOLIBS:=$(SOLIBS) $(addsuffix .so,$(SHARED:%=$(DIR)/lib%))
$(foreach so,$(SHARED),\
$(call buildshared,"$(call adddir,lib$(so)).so","$(call adddir,$(call makeobj,$($(so)_sources))) $($(so)_slibs)",$(addprefix -l,$($(so)_dlibs)),"$($(so)_depends)")\
)

STATICLIBS:=$(STATICLIBS) $(addsuffix .a,$(STATIC:%=$(DIR)/%))
$(foreach so,$(STATIC),\
$(call buildstatic,"$(call adddir,$(so)).a","$(call adddir,$(call makeobj,$($(so)_sources))) $($(so)_slibs)","$($(so)_depends)") \
)

SOURCES:=$(SOURCES) $(SRC:%=$(DIR)/%)

$(shell rm -f $(DIR)/makext.touch)

TOP:=$(TOP)/..

ifneq ($(DIRMAKS),)
$(DIRMAKS):
-include $(DIRMAKS)
endif

TOP:=$(TOP:%/..=%)

DIR:=./$($(dir $(DIR)):%/=%)

ifeq ($(TOP),$(ORIGTOP))
#when done with all that
OBJS= $(call makeobj,$(SOURCES))
ALLDIRS:=$(sort $(ALLDIRS))

DEPS:=$(call makedep,$(SOURCES))
-include dummy.d $(DEPS)

$(shell mkdir -p $(ALLDIRS) 2>/dev/null)

compile: $(ALLMAKS) $(DEPS) $(OBJS) $(STATICLIBS) $(SOLIBS) $(EXECUTABLES)

distclean clean:
	rm -rf $(OBJS) $(ALLMAKS) $(EXECUTABLES) $(SOLIBS) $(STATICLIBS)

install: $(INSTALLTARGETS) $(INSTALLDATATARGET)

endif
