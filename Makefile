OBJ= asicpacker asicpacker_all ts_repair

all: $(OBJ)

ROOTCFLAGS	= $(shell root-config --cflags)
ROOTLIBS	= $(shell root-config --libs)
ROOTGLiBS	= $(shell root-config --glibs)

INCLUDES	= -I$(ROOTSYS)include

CXX	= g++

asicpacker: asicpacker.cpp AIDAraw.h
	$(CXX) $< -o $@ $(INCLUDES) $(ROOTLIBS) $(ROOTGLIBS) $(ROOTCFLAGS)

ts_repair: ts_repair.cpp AIDAraw.h
	$(CXX) $< -o $@ $(INCLUDES) $(ROOTLIBS) $(ROOTGLIBS) $(ROOTCFLAGS)

asicpacker_all: asicpacker_all.cpp AIDAraw.h
	$(CXX) $< -o $@ $(INCLUDES) $(ROOTLIBS) $(ROOTGLIBS) $(ROOTCFLAGS)

clean:
	rm -f *~ *.o $(OBJ)
