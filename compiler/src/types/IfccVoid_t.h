#pragma once

#include "IfccType.h"

class IfccVoid_t : public IfccType {
  public:
    std::string getTypeName() const { return "void"; };
    size_t getSize() const { return 0; };

    IfccVoid_t(){};
    ~IfccVoid_t(){};

    IfccVoid_t *clone() const { return new IfccVoid_t(*this); }
};
