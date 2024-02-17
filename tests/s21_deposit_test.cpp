#include "s21_test_main.h"

namespace s21 {}  // namespace s21

TEST(DepositSuite, SetValues) {
  s21::Deposit instance;
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(100000.123);
  EXPECT_NEAR(instance.getDeposit(), 100000.123, EPS);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(1);
  EXPECT_TRUE(instance.calculate());
  instance.setTermType(s21::Deposit::T_YEAR);
  EXPECT_TRUE(instance.calculate());
  EXPECT_TRUE(instance.setStartDate(14, 2, 1994));
  EXPECT_TRUE(instance.calculate());
  instance.setInterest(0.2);
  EXPECT_TRUE(instance.calculate());
  instance.setInterest(0.0);
  EXPECT_TRUE(instance.calculate());
  instance.setTax(0.1);
  EXPECT_TRUE(instance.calculate());
  instance.setCapitalization(false);
  EXPECT_FALSE(instance.isCapitalization());
  EXPECT_TRUE(instance.calculate());
  instance.setCapitalization(true);
  EXPECT_TRUE(instance.isCapitalization());
  EXPECT_TRUE(instance.calculate());
  instance.setPeriodicity(s21::Deposit::P_MONTHLY);
  EXPECT_TRUE(instance.calculate());
  instance.setRemainderLimit(1000.0);
  EXPECT_NEAR(instance.getRemainderLimit(), 1000.0, EPS);
  EXPECT_TRUE(instance.calculate());
  instance.addReplenish(s21::Deposit::O_ONCE, s21::Date(2, 11, 1994), 1001);
  EXPECT_TRUE(instance.calculate());
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(31, 12, 1993),
                         4000);
  EXPECT_TRUE(instance.calculate());
}

