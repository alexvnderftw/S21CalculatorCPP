#include <fstream>
#include <sstream>
#include <string>

#include "s21_test_main.h"

namespace s21 {

Date proceedDate(std::string& str) {
  int d, m, y;
  std::string token;
  std::stringstream dss(str);
  std::getline(dss, token, '.');
  d = std::stoi(token);
  std::getline(dss, token, '.');
  m = std::stoi(token);
  std::getline(dss, token, '.');
  y = std::stoi(token);
  return Date(d, m, y);
}

void checkEvent(Deposit& instance, std::stringstream& ss, size_t& id) {
  std::string str;
  double num;
  ss >> str;
  Date date = proceedDate(str);
  EXPECT_EQ(Date(date), instance.getEventListElement(id)->date());
  ss >> num;
  EXPECT_NEAR(num, instance.getEventListElement(id)->gain(), D_EPS);
  ss >> num;
  EXPECT_NEAR(num, instance.getEventListElement(id)->balance_change(), D_EPS);
  ss >> num;
  EXPECT_NEAR(num, instance.getEventListElement(id)->payment(), D_EPS);
  ss >> num;
  EXPECT_NEAR(num, instance.getEventListElement(id)->balance(), D_EPS);
  ss >> str;
  Deposit::EventType event;
  if (str == "replenish") event = Deposit::E_REPLENISH;
  if (str == "withdrawal") event = Deposit::E_WITHDRAWAL;
  if (str == "payment") event = Deposit::E_PAYDAY;
  if (str == "newyear") event = Deposit::E_NEWYEAR;
  if (str == "decline") event = Deposit::E_DECLINE;
  EXPECT_EQ(event, instance.getEventListElement(id)->event());
  ++id;
}

void checkDepositValues(std::string file) {
  std::ifstream read(file);
  Deposit instance;
  size_t eid = 0;
  size_t tid = 0;

  EXPECT_EQ(read.is_open(), true);
  for (std::string line; std::getline(read, line);) {
    // std::replace_if(
    //     std::begin(line), std::end(line),
    //     [](char x) { return std::ispunct(x); }, ' ');
    std::stringstream ss(line);
    std::string node;
    ss >> node;

    if (node == "TEST") {
      instance.clearReplenish();
      instance.clearWithdrawal();
      eid = 0;
      tid = 0;
    } else if (node == "deposit") {
      double deposit;
      ss >> deposit;
      instance.setDeposit(deposit);
    } else if (node == "term") {
      double term;
      ss >> term;
      instance.setTerm(term);
    } else if (node == "termtype") {
      std::string termtype;
      ss >> termtype;
      if (termtype == "year") instance.setTermType(Deposit::T_YEAR);
      if (termtype == "month") instance.setTermType(Deposit::T_MONTH);
      if (termtype == "day") instance.setTermType(Deposit::T_DAY);
    } else if (node == "date") {
      std::string sdate;
      ss >> sdate;
      Date date = proceedDate(sdate);
      instance.setStartDate(date);
    } else if (node == "rate") {
      double rate;
      ss >> rate;
      instance.setInterest(rate / 100.0);
    } else if (node == "taxrate") {
      double tax;
      ss >> tax;
      instance.setTax(tax);
    } else if (node == "capital") {
      int cap;
      ss >> cap;
      if (cap == 0)
        instance.setCapitalization(false);
      else
        instance.setCapitalization(true);
    } else if (node == "replenish") {
      std::string str;
      double value;
      Date date;
      Deposit::OperPeriod period;
      ss >> str;
      if (str == "once") period = Deposit::O_ONCE;
      if (str == "monthly") period = Deposit::O_MONTHLY;
      if (str == "bimonthly") period = Deposit::O_BIMONTHLY;
      if (str == "quarterly") period = Deposit::O_QUARTERLY;
      if (str == "biannually") period = Deposit::O_BIANNUALLY;
      if (str == "annually") period = Deposit::O_ANNUALLY;
      ss >> str;
      date = proceedDate(str);
      ss >> value;
      instance.addReplenish(period, date, value);
    } else if (node == "withdrawal") {
      std::string str;
      double value;
      Date date;
      Deposit::OperPeriod period;
      ss >> str;
      if (str == "once") period = Deposit::O_ONCE;
      if (str == "monthly") period = Deposit::O_MONTHLY;
      if (str == "bimonthly") period = Deposit::O_BIMONTHLY;
      if (str == "quarterly") period = Deposit::O_QUARTERLY;
      if (str == "biannually") period = Deposit::O_BIANNUALLY;
      if (str == "annually") period = Deposit::O_ANNUALLY;
      ss >> str;
      date = proceedDate(str);
      ss >> value;
      instance.addWithdrawal(period, date, value);
    } else if (node == "period") {
      std::string period;
      ss >> period;
      if (period == "at_end") instance.setPeriodicity(Deposit::P_AT_END);
      if (period == "daily") instance.setPeriodicity(Deposit::P_DAILY);
      if (period == "weekly") instance.setPeriodicity(Deposit::P_WEEKLY);
      if (period == "monthly") instance.setPeriodicity(Deposit::P_MONTHLY);
      if (period == "quarterly") instance.setPeriodicity(Deposit::P_QUARTERLY);
      if (period == "biannually")
        instance.setPeriodicity(Deposit::P_BIANNUALLY);
      if (period == "annually") instance.setPeriodicity(Deposit::P_ANNUALLY);
      instance.calculate();
    } else if (node == "event") {
      checkEvent(instance, ss, eid);
    } else if (node == "interest") {
      EXPECT_EQ(eid, instance.getEventListSize());
      double interest;
      ss >> interest;
      EXPECT_NEAR(interest, instance.getInterestTotal(), D_EPS);
    } else if (node == "balance") {
      double balance;
      ss >> balance;
      EXPECT_NEAR(balance, instance.getBalance(), D_EPS);
    } else if (node == "sumreplenish") {
      double sumreplenish;
      ss >> sumreplenish;
      EXPECT_NEAR(sumreplenish, instance.getReplenishTotal(), D_EPS);
    } else if (node == "sumwithdrawal") {
      double sumwithdrawal;
      ss >> sumwithdrawal;
      EXPECT_NEAR(sumwithdrawal, instance.getWithdrawalTotal(), D_EPS);
    } else if (node == "sumtax") {
      EXPECT_EQ(tid, instance.getTaxListSize());
      double sumtax;
      ss >> sumtax;
      EXPECT_NEAR(sumtax, instance.getTaxTotal(), D_EPS);
    } else if (node == "tax") {
      int num;
      double n;
      ss >> num;
      EXPECT_EQ(num, instance.getTaxListElement(tid)->year());
      ss >> n;
      EXPECT_NEAR(n, instance.getTaxListElement(tid)->income(), D_EPS);
      ss >> n;
      EXPECT_NEAR(n, instance.getTaxListElement(tid)->tax(), D_EPS);
      ++tid;
    }
  }
  read.close();
}
}  // namespace s21

