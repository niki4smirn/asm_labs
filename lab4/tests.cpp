#include "gtest/gtest.h"

extern "C" int32_t AsmProduct(
    const int32_t* array, int32_t size, int32_t module);

// ---------------------------------------------------------

#include <algorithm>
#include <random>

template<typename T>
class RandomGenerator {
 public:
  RandomGenerator() = default;
  RandomGenerator(T min_value, T max_value);

  T GetValue();

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

// ---------------------------------------------------------

int32_t RightProduct(
    const int32_t* array, int32_t size, int32_t module) {
  int64_t ans = 1;
  for (int i = 0; i < size; ++i) {
    ans *= array[i];
    ans %= module;
    ans += module;
    ans %= module;
  }
  return ans;
}

TEST(AsmProduct, Simple) {
  {
    auto* array = new int32_t[]{1, 2, 3, 42};
    ASSERT_EQ(AsmProduct(array, 4, 2), 0);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{1, 3, 3, 49};
    ASSERT_EQ(AsmProduct(array, 4, 2), 1);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{1, 2, 3, 49};
    ASSERT_EQ(AsmProduct(array, 4, 15), 9);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{1, 2, 3, 0, 49, 215125, 1436};
    ASSERT_EQ(AsmProduct(array, 7, 2364), 0);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{1, -2, 4, -5, 6, -15};
    ASSERT_EQ(AsmProduct(array, 6, 17), 4);
    delete[] array;
  }
}

std::string ArrToStr(const int32_t* array, int32_t size) {
  std::string result;
  for (int i = 0; i < size; ++i) {
    result += std::to_string(array[i]);
    if (i + 1 != size) {
      result += ", ";
    }
  }
  return result;
}

TEST(AsmProduct, Generator) {
  const int kTestsCount = 1e4;
  RandomGenerator<int32_t> size_gen{1, 1000};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int32_t size = size_gen.GetValue();
    auto* array = new int32_t[size];
    int32_t bound_value = std::numeric_limits<int32_t>::max();
    RandomGenerator<int32_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      array[i] = array_gen.GetValue();
    }
    RandomGenerator<int32_t> module_generator{2, (1 << 30)};
    int32_t module = module_generator.GetValue();
    ASSERT_EQ(AsmProduct(array, size, module),
              RightProduct(array, size, module)) << ArrToStr(array, size)
                                                 << ' ' << module;
  }
}
