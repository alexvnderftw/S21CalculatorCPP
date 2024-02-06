#include "s21_test_main.h"

TEST(CreditSuite, AssignValues) {
  s21::Credit instance;
  EXPECT_EQ(instance.GetType(), s21::Credit::ANNUITY);
  EXPECT_EQ(instance.GetTime(), 0);
  EXPECT_NEAR(instance.GetRate(), 0.0, EPS);
  EXPECT_NEAR(instance.GetCredit(), 0.0, EPS);
  EXPECT_EQ(instance.GetStartMonth(), 0);
  EXPECT_EQ(instance.GetStartYear(), 0);
  EXPECT_NEAR(instance.GetSummaryPaid(), 0.0, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 0.0, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 0.0, EPS);
  EXPECT_EQ(instance.GetStartYear(), 0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetDifferential();
  instance.SetTime(12);
  instance.SetRate(10.0);
  instance.SetCredit(100000.0);
  EXPECT_EQ(instance.GetType(), s21::Credit::DIFFERENTIAL);
  instance.SetAnnuity();
  EXPECT_EQ(instance.GetType(), s21::Credit::ANNUITY);
  EXPECT_EQ(instance.GetTime(), 12);
  EXPECT_NEAR(instance.GetRate(), 10.0, EPS);
  EXPECT_NEAR(instance.GetCredit(), 100000.0, EPS);
  EXPECT_NEAR(instance.GetSummaryPaid(), 0.0, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 0.0, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 0.0, EPS);
  EXPECT_EQ(instance.GetDataSize(), 0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetStartDate();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_EQ(instance.GetDataSize(), 12);
}

TEST(CreditSuite, RegularValues) {
  s21::Credit instance;
  instance.SetAnnuity();
  instance.SetTime(4);
  instance.SetRate(0.05);
  instance.SetCredit(100000.0);
  instance.SetStartDate(11, 2024);
  EXPECT_EQ(instance.GetStartMonth(), 11);
  EXPECT_EQ(instance.GetStartYear(), 2024);
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 101043.84, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 100000.0, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 1043.84, EPS);
  EXPECT_EQ(instance.GetDataSize(), 4);
  EXPECT_EQ(instance.GetData()->GetMonth(), 11);
  EXPECT_EQ(instance.GetData()->GetYear(), 2024);
  EXPECT_NEAR(instance.GetData()->GetPayment(), 25260.96, EPS);
  EXPECT_NEAR(instance.GetData()->GetMainPart(), 24844.29, EPS);
  EXPECT_NEAR(instance.GetData()->GetRatePart(), 416.67, EPS);
  EXPECT_NEAR(instance.GetData()->GetLeftover(), 75155.71, EPS);

  EXPECT_EQ((instance.GetData() + 1)->GetMonth(), 12);
  EXPECT_EQ((instance.GetData() + 1)->GetYear(), 2024);
  EXPECT_NEAR((instance.GetData() + 1)->GetPayment(), 25260.96, EPS);
  EXPECT_NEAR((instance.GetData() + 1)->GetMainPart(), 24947.81, EPS);
  EXPECT_NEAR((instance.GetData() + 1)->GetRatePart(), 313.15, EPS);
  EXPECT_NEAR((instance.GetData() + 1)->GetLeftover(), 50207.90, EPS);

  EXPECT_EQ((instance.GetData() + 2)->GetMonth(), 1);
  EXPECT_EQ((instance.GetData() + 2)->GetYear(), 2025);
  EXPECT_NEAR((instance.GetData() + 2)->GetPayment(), 25260.96, EPS);
  EXPECT_NEAR((instance.GetData() + 2)->GetMainPart(), 25051.76, EPS);
  EXPECT_NEAR((instance.GetData() + 2)->GetRatePart(), 209.20, EPS);
  EXPECT_NEAR((instance.GetData() + 2)->GetLeftover(), 25156.14, EPS);

  EXPECT_EQ((instance.GetData() + 3)->GetMonth(), 2);
  EXPECT_EQ((instance.GetData() + 3)->GetYear(), 2025);
  EXPECT_NEAR((instance.GetData() + 3)->GetPayment(), 25260.96, EPS);
  EXPECT_NEAR((instance.GetData() + 3)->GetMainPart(), 25156.14, EPS);
  EXPECT_NEAR((instance.GetData() + 3)->GetRatePart(), 104.82, EPS);
  EXPECT_NEAR((instance.GetData() + 3)->GetLeftover(), -0.00, EPS);

  instance.SetDifferential();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 101041.67, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 100000.0, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 1041.67, EPS);
  EXPECT_EQ(instance.GetDataSize(), 4);
  EXPECT_EQ(instance.GetData()->GetMonth(), 11);
  EXPECT_EQ(instance.GetData()->GetYear(), 2024);
  EXPECT_NEAR(instance.GetData()->GetPayment(), 25416.67, EPS);
  EXPECT_NEAR(instance.GetData()->GetMainPart(), 25000.00, EPS);
  EXPECT_NEAR(instance.GetData()->GetRatePart(), 416.67, EPS);
  EXPECT_NEAR(instance.GetData()->GetLeftover(), 75000.00, EPS);

  EXPECT_EQ((instance.GetData() + 1)->GetMonth(), 12);
  EXPECT_EQ((instance.GetData() + 1)->GetYear(), 2024);
  EXPECT_NEAR((instance.GetData() + 1)->GetPayment(), 25312.50, EPS);
  EXPECT_NEAR((instance.GetData() + 1)->GetMainPart(), 25000.00, EPS);
  EXPECT_NEAR((instance.GetData() + 1)->GetRatePart(), 312.50, EPS);
  EXPECT_NEAR((instance.GetData() + 1)->GetLeftover(), 50000.00, EPS);

  EXPECT_EQ((instance.GetData() + 2)->GetMonth(), 1);
  EXPECT_EQ((instance.GetData() + 2)->GetYear(), 2025);
  EXPECT_NEAR((instance.GetData() + 2)->GetPayment(), 25208.33, EPS);
  EXPECT_NEAR((instance.GetData() + 2)->GetMainPart(), 25000.00, EPS);
  EXPECT_NEAR((instance.GetData() + 2)->GetRatePart(), 208.33, EPS);
  EXPECT_NEAR((instance.GetData() + 2)->GetLeftover(), 25000.00, EPS);

  EXPECT_EQ((instance.GetData() + 3)->GetMonth(), 2);
  EXPECT_EQ((instance.GetData() + 3)->GetYear(), 2025);
  EXPECT_NEAR((instance.GetData() + 3)->GetPayment(), 25104.17, EPS);
  EXPECT_NEAR((instance.GetData() + 3)->GetMainPart(), 25000.00, EPS);
  EXPECT_NEAR((instance.GetData() + 3)->GetRatePart(), 104.17, EPS);
  EXPECT_NEAR((instance.GetData() + 3)->GetLeftover(), 0.00, EPS);

  instance.SetAnnuity();
  instance.SetTime(7);
  instance.SetRate(0.0001);
  instance.SetCredit(0.99);
  instance.SetStartDate(7, 2025);
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 0.98, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 0.98, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 0.00, EPS);
  EXPECT_EQ(instance.GetDataSize(), 7);
  EXPECT_EQ(instance[0].GetMonth(), 7);
  EXPECT_EQ(instance[0].GetYear(), 2025);
  EXPECT_NEAR(instance[0].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[0].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[0].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[0].GetLeftover(), 0.85, EPS);

  EXPECT_EQ(instance[1].GetMonth(), 8);
  EXPECT_EQ(instance[1].GetYear(), 2025);
  EXPECT_NEAR(instance[1].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[1].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[1].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[1].GetLeftover(), 0.71, EPS);

  EXPECT_EQ(instance[2].GetMonth(), 9);
  EXPECT_EQ(instance[2].GetYear(), 2025);
  EXPECT_NEAR(instance[2].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[2].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[2].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[2].GetLeftover(), 0.57, EPS);

  EXPECT_EQ(instance[3].GetMonth(), 10);
  EXPECT_EQ(instance[3].GetYear(), 2025);
  EXPECT_NEAR(instance[3].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[3].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[3].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[3].GetLeftover(), 0.43, EPS);

  EXPECT_EQ(instance[4].GetMonth(), 11);
  EXPECT_EQ(instance[4].GetYear(), 2025);
  EXPECT_NEAR(instance[4].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[4].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[4].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[4].GetLeftover(), 0.29, EPS);

  EXPECT_EQ(instance[5].GetMonth(), 12);
  EXPECT_EQ(instance[5].GetYear(), 2025);
  EXPECT_NEAR(instance[5].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[5].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[5].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[5].GetLeftover(), 0.15, EPS);

  EXPECT_EQ(instance[6].GetMonth(), 1);
  EXPECT_EQ(instance[6].GetYear(), 2026);
  EXPECT_NEAR(instance[6].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[6].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[6].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[6].GetLeftover(), 0.01, EPS);

  instance.SetDifferential();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 0.99, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 0.99, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 0.00, EPS);
  EXPECT_EQ(instance.GetDataSize(), 7);
  EXPECT_EQ(instance[0].GetMonth(), 7);
  EXPECT_EQ(instance[0].GetYear(), 2025);
  EXPECT_NEAR(instance[0].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[0].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[0].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[0].GetLeftover(), 0.85, EPS);

  EXPECT_EQ(instance[1].GetMonth(), 8);
  EXPECT_EQ(instance[1].GetYear(), 2025);
  EXPECT_NEAR(instance[1].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[1].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[1].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[1].GetLeftover(), 0.71, EPS);

  EXPECT_EQ(instance[2].GetMonth(), 9);
  EXPECT_EQ(instance[2].GetYear(), 2025);
  EXPECT_NEAR(instance[2].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[2].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[2].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[2].GetLeftover(), 0.57, EPS);

  EXPECT_EQ(instance[3].GetMonth(), 10);
  EXPECT_EQ(instance[3].GetYear(), 2025);
  EXPECT_NEAR(instance[3].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[3].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[3].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[3].GetLeftover(), 0.42, EPS);

  EXPECT_EQ(instance[4].GetMonth(), 11);
  EXPECT_EQ(instance[4].GetYear(), 2025);
  EXPECT_NEAR(instance[4].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[4].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[4].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[4].GetLeftover(), 0.28, EPS);

  EXPECT_EQ(instance[5].GetMonth(), 12);
  EXPECT_EQ(instance[5].GetYear(), 2025);
  EXPECT_NEAR(instance[5].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[5].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[5].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[5].GetLeftover(), 0.14, EPS);

  EXPECT_EQ(instance[6].GetMonth(), 1);
  EXPECT_EQ(instance[6].GetYear(), 2026);
  EXPECT_NEAR(instance[6].GetPayment(), 0.14, EPS);
  EXPECT_NEAR(instance[6].GetMainPart(), 0.14, EPS);
  EXPECT_NEAR(instance[6].GetRatePart(), 0.00, EPS);
  EXPECT_NEAR(instance[6].GetLeftover(), 0.00, EPS);

  instance.SetAnnuity();
  instance.SetTime(1);
  instance.SetRate(0.10);
  instance.SetCredit(10000);
  instance.SetStartDate(12, 2025);
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 10083.33, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 10000.00, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 83.33, EPS);
  EXPECT_EQ(instance.GetDataSize(), 1);
  EXPECT_EQ(instance[0].GetMonth(), 12);
  EXPECT_EQ(instance[0].GetYear(), 2025);
  EXPECT_NEAR(instance[0].GetPayment(), 10083.33, EPS);
  EXPECT_NEAR(instance[0].GetMainPart(), 10000.00, EPS);
  EXPECT_NEAR(instance[0].GetRatePart(), 83.33, EPS);
  EXPECT_NEAR(instance[0].GetLeftover(), 0.00, EPS);

  instance.SetDifferential();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 10083.33, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 10000.00, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 83.33, EPS);
  EXPECT_EQ(instance.GetDataSize(), 1);
  EXPECT_EQ(instance[0].GetMonth(), 12);
  EXPECT_EQ(instance[0].GetYear(), 2025);
  EXPECT_NEAR(instance[0].GetPayment(), 10083.33, EPS);
  EXPECT_NEAR(instance[0].GetMainPart(), 10000.00, EPS);
  EXPECT_NEAR(instance[0].GetRatePart(), 83.33, EPS);
  EXPECT_NEAR(instance[0].GetLeftover(), 0.00, EPS);

  instance.SetAnnuity();
  instance.SetTime(13);
  instance.SetRate(0.521);
  instance.SetCredit(1000000000);
  instance.SetStartDate();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 1329618681.69, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 999999999.91, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 329618681.78, EPS);
  EXPECT_NEAR(instance[0].GetPayment(), 102278360.13, EPS);
  EXPECT_NEAR(instance[0].GetMainPart(), 58861693.46, EPS);
  EXPECT_NEAR(instance[0].GetRatePart(), 43416666.67, EPS);
  EXPECT_NEAR(instance[0].GetLeftover(), 941138306.54, EPS);

  instance.SetDifferential();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 1303916666.67, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 1000000000.00, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 303916666.67, EPS);
  EXPECT_NEAR(instance[12].GetPayment(), 80262820.51, EPS);
  EXPECT_NEAR(instance[12].GetMainPart(), 76923076.92, EPS);
  EXPECT_NEAR(instance[12].GetRatePart(), 3339743.59, EPS);
  EXPECT_NEAR(instance[12].GetLeftover(), 0.00, EPS);

  instance.SetAnnuity();
  instance.SetTime(600);
  instance.SetRate(9.99);
  instance.SetCredit(100000000000.0);
  instance.SetStartDate();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 49950000000000.00, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 0.00, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 49950000000000.00, EPS);
  EXPECT_NEAR(instance[599].GetPayment(), 83250000000.00, EPS);
  EXPECT_NEAR(instance[599].GetMainPart(), 0.00, EPS);
  EXPECT_NEAR(instance[599].GetRatePart(), 83250000000.00, EPS);
  EXPECT_NEAR(instance[599].GetLeftover(), 100000000000.00, EPS);

  instance.SetDifferential();
  instance.SetTime(600);
  instance.SetRate(9.99);
  instance.SetCredit(100000000000.0);
  instance.SetStartDate();
  EXPECT_EQ(instance.Calculate(), true);
  EXPECT_NEAR(instance.GetSummaryPaid(), 25116625000000.34, EPS);
  EXPECT_NEAR(instance.GetSummaryMainPart(), 100000000000.0, EPS);
  EXPECT_NEAR(instance.GetSummaryRatePart(), 25016625000000.0, EPS);
  EXPECT_NEAR(instance[0].GetPayment(), 83416666666.67, EPS);
  EXPECT_NEAR(instance[0].GetMainPart(), 166666666.67, EPS);
  EXPECT_NEAR(instance[0].GetRatePart(), 83250000000.00, EPS);
  EXPECT_NEAR(instance[0].GetLeftover(), 99833333333.33, EPS);
}