TEST(DepositSuite, CheckLimitValues) {
  s21::Deposit instance;
  instance.setDeposit(100000.123);
  instance.setTerm(2);
  instance.setTermType(s21::Deposit::T_YEAR);
  EXPECT_TRUE(instance.setStartDate(14, 2, 1994));
  instance.setInterest(0.0);
  instance.setTax(0.1);
  instance.setRemainderLimit(1000.0);
  instance.addReplenish(s21::Deposit::O_ANNUALLY, s21::Date(2, 11, 1994), 1001);
  instance.addWithdrawal(s21::Deposit::O_BIANNUALLY, s21::Date(31, 12, 1993),
                         4000);
  EXPECT_TRUE(instance.calculate());

  /* Deposit value */
  instance.setDeposit(s21::Deposit::MAX_DOUBLE_VALUE);
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(NAN);
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(INFINITY);
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(0.0);
  EXPECT_TRUE(instance.calculate());
  instance.setDeposit(-100000.02);
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(s21::Deposit::MAX_DEPOSIT_VALUE);
  EXPECT_TRUE(instance.calculate());
  instance.setDeposit(s21::Deposit::MAX_DEPOSIT_VALUE * 10.0);
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(100000.123);

  /* Term value */
  instance.setTermType(s21::Deposit::T_YEAR);
  instance.setTerm(s21::Deposit::MAX_TERM_Y);
  EXPECT_TRUE(instance.calculate());
  instance.setTerm(s21::Deposit::MAX_TERM_Y + 1);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(s21::Deposit::MAX_TERM_Y - 1);
  EXPECT_TRUE(instance.calculate());
  instance.setTerm(0);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(-1);
  EXPECT_FALSE(instance.calculate());
  instance.setTermType(s21::Deposit::T_MONTH);
  instance.setTerm(s21::Deposit::MAX_TERM_M);
  EXPECT_TRUE(instance.calculate());
  instance.setTerm(s21::Deposit::MAX_TERM_M + 1);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(s21::Deposit::MAX_TERM_M - 1);
  EXPECT_TRUE(instance.calculate());
  instance.setTerm(0);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(-1);
  EXPECT_FALSE(instance.calculate());
  instance.setTermType(s21::Deposit::T_DAY);
  instance.setTerm(s21::Deposit::MAX_TERM_D);
  EXPECT_TRUE(instance.calculate());
  instance.setTerm(s21::Deposit::MAX_TERM_D + 1);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(s21::Deposit::MAX_TERM_D - 1);
  EXPECT_TRUE(instance.calculate());
  instance.setTerm(0);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(-1);
  EXPECT_FALSE(instance.calculate());
  instance.setTerm(525);

  /* Date value */
  EXPECT_TRUE(instance.setStartDate(1, 1, 1900));
  EXPECT_TRUE(instance.calculate());
  EXPECT_FALSE(instance.setStartDate(31, 12, 1899));
  EXPECT_TRUE(instance.calculate());
  EXPECT_TRUE(instance.setStartDate(31, 12, s21::Deposit::MAX_START_YEAR));
  EXPECT_TRUE(instance.calculate());
  EXPECT_TRUE(instance.setStartDate(1, 1, s21::Deposit::MAX_START_YEAR + 1));
  EXPECT_FALSE(instance.calculate());
  EXPECT_FALSE(instance.setStartDate(31, 12, -1899));
  EXPECT_FALSE(instance.setStartDate(0, 12, 1920));
  EXPECT_TRUE(instance.setStartDate(31, 12, s21::Deposit::MAX_START_YEAR));
  EXPECT_TRUE(instance.calculate());

  /* Interest rate value */
  instance.setInterest(-0.01);
  EXPECT_FALSE(instance.calculate());
  instance.setInterest(0.0);
  EXPECT_TRUE(instance.calculate());
  instance.setInterest(NAN);
  EXPECT_FALSE(instance.calculate());
  instance.setInterest(INFINITY);
  EXPECT_FALSE(instance.calculate());
  instance.setInterest(s21::Deposit::MAX_DOUBLE_VALUE);
  EXPECT_FALSE(instance.calculate());
  instance.setInterest(s21::Deposit::MAX_RATE);
  EXPECT_TRUE(instance.calculate());
  instance.setInterest(s21::Deposit::MAX_RATE + 0.01);
  EXPECT_FALSE(instance.calculate());
  instance.setInterest(0.000001);
  EXPECT_TRUE(instance.calculate());

  /* Tax rate value */
  instance.setTax(-0.01);
  EXPECT_FALSE(instance.calculate());
  instance.setTax(0.0);
  EXPECT_TRUE(instance.calculate());
  instance.setTax(NAN);
  EXPECT_FALSE(instance.calculate());
  instance.setTax(INFINITY);
  EXPECT_FALSE(instance.calculate());
  instance.setTax(s21::Deposit::MAX_DOUBLE_VALUE);
  EXPECT_FALSE(instance.calculate());
  instance.setTax(s21::Deposit::MAX_TAX);
  EXPECT_TRUE(instance.calculate());
  instance.setTax(s21::Deposit::MAX_RATE + 0.0001);
  EXPECT_FALSE(instance.calculate());
  instance.setTax(0.000001);
  EXPECT_TRUE(instance.calculate());

  /* Remainder limit value */
  instance.setRemainderLimit(0.0);
  EXPECT_TRUE(instance.calculate());
  instance.setRemainderLimit(-0.1);
  EXPECT_FALSE(instance.calculate());
  instance.setRemainderLimit(NAN);
  EXPECT_FALSE(instance.calculate());
  instance.setRemainderLimit(INFINITY);
  EXPECT_FALSE(instance.calculate());
  instance.setRemainderLimit(s21::Deposit::MAX_DOUBLE_VALUE);
  EXPECT_FALSE(instance.calculate());
  instance.setRemainderLimit(s21::Deposit::MAX_DOUBLE_VALUE / 10.0);
  EXPECT_TRUE(instance.calculate());
  instance.setRemainderLimit(1000.0);
  EXPECT_TRUE(instance.calculate());

  /* Replenish values */
  EXPECT_TRUE(instance.setStartDate());
  instance.addReplenish(s21::Deposit::O_ONCE, s21::Date(), 1000);
  EXPECT_TRUE(instance.calculate());
  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(2, 11, 1994), 1000);
  EXPECT_TRUE(instance.calculate());
  instance.addReplenish(s21::Deposit::O_BIMONTHLY, s21::Date(2, 12, 1994),
                        1000);
  EXPECT_TRUE(instance.calculate());
  instance.addReplenish(s21::Deposit::O_QUARTERLY, s21::Date(29, 2, 2000),
                        1000);
  EXPECT_TRUE(instance.calculate());
  instance.addReplenish(s21::Deposit::O_ANNUALLY, s21::Date(20, 7, 1998), 2000);
  EXPECT_TRUE(instance.calculate());
  EXPECT_EQ(instance.getReplenishListSize(), 6);
  EXPECT_EQ(instance.getReplenishListElement(0)->period(),
            s21::Deposit::O_ANNUALLY);
  EXPECT_EQ(instance.getReplenishListElement(0)->date(),
            s21::Date(2, 11, 1994));
  EXPECT_EQ(instance.getReplenishListElement(0)->value(), 1001);
  EXPECT_EQ(instance.getReplenishListElement(5)->period(),
            s21::Deposit::O_ANNUALLY);
  EXPECT_EQ(instance.getReplenishListElement(5)->date(),
            s21::Date(20, 7, 1998));
  EXPECT_EQ(instance.getReplenishListElement(5)->value(), 2000);
  instance.popBackReplenish();
  EXPECT_EQ(instance.getReplenishListSize(), 5);

  instance.clearReplenish();
  EXPECT_EQ(instance.getReplenishListSize(), 0);
  instance.popBackReplenish();
  EXPECT_EQ(instance.getReplenishListSize(), 0);
  instance.clearReplenish();
  EXPECT_EQ(instance.getReplenishListSize(), 0);

  instance.addReplenish(s21::Deposit::O_MONTHLY,
                        s21::Date(31, 12, s21::Deposit::MAX_START_YEAR), 1000);
  EXPECT_TRUE(instance.calculate());
  instance.popBackReplenish();
  instance.addReplenish(s21::Deposit::O_MONTHLY,
                        s21::Date(1, 1, s21::Deposit::MAX_START_YEAR + 1),
                        1000);
  EXPECT_FALSE(instance.calculate());
  instance.popBackReplenish();

  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(31, 12, 2000), 0.0);
  EXPECT_TRUE(instance.calculate());
  instance.popBackReplenish();
  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000), -1);
  EXPECT_FALSE(instance.calculate());
  instance.removeReplenish(0);
  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
                        s21::Deposit::MAX_DOUBLE_VALUE);
  EXPECT_FALSE(instance.calculate());
  instance.removeReplenish(0);
  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
                        s21::Deposit::MAX_DEPOSIT_VALUE);
  EXPECT_TRUE(instance.calculate());
  instance.removeReplenish(0);
  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
                        s21::Deposit::MAX_DEPOSIT_VALUE * 10);
  EXPECT_FALSE(instance.calculate());
  instance.removeReplenish(0);

  /* Withdrawal values */
  EXPECT_TRUE(instance.setStartDate());
  instance.addWithdrawal(s21::Deposit::O_ONCE, s21::Date(), 1000);
  EXPECT_TRUE(instance.calculate());
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(2, 11, 1994), 1000);
  EXPECT_TRUE(instance.calculate());
  instance.addWithdrawal(s21::Deposit::O_BIMONTHLY, s21::Date(2, 12, 1994),
                         1000);
  EXPECT_TRUE(instance.calculate());
  instance.addWithdrawal(s21::Deposit::O_QUARTERLY, s21::Date(29, 2, 2000),
                         1000);
  EXPECT_TRUE(instance.calculate());
  instance.addWithdrawal(s21::Deposit::O_ANNUALLY, s21::Date(20, 7, 1998),
                         2000);
  EXPECT_TRUE(instance.calculate());
  EXPECT_EQ(instance.getWithdrawalListSize(), 6);
  EXPECT_EQ(instance.getWithdrawalListElement(0)->period(),
            s21::Deposit::O_BIANNUALLY);
  EXPECT_EQ(instance.getWithdrawalListElement(0)->date(),
            s21::Date(31, 12, 1993));
  EXPECT_EQ(instance.getWithdrawalListElement(0)->value(), 4000);
  EXPECT_EQ(instance.getWithdrawalListElement(5)->period(),
            s21::Deposit::O_ANNUALLY);
  EXPECT_EQ(instance.getWithdrawalListElement(5)->date(),
            s21::Date(20, 7, 1998));
  EXPECT_EQ(instance.getWithdrawalListElement(5)->value(), 2000);
  instance.popBackWithdrawal();
  EXPECT_EQ(instance.getWithdrawalListSize(), 5);

  instance.clearWithdrawal();
  EXPECT_EQ(instance.getWithdrawalListSize(), 0);
  instance.popBackWithdrawal();
  EXPECT_EQ(instance.getWithdrawalListSize(), 0);
  instance.clearWithdrawal();
  EXPECT_EQ(instance.getWithdrawalListSize(), 0);

  instance.addWithdrawal(s21::Deposit::O_MONTHLY,
                         s21::Date(31, 12, s21::Deposit::MAX_START_YEAR), 1000);
  EXPECT_TRUE(instance.calculate());
  instance.popBackWithdrawal();
  instance.addWithdrawal(s21::Deposit::O_MONTHLY,
                         s21::Date(1, 1, s21::Deposit::MAX_START_YEAR + 1),
                         1000);
  EXPECT_FALSE(instance.calculate());
  instance.popBackWithdrawal();

  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(31, 12, 2000), 0.0);
  EXPECT_TRUE(instance.calculate());
  instance.popBackWithdrawal();
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000), -1);
  EXPECT_FALSE(instance.calculate());
  instance.removeWithdrawal(0);
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
                         s21::Deposit::MAX_DOUBLE_VALUE);
  EXPECT_FALSE(instance.calculate());
  instance.removeWithdrawal(0);
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
                         s21::Deposit::MAX_DEPOSIT_VALUE);
  EXPECT_TRUE(instance.calculate());
  instance.removeWithdrawal(0);
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
                         s21::Deposit::MAX_DEPOSIT_VALUE * 10);
  EXPECT_FALSE(instance.calculate());
  instance.removeWithdrawal(0);
}

