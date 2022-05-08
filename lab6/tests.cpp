#include <gtest/gtest.h>

extern "C" char* AsmStrChr(const char* s, char c);
extern "C" void AsmStrCpy(char* dst, const char* src);
extern "C" void AsmStrNCpy(char* dst, const char* src, uint32_t size);
extern "C" char* AsmStrCat(char* dst, const char* src);
extern "C" char* AsmStrStr(const char* str, const char* substr);

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

TEST(AsmStrCpy, Simple) {
  const char* src = "abacabe";
  const int size = 8;
  {
    char* dst = new char[size];
    AsmStrCpy(dst, src);
    dst[size - 1] = 0;
    EXPECT_STREQ(dst, src);
  }
  {
    char* dst = new char[size];
    for (int i = 0; i < size; ++i) {
      dst[i] = i;
    }
    AsmStrCpy(dst, src);
    dst[size - 1] = 0;
    EXPECT_STREQ(dst, src);
  }
  {
    char* dst = new char[]{"abacabeabacabe"};
    AsmStrCpy(dst, src);
    EXPECT_STREQ(dst, "abacabeabacabe");
  }
}

TEST(AsmStrNCpy, Simple) {
  const char* src = "abacabe";
  const int size = 8;
  {
    char* dst = new char[size]{"aba"};
    AsmStrNCpy(dst, src, size - 1);
    EXPECT_STREQ(dst, src);
  }
  {
    char* dst = new char[size + 4]{"abaaaaaaaaa"};
    AsmStrNCpy(dst, src, size - 1);
    EXPECT_STREQ(dst, "abacabeaaaa");
  }
  {
    char* dst = new char[size + 4]{"aaaaaaaaaaa"};
    AsmStrNCpy(dst, src, 2);
    EXPECT_STREQ(dst, "abaaaaaaaaa");
  }
  {
    char* dst = new char[size + 4]{"aaaaaaaaaaa"};
    AsmStrNCpy(dst, src, 10);
    EXPECT_STREQ(dst, src);
  }
}

TEST(AsmStrCat, Basic) {
  {
    const char* source = "longlong";
    char dest[18] = {'a','g','\0','h','d','h','j','s','k',
                     'h','j','k','f','d','h','k','d','h'};
    const char* need = "aglonglong";
    ASSERT_STREQ(AsmStrCat(dest, source), need);
  }
  {
    const char* source = "longlong";
    char dest[18] = {'a','g','h','d','h','j','s','k','\0',
                     'h','j','k','f','d','h','k','d','h'};
    const char* need = "aghdhjsklonglong";
    ASSERT_STREQ(AsmStrCat(dest, source), need);
  }
}

TEST(AsmStrStr, Basic) {
  {
    const char* source = "a";
    const char* dest = "a";
    ASSERT_STREQ(AsmStrStr(dest, source), dest);
  }
  {
    const char* source = "a";
    const char* dest = "b";
    ASSERT_STREQ(AsmStrStr(dest, source), nullptr);
  }
  {
    const char* source = "longlong";
    const char* dest = "bobalonglonga";
    ASSERT_STREQ(AsmStrStr(dest, source), dest + 4);
  }
  {
    const char* source = "longlona";
    const char* dest = "bobalonglong";
    ASSERT_STREQ(AsmStrStr(dest, source), nullptr);
  }
}
