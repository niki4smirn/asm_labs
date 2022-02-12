#include "gtest/gtest.h"

extern "C" int64_t Sum(int32_t x, uint8_t y);
extern "C" bool CheckOverflow(int64_t x, int64_t y);
extern "C" int64_t ComputeFn(int64_t x, int64_t y);
extern "C" int64_t Clock(int32_t h, int32_t m, int32_t f);
extern "C" int64_t Polynom(int32_t x);

// ---------------------------------------------------------

// YOUR_TESTS_HERE

TEST(Sum, Simple) {
  ASSERT_EQ(Sum(0, 0), 0);
  ASSERT_EQ(Sum(2, 2), 4);
}

// ---------------------------------------------------------
