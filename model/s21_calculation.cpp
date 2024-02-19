#include "s21_calculation.h"

namespace s21 {

void Calculation::SetX(double x) noexcept { x_ = x; }
void Calculation::SetX(const std::string& x_str) noexcept {
  double x = NAN;
  int shift = 0;
  std::string str = x_str;
  TrimSpaces(str);
  CommaToDot(str);
  sscanf(str.c_str(), "%lf%n", &x, &shift);
  if (shift == 0 || shift != static_cast<int>(str.size()))
    SetX(NAN);
  else
    SetX(x);
}
void Calculation::SetExpression(const std::string& input) noexcept {
  expr_ = input;
  status_ = NEW_EXPRESSION;
}
void Calculation::SetRadian() noexcept { trig_value_ = RAD; }
void Calculation::SetDegree() noexcept { trig_value_ = DEG; }

double Calculation::GetX() const noexcept { return x_; }
const std::string Calculation::GetExpression() const noexcept { return expr_; }
Calculation::TrigType Calculation::GetTrigValue() const noexcept {
  return trig_value_;
}
Calculation::Status Calculation::GetStatus() const noexcept { return status_; }
double Calculation::GetResult() {
  if (status_ == NEW_EXPRESSION) Reset();
  if (status_ == READY) Parse();
  if (status_ == PARSED || status_ == COMPLETED) Calculate();
  return result_;
}
double Calculation::GetResult(double x) {
  SetX(x);
  return GetResult();
}
double Calculation::GetResult(const std::string& input) {
  SetExpression(input);
  return GetResult();
}
double Calculation::GetResult(const std::string& input, double x) {
  SetX(x);
  return GetResult(input);
}
double Calculation::GetResult(const std::string& input, const std::string& x) {
  SetX(x);
  return GetResult(input);
}

void Calculation::Reset() {
  result_ = NAN;
  status_ = READY;
  prev_ = UNDEF;
  while (!stack_.empty()) stack_.pop();
  while (!calc_stack_.empty()) calc_stack_.pop();
  output_queue_.clear();
}

void Calculation::Parse() {
  TrimSpaces(expr_);
  if (expr_.empty()) {
    status_ = EMPTY;
    return;
  }
  CommaToDot(expr_);
  iter_ = expr_.begin();
  while (iter_ != expr_.end()) {
    if (*iter_ == ' ') {
      iter_++;
    } else {
      CheckHiddenMultiplication();
    }
    ParseToken();
    if (status_ == PARSE_ERROR) return;
  }
  while (!stack_.empty()) {
    if (stack_.top() == RIGHT_PAR || stack_.top() == LEFT_PAR) {
      status_ = PARSE_ERROR;
      return;
    }
    if (IsBinaryOperator(stack_.top()) || IsUnaryOperator(stack_.top())) {
      output_queue_.push_back(Token{stack_.top(), NAN});
      stack_.pop();
    }
  }
  status_ = PARSED;
}

void Calculation::Calculate() {
  double x1 = NAN, x2 = NAN;
  for (Token token : output_queue_) {
    if (token.type == NUM) {
      calc_stack_.push(token.number);
    } else if (token.type == X) {
      calc_stack_.push(x_);
    } else if (IsFunction(token.type) || IsUnaryOperator(token.type)) {
      if (calc_stack_.empty()) {
        status_ = CALCULATE_ERROR;
        return;
      }
      CalculateUnaryOrFunction(token.type);
    } else if (IsBinaryOperator(token.type)) {
      if (calc_stack_.empty()) {
        status_ = CALCULATE_ERROR;
        return;
      }
      x2 = calc_stack_.top();
      calc_stack_.pop();
      if (calc_stack_.empty()) {
        status_ = CALCULATE_ERROR;
        return;
      }
      x1 = calc_stack_.top();
      calc_stack_.pop();
      calc_stack_.push(GetBinaryCallback(token.type)(x1, x2));
    }
  }
  result_ = calc_stack_.top();
  calc_stack_.pop();
  status_ = COMPLETED;
}

/* Misc */

void Calculation::CalculateUnaryOrFunction(TokenType token_type) {
  double x1 = calc_stack_.top();
  calc_stack_.pop();
  if (trig_value_ == DEG) {
    if (IsFunction(token_type) &&
        GetFunctionType(token_type) == TRIGONOMICAL_STRAIGHT)
      calc_stack_.push(GetUnaryCallback(token_type)(x1 * M_PI / 180.0));
    else if (IsFunction(token_type) &&
             GetFunctionType(token_type) == TRIGONOMICAL_ARC)
      calc_stack_.push(GetUnaryCallback(token_type)(x1) * 180.0 / M_PI);
    else
      calc_stack_.push(GetUnaryCallback(token_type)(x1));
  } else
    calc_stack_.push(GetUnaryCallback(token_type)(x1));
}

void Calculation::ParseToken() {
  bool parsed = false;
  std::string::const_iterator init = iter_;
  parsed = CheckNumber(init);
  if (!parsed) parsed = CheckX(init);
  if (!parsed) parsed = CheckFunction(init);
  if (!parsed) parsed = CheckLeftParenthesis(init);
  if (!parsed) parsed = CheckUnarOperator(init);
  if (!parsed) parsed = CheckOperator(init);
  if (!parsed) parsed = CheckRightParenthesis(init);
  if (!parsed) status_ = PARSE_ERROR;
}

void Calculation::CheckHiddenMultiplication() {
  if ((prev_ == NUM &&
       (ParseFunction(iter_) != UNDEF || *iter_ == '(' || *iter_ == 'x')) ||
      (prev_ == X && *iter_ == '(') || (prev_ == RIGHT_PAR && *iter_ == '(') ||
      (prev_ == RIGHT_PAR && IsFunction(ParseFunction(iter_)))) {
    while (!stack_.empty() &&
           ((IsBinaryOperator(stack_.top()) &&
             GetPriority(stack_.top()) >= GetPriority(MULT)) ||
            IsUnaryOperator(stack_.top()))) {
      output_queue_.push_back(Token{stack_.top(), NAN});
      stack_.pop();
    }
    stack_.push(MULT);
    prev_ = MULT;
  }
}

bool Calculation::CheckNumber(std::string::const_iterator input) {
  if (isdigit(*input) || *input == '.') {
    int shift = 0;
    double number = 0.0;
    sscanf(&input[0], "%lf%n", &number, &shift);
    if (shift > 0 && prev_ != X && prev_ != NUM && prev_ != RIGHT_PAR) {
      output_queue_.push_back(Token{NUM, number});
      prev_ = NUM;
      iter_ += shift;
      return true;
    }
  }
  return false;
}

bool Calculation::CheckX(std::string::const_iterator input) {
  if (*input == 'x' && prev_ != X && prev_ != RIGHT_PAR) {
    output_queue_.push_back(Token{X, NAN});
    prev_ = X;
    iter_++;
    return true;
  }
  return false;
}

bool Calculation::CheckFunction(std::string::const_iterator input) {
  TokenType value = UNDEF;
  value = ParseFunction(input);
  if (value != UNDEF && prev_ != RIGHT_PAR && prev_ != X && prev_ != NUM) {
    stack_.push(value);
    prev_ = value;
    iter_ += GetString(value).size();
    return true;
  }
  return false;
}

Calculation::TokenType Calculation::ParseFunction(
    std::string::const_iterator input) {
  for (const auto& [key, value] : functions) {
    size_t shift = GetString(key).size();
    if (!strncmp(&input[0], GetString(key).data(), shift) &&
        *(input + shift) == '(') {
      return key;
    }
  }
  return UNDEF;
}

bool Calculation::CheckLeftParenthesis(std::string::const_iterator input) {
  if (*input == '(') {
    stack_.push(LEFT_PAR);
    prev_ = LEFT_PAR;
    iter_++;
    return true;
  }
  return false;
}

bool Calculation::CheckUnarOperator(std::string::const_iterator input) {
  if (prev_ != NUM && prev_ != X && prev_ != RIGHT_PAR) {
    for (const auto& [key, value] : unary_operators) {
      if (!strncmp(&input[0], GetString(key).data(), GetString(key).size()) &&
          *(input + GetString(key).size()) != ' ') {
        stack_.push(key);
        prev_ = key;
        iter_ += GetString(key).size();
        return true;
      }
    }
  }
  return false;
}

bool Calculation::CheckOperator(std::string::const_iterator input) {
  for (const auto& [key, value] : operators) {
    if (!strncmp(&input[0], GetString(key).data(), GetString(key).size())) {
      while (
          !stack_.empty() &&
          ((IsBinaryOperator(stack_.top()) &&
            (GetPriority(stack_.top()) > GetPriority(key) ||
             (GetPriority(stack_.top()) == GetPriority(key) && key != POW))) ||
           IsUnaryOperator(stack_.top()))) {
        output_queue_.push_back(Token{stack_.top(), NAN});
        stack_.pop();
      }
      stack_.push(key);
      prev_ = key;
      iter_ += GetString(key).size();
      return true;
    }
  }
  return false;
}

bool Calculation::CheckRightParenthesis(std::string::const_iterator input) {
  if (*input == ')') {
    while (!stack_.empty() && stack_.top() != LEFT_PAR &&
           (IsBinaryOperator(stack_.top()) || IsUnaryOperator(stack_.top()))) {
      output_queue_.push_back(Token{stack_.top(), NAN});
      stack_.pop();
    }
    if (stack_.empty())
      status_ = PARSE_ERROR;
    else if (stack_.top() == LEFT_PAR) {
      stack_.pop();
      if (!stack_.empty() && IsFunction(stack_.top())) {
        output_queue_.push_back(Token{stack_.top(), NAN});
        stack_.pop();
      }
      while (!stack_.empty() && IsUnaryOperator(stack_.top())) {
        output_queue_.push_back(Token{stack_.top(), NAN});
        stack_.pop();
      }
    }
    prev_ = RIGHT_PAR;
    iter_++;
    return true;
  }
  return false;
}

int Calculation::GetPriority(TokenType value) {
  return std::get<2>(operators.at(value));
}

const std::string Calculation::GetString(TokenType value) {
  if (IsBinaryOperator(value)) return std::get<0>(operators.at(value));
  if (IsUnaryOperator(value)) return std::get<0>(unary_operators.at(value));
  return std::get<0>(functions.at(value));
}

Calculation::UnaryCallback Calculation::GetUnaryCallback(TokenType value) {
  if (IsUnaryOperator(value)) return std::get<1>(unary_operators.at(value));
  return std::get<1>(functions.at(value));
}

Calculation::BinaryCallback Calculation::GetBinaryCallback(TokenType value) {
  return std::get<1>(operators.at(value));
}

Calculation::FuncType Calculation::GetFunctionType(TokenType value) {
  return std::get<2>(functions.at(value));
}

bool Calculation::IsBinaryOperator(TokenType value) noexcept {
  return value >= SUM && value <= MOD;
}

bool Calculation::IsUnaryOperator(TokenType value) noexcept {
  return value >= PLUS && value <= MINUS_ALT;
}

bool Calculation::IsFunction(TokenType value) noexcept {
  return value >= SQRT && value <= ATAN;
}

/* Remove all whitespaces at start and end of the string.
Shrink whitespace sequences to single symbols. */
void Calculation::TrimSpaces(std::string& str) {
  std::string::iterator iter = str.begin();
  if (!str.empty()) {
    while (*iter == ' ') iter++;
    str.erase(str.begin(), iter);
  }

  iter = str.end();
  if (!str.empty()) {
    while (*(iter - 1) == ' ') iter--;
    str.erase(iter, str.end());
  }

  for (iter = str.begin(); iter != str.end(); iter++) {
    if (*iter == ' ' && *(iter + 1) == ' ') iter = str.erase(iter) - 1;
  }
}

/* Replace all commas in the string with dots. */
void Calculation::CommaToDot(std::string& str) {
  std::string::iterator iter = str.begin();
  for (iter = str.begin(); iter != str.end(); iter++) {
    if (*iter == ',') *iter = '.';
  }
}

}  // namespace s21
