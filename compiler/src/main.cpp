#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "CompilerState.h"
#include "antlr4-generated/ifccBaseVisitor.h"
#include "antlr4-generated/ifccLexer.h"
#include "antlr4-generated/ifccParser.h"
#include "antlr4-runtime.h"
#include "visitorAST.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv) {
    stringstream in;

    if (argn == 2) {
        ifstream lecture(argv[1]);
        in << lecture.rdbuf();
    }
    ANTLRInputStream input(in.str());
    ifccLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    ifccParser parser(&tokens);
    tree::ParseTree *tree = parser.axiom();

    VisitorAST visitorAST;
    visitorAST.visit(tree);

    const size_t nSyntaxErrors = parser.getNumberOfSyntaxErrors();
    if (nSyntaxErrors > 0) {
        std::cerr << std::endl;
        std::cerr << "err: " << nSyntaxErrors
                  << " syntax error(s) while parsing." << std::endl;
        return 1;
    }

    CompilerState::Get().ast->optimize();
    CompilerState::Get().ast->generateAsm(std::cout);

    CompilerState::Get().summary();
    CompilerState::Get().functions->checkUnusedFunctions();

    return 0;
}
