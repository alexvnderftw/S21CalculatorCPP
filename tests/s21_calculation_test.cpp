#include "s21_test_main.h"

TEST(CalculationSuite, RegularValues) {
  s21::Calculation instance;

  // instance.SetExpression("2 + 2");
  // std::cout << instance.GetStatus() << std::endl;
  // EXPECT_EQ(instance.GetStatus(), PARSE_ERROR);
  EXPECT_NEAR(instance.GetResult("0"), 0.0, EPS);
  EXPECT_EQ(instance.GetStatus(), s21::Calculation::COMPLETED);
  EXPECT_EQ(instance.GetExpression(), "0");
  instance.SetExpression("TESTING NOW ORRIES");
  EXPECT_EQ(instance.GetExpression(), "TESTING NOW ORRIES");
  EXPECT_NEAR(instance.GetResult("+sqrt(4)"), 2, EPS);
  EXPECT_NEAR(instance.GetResult("2 + 2"), 4.0, EPS);
  EXPECT_NEAR(instance.GetResult(" 2 + 2"), 4.0, EPS);
  EXPECT_NEAR(instance.GetResult("  2 + 2"), 4.0, EPS);
  EXPECT_NEAR(instance.GetResult(" 2 + 2 "), 4.0, EPS);
  EXPECT_NEAR(instance.GetResult("  2 + 2  "), 4.0, EPS);
  EXPECT_NEAR(instance.GetResult("2 +  2  "), 4.0, EPS);
  EXPECT_NEAR(instance.GetResult("999999999 + 2.9"), 1000000001.9, EPS);
  EXPECT_NEAR(instance.GetResult("1 -2"), -1, EPS);
  EXPECT_NEAR(instance.GetResult("1 --2"), 3, EPS);
  EXPECT_NEAR(instance.GetResult("1 - ~2"), 3, EPS);
  EXPECT_NEAR(instance.GetResult("+1 - +2"), -1, EPS);
  EXPECT_NEAR(instance.GetResult("1 ---2"), -1, EPS);
  EXPECT_NEAR(instance.GetResult("2.8 * 2.9"), 8.12, EPS);
  EXPECT_NEAR(instance.GetResult("2.8- 2.9"), -0.1, EPS);
  EXPECT_NEAR(instance.GetResult("2.8 /2.9"), 0.96551724137931, EPS);
  EXPECT_NEAR(instance.GetResult("2.8mod +2.9"), 2.8, EPS);
  EXPECT_NEAR(instance.GetResult("2 mod-1"), 0.0, EPS);
  EXPECT_NEAR(instance.GetResult("999999999e-10 + 2.9"), 2.9999999999, EPS);
  EXPECT_NEAR(instance.GetResult("-2.8e+4 * 2.9"), -81200, EPS);
  EXPECT_NEAR(instance.GetResult("0.2e25 - 2.9e-4"), 2e24, EPS);
  EXPECT_NEAR(instance.GetResult("2.8e3 / 2.9e+7"), 0.000096551724138, EPS);
  EXPECT_NEAR(instance.GetResult("999999999 + +2.9"), 1000000001.9, EPS);
  EXPECT_NEAR(instance.GetResult("2.8 * -2.9"), -8.12, EPS);
  EXPECT_NEAR(instance.GetResult("2.8--2.9"), 5.7, EPS);
  EXPECT_NEAR(instance.GetResult("2.8 /-2.9"), -0.96551724137931, EPS);
  EXPECT_NEAR(instance.GetResult("2.8 + --+-+2.9"), -0.1, EPS);
  EXPECT_NEAR(instance.GetResult("tan(2.8) + 2.9"), 2.544470168348824, EPS);
  EXPECT_NEAR(instance.GetResult("(2.8) + 2.9"), 5.7, EPS);
  EXPECT_NEAR(instance.GetResult("2.8 + .9"), 3.7, EPS);
  //  EXPECT_NEAR(instance.GetResult(""), , EPS);
}

