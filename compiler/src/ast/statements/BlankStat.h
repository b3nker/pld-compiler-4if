#pragma once

#include "StatNode.h"

class BlankStat : public StatNode {
  public:
    BlankStat();
    virtual ~BlankStat();

    virtual std::string generateAsm(std::ostream &out) override;
};
