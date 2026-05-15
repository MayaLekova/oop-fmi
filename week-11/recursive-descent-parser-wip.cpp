#include <string>

// Имплементация на рекурсивен парсър (незавършена), като решение
// на задачата за самоподготовка от лекции:
// https://learn.fmi.uni-sofia.bg/course/section.php?id=197188

class Expression {
  public:
    Expression();

    virtual double value() const = 0;
};

class ExprConstant : public Expression {
  public:
    virtual double value() const {
        return number_value;
    }

  private:
    double number_value;
};

class ExprOperator : public Expression {
  public:
    ExprOperator(char operator_);

    virtual double value() const {
        //...
        if (op == '+') {
            return left_operand->value() + right_operand->value();
        } else if (op == '-') {
            return left_operand->value() - right_operand->value();
        } // implement /, *
        return 0;

    }

  private:
    char op;
    Expression *left_operand, *right_operand;
};

bool isNumber(const std::string& str) {
    // връща дали са само цифри
    // TODO: трябва да се разходим по целия низ
    return true;
}

bool isOperator(const std::string& str) {
    if (str.length() > 1) {
        return false;
    }
    char op = str[0];
    return op == '+' || op == '-' || op == '*' || op == '/';
}

// строи дърво на база на израз
Expression* parse(const std::string& sub_expr, Expression* expr_left = nullptr) {
    if (isNumber(sub_expr.c_str())) {
        Expression* result = new ExprConstant(atoi(sub_expr.c_str()));
        return result;
    } else if (isOperator(sub_expr)) {
        Expression* right_operand
             = parse(sub_expr.substr(1, sub_expr.length() - 1), nullptr);
        Expression* result = new ExprOperator(sub_expr[0], expr_left, right_operand);
    }
}
