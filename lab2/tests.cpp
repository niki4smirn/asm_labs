#include <gtest/gtest.h>

extern "C" const char* Positivity(int64_t x);
extern "C" bool CheckOverflow(int64_t x, int64_t y);
extern "C" int64_t Function(int64_t x);
extern "C" bool IsInCircle(int64_t x, int64_t y, int64_t d);
extern "C" bool TwoIntegers(int64_t x, int64_t y);
extern "C" int8_t CountNumbers(int64_t a, int64_t b, int64_t c);
extern "C" const char* Switch(int64_t k);
extern "C" int8_t MagicMetric(int64_t x);

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

namespace positivity_responses {
const char* positive = "Yeah) It's positive :D";
const char* negative = "Ohh.. so much negative :(";
}

TEST(Positivity, Simple) {
  EXPECT_STREQ(Positivity(1), positivity_responses::positive);
  EXPECT_STREQ(Positivity(-1), positivity_responses::negative);
  EXPECT_STREQ(Positivity(0), positivity_responses::negative);
}

TEST(Positivity, Generator) {
  RandomGenerator gen(std::numeric_limits<int64_t>::min(),
                      std::numeric_limits<int64_t>::max());
  const int kTestsCount = 1e5;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen.GetValue();
    if (x > 0) {
      EXPECT_STREQ(Positivity(x), positivity_responses::positive);
    } else {
      EXPECT_STREQ(Positivity(x), positivity_responses::negative);
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

TEST(CheckOverflow, SquareExpression) {
  ASSERT_EQ(CheckOverflow(overflow::edge::kFourth,
                          overflow::edge::kSecond), 1);
}

void OverflowGeneratorTest(int64_t min_val, int64_t max_val) {
  RandomGenerator gen(min_val, max_val);
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen.GetValue();
    int64_t y = gen.GetValue();
    bool ans = false;
    ans |= !IsInBounds(x, -overflow::edge::kFourth, overflow::edge::kFourth);
    ans |= !IsInBounds(y, -overflow::edge::kSecond, overflow::edge::kSecond);
    if (!ans && x + y != 0) {
      ASSERT_FALSE(CheckOverflow(x, y))  << x << ' ' << y;
    }
    if (x + y == 0) {
      ASSERT_TRUE(CheckOverflow(x, y))  << x << ' ' << y;
    }
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

TEST(CheckOverflow, Generator64) {
  OverflowGeneratorTest(std::numeric_limits<int64_t>::min(),
                        std::numeric_limits<int64_t>::max());
}

// ---------------------------------------------------------

TEST(Function, Simple) {
  ASSERT_EQ(Function(-2), 5);
  ASSERT_EQ(Function(0), 4);
  ASSERT_EQ(Function(1), 5);
  ASSERT_EQ(Function(4), -10);
  ASSERT_EQ(Function(18), -5834);
}

TEST(Function, Generator) {
  const int kAbsLimit = 2e5;
  RandomGenerator gen(-kAbsLimit, kAbsLimit);
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen.GetValue();
    int64_t res;
    if (x < -1) {
      res = 2 * x * x - 3;
    } else if (x <= 17) {
      double val = (36 - x * x) * 1.0 / (10 - 3 * x);
      if (val < 0) {
        res = std::floor(val);
      } else {
        res = std::ceil(val);
      }
    } else {
      res = - x * x * x - 2;
    }
    ASSERT_EQ(Function(x), res) << x;
  }
}

// ---------------------------------------------------------
