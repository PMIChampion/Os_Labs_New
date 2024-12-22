#include <gtest/gtest.h>

// Объявления функций из библиотек
extern "C" {
    int PrimeCount(int A, int B);
    float E(int x);
}

TEST(PrimeCountTest, NaiveImplementation) {
    EXPECT_EQ(PrimeCount(1, 10), 4);  // Простые числа: 2, 3, 5, 7
    EXPECT_EQ(PrimeCount(10, 20), 4); // Простые числа: 11, 13, 17, 19
    EXPECT_EQ(PrimeCount(1, 1), 0);   // Нет простых чисел
}

TEST(ETest, FormulaImplementation) {
    EXPECT_NEAR(E(1), 2.0f, 0.01f);
    EXPECT_NEAR(E(5), 2.48832f, 0.01f);
    EXPECT_NEAR(E(1000), 2.71692f, 0.0001f);
}
