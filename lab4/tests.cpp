#include "gtest/gtest.h"

extern "C" int32_t AsmProduct(
    const int32_t* array, int32_t size, int32_t module);

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