TEST(CalculationSuite, ErrorValues) {
  s21::Calculation instance;
  EXPECT_EQ(std::isnan(instance.GetResult("")), std::isnan(NAN));
  EXPECT_EQ(instance.GetStatus(), s21::Calculation::PARSE_ERROR);
  EXPECT_EQ(std::isnan(instance.GetResult("    ")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("1 ~2")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("aa +  2  ")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 * - 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 * - + 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 * /2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 aaaa2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 --- 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8+++ 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 +2.9 +")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult(")2.8 + 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8 +tan 2.9")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8+2 (")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("\t2.8+ 2")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8\n+ 2")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8+ 2\n ")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("2.8+ 2ADASDDSA")), std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("sin(1/3) sin(2*1.89)")),
            std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("x sin(2*1.89)", 2.9)),
            std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("xsin(2*1.89)", 2.9)),
            std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("25.1e10 sin(2*1.89)")),
            std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("+")), std::isnan(NAN));
  EXPECT_EQ(instance.GetStatus(), s21::Calculation::CALCULATE_ERROR);
  EXPECT_EQ(std::isnan(instance.GetResult("72+")), std::isnan(NAN));
  EXPECT_EQ(instance.GetStatus(), s21::Calculation::CALCULATE_ERROR);
  EXPECT_EQ(std::isnan(instance.GetResult("*")), std::isnan(NAN));
  EXPECT_EQ(instance.GetStatus(), s21::Calculation::CALCULATE_ERROR);
  EXPECT_EQ(std::isnan(instance.GetResult("sin()")), std::isnan(NAN));
  EXPECT_EQ(instance.GetStatus(), s21::Calculation::CALCULATE_ERROR);
}

TEST(CalculationSuite, MultipleOperators) {
  s21::Calculation instance;
  EXPECT_NEAR(instance.GetResult("2.8 + 1.9 + 2"), 6.7, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + (-2)^ 2"), 9.32, EPS);
  EXPECT_EQ(std::isnan(instance.GetResult("  2.8*1.9 + sqrt(-2)")),
            std::isnan(NAN));
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + -sqrt(2)"), 3.905786437626905,
              EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 +-(sqrt(2))"), 3.905786437626905,
              EPS);
  EXPECT_NEAR(instance.GetResult("  (((2.8*1.9))) +-(sqrt(2))"),
              3.905786437626905, EPS);
}

TEST(CalculationSuite, WithX) {
  s21::Calculation instance;
  instance.SetX(42.0);
  EXPECT_EQ(instance.GetX(), 42.0);
  EXPECT_NEAR(instance.GetResult("2.8 + x + 2", 2.0), 6.8, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + (-2)^ x", 7.0), -122.68, EPS);
  EXPECT_EQ(std::isnan(instance.GetResult("  x*1.9 + sqrt(-2)", -2.7)),
            std::isnan(NAN));
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + -sqrt(x)", 2.7),
              3.6768323274845017, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 +-(sqrt(2)) + -x", -2.7),
              6.605786437626905, EPS);
  EXPECT_NEAR(instance.GetResult("  (((2.8*1.9)^x)) +-(sqrt(2 * x))", 9.1),
              4034552.7185296757479916, EPS);
  EXPECT_NEAR(instance.GetResult("2^2x", -1.7), -6.8, EPS);
  EXPECT_EQ(instance.GetX(), -1.7);
}

