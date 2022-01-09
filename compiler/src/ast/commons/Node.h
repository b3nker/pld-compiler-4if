#pragma once

#include <iostream>
#include <string>

class Node {
  public:
    Node();
    virtual ~Node();

    virtual std::string generateAsm(std::ostream &out) = 0;
    virtual Node* optimize();
    static void error(std::string errorMsg);
    static void warning(std::string errorMsg);
  private:
};
