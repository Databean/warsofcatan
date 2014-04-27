#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "Renderer.h"

using std::vector;
using std::make_pair;

class CoordinateConversionTest : public ::testing::TestWithParam<Coordinate> {
	
};

TEST_P(CoordinateConversionTest, IsReversable) {
	ASSERT_EQ(GetParam(), screenToCoord(coordToScreen(GetParam())));
}

vector<Coordinate> makeInputs() {
	vector<Coordinate> ret;
	for(int x = -5; x < 5; x++) {
		for(int y = -5; y < 5; y++) {
			ret.push_back(Coordinate{x, y});
		}
	}
	return std::move(ret);
}

INSTANTIATE_TEST_CASE_P(LargeSquare, CoordinateConversionTest, ::testing::ValuesIn(makeInputs()));