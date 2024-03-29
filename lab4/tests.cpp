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
extern "C" void AsmSetToSequence(int64_t* array, int32_t size);
extern "C" void AsmRotateInGroups(
    int64_t* array, int32_t size, int32_t k);
extern "C" int32_t AsmRemoveIfSimilar(
    int64_t* array, int32_t size,
    int64_t x, int64_t d);
extern "C" void AsmReplaceWithGroup(
    int64_t* array, int32_t* size, int32_t k);
extern "C" void AsmMerge(
    const int64_t* array1, int64_t size1,
    const int64_t* array2, int64_t size2,
    int64_t* result);
extern "C" void AsmModify2D(
    int64_t** array, int64_t n, int64_t m);
extern "C" int32_t AsmFindSorted(
    const int32_t** array, int32_t n, int32_t m);

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
std::string ArrToStr(const T* array, int32_t size) {
  std::string result;
  for (int32_t i = 0; i < size; ++i) {
    result += std::to_string(array[i]);
    if (i + 1 != size) {
      result += ", ";
    }
  }
  return result;
}

template<typename T>
bool AreEqual(
    const T* array1, int32_t size1,
    const T* array2, int32_t size2) {
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
    delete[] array;
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
    delete[] array;
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
    delete[] array;
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
    delete[] array1;
    delete[] array2;
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
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int32_t[]{1, 2, 5};
    auto* ans = new int32_t[]{-1, 1, 0};
    int32_t size = 3;
    AsmSimpleModify(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int32_t[]{1, 2, 3, 4, 5, 6, 7, 8};
    auto* ans = new int32_t[]{-1, 1, -1, 1, 0, -1, 1, -1};
    int32_t size = 8;
    AsmSimpleModify(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
        << helpers::ArrToStr(array, size) << '\n'
        << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
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
    delete[] non_modified_array;
    delete[] array1;
    delete[] array2;
  }
}

// ---------------------------------------------------------

void RightSetToSequence(int64_t* array, int32_t size) {
  int64_t min = array[0];
  int64_t max = array[0];
  int32_t min_pos = 0;
  int32_t max_pos = 0;
  for (int i = 0; i < size; ++i) {
    if (min > array[i]) {
      min = array[i];
      min_pos = i;
    }
    if (max < array[i]) {
      max = array[i];
      max_pos = i;
    }
  }
  if (max_pos < min_pos) {
    std::swap(min_pos, max_pos);
  }
  for (int i = min_pos; i <= max_pos; ++i) {
    array[i] = i - min_pos + 1;
  }
}

TEST(AsmSetToSequence, Simple) {
  {
    auto* array = new int64_t[]{-1524};
    auto* ans = new int64_t[]{1};
    int32_t size = 1;
    AsmSetToSequence(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{5, 0, 1, 2, 3, 6, 4};
    auto* ans = new int64_t[]{5, 1, 2, 3, 4, 5, 4};
    int32_t size = 7;
    AsmSetToSequence(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{4, 6, 3, 2, 1, 0, 5};
    auto* ans = new int64_t[]{4, 1, 2, 3, 4, 5, 5};
    int32_t size = 7;
    AsmSetToSequence(array, size);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
}

TEST(AsmSetToSequence, Generator) {
  const int kTestsCount = 1e5;
  RandomGenerator<int32_t> size_gen{1, 100};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int32_t size = size_gen.GetValue();
    auto* non_modified_array = new int64_t[size];
    auto* array1 = new int64_t[size];
    auto* array2 = new int64_t[size];
    auto bound_value = std::numeric_limits<int64_t>::max();
    RandomGenerator<int64_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      non_modified_array[i] = array2[i] = array1[i] = array_gen.GetValue();
    }
    AsmSetToSequence(array1, size);
    RightSetToSequence(array2, size);
    ASSERT_TRUE(helpers::AreEqual(array1, size, array2, size))
                  << helpers::ArrToStr(non_modified_array, size) << '\n'
                  << helpers::ArrToStr(array1, size) << '\n'
                  << helpers::ArrToStr(array2, size);
    delete[] non_modified_array;
    delete[] array1;
    delete[] array2;
  }
}

// ---------------------------------------------------------

void RightRotateInGroups(int64_t* array, int32_t size, int32_t k) {
  for (int32_t i = 0; i < size; i += k) {
    int32_t bound = std::min(size, i + k);
    int64_t first = array[i];
    for (int32_t j = i; j < bound; ++j) {
      array[j] = array[j + 1];
    }
    array[bound - 1] = first;
  }
}

TEST(AsmRotateInGroups, Simple) {
  {
    int64_t* array = nullptr;
    int32_t size = 0;
    int32_t k = 228;
    EXPECT_NO_FATAL_FAILURE(AsmRotateInGroups(array, size, k));
  }
  {
    auto* array = new int64_t[]{1, 2, 3, 4, 5};
    auto* ans = new int64_t[]{1, 2, 3, 4, 5};
    int32_t size = 5;
    int32_t k = 1;
    AsmRotateInGroups(array, size, k);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{1, 2, 3, 4, 5};
    auto* ans = new int64_t[]{2, 1, 4, 3, 5};
    int32_t size = 5;
    int32_t k = 2;
    AsmRotateInGroups(array, size, k);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{1, 2, 3, 4, 5, 6, 7};
    auto* ans = new int64_t[]{2, 3, 1, 5, 6, 4, 7};
    int32_t size = 7;
    int32_t k = 3;
    AsmRotateInGroups(array, size, k);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{1, 2, 3, 4, 5, 6, 7, 8};
    auto* ans = new int64_t[]{2, 3, 1, 5, 6, 4, 8, 7};
    int32_t size = 8;
    int32_t k = 3;
    AsmRotateInGroups(array, size, k);
    EXPECT_TRUE(helpers::AreEqual(array, size, ans, size))
              << helpers::ArrToStr(array, size) << '\n'
              << helpers::ArrToStr(ans, size);
    delete[] array;
    delete[] ans;
  }
}

TEST(AsmRotateInGroups, Generator) {
  const int kTestsCount = 1e5;
  RandomGenerator<int32_t> size_gen{1, 100};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int32_t size = size_gen.GetValue();
    int32_t k = size_gen.GetValue();
    auto* non_modified_array = new int64_t[size];
    auto* array1 = new int64_t[size];
    auto* array2 = new int64_t[size];
    auto bound_value = std::numeric_limits<int64_t>::max();
    RandomGenerator<int64_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      non_modified_array[i] = array2[i] = array1[i] = array_gen.GetValue();
    }
    AsmRotateInGroups(array1, size, k);
    RightRotateInGroups(array2, size, k);
    ASSERT_TRUE(helpers::AreEqual(array1, size, array2, size))
                  << helpers::ArrToStr(non_modified_array, size) << '\n'
                  << helpers::ArrToStr(array1, size) << '\n'
                  << helpers::ArrToStr(array2, size) << '\n'
                  << k;
    delete[] non_modified_array;
    delete[] array1;
    delete[] array2;
  }
}

