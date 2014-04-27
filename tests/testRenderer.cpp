#include <iostream>
#include <tuple>

#define GTEST_USE_OWN_TR1_TUPLE 0
#define GTEST_ENV_HAS_STD_TUPLE_ 1

#include "gtest/gtest.h"

#include "Renderer.h"

using std::make_pair;

using std::tuple;
using std::get;

class CoordinateConversionTest : public ::testing::TestWithParam<tuple<int, int>> {
	
};

TEST_P(CoordinateConversionTest, IsReversable) {
	auto pair = make_pair(get<0>(GetParam()), get<1>(GetParam()));
	ASSERT_EQ(pair, screenToCoord(coordToScreen(pair)));
}

INSTANTIATE_TEST_CASE_P(LargeSquare, CoordinateConversionTest, ::testing::Combine(::testing::Range(-10, 10), ::testing::Range(-10, 10)));
