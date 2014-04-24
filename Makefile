export OBJ_HOME := $(realpath obj)
export SRC_HOME := $(realpath src)
export INCL_HOME := $(realpath include)
export TEST_LINK_FILES := -lgtest -lgtest_main
export TEST_INCLUDE := 
export EXECUTABLE := warsofcatan
ALLFILES := $(wildcard $(SRC_HOME)/*) $(wildcard $(INCL_HOME)/*)
export CXX := g++
export LD := g++
export CXXFLAGS := -g -I$(INCL_HOME) -std=c++0x -I/usr/include/SDL2 -I/usr/local/include/SDL2 -Wall
export LDFLAGS := -L/usr/local/lib -lSDL2 -lSDL2_ttf -lGL -lGLU -Wl,-R/usr/local/lib

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(ALLFILES)
	cd src && $(MAKE)
	${LD} obj/*.o $(LDFLAGS) -o $(EXECUTABLE)

.PHONY: tests
tests: $(EXECUTABLE)
	cd tests && $(MAKE)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
	rm -f obj/*.o
