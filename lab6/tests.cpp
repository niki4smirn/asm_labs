#include <gtest/gtest.h>

extern "C" char* AsmStrChr(const char* s, char c);

// ---------------------------------------------------------

#include <algorithm>
#include <random>

template<typename T>
class RandomGenerator {
 public:
  RandomGenerator() = default;
  RandomGenerator(T min_value, T max_value);

  T GetValue();
  std::vector<T> GetArray(size_t size);

 private:
  std::mt19937 gen{std::random_device()()};
  std::uniform_int_distribution<T> distribution;
};

template<typename T>
RandomGenerator<T>::RandomGenerator(T min_value, T max_value) :
    distribution(std::uniform_int_distribution<T>(min_value, max_value)) {}

template<typename T>
T RandomGenerator<T>::GetValue() {
  return distribution(gen);
}

template<typename T>
std::vector<T> RandomGenerator<T>::GetArray(size_t size) {
  std::vector<T> res(size);
  for (auto& el : res) {
    el = GetValue();
  }
  return res;
}


// ---------------------------------------------------------

TEST(AsmStrChr, Simple) {
  const char* str = "abacabe";

  EXPECT_EQ(AsmStrChr(str, 'a') - str, 0);
  EXPECT_EQ(AsmStrChr(str, 'b') - str, 1);
  EXPECT_EQ(AsmStrChr(str, 'c') - str, 3);
  EXPECT_EQ(AsmStrChr(str, 'e') - str, 6);
}