#include "gtest/gtest.h"

#include <cmath>

extern "C" int64_t Sum(int32_t x, uint8_t y);
extern "C" bool CheckOverflow(int64_t x, int64_t y);
extern "C" int64_t ComputeFn(int64_t x, int64_t y);
extern "C" int64_t Clock(int32_t h, int32_t m, int32_t f);
// int64_t Clock(int32_t h, int32_t m, int32_t f) {
//   f *= 120;
//   int64_t deg = 3600 * h - 660 * m;
//   int64_t dist = f + deg;
//   dist += 43200;
//   dist %= 43200;
//   dist /= 11;
//   return dist;
// }
extern "C" uint64_t Polynom(int32_t x);

// ---------------------------------------------------------

#include <random>

class RandomGenerator {
 public:
  RandomGenerator() = default;
  RandomGenerator(int64_t min_value, int64_t max_value);

  int64_t GetValue();

 private:
  std::mt19937 gen{std::random_device()()};
  std::uniform_int_distribution<int64_t> distribution;
};

RandomGenerator::RandomGenerator(int64_t min_value, int64_t max_value) :
    distribution(std::uniform_int_distribution<int64_t>(min_value,
                                                        max_value)) {}

int64_t RandomGenerator::GetValue() {
  return distribution(gen);
}

TEST(Sum, Simple) {
  EXPECT_EQ(Sum(0, 0), 0);
  EXPECT_EQ(Sum(2, 2), 4);
}

TEST(Sum, EdgeCases) {
  EXPECT_EQ(Sum(2, 128), 130);
  EXPECT_EQ(Sum(42, 200), 242);
  EXPECT_EQ(Sum(std::numeric_limits<int32_t>::max(),
                std::numeric_limits<uint8_t>::max()),
            static_cast<int64_t>(std::numeric_limits<int32_t>::max()) +
                std::numeric_limits<uint8_t>::max());

  EXPECT_EQ(Sum(-100, 128), 28);
  EXPECT_EQ(Sum(-200, 128), -72);
  EXPECT_EQ(Sum(std::numeric_limits<int32_t>::min(),
                std::numeric_limits<uint8_t>::max()),
            static_cast<int64_t>(std::numeric_limits<int32_t>::min()) +
                std::numeric_limits<uint8_t>::max());
}

TEST(Sum, Generator) {
  RandomGenerator gen(std::numeric_limits<int32_t>::min(),
                      std::numeric_limits<int32_t>::max());
  const int kTestsCount = 1e5;
  for (int _ = 0; _ < kTestsCount; ++_) {
    for (int y = 0; y <= std::numeric_limits<uint8_t>::max(); ++y) {
      int64_t x = gen.GetValue();
      ASSERT_EQ(Sum(x, y), x + y) << x << ' ' << y;
    }
  }
}

// ---------------------------------------------------------
namespace overflow {
namespace edge {
const int kFourth = 55108;
const int64_t kSecond = 3037000499;
}
namespace min_over {
const int kFourth = edge::kFourth + 1;
const int64_t kSecond = edge::kSecond + 1;
}
}

bool IsInBounds(int64_t value, int64_t left_bound, int64_t right_bound) {
  return left_bound <= value && value <= right_bound;
}

TEST(CheckOverflow, XFourthPower) {
  EXPECT_EQ(CheckOverflow(overflow::edge::kFourth, 0), 0);
  EXPECT_EQ(CheckOverflow(overflow::min_over::kFourth, 0), 1);
  EXPECT_EQ(CheckOverflow(-overflow::edge::kFourth, 0), 0);
  EXPECT_EQ(CheckOverflow(-overflow::min_over::kFourth, 0), 1);
  EXPECT_EQ(CheckOverflow(100000, 0), 1);
  EXPECT_EQ(CheckOverflow(std::numeric_limits<int64_t>::min(), 0), 1);
  EXPECT_EQ(CheckOverflow(std::numeric_limits<int64_t>::max(), 0), 1);
}

TEST(CheckOverflow, YSecondPower) {

  EXPECT_EQ(CheckOverflow(0, overflow::edge::kSecond), 0);
  EXPECT_EQ(CheckOverflow(0, overflow::min_over::kSecond), 1);
  EXPECT_EQ(CheckOverflow(0, -overflow::edge::kSecond), 0);
  EXPECT_EQ(CheckOverflow(0, -overflow::min_over::kSecond), 1);
  EXPECT_EQ(CheckOverflow(0, std::numeric_limits<int64_t>::min()), 1);
  EXPECT_EQ(CheckOverflow(0, std::numeric_limits<int64_t>::max()), 1);
}

