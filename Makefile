<<<<<<< HEAD
OBJ= asicpacker asicpacker_all ts_repair
=======
OBJ= asicpacker asicpacker_all
>>>>>>> 079cd85d2b7ebdc830da287210fd6d8a5f1c7e9e

all: $(OBJ)

ROOTCFLAGS	= $(shell root-config --cflags)
ROOTLIBS	= $(shell root-config --libs)
ROOTGLiBS	= $(shell root-config --glibs)

INCLUDES	= -I$(ROOTSYS)include

CXX	= g++

asicpacker: asicpacker.cpp AIDAraw.h
	$(CXX) $< -o $@ $(INCLUDES) $(ROOTLIBS) $(ROOTGLIBS) $(ROOTCFLAGS)

<<<<<<< HEAD
ts_repair: ts_repair.cpp AIDAraw.h
	$(CXX) $< -o $@ $(INCLUDES) $(ROOTLIBS) $(ROOTGLIBS) $(ROOTCFLAGS)

=======
>>>>>>> 079cd85d2b7ebdc830da287210fd6d8a5f1c7e9e
asicpacker_all: asicpacker_all.cpp AIDAraw.h
	$(CXX) $< -o $@ $(INCLUDES) $(ROOTLIBS) $(ROOTGLIBS) $(ROOTCFLAGS)

clean:
	rm -f *~ *.o $(OBJ)
