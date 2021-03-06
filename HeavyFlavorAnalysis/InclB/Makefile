# ======================================================================
# NOTE: make all
# -----
#
# ======================================================================


ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

ifeq ($(SCRAM_ARCH),"")
  CXX         = c++
else
  CXX         = $(shell scramv1 tool info cxxcompiler | grep CXX= | sed s/CXX=//)
endif

CXXFLAGS      = -g -Wall -fPIC
LD            = $(CXX)
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
#GLIBS         = $(ROOTGLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))


$(addprefix obj/,%.o) : $(addprefix rootio/,%.cc )
	$(CXX) $(CXXFLAGS) -c $< -o $@


ANA00   = TAna00Event.o TAna00EventDict.o \
          TGenCand.o TGenCandDict.o \
          TAnaTrack.o TAnaTrackDict.o \
          TAnaCand.o TAnaCandDict.o \
          TAnaVertex.o TAnaVertexDict.o \
          TAnaJet.o TAnaJetDict.o 

ANACLASSES = ana.o anaDict.o 

# ================================================================================
all: 
# --
	@$(MAKE) ana00
	@$(MAKE) links

# ================================================================================
ana00: $(addprefix obj/,$(ANA00))
# ----------------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANA00)) -o lib/libAna00.so

rootio/TAna00EventDict.cc: rootio/TAna00Event.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAna00EventDict.cc -c TAna00Event.hh && cd -

rootio/TGenCandDict.cc: rootio/TGenCand.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TGenCandDict.cc -c TGenCand.hh  && cd - 

rootio/TAnaTrackDict.cc: rootio/TAnaTrack.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaTrackDict.cc -c TAnaTrack.hh && cd - 

rootio/TAnaVertexDict.cc: rootio/TAnaVertex.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaVertexDict.cc -c TAnaVertex.hh && cd - 

rootio/TAnaCandDict.cc: rootio/TAnaCand.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaCandDict.cc -c TAnaCand.hh  && cd - 

rootio/TAnaJetDict.cc: rootio/TAnaJet.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaJetDict.cc -c TAnaJet.hh && cd - 

rootio/TAnaMuonDict.cc: rootio/TAnaMuon.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaMuonDict.cc -c TAnaMuon.hh && cd -

rootio/TGenMuonDict.cc: rootio/TGenMuon.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TGenMuonDict.cc -c TGenMuon.hh && cd - 

# ======================================================================

# ================================================================================
links:
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libAna00.so && ln -s ../../src/HeavyFlavorAnalysis/InclB/lib/libAna00.so && cd -

# ================================================================================
clean:
	rm -f obj/*.o rootio/*Dict.* test/*Dict.* lib/libAna00.so

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna00.so

