//TODO: assocaition
//TODO: build the ast
//TODO: return the result
%{
  #include "utils/ASTNode.h"
  ASTNode * astResult;

  extern "C" {
    int yyparse();
    int yylex(void);
    void yyerror(char *s){}
    int yywrap(void){return 1;}
};
%}

%code requires { #include "utils/ASTNode.h" }
%locations

%union {
  int intValue;
  std::string * strValue;
  ASTNode * astNode;
}

//Terminal
%token <strValue> ERROR				     	// lexical error token
%token END "end-of-file"
%token BOOL INT32 STRING UNIT
%token <strValue> TYPEID	    		     	//  user-defined type
%token TRUE FALSE
%token <intValue> INTLITERAL
%token <strValue> STRLITERAL
%token AND NOT
%token CLASS DO ELSE EXTENDS IF IN ISNULL LET NEW
%token THEN WHILE
%token <strValue> OBJECTID         	    		//  user-defined object
%token LBRACE RBRACE LPAR RPAR COLON SEMICOLON
%token COMMA PLUS MINUS TIMES DIV POW DOT EQUAL
%token LOWER LOWEREQUAL ASSIGN

//Nonterminal
%type <astNode> type literal program class class_body
%type <astNode> method formals formal block field
%type <astNode> expr args

//Precedence
%right ASSIGN
%left AND
%right NOT
%nonassoc LOWER LOWEREQUAL EQUAL
%left PLUS MINUS
%left TIMES DIV
%left ISNULL
//%precedence NEG
%right POW
%left DOT
%nonassoc LPAR RPAR
%nonassoc LBRACE RBRACE

//start state
%start program


%%
type:
    BOOL      { ASTNode * n = new ASTNode(BOOL, @1.first_line, @1.first_column);
                @$ = @1; $$ = n;
                astResult = n; }
  | INT32     { ASTNode * n = new ASTNode(INT32, @1.first_line, @1.first_column);
                @$ = @1; $$ = n;
                astResult = n; }
  | STRING    { ASTNode * n = new ASTNode(STRING, @1.first_line, @1.first_column);
                @$ = @1; $$ = n;
                astResult = n; }
  | UNIT      { ASTNode * n = new ASTNode(UNIT, @1.first_line, @1.first_column);
                @$ = @1; $$ = n;
                astResult = n; }
  | TYPEID    { ASTNode * n = new ASTNode(TYPEID, $1, @1.first_line, @1.first_column);
                @$ = @1; $$ = n;
                astResult = n; }
  ;

literal:
    TRUE          { ASTNode * n = new ASTNode(TRUE, @1.first_line, @1.first_column);
                    @$ = @1;$$ = n;
                    astResult = n; }
  | FALSE         { ASTNode * n = new ASTNode(FALSE, @1.first_line, @1.first_column);
                    @$ = @1; $$ = n;
                    astResult = n; }
  | INTLITERAL    { ASTNode * n = new ASTNode(TRUE, $1, @1.first_line, @1.first_column);
                    @$ = @1; $$ = n;
                    astResult = n; }
  | STRLITERAL    { ASTNode * n = new ASTNode(TRUE, $1, @1.first_line, @1.first_column);
                    @$ = @1; $$ = n;
                    astResult = n; }
  ;

program:
    class            { ASTNode * n = new ASTNode("program", @1.first_line, @1.first_column);
                       n->addChild($1);
                       @$ = @1; $$ = n;
                       astResult = n; }
  | program class    { ASTNode * n = new ASTNode("program", @1.first_line, @1.first_column);
                       n->addChild($1);
                       @$ = @1; $$ = n;
                       astResult = n; }
  ;

class:
    CLASS TYPEID LBRACE class_body RBRACE                   {  }
  | CLASS TYPEID EXTENDS TYPEID LBRACE class_body RBRACE    {  }
  ;
class_body:
    /*empty*/            {  }
  | class_body method    {  }
  | class_body field     {  }
  ;

method:
    OBJECTID LPAR formals RPAR COLON type LBRACE block RBRACE    {  }
  ;

formals:
    formal                  {  }
  | formals COMMA formal    {  }
  ;

formal:
    OBJECTID COLON type    {  }
  ;

block:		//put the L/RBARCE in the parent rule
    expr                    {  }
  | block SEMICOLON expr    {  }
  ;

field:
    OBJECTID COLON type SEMICOLON                {  }
  | OBJECTID COLON type ASSIGN expr SEMICOLON    {  }
  ;

expr:
    IF expr THEN expr                              {  }
  | IF expr THEN expr ELSE expr                    {  }
  | WHILE expr DO expr                             {  }
  | LET OBJECTID COLON type IN expr                {  }
  | LET OBJECTID COLON type ASSIGN expr IN expr    {  }
  | OBJECTID ASSIGN expr                           {  }
  | NOT expr                                       {  }
  | expr AND expr                                  {  }
  | expr EQUAL expr                                {  }
  | expr LOWER expr                                {  }
  | expr LOWEREQUAL expr                           {  }
  | expr PLUS expr                                 {  }
  | expr MINUS expr                                {  }
  | expr TIMES expr                                {  }
  | expr DIV expr                                  {  }
  | expr POW expr                                  {  }
  | MINUS expr                                     {  }
  | ISNULL expr                                    {  }
  | OBJECTID LPAR RPAR                             {  }
  | OBJECTID LPAR args RPAR                        {  }
  | expr DOT OBJECTID LPAR RPAR                    {  }
  | expr DOT OBJECTID LPAR args RPAR               {  }
  | NEW TYPEID                                     {  }
  | OBJECTID                                       {  }
  | literal                                        {  }
  | LPAR RPAR                                      {  }
  | LPAR expr RPAR                                 {  }
  | LBRACE block RBRACE                            {  }
  ;

args:		// if use duplicate the rule with /*empty*/
    expr               {  }
  | args COMMA expr    {  }
  ;


%%