SOURCES := $(wildcard *.cpp)
OBJECTS := $(addprefix $(OBJ_HOME)/, $(SOURCES:.cpp=.o))
HEADERS := $(wildcard $(INCL_HOME)/*.h)

.PHONY: main 
main: $(OBJECTS)

$(OBJ_HOME)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@