TEST(DepositSuite, SetValues) {
  s21::Deposit instance;
  EXPECT_FALSE(instance.calculate());
  instance.setDeposit(100000.123);
  EXPECT_EQ(instance.getDeposit(), 100000.123);
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
  EXPECT_TRUE(instance.calculate());
  instance.setCapitalization(true);
  EXPECT_TRUE(instance.calculate());
  instance.setPeriodicity(s21::Deposit::P_MONTHLY);
  EXPECT_TRUE(instance.calculate());
  instance.setRemainderLimit(1000.0);
  EXPECT_TRUE(instance.calculate());
  instance.addReplenish(s21::Deposit::O_ONCE, s21::Date(2, 11, 1994), 1001);
  EXPECT_TRUE(instance.calculate());
  instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(31, 12, 1993),
                         4000);
  EXPECT_TRUE(instance.calculate());
}

// TEST(DepositSuite, CheckLimitValues) {
//   s21::Deposit instance;
//   instance.setDeposit(100000.123);
//   instance.setTerm(2);
//   instance.setTermType(s21::Deposit::T_YEAR);
//   EXPECT_TRUE(instance.setStartDate(14, 2, 1994));
//   instance.setInterest(0.0);
//   instance.setTax(0.1);
//   instance.setRemainderLimit(1000.0);
//   instance.addReplenish(s21::Deposit::O_ANNUALLY, s21::Date(2, 11, 1994),
//   1001); instance.addWithdrawal(s21::Deposit::O_BIANNUALLY, s21::Date(31, 12,
//   1993),
//                          4000);
//   EXPECT_TRUE(instance.calculate());

