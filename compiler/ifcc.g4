grammar ifcc;

axiom: prog;

prog: topLevelStatement+;

topLevelStatement: functionDeclarationOrDefinition;

// Functions
functionDeclarationOrDefinition:
  returnType=atomicType IDENT '(' functionDeclParamList ')' block
  | returnType=atomicType IDENT '(' functionDeclParamList ')' ';';
functionDeclParamList: (typedParameter (',' typedParameter)*)?;
typedParameter:
  typePrefix=typeSpecifierPrefix IDENT typeSuffix=typeSpecifierSuffix;

callFunctionExpression: IDENT '(' functionCallParamList ')';
functionCallParamList: (expression (',' expression)*)?;

block: '{' statement* '}';

atomicType: 'int' | 'char';

typeSpecifierPrefix: atomicType;
typeSpecifierSuffix: arraySuffix;

arraySuffix: ('[' indices+=CONST? ']')*;

// Nommage des règles pour séparer les traitements et les adapter au cas spécifique.
// Chaque règle nommée présentera son propre visitor dans le code.

statement:
  declareStatement
  | exprStatement
  | blankStatement
  | returnStatement
  | ifStatement
  | whileStatement
  | forStatement
  | block;

declareStatement:
  typePrefix=typeSpecifierPrefix (
    varDeclaration (',' varDeclaration)*
  )? ';';

varDeclaration:
  IDENT typeSuffix=typeSpecifierSuffix ('=' expression)?;

exprStatement: expression ';';

blankStatement: ';';

returnStatement: 'return' expression ';';

blockOrStatement: block | statement;
ifStatement:
  'if' '(' ifCondition=expression ')' // condition (cannot be empty)
  ifBodyThen=blockOrStatement // then (required)
  ('else' ifBodyElse=blockOrStatement)?; // else (optional)

whileStatement:
  'while' '(' whileCondition=expression ')' whileBody=blockOrStatement;

forInit:
  exprStatement
  | blankStatement
  | declareStatement /* not really well supported */;
forStatement:
  'for' '(' forInitialisation=forInit forCondition=expression? ';' forProgress=expression? ')'
    forBody=blockOrStatement;

// Expressions

arrayMemberExpression: IDENT ('[' indices+=expression ']')*;

expression:
  '(' expression ')'                                        # ParentExpr
  | callFunctionExpression                                  # CallExpr
  | arrayMemberExpression                                   # ArrayMemberExpr
  | IDENT                                                   # IdentExpr
  | literal                                                 # LiteralExpr
  | <assoc=right> op=('++' | '--') id=arrayMemberExpression # PreIncrExpr
  | <assoc=right> op=('-' | '!' | '~') expression           # UnaryExpr
  | expression op=('*' | '/' | '%') expression              # MultExpr
  | expression op=('+' | '-') expression                    # AddExpr
  | expression op=('<' | '<=' | '>=' | '>') expression      # OrderCompExpr
  | expression op=('==' | '!=') expression                  # EqualCompExpr
  | expression '&' expression                               # BitwiseAndExpr
  | expression '^' expression                               # BitwiseXorExpr
  | expression '|' expression                               # BitwiseOrExpr
  | expression '&&' expression                              # AndExpr
  | expression '||' expression                              # OrExpr
  | assignExpression                                        # AssignExpr;

assignExpression:
  <assoc=right> arrayMemberExpression '=' expression;

literal: decimalLiteral | hexLiteral | charLiteral;

decimalLiteral: CONST;
hexLiteral: HEX;
charLiteral: CHAR;

CONST: ([0-9])+;
HEX: '0x' HEX_DIGIT+;
IDENT: [a-zA-Z_][a-zA-Z0-9_]*;

CHAR:
  QUOTE ANTISLASH OCT_DIGIT OCT_DIGIT? OCT_DIGIT? QUOTE
  | QUOTE ANTISLASH 'x' HEX_DIGIT HEX_DIGIT? QUOTE
  | QUOTE ANTISLASH ['"?abfnrtv\\] QUOTE
  | QUOTE ~['\\\r\n] QUOTE;
fragment QUOTE: '\'';
fragment ANTISLASH: '\\';
fragment HEX_DIGIT: [a-fA-F0-9];
fragment OCT_DIGIT: [0-7];

COMMENT_MULTI: '/*' .*? '*/' -> skip;
COMMENT_SINGLE: '//' .*? '\n' -> skip;
DIRECTIVE: '#' .*? '\n' -> skip;
WS: [ \t\r\n] -> channel(HIDDEN);
