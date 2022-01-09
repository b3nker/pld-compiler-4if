#pragma once

#include "StatNode.h"
#include <vector>

class BlockStat : public StatNode {
  public:
    BlockStat(std::vector<StatNode *> *statements);
    virtual ~BlockStat();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    std::vector<StatNode *> *block;
};
