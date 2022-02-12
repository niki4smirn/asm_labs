#include "gtest/gtest.h"

extern "C" int64_t Sum(int32_t x, uint8_t y);
extern "C" bool CheckOverflow(int64_t x, int64_t y);
extern "C" int64_t ComputeFn(int64_t x, int64_t y);
extern "C" int64_t Clock(int32_t h, int32_t m, int32_t f);
extern "C" int64_t Polynom(int32_t x);

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
  ASSERT_EQ(Sum(0, 0), 0);
  ASSERT_EQ(Sum(2, 2), 4);
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
      EXPECT_EQ(Sum(x, y), x + y);
    }
  }
}

// ---------------------------------------------------------
namespace overflow {
namespace edge {
const int kX = 55108;
const int64_t kY = 3037000499;
}
namespace min_over {
const int kX = edge::kX + 1;
const int64_t kY = edge::kY + 1;
}
}

bool IsInBounds(int64_t value, int64_t left_bound, int64_t right_bound) {
  return left_bound <= value && value <= right_bound;
}

TEST(CheckOverflow, XFourthPower) {
  EXPECT_EQ(CheckOverflow(overflow::edge::kX, 0), 0);
  EXPECT_EQ(CheckOverflow(overflow::min_over::kX, 0), 1);
  EXPECT_EQ(CheckOverflow(-overflow::edge::kX, 0), 0);
  EXPECT_EQ(CheckOverflow(-overflow::min_over::kX, 0), 1);
  EXPECT_EQ(CheckOverflow(100000, 0), 1);
  EXPECT_EQ(CheckOverflow(std::numeric_limits<int64_t>::min(), 0), 1);
  EXPECT_EQ(CheckOverflow(std::numeric_limits<int64_t>::max(), 0), 1);
}

TEST(CheckOverflow, YSecondPower) {

  EXPECT_EQ(CheckOverflow(0, overflow::edge::kY), 0);
  EXPECT_EQ(CheckOverflow(0, overflow::min_over::kY), 1);
  EXPECT_EQ(CheckOverflow(0, -overflow::edge::kY), 0);
  EXPECT_EQ(CheckOverflow(0, -overflow::min_over::kY), 1);
  EXPECT_EQ(CheckOverflow(0, std::numeric_limits<int64_t>::min()), 1);
  EXPECT_EQ(CheckOverflow(0, std::numeric_limits<int64_t>::max()), 1);
}

void OverflowGeneratorTest(RandomGenerator gen) {
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = gen.GetValue();
    int64_t y = gen.GetValue();
    bool ans = false;
    ans |= !IsInBounds(x, -overflow::edge::kX, overflow::edge::kX);
    ans |= !IsInBounds(y, -overflow::edge::kY, overflow::edge::kY);
    ASSERT_EQ(CheckOverflow(x, y), ans);
  }
}

TEST(CheckOverflow, Generator) {
  const int kAbsLimit = 2e5;
  OverflowGeneratorTest(RandomGenerator(-kAbsLimit, kAbsLimit));

}

TEST(CheckOverflow, Generator32) {
  OverflowGeneratorTest(RandomGenerator(std::numeric_limits<int32_t>::min(),
                                        std::numeric_limits<int32_t>::max()));
}

// ---------------------------------------------------------
