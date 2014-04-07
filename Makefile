export OBJ_HOME := $(realpath obj)
export SRC_HOME := $(realpath src)
export INCL_HOME := $(realpath include)
export TEST_LINK_FILES := $(realpath UnitTest++)/libUnitTest++.a
export TEST_INCLUDE := $(realpath UnitTest++/src)
export EXECUTABLE := warsofcatan
ALLFILES := $(wildcard $(SRC_HOME)/*) $(wildcard $(INCL_HOME)/*)
export CXX := g++
export LDX := g++
export CXXFLAGS := -g -I$(INCL_HOME) -std=c++0x -Wall -I$(realpath .)/sdl2ttf -I/usr/include/SDL2/ -I/usr/local/include/SDL2/
export LDXFLAGS := -L/usr/local/lib -lSDL2 -L$(realpath .)/sdl2ttf/.libs/ -Wl,-R$(realpath .)/sdl2ttf/.libs/ -lSDL2_ttf -lGL -lGLU

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): sdl2ttf/.libs/libSDL2_ttf.so $(ALLFILES)
	cd src && $(MAKE)
	${LDX} obj/*.o $(LDXFLAGS) -o $(EXECUTABLE)

sdl2ttf/.libs/libSDL2_ttf.so:
	cd sdl2ttf && ./configure
	cd sdl2ttf && $(MAKE)

.PHONY: tests
tests: $(EXECUTABLE)
	cd UnitTest++ && $(MAKE) libUnitTest++.a
	cd tests && $(MAKE)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
	rm -f obj/*.o
	cd sdl2ttf && $(MAKE) clean