// ---------------------------------------------------------

int32_t RightRemoveIfSimilar(
    int64_t* array, int32_t size,
    int64_t x, int64_t d) {
  int32_t insert_pos = 0;
  for (int i = 0; i < size; ++i) {
    int64_t cur_val = array[i];
    if (x > cur_val + d ||
        cur_val - d > x ||
        cur_val < 0 ||
        (cur_val & 1) == 0) {
      array[insert_pos] = cur_val;
      ++insert_pos;
    }
  }
  return insert_pos;
}

TEST(AsmRemoveIfSimilar, Simple) {
  {
    int64_t* array = nullptr;
    int32_t size = 0;
    int64_t x = 228;
    int64_t d = 42;
    EXPECT_NO_FATAL_FAILURE(AsmRemoveIfSimilar(array, size, x, d));
  }
  {
    auto* array = new int64_t[]{2, 4};
    auto* ans = new int64_t[]{2, 4};
    int32_t size = 2;
    int32_t ans_size = 2;
    int64_t x = 4;
    int64_t d = 2;
    EXPECT_EQ(AsmRemoveIfSimilar(array, size, x, d), ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{3};
    auto* ans = new int64_t[]{};
    int32_t size = 1;
    int32_t ans_size = 0;
    int64_t x = 3;
    int64_t d = 0;
    EXPECT_EQ(AsmRemoveIfSimilar(array, size, x, d), ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{1, 2, -3, 4};
    auto* ans = new int64_t[]{2, -3, 4};
    int32_t size = 4;
    int32_t ans_size = 3;
    int64_t x = 5;
    int64_t d = 1000;
    EXPECT_EQ(AsmRemoveIfSimilar(array, size, x, d), ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{1, 2, 3, 4};
    auto* ans = new int64_t[]{1, 2, 4};
    int32_t size = 4;
    int32_t ans_size = 3;
    int64_t x = 4;
    int64_t d = 2;
    EXPECT_EQ(AsmRemoveIfSimilar(array, size, x, d), ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[]{3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8, 2, 4, 6};
    auto* ans = new int64_t[]{2, 2, 4, 6, 7, 8, 2, 4, 6};
    int32_t size = 14;
    int32_t ans_size = 9;
    int64_t x = 3;
    int64_t d = 2;
    EXPECT_EQ(AsmRemoveIfSimilar(array, size, x, d), ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
}

TEST(AsmRemoveIfSimilar, Generator) {
  const int kTestsCount = 1e5;
  RandomGenerator<int32_t> size_gen{1, 100};
  for (int _ = 0; _ < kTestsCount; ++_) {
    int32_t size = size_gen.GetValue();
    auto* non_modified_array = new int64_t[size];
    auto* array1 = new int64_t[size];
    auto* array2 = new int64_t[size];
    auto bound_value = std::numeric_limits<int64_t>::max() / 3;
    RandomGenerator<int64_t> array_gen{-bound_value, bound_value};
    for (int i = 0; i < size; ++i) {
      non_modified_array[i] = array2[i] = array1[i] = array_gen.GetValue();
    }
    auto x = array_gen.GetValue();
    auto d = std::abs(array_gen.GetValue());

    int32_t new_size1 = AsmRemoveIfSimilar(array1, size, x, d);
    int32_t new_size2 = RightRemoveIfSimilar(array2, size, x, d);
    ASSERT_TRUE(helpers::AreEqual(array1, new_size1, array2, new_size2))
                  << helpers::ArrToStr(non_modified_array, size) << '\n'
                  << helpers::ArrToStr(array1, new_size1) << '\n'
                  << helpers::ArrToStr(array2, new_size2) << '\n'
                  << x << ' ' << d;
    delete[] non_modified_array;
    delete[] array1;
    delete[] array2;
  }
}

// ---------------------------------------------------------

void RightReplaceWithGroup(int64_t* array, int32_t* size, int32_t k) {
  int32_t new_size = 0;
  for (int i = 0; i < *size; ++i) {
    new_size += std::ceil(1.0 * array[i] / k);
  }
  int insert_pos = new_size - 1;
  for (int i = *size - 1; i >= 0; --i) {
    int64_t cur_val = array[i];
    int32_t count = std::ceil(1.0 * cur_val / k);
    while (count > 0) {
      array[insert_pos] = cur_val;
      --insert_pos;
      --count;
    }
  }
  *size = new_size;
}

TEST(AsmReplaceWithGroup, Simple) {
  {
    int64_t* array = nullptr;
    int32_t size = 0;
    int32_t k = 228;
    EXPECT_NO_FATAL_FAILURE(AsmReplaceWithGroup(array, &size, k));
  }
  {
    auto* array = new int64_t[]{2, 4};
    auto* ans = new int64_t[]{2, 4};
    int32_t size = 2;
    int32_t ans_size = 2;
    int32_t k = 4;
    AsmReplaceWithGroup(array, &size, k);
    EXPECT_EQ(size, ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[6]{2, 4};
    auto* ans = new int64_t[]{2, 2, 4, 4, 4, 4};
    int32_t size = 2;
    int32_t ans_size = 6;
    int32_t k = 1;
    AsmReplaceWithGroup(array, &size, k);
    EXPECT_EQ(size, ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
  {
    auto* array = new int64_t[721]{10, 711};
    int32_t size = 2;
    int32_t ans_size = 721;
    int32_t k = 1;
    AsmReplaceWithGroup(array, &size, k);
    EXPECT_EQ(size, ans_size);
    delete[] array;
  }
  {
    auto* array = new int64_t[22]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto* ans = new int64_t[]{1, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8,
                              9, 9, 9, 10, 10, 10, 10};
    int32_t size = 10;
    int32_t ans_size = 22;
    int32_t k = 3;
    AsmReplaceWithGroup(array, &size, k);
    EXPECT_EQ(size, ans_size);
    EXPECT_TRUE(helpers::AreEqual(array, ans_size, ans, ans_size))
              << helpers::ArrToStr(array, ans_size) << '\n'
              << helpers::ArrToStr(ans, ans_size);
    delete[] array;
    delete[] ans;
  }
}

TEST(AsmReplaceWithGroup, Generator) {
    // I have had some problems with it, so it's empty :(
}

// ---------------------------------------------------------

void RightMerge(
    const int64_t* array1, int64_t size1,
    const int64_t* array2, int64_t size2,
    int64_t* result) {
  int insert_pos = 0;
  int pos1 = 0;
  int pos2 = 0;
  while (pos1 < size1 && pos2 < size2) {
    if (array1[pos1] < array2[pos2]) {
      result[insert_pos] = array1[pos1];
      ++pos1;
    } else {
      result[insert_pos] = array2[pos2];
      ++pos2;
    }
    ++insert_pos;
  }
  while (pos1 < size1) {
    result[insert_pos] = array1[pos1];
    ++pos1;
    ++insert_pos;
  }
  while (pos2 < size2) {
    result[insert_pos] = array2[pos2];
    ++pos2;
    ++insert_pos;
  }
}
