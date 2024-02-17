#ifndef S21_CALCULATOR_TEST_H
#define S21_CALCULATOR_TEST_H

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

#include "../model/s21_calculation.h"
#include "../model/s21_common.h"
#include "../model/s21_credit.h"
#include "../model/s21_deposit.h"

#define EPS 1e-07
#define DECIMAL_EPS 0.05
#define DECIMAL_LOOSE_EPS 2.0

namespace s21 {

class TestDeposit {
 public:
  TestDeposit() = default;
  ~TestDeposit() = default;

  static void runTestDeposit(const std::string file, double eps) {
    checkDepositValues(file, eps);
  }

 private:
  static Date proceedDate(std::string& str) {
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

  static void checkEvent(Deposit& instance, std::stringstream& ss, size_t& id,
                         double eps) {
    std::string str;
    double num;
    ss >> str;
    Date date = proceedDate(str);
    EXPECT_EQ(Date(date), instance.getEventListElement(id)->date());
    ss >> num;
    EXPECT_NEAR(num, instance.getEventListElement(id)->gain(), eps);
    ss >> num;
    EXPECT_NEAR(num, instance.getEventListElement(id)->balance_change(), eps);
    ss >> num;
    EXPECT_NEAR(num, instance.getEventListElement(id)->payment(), eps);
    ss >> num;
    EXPECT_NEAR(num, instance.getEventListElement(id)->balance(), eps);
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

  static void checkDepositValues(const std::string file, double eps) {
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
        instance.setTax(tax / 100.0);
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
        if (period == "quarterly")
          instance.setPeriodicity(Deposit::P_QUARTERLY);
        if (period == "biannually")
          instance.setPeriodicity(Deposit::P_BIANNUALLY);
        if (period == "annually") instance.setPeriodicity(Deposit::P_ANNUALLY);
        instance.calculate();
      } else if (node == "event") {
        checkEvent(instance, ss, eid, eps);
      } else if (node == "interest") {
        EXPECT_EQ(eid, instance.getEventListSize());
        double interest;
        ss >> interest;
        EXPECT_NEAR(interest, instance.getInterestTotal(), eps);
      } else if (node == "balance") {
        double balance;
        ss >> balance;
        EXPECT_NEAR(balance, instance.getBalance(), eps);
      } else if (node == "sumreplenish") {
        double sumreplenish;
        ss >> sumreplenish;
        EXPECT_NEAR(sumreplenish, instance.getReplenishTotal(), eps);
      } else if (node == "sumwithdrawal") {
        double sumwithdrawal;
        ss >> sumwithdrawal;
        EXPECT_NEAR(sumwithdrawal, instance.getWithdrawalTotal(), eps);
      } else if (node == "sumtax") {
        EXPECT_EQ(tid, instance.getTaxListSize());
        double sumtax;
        ss >> sumtax;
        EXPECT_NEAR(sumtax, instance.getTaxTotal(), eps);
      } else if (node == "tax") {
        int num;
        double n;
        ss >> num;
        EXPECT_EQ(num, instance.getTaxListElement(tid)->year());
        ss >> n;
        EXPECT_NEAR(n, instance.getTaxListElement(tid)->income(), eps);
        ss >> n;
        EXPECT_NEAR(n, instance.getTaxListElement(tid)->tax(), eps);
        ++tid;
      }
    }
    read.close();
  }
};

}  // namespace s21

#endif  // S21_CALCULATOR_TEST_H
