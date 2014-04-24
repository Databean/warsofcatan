#include <iostream>

#include "gtest/gtest.h"

#include "Renderer.h"

using std::cout;
using std::endl;
using std::make_pair;

TEST(RendererTest, coordToScreen) {
	for(int x = -5; x < 5; x++) {
		for(auto y = -5; y < 5; y++) {
			auto original = make_pair(x, y);
			auto screen = coordToScreen(original);
			auto back = screenToCoord(screen);
			ASSERT_EQ(original.first, back.first);
			ASSERT_EQ(original.second, back.second);
		}
	}
}
