include Makefile.arch

#
# stuff to make
#
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
HEADERS=$(SOURCES:.cc=.h)
SOURCES=VBSHWW.cc
HEADERS=VBSHWW.h
OBJECTS=VBSHWW.o
LIB=VBSHWW_CORE.so

#
# how to make it 
#

$(LIB): $(SOURCES) $(HEADERS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) -fPIC -ITMultiDrawTreePlayer -Wunused-variable $(SOFLAGS) $(SOURCES) $(ROOTLIBS) -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer -I../NanoTools/NanoCORE/ -I../FTAnalysis/analysis/misc -I../rooutil -DLorentzVectorPtEtaPhiM4D -o $@
	ln -sf VBSHWW_CORE.so libVBSHWW_CORE.so

all: $(LIB) 
clean:
	rm -f *.o \
	rm -f *.d \
	rm -f *.so \