TEST(CreditSuite, ReturnValues) {
  s21::Credit instance;
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetTime(600);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetRate(10.0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetCredit(100000000000.0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetStartDate(1, 1899);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetStartDate(1, 2701);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetStartDate();
  EXPECT_EQ(instance.Calculate(), true);
  instance.SetDifferential();
  EXPECT_EQ(instance.Calculate(), true);
  instance.SetTime(600);
  instance.SetRate(9.99);
  instance.SetCredit(100000000000.0);
  instance.SetStartDate();
  EXPECT_EQ(instance.Calculate(), true);
  instance.SetRate(11.0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetRate(0.0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetRate(NAN);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetRate(INFINITY);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetRate(9.99);
  EXPECT_EQ(instance.Calculate(), true);
  instance.SetTime(1001);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetTime(0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetTime(1000);
  EXPECT_EQ(instance.Calculate(), true);
  instance.SetCredit(1000000000000.0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetCredit(NAN);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetCredit(INFINITY);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetCredit(0.0);
  EXPECT_EQ(instance.Calculate(), false);
  instance.SetCredit(100000000000.0);
  EXPECT_EQ(instance.Calculate(), true);
  instance.SetTime(1000);
  instance.SetRate(10.0);
  instance.SetCredit(100000000000.0);
  EXPECT_EQ(instance.Calculate(), true);
}