//   /* Deposit value */
//   instance.setDeposit(s21::Deposit::MAX_DOUBLE_VALUE);
//   EXPECT_FALSE(instance.calculate());
//   instance.setDeposit(NAN);
//   EXPECT_FALSE(instance.calculate());
//   instance.setDeposit(INFINITY);
//   EXPECT_FALSE(instance.calculate());
//   instance.setDeposit(0.0);
//   EXPECT_TRUE(instance.calculate());
//   instance.setDeposit(-100000.02);
//   EXPECT_FALSE(instance.calculate());
//   instance.setDeposit(s21::Deposit::MAX_DEPOSIT_VALUE);
//   EXPECT_TRUE(instance.calculate());
//   instance.setDeposit(s21::Deposit::MAX_DEPOSIT_VALUE * 10.0);
//   EXPECT_FALSE(instance.calculate());
//   instance.setDeposit(100000.123);

//   /* Term value */
//   instance.setTermType(s21::Deposit::T_YEAR);
//   instance.setTerm(s21::Deposit::MAX_TERM_Y);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTerm(s21::Deposit::MAX_TERM_Y + 1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(s21::Deposit::MAX_TERM_Y - 1);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTerm(0);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(-1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTermType(s21::Deposit::T_MONTH);
//   instance.setTerm(s21::Deposit::MAX_TERM_M);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTerm(s21::Deposit::MAX_TERM_M + 1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(s21::Deposit::MAX_TERM_M - 1);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTerm(0);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(-1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTermType(s21::Deposit::T_DAY);
//   instance.setTerm(s21::Deposit::MAX_TERM_D);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTerm(s21::Deposit::MAX_TERM_D + 1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(s21::Deposit::MAX_TERM_D - 1);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTerm(0);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(-1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTerm(525);

//   /* Date value */
//   EXPECT_TRUE(instance.setStartDate(1, 1, 1900));
//   EXPECT_TRUE(instance.calculate());
//   EXPECT_FALSE(instance.setStartDate(31, 12, 1899));
//   EXPECT_TRUE(instance.calculate());
//   EXPECT_TRUE(instance.setStartDate(31, 12, s21::Deposit::MAX_START_YEAR));
//   EXPECT_TRUE(instance.calculate());
//   EXPECT_TRUE(instance.setStartDate(1, 1, s21::Deposit::MAX_START_YEAR + 1));
//   EXPECT_FALSE(instance.calculate());
//   EXPECT_FALSE(instance.setStartDate(31, 12, -1899));
//   EXPECT_FALSE(instance.setStartDate(0, 12, 1920));
//   EXPECT_TRUE(instance.setStartDate(31, 12, s21::Deposit::MAX_START_YEAR));
//   EXPECT_TRUE(instance.calculate());

//   /* Interest rate value */
//   instance.setInterest(-0.01);
//   EXPECT_FALSE(instance.calculate());
//   instance.setInterest(0.0);
//   EXPECT_TRUE(instance.calculate());
//   instance.setInterest(NAN);
//   EXPECT_FALSE(instance.calculate());
//   instance.setInterest(INFINITY);
//   EXPECT_FALSE(instance.calculate());
//   instance.setInterest(s21::Deposit::MAX_DOUBLE_VALUE);
//   EXPECT_FALSE(instance.calculate());
//   instance.setInterest(s21::Deposit::MAX_RATE);
//   EXPECT_TRUE(instance.calculate());
//   instance.setInterest(s21::Deposit::MAX_RATE + 0.01);
//   EXPECT_FALSE(instance.calculate());
//   instance.setInterest(0.000001);
//   EXPECT_TRUE(instance.calculate());

//   /* Tax rate value */
//   instance.setTax(-0.01);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTax(0.0);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTax(NAN);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTax(INFINITY);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTax(s21::Deposit::MAX_DOUBLE_VALUE);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTax(s21::Deposit::MAX_TAX);
//   EXPECT_TRUE(instance.calculate());
//   instance.setTax(s21::Deposit::MAX_RATE + 0.0001);
//   EXPECT_FALSE(instance.calculate());
//   instance.setTax(0.000001);
//   EXPECT_TRUE(instance.calculate());