void OverflowGeneratorTest(int32_t min_val, int32_t max_val) {
  RandomGenerator gen(min_val, max_val);
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen.GetValue();
    int64_t y = gen.GetValue();
    bool ans = false;
    ans |= !IsInBounds(x, -overflow::edge::kFourth, overflow::edge::kFourth);
    ans |= !IsInBounds(y, -overflow::edge::kSecond, overflow::edge::kSecond);
    ASSERT_EQ(CheckOverflow(x, y), ans) << x << ' ' << y;
  }
}

TEST(CheckOverflow, SmallGenerator) {
  const int kAbsLimit = 2e5;
  OverflowGeneratorTest(-kAbsLimit, kAbsLimit);

}

TEST(CheckOverflow, Generator32) {
  OverflowGeneratorTest(std::numeric_limits<int32_t>::min(),
                        std::numeric_limits<int32_t>::max());
}

// ---------------------------------------------------------

TEST(ComputeFn, Simple) {
  EXPECT_EQ(ComputeFn(2, 1), 45);
  EXPECT_EQ(ComputeFn(0, 1), -9);
  EXPECT_EQ(ComputeFn(10, 5), 1117);
}

void ComputeFnGeneratorTest(
    int64_t min_val_x,
    int64_t max_val_x,
    int64_t min_val_y,
    int64_t max_val_y) {
  RandomGenerator gen_x(min_val_x, max_val_x);
  RandomGenerator gen_y(min_val_y, max_val_y);
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen_x.GetValue();
    int64_t y = gen_y.GetValue();
    if (x - y * y != 0) {
      int64_t res =
          (2 + x * x - y * y * y) * (y * y + 2) * (y * y + 2) / (x - y * y);
      ASSERT_EQ(ComputeFn(x, y), res) << x << ' ' << y;
    }
  }
}

TEST(ComputeFn, SmallGenerator) {
  ComputeFnGeneratorTest(-10, 10, -10, 10);
}

TEST(ComputeFn, Generator100) {
  ComputeFnGeneratorTest(-100, 100, -100, 100);
}

TEST(ComputeFn, Generator) {
  ComputeFnGeneratorTest(
      -10000,
      10000,
      -450,
      450);
}
// ---------------------------------------------------------

uint64_t RightPolynom(int32_t x) {
  uint64_t x64 = x;
  return (((2 * x64 - 3) * x64 + 4) * x64 - 5) * x64 + 6;
}

TEST(Polynom, Simple) {
  EXPECT_EQ(Polynom(0), 6);
  EXPECT_EQ(Polynom(1), 4);
  EXPECT_EQ(Polynom(-1), RightPolynom(-1));
}

void PolynomGeneratorTest(
    int64_t min_val_x,
    int64_t max_val_x) {
  RandomGenerator gen(min_val_x, max_val_x);
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen.GetValue();
    ASSERT_EQ(Polynom(x), RightPolynom(x)) << x;
  }
}

TEST(Polynom, SmallGenerator) {
  PolynomGeneratorTest(-10, 10);
}

TEST(Polynom, Generator100) {
  PolynomGeneratorTest(-100, 100);
}

TEST(Polynom, Generator32) {
  PolynomGeneratorTest(std::numeric_limits<int32_t>::min(),
                       std::numeric_limits<int32_t>::max());
}

// ---------------------------------------------------------

bool CheckClock(float h, float m, float need) {
  float ans = Clock(h, m, need);
  float ans_copy = ans;
  m += ans / 60;
  while (m >= 60) {
    m -= 60;
    ++h;
  }
  float deg = 5.5f * m - 30 * h;
  while (deg < 0) {
    deg += 360;
  }
  return std::fabs(deg - need) < 0.5 || std::fabs(deg - need - 360) < 0.5;
}

TEST(Clock, Generator) {
  for (int h = 0; h < 12; ++h) {
    for (int m = 0; m < 60; ++m) {
      for (int deg = 0; deg < 360; ++deg) {
        ASSERT_TRUE(CheckClock(h, m, deg)) << h << ' ' << m << ' ' << deg;
      }
    }
  }
}

// ---------------------------------------------------------
