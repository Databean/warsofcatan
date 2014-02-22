TEST_FILE := main.test

.PHONY: run
run: $(TEST_FILE)
	./$(TEST_FILE)

$(TEST_FILE): ../$(LIBRARY) $(TEST_LINK_FILES)
	$(CXX) $(CXXFLAGS) -I$(TEST_INCLUDE) $(wildcard *.cpp) -o $@ $(TEST_LINK_FILES)

.PHONY: clean
clean:
	rm -f $(TEST_FILE)