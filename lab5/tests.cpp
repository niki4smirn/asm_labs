#include <gtest/gtest.h>

extern "C" uint32_t AsmFindNearest(
    uint32_t x, uint32_t y, uint32_t n, uint32_t x1, uint32_t y1, ...);

extern "C" void AsmSummarizeRows(
    const uint64_t** a, uint32_t n, uint32_t m, uint64_t* b);

extern "C" uint64_t AsmCountIfNot(
    const uint16_t* a, uint32_t n, bool(*pred)(uint16_t x));

extern "C" uint64_t AsmGetMoreMagic();

extern "C" void* AsmCopy(const void* data, uint32_t size);

extern "C" uint64_t AsmSequencesCount(uint64_t n, uint64_t k);
extern "C" uint64_t AsmSequencesCountRecursion(
    uint64_t n, uint64_t k, uint64_t* buffer, bool* ready);


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

extern "C" uint64_t Metric(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2);

TEST(Metric, MinMaxTest) {
  uint64_t min = 0;
  uint64_t max = (1LL << 31) + 42;
  ASSERT_EQ(Metric(min, min, max, max), 2 * max * max);
  ASSERT_EQ(Metric(max, max, min, min), 2 * max * max);
}

TEST(FindNearest, Sample) {
  uint32_t max = (1LL << 31) + 42;
  ASSERT_EQ(AsmFindNearest(0, 0, 1, 0, 0), 1);
  ASSERT_EQ(AsmFindNearest(0, 0, 1, max, max), 1);
  ASSERT_EQ(AsmFindNearest(max, max, 2, 0, 0, 1, 1), 2);
  ASSERT_EQ(AsmFindNearest(0, 0, 3, max, max, max / 2, max / 2, 1, 1), 3);
  ASSERT_EQ(AsmFindNearest(0, 0, 4, 100, 100, 2, 3, 0, 9, 10, 11), 2);
  ASSERT_EQ(AsmFindNearest(0, 0, 4, 100, 100, 1, 1, 1, 1, 1, 1), 2);
  ASSERT_EQ(AsmFindNearest(0, 0, 4, 1, 1, 1, 1, 1, 1, 1, 1), 1);
}

// ---------------------------------------------------------

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

TEST(SummarizeRows, Generator) {
  RandomGenerator<uint32_t> size_gen(1, 1e3);
  RandomGenerator<uint64_t> fill_gen(std::numeric_limits<uint64_t>::min(),
                                     std::numeric_limits<uint64_t>::max());
  const int kTestsCount = 100;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint32_t rows_count = size_gen.GetValue();
    uint32_t cols_count = size_gen.GetValue();
    auto** a = new uint64_t*[rows_count];
    for (int i = 0; i < rows_count; ++i) {
      a[i] = new uint64_t[cols_count];
    }
    for (int i = 0; i < rows_count; ++i) {
      for (int j = 0; j < cols_count; ++j) {
        a[i][j] = fill_gen.GetValue();
      }
    }
    auto* b = new uint64_t[rows_count];
    AsmSummarizeRows(const_cast<const uint64_t**>(a), rows_count, cols_count, b);
    for (int i = 0; i < rows_count; ++i) {
      uint64_t cur_sum = 0;
      for (int j = 0; j < cols_count; ++j) {
        cur_sum += a[i][j];
      }
      EXPECT_EQ(b[i], cur_sum);
      delete[] a[i];
    }
    delete[] a;
    delete[] b;
  }
}

// ---------------------------------------------------------

TEST(CountIfNot, Sample) {
  std::vector<uint16_t> vec = {1, 2, 3, 4, 5, 6, 7};
  auto predicate = [](uint16_t value) {
    return value > 1525;
  };
  EXPECT_EQ(AsmCountIfNot(vec.data(), vec.size(), predicate),
            vec.size() - std::count_if(vec.begin(), vec.end(), predicate));
}

