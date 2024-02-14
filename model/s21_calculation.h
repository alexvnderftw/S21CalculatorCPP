#ifndef S21_CALCULATION_H
#define S21_CALCULATION_H

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#define _USE_MATH_DEFINES

namespace s21 {
class Calculation {
 public:
  Calculation() = default;
  ~Calculation() = default;

  enum Status {
    READY,
    NEW_EXPRESSION,
    PARSED,
    COMPLETED,
    PARSE_ERROR,
    CALCULATE_ERROR,
    EMPTY
  };

  enum TrigType { RAD, DEG };

  /* Set methods */
  void SetX(double x) noexcept;
  void SetX(const std::string x_str) noexcept;
  void SetExpression(const std::string input) noexcept;
  void SetRadian() noexcept;
  void SetDegree() noexcept;

  /* Get methods */
  TrigType GetTrigValue() const noexcept;
  Status GetStatus() const noexcept;
  const std::string GetExpression() const noexcept;
  double GetX() const noexcept;
  double GetResult();
  double GetResult(double x);
  double GetResult(const std::string input);
  double GetResult(const std::string input, double x);
  double GetResult(const std::string input, const std::string x);

 private:
  enum TokenType {
    /* Unary functions */
    SQRT,
    LN,
    LOG,
    SIN,
    COS,
    TAN,
    ASIN,
    ACOS,
    ATAN,
    /* Unary operators */
    PLUS,
    MINUS,
    MINUS_ALT,
    /* Binary operators */
    SUM,
    SUB,
    MULT,
    DIV,
    POW,
    MOD,
    /* Other */
    UNDEF,
    NUM,
    LEFT_PAR,
    RIGHT_PAR,
    X
  };

  enum FuncType { REGULAR, TRIGONOMICAL_ARC, TRIGONOMICAL_STRAIGHT };
  typedef double (*UnaryCallback)(double);
  typedef double (*BinaryCallback)(double, double);

  struct Token {
    TokenType type = UNDEF;
    double number = NAN;
  };

  /* Main variables */
  Status status_ = READY;
  std::string expr_{};
  double x_ = NAN;
  double result_ = NAN;
  TrigType trig_value_ = RAD;

  /* Parsing variables */
  std::string::iterator iter_;
  std::stack<TokenType> stack_{};
  TokenType prev_ = UNDEF;

  /* Calculation variables */
  std::vector<Token> output_queue_{};
  std::stack<double> calc_stack_{};

  void Reset();
  void Parse();
  void Calculate();

  /* Misc */
  void TrimSpaces(std::string& str);
  void CommaToDot(std::string& str);

  void CalculateUnaryOrFunction(TokenType token);

  void ParseToken();
  void CheckHiddenMultiplication();
  bool CheckNumber(std::string::iterator input);
  bool CheckX(std::string::iterator input);
  bool CheckFunction(std::string::iterator input);
  TokenType ParseFunction(std::string::iterator input);
  bool CheckLeftParenthesis(std::string::iterator input);
  bool CheckRightParenthesis(std::string::iterator input);
  bool CheckUnarOperator(std::string::iterator input);
  bool CheckOperator(std::string::iterator input);

  int GetPriority(TokenType value);
  const std::string GetString(TokenType value);
  UnaryCallback GetUnaryCallback(TokenType value);
  BinaryCallback GetBinaryCallback(TokenType value);
  FuncType GetFunctionType(TokenType value);

  static bool IsFunction(TokenType value) noexcept;
  static bool IsBinaryOperator(TokenType value) noexcept;
  static bool IsUnaryOperator(TokenType value) noexcept;

  /* Custom math functions */
  static double sum(double a, double b) noexcept { return a + b; };
  static double sub(double a, double b) noexcept { return a - b; };
  static double mult(double a, double b) noexcept { return a * b; };
  static double fdiv(double a, double b) noexcept { return a / b; };
  static double plus(double a) noexcept { return +a; };
  static double minus(double a) noexcept { return -a; };

  /* Key - {Parse pattern, Callback function, Function type} */
  const std::map<TokenType, std::tuple<std::string, UnaryCallback, FuncType>>
      functions = {{SQRT, {"sqrt", std::sqrt, REGULAR}},
                   {LN, {"ln", std::log, REGULAR}},
                   {LOG, {"log", std::log10, REGULAR}},
                   {SIN, {"sin", std::sin, TRIGONOMICAL_STRAIGHT}},
                   {COS, {"cos", std::cos, TRIGONOMICAL_STRAIGHT}},
                   {TAN, {"tan", std::tan, TRIGONOMICAL_STRAIGHT}},
                   {ASIN, {"asin", std::asin, TRIGONOMICAL_ARC}},
                   {ACOS, {"acos", std::acos, TRIGONOMICAL_ARC}},
                   {ATAN, {"atan", std::atan, TRIGONOMICAL_ARC}}};

  /* Key - {Parse pattern, Callback function} */
  const std::map<TokenType, std::pair<std::string, UnaryCallback>>
      unary_operators = {{PLUS, {"+", plus}},
                         {MINUS, {"-", minus}},
                         {MINUS_ALT, {"~", minus}}};

  /* Key - {Parse pattern, Callback function, Priority} */
  const std::map<TokenType, std::tuple<std::string, BinaryCallback, int>>
      operators = {{SUM, {"+", sum, 1}},      {SUB, {"-", sub, 1}},
                   {MULT, {"*", mult, 2}},    {DIV, {"/", fdiv, 2}},
                   {POW, {"^", std::pow, 3}}, {MOD, {"mod", std::fmod, 2}}};
};
}  // namespace s21

#endif  // S21_CALCULATION_H