//   /* Remainder limit value */
//   instance.setRemainderLimit(0.0);
//   EXPECT_TRUE(instance.calculate());
//   instance.setRemainderLimit(-0.1);
//   EXPECT_FALSE(instance.calculate());
//   instance.setRemainderLimit(NAN);
//   EXPECT_FALSE(instance.calculate());
//   instance.setRemainderLimit(INFINITY);
//   EXPECT_FALSE(instance.calculate());
//   instance.setRemainderLimit(s21::Deposit::MAX_DOUBLE_VALUE);
//   EXPECT_FALSE(instance.calculate());
//   instance.setRemainderLimit(s21::Deposit::MAX_DOUBLE_VALUE / 10.0);
//   EXPECT_TRUE(instance.calculate());
//   instance.setRemainderLimit(1000.0);
//   EXPECT_TRUE(instance.calculate());

//   /* Replenish values */
//   EXPECT_TRUE(instance.setStartDate(s21::Date()));
//   instance.addReplenish(s21::Deposit::O_ONCE, s21::Date(), 1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(2, 11, 1994),
//   1000); EXPECT_TRUE(instance.calculate());
//   instance.addReplenish(s21::Deposit::O_BIMONTHLY, s21::Date(2, 12, 1994),
//                         1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.addReplenish(s21::Deposit::O_QUARTERLY, s21::Date(29, 2, 2000),
//                         1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.addReplenish(s21::Deposit::O_ANNUALLY, s21::Date(20, 7, 1998),
//   2000); EXPECT_TRUE(instance.calculate());
//   EXPECT_EQ(instance.getReplenishListSize(), 6);
//   EXPECT_EQ(instance.getReplenishListElement(0)->period(),
//             s21::Deposit::O_ANNUALLY);
//   EXPECT_EQ(instance.getReplenishListElement(0)->date(),
//             s21::Date(2, 11, 1994));
//   EXPECT_EQ(instance.getReplenishListElement(0)->value(), 1001);
//   EXPECT_EQ(instance.getReplenishListElement(5)->period(),
//             s21::Deposit::O_ANNUALLY);
//   EXPECT_EQ(instance.getReplenishListElement(5)->date(),
//             s21::Date(20, 7, 1998));
//   EXPECT_EQ(instance.getReplenishListElement(5)->value(), 2000);
//   instance.popBackReplenish();
//   EXPECT_EQ(instance.getReplenishListSize(), 5);

//   instance.clearReplenish();
//   EXPECT_EQ(instance.getReplenishListSize(), 0);
//   instance.popBackReplenish();
//   EXPECT_EQ(instance.getReplenishListSize(), 0);
//   instance.clearReplenish();
//   EXPECT_EQ(instance.getReplenishListSize(), 0);

//   instance.addReplenish(s21::Deposit::O_MONTHLY,
//                         s21::Date(31, 12, s21::Deposit::MAX_START_YEAR),
//                         1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.popBackReplenish();
//   instance.addReplenish(s21::Deposit::O_MONTHLY,
//                         s21::Date(1, 1, s21::Deposit::MAX_START_YEAR + 1),
//                         1000);
//   EXPECT_FALSE(instance.calculate());
//   instance.popBackReplenish();

//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(31, 12, 2000),
//   0.0); EXPECT_TRUE(instance.calculate()); instance.popBackReplenish();
//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000), -1);
//   EXPECT_FALSE(instance.calculate());
//   instance.removeReplenish(0);
//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
//                         s21::Deposit::MAX_DOUBLE_VALUE);
//   EXPECT_FALSE(instance.calculate());
//   instance.removeReplenish(0);
//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
//                         s21::Deposit::MAX_DEPOSIT_VALUE);
//   EXPECT_TRUE(instance.calculate());
//   instance.removeReplenish(0);
//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
//                         s21::Deposit::MAX_DEPOSIT_VALUE * 10);
//   EXPECT_FALSE(instance.calculate());
//   instance.removeReplenish(0);