TEST(CountIfNot, Generator) {
  const std::vector<bool(*)(uint16_t x)> kPredicates = {
      [](uint16_t value) {
        return value % 2 == 0;
      },
      [](uint16_t value) {
        return false;
      },
      [](uint16_t value) {
        return value > 1525;
      }
  };
  const uint32_t kMaxSize = 2e5;
  RandomGenerator<uint32_t> size_gen(std::numeric_limits<uint32_t>::min(),
                                   kMaxSize);
  RandomGenerator<uint16_t> array_gen(std::numeric_limits<uint16_t>::min(),
                                      std::numeric_limits<uint16_t>::max());
  RandomGenerator<size_t> predicate_index_gen(0, kPredicates.size() - 1);
  const int kTestsCount = 1000;
  for (int _ = 0; _ < kTestsCount; ++_) {
    auto size = size_gen.GetValue();
    auto array = array_gen.GetArray(size);
    auto predicate_index = predicate_index_gen.GetValue();
    auto predicate = kPredicates[predicate_index];
    EXPECT_EQ(AsmCountIfNot(array.data(), size, predicate),
              size - std::count_if(array.begin(), array.end(), predicate));
  }
}

// ---------------------------------------------------------

// #define FIRST_MAGIC
#define SECOND_MAGIC

#ifdef FIRST_MAGIC
extern "C" uint8_t GetMagic(uint64_t x) {
  return x;
}

TEST(GetMoreMagic, Sample) {
  EXPECT_EQ(AsmGetMoreMagic(), 36);
}
#else
#ifdef SECOND_MAGIC
extern "C" uint8_t GetMagic(uint64_t x) {
  return 2 * x;
}

TEST(GetMoreMagic, Sample) {
  EXPECT_EQ(AsmGetMoreMagic(), 147456);
}
#else
extern "C" uint8_t GetMagic(uint64_t x) {
  return 0;
}

TEST(GetMoreMagic, Sample) {
  EXPECT_EQ(AsmGetMoreMagic(), 0);
}
#endif
#endif

// ---------------------------------------------------------

TEST(Copy, Sample) {
  {
    uint32_t size = 0;
    EXPECT_EQ(AsmCopy(nullptr, size), nullptr);
  }
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
  for (uint64_t i = k; i >= 1; --i) {
    if (n - i >= 0 && ready[n - i]) {
      res += mem[n - i];
    } else {
      res += RecRightSequenceCount(n - i, k, mem, ready);
    }
  }
  mem[n] = res;
  ready[n] = true;
  return res;
}

uint64_t RightSequenceCount(uint64_t n, uint64_t k) {
  std::vector<uint64_t> mem(n + 1);
  std::vector<bool> ready(n + 1);
  return RecRightSequenceCount(n, k, mem, ready);
}

TEST(RecSequencesCount, Sample) {
  {
    uint64_t n = 4;
    uint64_t k = 2;
    auto* buffer = new uint64_t[n + 1];
    auto* ready = new bool[n + 1];
    for (int i = 0; i <= n; ++i) {
      ready[i] = false;
    }
    EXPECT_EQ(AsmSequencesCountRecursion(n - 2, k, buffer, ready), 3);
    EXPECT_EQ(AsmSequencesCountRecursion(n - 1, k, buffer, ready), 5);
    EXPECT_EQ(AsmSequencesCountRecursion(n, k, buffer, ready), 8);

    delete[] ready;
    delete[] buffer;
  }

}

TEST(SequencesCount, Sample) {
  EXPECT_EQ(AsmSequencesCount(0, 2), 1);
  EXPECT_EQ(AsmSequencesCount(1, 2), 2);
  EXPECT_EQ(AsmSequencesCount(2, 2), 3);
  EXPECT_EQ(AsmSequencesCount(3, 2), 5);
  EXPECT_EQ(AsmSequencesCount(4, 2), 8);
  EXPECT_EQ(AsmSequencesCount(3, 3), 7);
  EXPECT_EQ(AsmSequencesCount(5, 2), 13);
  EXPECT_EQ(AsmSequencesCount(4, 3), 13);
}

TEST(SequencesCount, Generator) {
  const int kTestsCount = 100;
  RandomGenerator<uint64_t> n_gen(5000, 10000);
  RandomGenerator<uint64_t> k_gen(2, 1000);
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t n = n_gen.GetValue();
    uint64_t k = k_gen.GetValue();
    EXPECT_EQ(RightSequenceCount(n, k),
              AsmSequencesCount(n, k)) << n << ' ' << k;
  }
}
