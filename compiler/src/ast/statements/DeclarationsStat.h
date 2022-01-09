#pragma once

#include <vector>

#include "Declaration.h"
#include "StatNode.h"

class DeclarationsStat : public StatNode {
  public:
    DeclarationsStat(std::vector<Declaration *> *declarations);
    virtual ~DeclarationsStat();

    virtual std::string generateAsm(std::ostream &out) override;
    virtual Node* optimize() override;

  private:
    std::vector<Declaration *> *declarations;
};
