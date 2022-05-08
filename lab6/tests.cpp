#include <gtest/gtest.h>

extern "C" char* AsmStrChr(const char* s, char c);
extern "C" void AsmStrCpy(char* dst, const char* src);
extern "C" void AsmStrNCpy(char* dst, const char* src, uint32_t size);
extern "C" char* AsmStrCat(char* dst, const char* src);
extern "C" char* AsmStrStr(const char* str, const char* substr);
extern "C" int64_t AsmStrToInt64(const char* s);
extern "C" void AsmIntToStr64(int64_t x, int32_t b, char* s);
extern "C" bool AsmSafeStrToUInt64(const char* s, uint64_t* result);

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

TEST(AsmStrToInt64, Basic) {
  {
    const char* val = "-5";
    ASSERT_EQ(AsmStrToInt64(val), -5);
  }
  {
    const char* val = "-50";
    ASSERT_EQ(AsmStrToInt64(val), -50);
  }
  {
    const char* val = "22801488";
    ASSERT_EQ(AsmStrToInt64(val), 22801488);
  }
}

TEST(AsmIntToStr64, Basic) {
  /// bad base
  {
    char result[50];
    const char* need = "";
    AsmIntToStr64(228, 11, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "";
    AsmIntToStr64(228, 228, result);
    ASSERT_STREQ(result, need);
  }

  /// binary
  {
    char result[50];
    const char* need = "110";
    AsmIntToStr64(6, 2, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "-101";
    AsmIntToStr64(-5, 2, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "11010";
    AsmIntToStr64(26, 2, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "-11010";
    AsmIntToStr64(-26, 2, result);
    ASSERT_STREQ(result, need);
  }

  /// octal
  {
    char result[50];
    const char* need = "5";
    AsmIntToStr64(5, 8, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "-5";
    AsmIntToStr64(-5, 8, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "32";
    AsmIntToStr64(26, 8, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "-15753";
    AsmIntToStr64(-7147, 8, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "15753";
    AsmIntToStr64(7147, 8, result);
    ASSERT_STREQ(result, need);
  }


  /// hexadecimal
  {
    char result[50];
    const char* need = "5";
    AsmIntToStr64(5, 16, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "-5";
    AsmIntToStr64(-5, 16, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "1A";
    AsmIntToStr64(26, 16, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "-1BEB";
    AsmIntToStr64(-7147, 16, result);
    ASSERT_STREQ(result, need);
  }
  {
    char result[50];
    const char* need = "1BEB";
    AsmIntToStr64(7147, 16, result);
    ASSERT_STREQ(result, need);
  }

  /// decimal
  for (int i = -1000000; i <= 1000000; i++) {
    char result[50];
    std::string need = std::to_string(i);
    AsmIntToStr64(i, 10, result);
    ASSERT_STREQ(result, need.c_str());
  }
}

TEST(AsmSafeStrToUInt64, Basic) {
  {
    const char* val = "-5";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_FALSE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(old, ptr);
  }
  {
    const char* val = "-5";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_FALSE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(old, ptr);
  }
  {
    const char* val = "22801488";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_TRUE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(ptr, 22801488);
  }
  {
    const char* val = "18446744073709551615";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_TRUE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(ptr, 18446744073709551615ull);
  }
  {
    const char* val = "00000000000000000018446744073709551615";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_TRUE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(ptr, 18446744073709551615ull);
  }
  {
    const char* val = "228o1488";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_FALSE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(old, ptr);
  }
  {
    const char* val = "18446744073709551616";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_FALSE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(old, ptr);
  }
  {
    const char* val = "3016701673190176091376713067130677310";
    uint64_t ptr = 3701713071;
    uint64_t old = ptr;
    ASSERT_FALSE(AsmSafeStrToUInt64(val, &ptr));
    ASSERT_EQ(old, ptr);
  }
}
