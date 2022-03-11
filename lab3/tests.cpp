#include "gtest/gtest.h"

extern "C" int32_t AsmBitCount(uint64_t n);
extern "C" int64_t AsmFactorial(int32_t n);
extern "C" bool AsmIsSquare(int64_t x);
extern "C" int32_t AsmRemoveDigits(int32_t x);
extern "C" int64_t AsmFormula(int64_t x, int64_t n);
extern "C" uint64_t AsmBankDeposit(
    uint64_t initial_sum, int32_t percentage, int32_t years);
extern "C" uint16_t AsmEvenDivisors(uint16_t n);
extern "C" uint64_t AsmInfiniteManipulations(uint64_t x);

// ---------------------------------------------------------

#include <algorithm>
#include <random>

class RandomGenerator {
 public:
  RandomGenerator() = default;
  RandomGenerator(uint64_t min_value, uint64_t max_value);

  uint64_t GetValue();

 private:
  std::mt19937 gen{std::random_device()()};
  std::uniform_int_distribution<uint64_t> distribution;
};

RandomGenerator::RandomGenerator(uint64_t min_value, uint64_t max_value) :
    distribution(std::uniform_int_distribution<uint64_t>(min_value,
                                                        max_value)) {}

uint64_t RandomGenerator::GetValue() {
  return distribution(gen);
}

TEST(AsmBitCount, Simple) {
  ASSERT_EQ(AsmBitCount(0), 0);
  ASSERT_EQ(AsmBitCount(1), 1);
  ASSERT_EQ(AsmBitCount(5), 2);
  ASSERT_EQ(AsmBitCount(std::numeric_limits<uint64_t>::max()), 64);
}

TEST(AsmBitCount, Generator) {
  RandomGenerator gen(std::numeric_limits<uint64_t>::min(),
                      std::numeric_limits<uint64_t>::max());
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = gen.GetValue();
    ASSERT_EQ(AsmBitCount(x), __builtin_popcountll(x)) << x;
  }
}

// ---------------------------------------------------------