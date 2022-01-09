#include "Node.h"
#include "../../CompilerState.h"

Node::Node() {}

Node::~Node() {}

Node* Node::optimize() {
    return nullptr;
}

void Node::error(std::string errorMsg) {
    CompilerState::Get().globalNbErrors += 1;
    std::cerr << "[ERROR] " << errorMsg << std::endl;
    exit(1);
}

void Node::warning(std::string warningMsg) {
    CompilerState::Get().globalNbWarnings += 1;
    std::cerr << "[WARNING] " << warningMsg << std::endl;
}
