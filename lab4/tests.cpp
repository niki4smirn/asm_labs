#include "gtest/gtest.h"

extern "C" int32_t AsmProduct(
    const int32_t* array, int32_t size, int32_t module);
extern "C" int64_t AsmSpecialSum(
    const int64_t* array, int64_t size, int64_t module);
extern "C" int64_t AsmArrayFormula(
    const int32_t* array, int32_t size);
extern "C" int64_t AsmCompare(
    const int64_t* array1, int64_t size1,
    const int64_t* array2, int64_t size2);
extern "C" void AsmSimpleModify(int32_t* array, int32_t size);

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

namespace helpers {
template<typename T>
std::string ArrToStr(const T* array, T size) {
  std::string result;
  for (int i = 0; i < size; ++i) {
    result += std::to_string(array[i]);
    if (i + 1 != size) {
      result += ", ";
    }
  }
  return result;
}

bool AreEqual(
    const int32_t* array1, int32_t size1,
    const int32_t* array2, int32_t size2) {
  if (size1 != size2) {
    return false;
  }
  for (int i = 0; i < size1; ++i) {
    if (array1[i] != array2[i]) {
      return false;
    }
  }
  return true;
}
}  // namespace helpers

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
              RightProduct(array, size, module))
              << helpers::ArrToStr(array, size) << ' ' << module;
  }
}

// ---------------------------------------------------------

int64_t RightSpecialSum(const int64_t* array, int64_t size, int64_t module) {
  int64_t ans = 0;
  for (int i = 0; i < size; ++i) {
    int mod3 = array[i] % 3;
    if (mod3 < 0) {
      mod3 += 3;
    }
    int mod5 = array[i] % 5;
    if (mod5 < 0) {
      mod5 += 5;
    }
    if ((mod3 & 1) != 0 && (mod5 & 1) != 0) {
      int64_t val = array[i] % module;
      val += module;
      val %= module;
      ans += val;
      ans %= module;
      ans += module;
      ans %= module;
    }
  }
  return ans;
}


TEST(AsmSpecialSum, Simple) {
  {
    auto* array = new int64_t[]{-13};
    ASSERT_EQ(AsmSpecialSum(array, 1, 1000), 0);
    delete[] array;
  }
  {
    auto* array = new int64_t[]{-3, 5, -5, 3};
    ASSERT_EQ(AsmSpecialSum(array, 4, 2), 0);
    delete[] array;
  }
  {
    auto* array = new int64_t[]{1, 13};
    ASSERT_EQ(AsmSpecialSum(array, 2, 15), 14);
    delete[] array;
  }
  {
    auto* array = new int64_t[]{1, 6, 3, 7, 13};
    ASSERT_EQ(AsmSpecialSum(array, 5, 15), 14);
    delete[] array;
  }
}

TEST(AsmSpecialSum, Generator) {
  const int kTestsCount = 1e4;
  RandomGenerator<int64_t> size_gen{1, 1000};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t size = size_gen.GetValue();
    auto* array = new int64_t[size];
    int64_t bound_value = std::numeric_limits<int64_t>::max();
    RandomGenerator<int64_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      array[i] = array_gen.GetValue();
    }
    RandomGenerator<int64_t> module_generator{2, (1LL << 50)};
    int64_t module = module_generator.GetValue();
    ASSERT_EQ(AsmSpecialSum(array, size, module),
              RightSpecialSum(array, size, module))
              << helpers::ArrToStr(array, size) << ' ' << module;
  }
}

// ---------------------------------------------------------

extern "C" int64_t RightArrayFormula(
    const int32_t* array, int32_t size) {
  int64_t res = 0;
  for (int i = 0; i < size; i += 2) {
    int64_t cur = (i + 1) * array[i] * (i + 2) * array[i + 1];
    if (i % 4 == 0) {
      res += cur;
    } else {
      res -= cur;
    }
  }
  return res;
}

TEST(AsmArrayFormula, Simple) {
  {
    ASSERT_EQ(AsmArrayFormula(nullptr, 0), 0);
  }
  {
    auto* array = new int32_t[]{1, 2, 3, 42};
    ASSERT_EQ(AsmArrayFormula(array, 4), -1508);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{100, 1};
    ASSERT_EQ(AsmArrayFormula(array, 2), 200);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{1, 1, 1, 1, 1, 1};
    ASSERT_EQ(AsmArrayFormula(array, 6), 20);
    delete[] array;
  }
  {
    auto* array = new int32_t[]{0, 2, 3, 0, 49, 0};
    ASSERT_EQ(AsmArrayFormula(array, 6), 0);
    delete[] array;
  }
}

