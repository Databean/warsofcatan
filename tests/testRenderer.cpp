#include <iostream>

#include "gtest/gtest.h"

#include "Renderer.h"

using std::make_pair;

// fix for travis
#if (!__GNUC__) || ((__GNUC__ == 4 && __GNUC_MINOR__ >= 8) || (__GNUC__ >= 5))
using std::tuple;
using std::get;
#else
using ::testing::tuple;
using ::testing::get;
#endif

class CoordinateConversionTest : public ::testing::TestWithParam<tuple<int, int>> {
	
};

TEST_P(CoordinateConversionTest, IsReversable) {
	auto pair = make_pair(get<0>(GetParam()), get<1>(GetParam()));
	ASSERT_EQ(pair, screenToCoord(coordToScreen(pair)));
}

INSTANTIATE_TEST_CASE_P(LargeSquare, CoordinateConversionTest, ::testing::Combine(::testing::Range(-10, 10), ::testing::Range(-10, 10)));
