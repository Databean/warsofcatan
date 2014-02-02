export OBJ_HOME := $(realpath obj)
export SRC_HOME := $(realpath src)
export INCL_HOME := $(realpath include)
EXECUTABLE := checkers-spike
ALLFILES := $(wildcard $(SRC_HOME)/*) $(wildcard $(INCL_HOME)/*)
export CXX := g++ -c
export LD := g++
export CXXFLAGS := -g -I$(INCL_HOME) -std=gnu++0x
export LDFLAGS := -lSDL2 -lGL -lGLU

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(ALLFILES)
	cd src && $(MAKE)
	${LD} obj/*.o $(LDFLAGS) -o $(EXECUTABLE)