TEST(CalculationSuite, WithStringX) {
  s21::Calculation instance;
  instance.SetX(42.0);
  EXPECT_EQ(instance.GetX(), 42.0);
  EXPECT_NEAR(instance.GetResult("2.8 + x + 2", "2.0"), 6.8, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + (-2)^ x", "7.0"), -122.68, EPS);
  EXPECT_EQ(std::isnan(instance.GetResult("  x*1.9 + sqrt(-2)", "-2.7")),
            std::isnan(NAN));
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + -sqrt(x)", " 2.7  "),
              3.6768323274845017, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 +-(sqrt(2)) + -x", "-2.7"),
              6.605786437626905, EPS);
  EXPECT_NEAR(instance.GetResult("  (((2.8*1.9)^x)) +-(sqrt(2 * x))", "9.1"),
              4034552.7185296757479916, EPS);
  EXPECT_NEAR(instance.GetResult("2^2x", "-1.7"), -6.8, EPS);
  EXPECT_NEAR(instance.GetResult("2^2x", "+1.12e4"), 44800, EPS);
  EXPECT_EQ(instance.GetX(), 1.12e4);
  EXPECT_EQ(std::isnan(instance.GetResult("2^2x", "NAN")), std::isnan(NAN));
  EXPECT_EQ(std::isinf(instance.GetResult("  x", "inf")), std::isinf(INFINITY));
  EXPECT_EQ(std::isinf(instance.GetResult("  x", "+inf")),
            std::isinf(INFINITY));
  EXPECT_EQ(std::isinf(instance.GetResult("  x", "INFINITY")),
            std::isinf(INFINITY));
  EXPECT_EQ(std::isnan(instance.GetResult("  x*1.9 + sqrt(-2)", "nan")),
            std::isnan(NAN));
  EXPECT_EQ(std::isnan(instance.GetResult("  x*1.9 + sqrt(-2)", "asdfDS")),
            std::isnan(NAN));
  EXPECT_EQ(instance.GetStatus(), instance.COMPLETED);
  EXPECT_EQ(std::isnan(instance.GetResult("  x*1.9 + sqrt(-2)", "")),
            std::isnan(NAN));
}

TEST(CalculationSuite, HiddenMultiplication) {
  s21::Calculation instance;
  EXPECT_NEAR(instance.GetResult("2.8x + 1.9 + 2", 2.0), 9.5, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + 2(-2)^ 2", 2.0), 13.32, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + 5.19sqrt(2)"), 12.659768389, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + -2sqrt(2)", -2.7), 2.491572875,
              EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 +-(sqrt(2x))", 9.1),
              1.0538541984596916, EPS);
  EXPECT_NEAR(instance.GetResult("  (99((2.8*1.9))) +-(sqrt(2))", -1.7),
              525.265786438, EPS);
  EXPECT_NEAR(instance.GetResult("1 / -1 + 100"), 99.0, EPS);
  EXPECT_EQ(instance.GetTrigValue(), s21::Calculation::RAD);
  instance.SetDegree();
  EXPECT_EQ(instance.GetTrigValue(), s21::Calculation::DEG);
  EXPECT_NEAR(instance.GetResult("sin(1/3)sin(2*1.89)"), 0.0003835374182967,
              EPS);
  EXPECT_NEAR(instance.GetResult("sin(1/3)*sin(2*1.89)"), 0.0003835374182967,
              EPS);
  EXPECT_NEAR(instance.GetResult("sin(1/3)(2*1.89)"), 0.0219910245219, EPS);
  EXPECT_NEAR(instance.GetResult("(1/3)(2*1.89)"), 1.26, EPS);
  instance.SetRadian();
  EXPECT_EQ(instance.GetTrigValue(), s21::Calculation::RAD);
  EXPECT_NEAR(instance.GetResult("sin(1/3)sin(2*1.89)"), -0.1949809554, EPS);
  EXPECT_NEAR(instance.GetResult("sin(1/3)*sin(2*1.89)"), -0.1949809554, EPS);
  EXPECT_NEAR(instance.GetResult("sin(1/3)(2*1.89)"), 1.2367959539, EPS);
}