TEST(AsmArrayFormula, Generator) {
  const int kTestsCount = 1e5;
  RandomGenerator<int32_t> size_gen{1, 8};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int32_t size = 2 * size_gen.GetValue();
    auto* array = new int32_t[size];
    int32_t bound_value = 10;
    RandomGenerator<int32_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      array[i] = array_gen.GetValue();
    }
    ASSERT_EQ(AsmArrayFormula(array, size),
              RightArrayFormula(array, size)) << helpers::ArrToStr(array, size);
  }
}

// ---------------------------------------------------------
int64_t RightCompare(
    const int64_t* array1, int64_t size1,
    const int64_t* array2, int64_t size2) {
  int64_t ans = 0;
  for (int i = 0; i < size1; ++i) {
    bool was = false;
    for (int j = 0; j < size2; ++j) {
      if (array1[i] == array2[j]) {
        was = true;
      }
    }
    if (!was) {
      ++ans;
    }
  }
  return ans;
}

TEST(AsmCompare, Simple) {
  {
    ASSERT_EQ(AsmCompare(nullptr, 0, nullptr, 0), 0);
  }
  {
    auto* array1 = new int64_t[]{1, 2, 3, 42};
    auto* array2 = new int64_t[]{1, 2, 3, 42};
    ASSERT_EQ(AsmCompare(array1, 4, array2, 4), 0);
    delete[] array1;
    delete[] array2;
  }
  {
    auto* array1 = new int64_t[]{1, 2, 3, 42};
    auto* array2 = new int64_t[]{1, 2, 3};
    ASSERT_EQ(AsmCompare(array1, 4, array2, 3), 1);
    delete[] array1;
    delete[] array2;
  }
  {
    auto* array1 = new int64_t[]{1, 2, 3, 42, 15};
    ASSERT_EQ(AsmCompare(array1, 5, nullptr, 0), 5);
    delete[] array1;
  }
}

TEST(AsmCompare, Generator) {
  const int kTestsCount = 1e5;
  RandomGenerator<int64_t> size_gen{1, 100};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t size1 = size_gen.GetValue();
    int64_t size2 = size_gen.GetValue();
    auto* array1 = new int64_t[size1];
    auto* array2 = new int64_t[size2];
    int64_t bound_value = std::numeric_limits<int64_t>::max();
    RandomGenerator<int64_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size1; ++i) {
      array1[i] = array_gen.GetValue();
    }
    for (int i = 0; i < size2; ++i) {
      array2[i] = array_gen.GetValue();
    }
    ASSERT_EQ(AsmCompare(array1, size1, array2, size2),
              RightCompare(array1, size1, array2, size2))
              << helpers::ArrToStr(array1, size1) << '\n'
              << helpers::ArrToStr(array2, size2);
  }
}

// ---------------------------------------------------------

void RightSimpleModify(int32_t* array, int32_t size) {
  for (int i = 0; i < size; ++i) {
    int32_t mod5 = array[i] % 5;
    if (mod5 < 0)  {
      mod5 += 5;
    }
    if (mod5 == 0) {
      array[i] = 0;
    } else if ((mod5 & 1) == 0) {
        array[i] = 1;
    } else {
      array[i] = -1;
    }
  }
}

TEST(AsmSimpleModify, Simple) {
  {
    int32_t* array = nullptr;
    EXPECT_NO_FATAL_FAILURE(AsmSimpleModify(array, 0));
  }
  {
    auto* array = new int32_t[]{-1};
    auto* ans = new int32_t[]{1};
    int32_t size = 1;
    AsmSimpleModify(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
  }
  {
    auto* array = new int32_t[]{1, 2, 5};
    auto* ans = new int32_t[]{-1, 1, 0};
    int32_t size = 3;
    AsmSimpleModify(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
  }
  {
    auto* array = new int32_t[]{1, 2, 3, 4, 5, 6, 7, 8};
    auto* ans = new int32_t[]{-1, 1, -1, 1, 0, -1, 1, -1};
    int32_t size = 8;
    AsmSimpleModify(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
        << helpers::ArrToStr(array, size) << '\n'
        << helpers::ArrToStr(ans, size);;
  }
}

TEST(AsmSimpleModify, Generator) {
  const int kTestsCount = 1e5;
  RandomGenerator<int32_t> size_gen{1, 100};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int32_t size = size_gen.GetValue();
    auto* non_modified_array = new int32_t[size];
    auto* array1 = new int32_t[size];
    auto* array2 = new int32_t[size];
    int32_t bound_value = std::numeric_limits<int32_t>::max();
    RandomGenerator<int32_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      non_modified_array[i] = array2[i] = array1[i] = array_gen.GetValue();
    }
    AsmSimpleModify(array1, size);
    RightSimpleModify(array2, size);
    ASSERT_TRUE(helpers::AreEqual(array1, size, array2, size))
                  << helpers::ArrToStr(non_modified_array, size) << '\n'
                  << helpers::ArrToStr(array1, size) << '\n'
                  << helpers::ArrToStr(array2, size);
  }
}