TEST(DepositSuite, NullValues) {
  s21::Deposit instance;
  instance.setDeposit(0.0);
  instance.setTerm(1);
  instance.setTermType(s21::Deposit::T_YEAR);
  EXPECT_TRUE(instance.setStartDate(1, 1, 1994));
  instance.setInterest(0.0);
  instance.setTax(0.0);
  instance.setRemainderLimit(0.0);
  instance.setCapitalization(false);
  instance.setPeriodicity(s21::Deposit::P_AT_END);
  instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(2, 11, 1994), 0);
  instance.addWithdrawal(s21::Deposit::O_BIMONTHLY, s21::Date(31, 12, 1993), 0);
  EXPECT_TRUE(instance.calculate());

  EXPECT_EQ(instance.getBalance(), 0.0);
  EXPECT_EQ(instance.getInterestTotal(), 0.0);
  EXPECT_EQ(instance.getTaxTotal(), 0.0);
  EXPECT_EQ(instance.getReplenishTotal(), 0.0);
  EXPECT_EQ(instance.getWithdrawalTotal(), 0.0);
  EXPECT_EQ(instance.getEventListSize(), 11);
  EXPECT_EQ(instance.getTaxListSize(), 2);
  // EXPECT_EQ(instance.getEventListElement(0)->date(), s21::Date(1, 1, 1994));
  // EXPECT_EQ(instance.getEventListElement(1)->date(), s21::Date(28, 2, 1994));
  // EXPECT_EQ(instance.getEventListElement(2)->date(), s21::Date(1, 5, 1994));
  // EXPECT_EQ(instance.getEventListElement(3)->date(), s21::Date(31, 7, 1994));
  // EXPECT_EQ(instance.getEventListElement(4)->date(), s21::Date(30, 9, 1994));
  // EXPECT_EQ(instance.getEventListElement(5)->date(), s21::Date(2, 11, 1994));
  // EXPECT_EQ(instance.getEventListElement(6)->date(), s21::Date(1, 12, 1994));
  // EXPECT_EQ(instance.getEventListElement(7)->date(), s21::Date(2, 12, 1994));
  // EXPECT_EQ(instance.getEventListElement(8)->date(), s21::Date(31, 12,
  // 1994)); EXPECT_EQ(instance.getEventListElement(9)->date(), s21::Date(1, 1,
  // 1995));

  EXPECT_EQ(instance.getEventListElement(0)->date(), s21::Date(1, 1, 1994));
  EXPECT_EQ(instance.getEventListElement(1)->date(), s21::Date(28, 2, 1994));
  EXPECT_EQ(instance.getEventListElement(2)->date(), s21::Date(30, 4, 1994));
  EXPECT_EQ(instance.getEventListElement(3)->date(), s21::Date(30, 6, 1994));
  EXPECT_EQ(instance.getEventListElement(4)->date(), s21::Date(31, 8, 1994));
  EXPECT_EQ(instance.getEventListElement(5)->date(), s21::Date(31, 10, 1994));
  EXPECT_EQ(instance.getEventListElement(6)->date(), s21::Date(2, 11, 1994));
  EXPECT_EQ(instance.getEventListElement(7)->date(), s21::Date(2, 12, 1994));
  EXPECT_EQ(instance.getEventListElement(8)->date(), s21::Date(31, 12, 1994));
  EXPECT_EQ(instance.getEventListElement(9)->date(), s21::Date(31, 12, 1994));
  EXPECT_EQ(instance.getEventListElement(10)->date(), s21::Date(1, 1, 1995));
}

TEST(DepositSuite, RegularValues) { /* Examples from calcus.ru */
  s21::TestDeposit::runTestDeposit("tests/deposit_test.txt", DECIMAL_EPS);
}

TEST(DepositSuite, HugeValues) { /* Examples from calcus.ru */
  s21::TestDeposit::runTestDeposit("tests/deposit_test_hard.txt",
                                   DECIMAL_LOOSE_EPS);
}