TEST(CalculationSuite, Trigonometry) {
  s21::Calculation instance;
  instance.SetDegree();
  EXPECT_NEAR(instance.GetResult("sin(70)"), 0.939692620786, EPS);
  EXPECT_NEAR(instance.GetResult("cos(88.1223)"), 0.032766181445, EPS);
  EXPECT_NEAR(instance.GetResult("tan(-123.11)"), 1.533411824626, EPS);
  EXPECT_NEAR(instance.GetResult("asin(0.123)"), 7.06527293065, EPS);
  EXPECT_NEAR(instance.GetResult("acos(0.99999)"), 0.256234729159, EPS);
  EXPECT_NEAR(instance.GetResult("atan(-133.923)"), -89.572181666388, EPS);
  EXPECT_NEAR(instance.GetResult("asin(-0.123)"), -7.06527293065, EPS);
  EXPECT_NEAR(instance.GetResult("acos(-0.99999)"), 179.743765270841, EPS);
  EXPECT_NEAR(instance.GetResult("ln(sqrt(acos(-0.99999)))"), 2.595766155220497,
              EPS);
  instance.SetRadian();
  EXPECT_NEAR(instance.GetResult("sin(70)"), 0.7738906816, EPS);
  EXPECT_NEAR(instance.GetResult("cos(88.1223)"), 0.987590208609, EPS);
  EXPECT_NEAR(instance.GetResult("tan(-123.11)"), -0.666498988961, EPS);
  EXPECT_NEAR(instance.GetResult("asin(0.123)"), 0.123312275192, EPS);
  EXPECT_NEAR(instance.GetResult("acos(0.99999)"), 0.004472139682, EPS);
  EXPECT_NEAR(instance.GetResult("atan(-133.923)"), -1.563329488273, EPS);
  EXPECT_NEAR(instance.GetResult("asin(-0.123)"), -0.123312275192, EPS);
  EXPECT_NEAR(instance.GetResult("acos(-0.99999)"), 3.137120513908, EPS);
  EXPECT_NEAR(instance.GetResult("ln(sqrt(acos(-0.99999)))"),
              0.5716526727000901, EPS);
}

TEST(CalculationSuite, Others) {
  s21::Calculation instance;
  EXPECT_NEAR(instance.GetResult("2.8 + x + 2", 2.1), 6.9, EPS);
  EXPECT_NEAR(instance.GetResult("  2.8*1.9 + (-2)^ x", 9.0), -506.68, EPS);
  // #ifdef __linux__
  //   EXPECT_NEAR(instance.GetResult("  2.8*1.9 + (-2)^ x", 9999), -INFINITY,
  //   EPS);
  // #elif __APPLE__
  //   EXPECT_NEAR(instance.GetResult("  2.8*1.9 + (-2)^ x", 9999), INFINITY,
  //   EPS);
  // #endif
  EXPECT_EQ(std::isinf(instance.GetResult("  2.8*1.9 + (-2)^ x", 9999)), true);
  EXPECT_EQ(std::isnan(instance.GetResult("  x*1.9 + sqrt(-2)", 1.0)),
            std::isnan(NAN));
  EXPECT_NEAR(instance.GetResult("  x*1.9 + sqrt(2)", -199.1231),
              -376.919676438, EPS);
  EXPECT_NEAR(instance.GetResult(" x^2", 3.0), 9.0, EPS);
  EXPECT_NEAR(instance.GetResult(" 1/2.8*1.9", 3.0), 0.678571429, EPS);
  EXPECT_NEAR(instance.GetResult(" x^2+1/2.8*1.9", 3.0), 9.678571429, EPS);
  EXPECT_NEAR(instance.GetResult(" x^2+1/2.8*1.9 + -ln(x)", 3.0),
              pow(3.0, 2.0) + 1.0 / 2.8 * 1.9 - log(3.0), EPS);
  EXPECT_NEAR(instance.GetResult("2.8*1.9", -27.5), 5.32, EPS);
  EXPECT_NEAR(instance.GetResult("-(atan(2))", -27.5), -1.1071487178, EPS);
  EXPECT_NEAR(instance.GetResult("cos(x)", -27.5), -0.7148869688, EPS);
  EXPECT_NEAR(instance.GetResult("sin(x)", -27.5), -0.6992400317, EPS);
  EXPECT_NEAR(instance.GetResult("tan(x)", -27.5), 0.9781127118, EPS);
  EXPECT_NEAR(instance.GetResult("asin(x)", -0.8), -0.927295218, EPS);
  EXPECT_NEAR(instance.GetResult("acos(x)", -0.8), 2.4980915448, EPS);
  EXPECT_NEAR(instance.GetResult("2.8*1.9 +-(atan(2)) + -x", -27.5),
              31.712851282, EPS);
  EXPECT_NEAR(instance.GetResult("(((2.8*1.9)^x)) +-cos(sin(0.09012))", 7.2),
              168484.977910919, EPS);
  EXPECT_NEAR(instance.GetResult("2^2x", 7.2), 28.8, EPS);
}