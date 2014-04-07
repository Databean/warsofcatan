export OBJ_HOME := $(realpath obj)
export SRC_HOME := $(realpath src)
export INCL_HOME := $(realpath include)
export TEST_LINK_FILES := $(realpath UnitTest++)/libUnitTest++.a
export TEST_INCLUDE := $(realpath UnitTest++/src)
export EXECUTABLE := warsofcatan
ALLFILES := $(wildcard $(SRC_HOME)/*) $(wildcard $(INCL_HOME)/*)
export CXX := g++
export LD := g++
export CXXFLAGS := -g -I$(INCL_HOME) -std=c++0x -Wall
export LDFLAGS := -L/usr/local/lib -lSDL2 -lSDL2_ttf -lGL -lGLU

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(ALLFILES)
	cd src && $(MAKE)
	${LD} obj/*.o $(LDFLAGS) -o $(EXECUTABLE)

.PHONY: tests
tests: $(EXECUTABLE)
	cd UnitTest++ && $(MAKE) libUnitTest++.a
	cd tests && $(MAKE)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
	rm -f obj/*.o