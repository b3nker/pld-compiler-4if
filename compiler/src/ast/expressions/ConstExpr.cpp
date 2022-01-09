#include "ConstExpr.h"

ConstExpr::ConstExpr(intmax_t value) : value(value) {}

std::string ConstExpr::generateAsm(std::ostream &out) {
    return "$" + std::to_string(value);
}

Node *ConstExpr::optimize() {
    return new ConstExpr(value);
}

intmax_t ConstExpr::getValue() {
    return value;
}
