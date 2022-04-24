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

extern "C" uint64_t AsmSequencesCount(uint64_t n, uint64_t k);
extern "C" uint64_t AsmSequencesCountRecursion(
    uint64_t n, uint64_t k, uint64_t* buffer);


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
    EXPECT_EQ(b[i], (i + 1) * 3);
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

// ---------------------------------------------------------

uint64_t RecRightSequenceCount(
    uint64_t n,
    uint64_t k,
    std::vector<uint64_t>& mem,
    std::vector<bool>& ready) {
  if (n < 0) {
    return 0;
  }
  if (n < k) {
    if (ready[n]) {
      return mem[n];
    }
    uint64_t ans = 0;
    if (n < 64) {
      ans = (1LL << n);
    }
    ready[n] = true;
    mem[n] = ans;
    return ans;
  }
  uint64_t res = 0;
  if (n - k - 1 >= 0 && ready[n - k - 1]) {
    res += mem[n - k - 1];
  } else {
    res += RecRightSequenceCount(n - k - 1, k, mem, ready);
  }
  if (ready[n - 1]) {
    res += mem[n - 1];
  } else {
    res += RecRightSequenceCount(n - 1, k, mem, ready);
  }
  return res;
}

uint64_t RightSequenceCount(uint64_t n, uint64_t k) {
  std::vector<uint64_t> mem(n + 1);
  std::vector<bool> ready(n + 1);
  return RecRightSequenceCount(n, k, mem, ready);
}

TEST(SequencesCount, Sample) {
  ASSERT_EQ(AsmSequencesCount(4, 2), 8);
  ASSERT_EQ(AsmSequencesCount(0, 2), 1);
  ASSERT_EQ(AsmSequencesCount(1, 2), 2);
  ASSERT_EQ(AsmSequencesCount(2, 2), 3);
  ASSERT_EQ(AsmSequencesCount(3, 3), 7);
  ASSERT_EQ(AsmSequencesCount(5, 2), 13);
  ASSERT_EQ(AsmSequencesCount(4, 3), 13);
}

TEST(SequencesCount, Generator) {
  const int kTestsCount = 1000;
  RandomGenerator<uint64_t> n_gen(50, 1000);
  RandomGenerator<uint64_t> k_gen(2, 10000000);
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t n = n_gen.GetValue();
    uint64_t k = k_gen.GetValue();
    EXPECT_EQ(RightSequenceCount(n, k),
              RightSequenceCount(n, k)) << n << ' ' << k;
  }
}
