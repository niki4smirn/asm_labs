#include <gtest/gtest.h>

extern "C" uint32_t AsmFindNearest(
    uint32_t x, uint32_t y, uint32_t n, uint32_t x1, uint32_t y1, ...);

extern "C" void AsmSummarizeRows(
    const uint64_t** a, uint32_t n, uint32_t m, uint64_t* b);

extern "C" uint32_t AsmCountIfNot(
    const uint16_t* a, uint32_t n, bool(*pred)(uint16_t x));

extern "C" uint8_t GetMagic(uint64_t x) {
  return x;
}

extern "C" uint64_t AsmGetMoreMagic();

extern "C" void* AsmCopy(const void* data, uint32_t size);


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

TEST(FindNearest, Sample) {
  std::cout << AsmFindNearest(0, 0, 4, 100, 100, 2, 3, 0, 9, 10, 11);
}

// ---------------------------------------------------------

extern "C" uint64_t CalculateArraySum(const uint64_t* array, int64_t size);

TEST(SummarizeRows, Sample) {
  int rows_count = 5;
  int cols_count = 3;
  auto** a = new uint64_t*[rows_count];
  for (int i = 0; i < rows_count; ++i) {
    a[i] = new uint64_t[cols_count];
  }
  for (int i = 0; i < rows_count; ++i) {
    for (int j = 0; j < cols_count; ++j) {
      a[i][j] = i + j;
    }
  }
  auto* b = new uint64_t[cols_count];
  AsmSummarizeRows(const_cast<const uint64_t**>(a), rows_count, cols_count, b);
  for (int i = 0; i < rows_count; ++i) {
    std::cout << b[i] << '\n';
    delete[] a[i];
  }
  delete[] a;
  delete[] b;
}

// ---------------------------------------------------------

TEST(CountIfNot, Sample) {
  std::vector<uint16_t> vec = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(AsmCountIfNot(vec.data(), vec.size(), [](uint16_t value) {
    return value % 2 == 0;
  }), 4);
}

// ---------------------------------------------------------

TEST(GetMoreMagic, Sample) {
  EXPECT_EQ(AsmGetMoreMagic(), 36);
}

// ---------------------------------------------------------

TEST(Copy, Sample) {
  {
    uint32_t size = 10;
    char* array = new char[size];
    for (int i = 0; i < size; ++i) {
      array[i] = 'a' + i;
    }
    char* result = static_cast<char*>(AsmCopy(static_cast<const void*>(array),
                                              size));
    for (int i = 0; i < size; ++i) {
      EXPECT_EQ(result[i], 'a' + i);
    }
  }
}
