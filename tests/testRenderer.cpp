#include <iostream>

#include "gtest/gtest.h"

#include "Renderer.h"

using std::make_pair;

class CoordinateConversionTest : public ::testing::TestWithParam<std::tuple<int, int>> {
	
};

TEST_P(CoordinateConversionTest, IsReversable) {
	auto pair = make_pair(std::get<0>(GetParam()), std::get<1>(GetParam()));
	ASSERT_EQ(pair, screenToCoord(coordToScreen(pair)));
}

INSTANTIATE_TEST_CASE_P(LargeSquare, CoordinateConversionTest, ::testing::Combine(::testing::Range(-10, 10), ::testing::Range(-10, 10)));