//   /* Withdrawal values */
//   EXPECT_TRUE(instance.setStartDate(s21::Date()));
//   instance.addWithdrawal(s21::Deposit::O_ONCE, s21::Date(), 1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(2, 11, 1994),
//   1000); EXPECT_TRUE(instance.calculate());
//   instance.addWithdrawal(s21::Deposit::O_BIMONTHLY, s21::Date(2, 12, 1994),
//                          1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.addWithdrawal(s21::Deposit::O_QUARTERLY, s21::Date(29, 2, 2000),
//                          1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.addWithdrawal(s21::Deposit::O_ANNUALLY, s21::Date(20, 7, 1998),
//                          2000);
//   EXPECT_TRUE(instance.calculate());
//   EXPECT_EQ(instance.getWithdrawalListSize(), 6);
//   EXPECT_EQ(instance.getWithdrawalListElement(0)->period(),
//             s21::Deposit::O_BIANNUALLY);
//   EXPECT_EQ(instance.getWithdrawalListElement(0)->date(),
//             s21::Date(31, 12, 1993));
//   EXPECT_EQ(instance.getWithdrawalListElement(0)->value(), 4000);
//   EXPECT_EQ(instance.getWithdrawalListElement(5)->period(),
//             s21::Deposit::O_ANNUALLY);
//   EXPECT_EQ(instance.getWithdrawalListElement(5)->date(),
//             s21::Date(20, 7, 1998));
//   EXPECT_EQ(instance.getWithdrawalListElement(5)->value(), 2000);
//   instance.popBackWithdrawal();
//   EXPECT_EQ(instance.getWithdrawalListSize(), 5);

//   instance.clearWithdrawal();
//   EXPECT_EQ(instance.getWithdrawalListSize(), 0);
//   instance.popBackWithdrawal();
//   EXPECT_EQ(instance.getWithdrawalListSize(), 0);
//   instance.clearWithdrawal();
//   EXPECT_EQ(instance.getWithdrawalListSize(), 0);

//   instance.addWithdrawal(s21::Deposit::O_MONTHLY,
//                          s21::Date(31, 12, s21::Deposit::MAX_START_YEAR),
//                          1000);
//   EXPECT_TRUE(instance.calculate());
//   instance.popBackWithdrawal();
//   instance.addWithdrawal(s21::Deposit::O_MONTHLY,
//                          s21::Date(1, 1, s21::Deposit::MAX_START_YEAR + 1),
//                          1000);
//   EXPECT_FALSE(instance.calculate());
//   instance.popBackWithdrawal();

//   instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(31, 12, 2000),
//   0.0); EXPECT_TRUE(instance.calculate()); instance.popBackWithdrawal();
//   instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000), -1);
//   EXPECT_FALSE(instance.calculate());
//   instance.removeWithdrawal(0);
//   instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
//                          s21::Deposit::MAX_DOUBLE_VALUE);
//   EXPECT_FALSE(instance.calculate());
//   instance.removeWithdrawal(0);
//   instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
//                          s21::Deposit::MAX_DEPOSIT_VALUE);
//   EXPECT_TRUE(instance.calculate());
//   instance.removeWithdrawal(0);
//   instance.addWithdrawal(s21::Deposit::O_MONTHLY, s21::Date(1, 1, 2000),
//                          s21::Deposit::MAX_DEPOSIT_VALUE * 10);
//   EXPECT_FALSE(instance.calculate());
//   instance.removeWithdrawal(0);
// }

// TEST(DepositSuite, NullValues) {
//   s21::Deposit instance;
//   instance.setDeposit(0.0);
//   instance.setTerm(1);
//   instance.setTermType(s21::Deposit::T_YEAR);
//   EXPECT_TRUE(instance.setStartDate(1, 1, 1994));
//   instance.setInterest(0.0);
//   instance.setTax(0.0);
//   instance.setRemainderLimit(0.0);
//   instance.setCapitalization(false);
//   instance.setPeriodicity(s21::Deposit::P_AT_END);
//   instance.addReplenish(s21::Deposit::O_MONTHLY, s21::Date(2, 11, 1994), 0);
//   instance.addWithdrawal(s21::Deposit::O_BIMONTHLY, s21::Date(31, 12, 1993),
//   0); EXPECT_TRUE(instance.calculate());

//   EXPECT_EQ(instance.getBalance(), 0.0);
//   EXPECT_EQ(instance.getInterestTotal(), 0.0);
//   EXPECT_EQ(instance.getTaxTotal(), 0.0);
//   EXPECT_EQ(instance.getReplenishTotal(), 0.0);
//   EXPECT_EQ(instance.getWithdrawalTotal(), 0.0);
//   EXPECT_EQ(instance.getEventListSize(), 11);
//   EXPECT_EQ(instance.getTaxListSize(), 2);
// }

// TEST(DepositSuite, RegularValues) { /* Examples from calcus.ru */
//   s21::checkDepositValues("tests/deposit_test.txt");
// }