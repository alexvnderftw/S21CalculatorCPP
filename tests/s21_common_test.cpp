#include "s21_test_main.h"

/*
double bank_round(double number)
*/
TEST(CommonSuite, BankRound) {
  EXPECT_NEAR(s21::bankRound(126.5), 126.0, EPS);
  EXPECT_NEAR(s21::bankRound(129.5), 130.0, EPS);
  EXPECT_NEAR(s21::bankRound(-123.5), -124.0, EPS);
  EXPECT_NEAR(s21::bankRound(-124.5), -124.0, EPS);
}
