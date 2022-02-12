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
  RandomGenerator(int32_t min_value, int32_t max_value);

  int32_t GetValue();

 private:
  std::mt19937 gen{std::random_device()()};
  std::uniform_int_distribution<int32_t> distribution;
};

RandomGenerator::RandomGenerator(int32_t min_value, int32_t max_value) :
    distribution(std::uniform_int_distribution<int32_t>(min_value,
                                                        max_value)) {}

int32_t RandomGenerator::GetValue() {
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
  const int kTestsCount = 100000;
  for (int _ = 0; _ < kTestsCount; ++_) {
    for (int y = 0; y <= std::numeric_limits<uint8_t>::max(); ++y) {
      int64_t x = gen.GetValue();
      EXPECT_EQ(Sum(x, y), x + y);
    }
  }
}

// ---------------------------------------------------------
