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

TEST(AsmFactorial, Simple) {
  ASSERT_EQ(AsmFactorial(1), 2);
  ASSERT_EQ(AsmFactorial(2), 6);
  ASSERT_EQ(AsmFactorial(6), 24);
}

int64_t Factorial(int32_t x) {
  int64_t res = 1;
  int64_t cur = 1;
  while (res <= x) {
    res *= cur;
    ++cur;
  }
  return res;
}

TEST(AsmFactorial, Generator) {
  RandomGenerator gen(1, std::numeric_limits<int32_t>::max());
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = gen.GetValue();
    ASSERT_EQ(AsmFactorial(x), Factorial(x)) << x;
  }
}

// ---------------------------------------------------------

TEST(AsmIsSquare, Simple) {
  ASSERT_TRUE(AsmIsSquare(1));
  ASSERT_FALSE(AsmIsSquare(2));
  ASSERT_FALSE(AsmIsSquare(3));
  ASSERT_TRUE(AsmIsSquare(4));
  ASSERT_TRUE(AsmIsSquare((1LL << 31) * (1LL << 31)));
}

bool IsSquare(int64_t x) {
  int64_t sq = sqrtl(x);
  return sq * sq == x;
}

TEST(AsmIsSquare, Generator) {
  RandomGenerator gen(1, std::numeric_limits<int64_t>::max());
  const int kTestsCount = 1e6;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = gen.GetValue();
    ASSERT_EQ(AsmIsSquare(x), IsSquare(x)) << x;
  }
}

// ---------------------------------------------------------

TEST(AsmRemoveDigits, Simple) {
  ASSERT_EQ(AsmRemoveDigits(1), 1);
  ASSERT_EQ(AsmRemoveDigits(2), 0);
  ASSERT_EQ(AsmRemoveDigits(324), 3);
  ASSERT_EQ(AsmRemoveDigits(541), 51);

  ASSERT_EQ(AsmRemoveDigits(-1), -1);
  ASSERT_EQ(AsmRemoveDigits(-2), 0);
  ASSERT_EQ(AsmRemoveDigits(-324), -3);
  ASSERT_EQ(AsmRemoveDigits(-541), -51);
}

bool IsEvenDigit(char c) {
  if (c < '0' || c > '9' || (c - '0') % 2 == 1) {
    return false;
  }
  return true;
}

int32_t RemoveEvenDigits(int32_t x) {
  std::string x_str = std::to_string(x);
  x_str = std::string(x_str.begin(),
                      std::remove_if(x_str.begin(), x_str.end(), IsEvenDigit));
  int32_t value;
  if (x_str.empty() || x_str == "-") {
    value = 0;
  } else {
    value = std::stoi(x_str);
  }
  return value;
}

TEST(AsmRemoveDigits, Generator) {
  RandomGenerator gen(std::numeric_limits<int32_t>::min(),
                      std::numeric_limits<int32_t>::max());
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = gen.GetValue();
    ASSERT_EQ(AsmRemoveDigits(x), RemoveEvenDigits(x)) << x;
  }
}

// ---------------------------------------------------------

int64_t Formula(int64_t x, int64_t n) {
  // no overflow check
  int64_t res = 1;
  int64_t prev_res = 1;
  for (int i = 1; i <= n; ++i) {
    prev_res *= x;
    if (i % 2) {
      prev_res -= i + 1;
    } else {
      prev_res += i + 1;
    }
    res *= prev_res;
  }
  return res;
}

TEST(AsmFormula, Simple) {
  ASSERT_EQ(AsmFormula(0, 1), -2);
  ASSERT_EQ(AsmFormula(0, 1), Formula(0, 1));
  ASSERT_EQ(AsmFormula(1, 1), -1);
  ASSERT_EQ(AsmFormula(1, 1), Formula(1, 1));
  ASSERT_EQ(AsmFormula(1, 2), -2);
  ASSERT_EQ(AsmFormula(1, 2), Formula(1, 2));
}

TEST(AsmFormula, Generator) {
  RandomGenerator x_gen(-10, 10);
  RandomGenerator n_gen(1, 10);
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    int64_t x = x_gen.GetValue();
    int64_t n = n_gen.GetValue();
    ASSERT_EQ(AsmFormula(x, n), Formula(x, n)) << x << ' ' << n;
  }
}

// ---------------------------------------------------------

int64_t BankDeposit(uint64_t initial_sum, int32_t percentage, int32_t years) {
  for (int i = 1; i <= years; ++i) {
    initial_sum += initial_sum * percentage / 100;
  }
  return initial_sum;
}

TEST(AsmBankDeposit, Simple) {
  ASSERT_EQ(BankDeposit(1, 100, 0), 1);
  ASSERT_EQ(BankDeposit(1, 100, 1), 2);
  ASSERT_EQ(BankDeposit(2, 50, 2), 4);
  ASSERT_EQ(BankDeposit(2, 50, 3), 6);
}

TEST(AsmBankDeposit, Generator) {
  RandomGenerator percentage_gen(0, 100);
  RandomGenerator years_gen(1, 20);
  const int kTestsCount = 1e6;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = 100;
    int32_t y = percentage_gen.GetValue();
    int32_t z = years_gen.GetValue();
    ASSERT_EQ(AsmBankDeposit(x, y, z), BankDeposit(x, y, z))
        << x << ' ' << y << ' ' << z;
  }
}


// ---------------------------------------------------------

uint16_t EvenDivisors(uint16_t n) {
  uint16_t res = 0;
  for (int i = 1; i < n; ++i) {
    if (n / i == n % i) {
      ++res;
    }
  }
  return res;
}

TEST(AsmEvenDivisors, Simple) {
  ASSERT_EQ(AsmEvenDivisors(1), 0);
  ASSERT_EQ(AsmEvenDivisors(2), 0);
  ASSERT_EQ(AsmEvenDivisors(3), 1);
  ASSERT_EQ(AsmEvenDivisors(5), 1);
}

TEST(AsmEvenDivisors, All) {
  RandomGenerator gen(0, std::numeric_limits<uint16_t>::max());
  const int kTestsCount = 500;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = gen.GetValue();
    ASSERT_EQ(AsmEvenDivisors(x), EvenDivisors(x)) << x;
  }
}

// ---------------------------------------------------------

uint64_t InfiniteManipulations(uint64_t n) {
  int ones_count = __builtin_popcountll(n);
  uint64_t min = (1ULL << ones_count) - 1;
  uint64_t max = (min << (64 - ones_count));
  return max - min;
}

TEST(AsmInfiniteManipulations, Simple) {
  ASSERT_EQ(AsmInfiniteManipulations(1), 9'223'372'036'854'775'807ull);
  ASSERT_EQ(AsmInfiniteManipulations(9'223'372'036'854'775'807),
            9'223'372'036'854'775'807ull);
  ASSERT_EQ(AsmInfiniteManipulations(std::numeric_limits<uint64_t>::max()), 0);
}

TEST(AsmInfiniteManipulations, All) {
  RandomGenerator gen(1, std::numeric_limits<uint64_t>::max());
  const int kTestsCount = 1e7;
  for (int _ = 0; _ < kTestsCount; ++_) {
    uint64_t x = gen.GetValue();
    ASSERT_EQ(AsmInfiniteManipulations(x), InfiniteManipulations(x)) << x;
  }
}

// ---------------------------------------------------------
