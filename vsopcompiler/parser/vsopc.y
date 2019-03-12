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
%debug

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
    BOOL      { ASTNode * t = new ASTNode(BOOL);
    		t->setPosition(@1.first_line, @1.first_column);
                @$ = @1;
                $$ = t;
                astResult = t; }
  | INT32     { ASTNode * t = new ASTNode(INT32);
    		t->setPosition(@1.first_line, @1.first_column);
                @$ = @1;
                $$ = t;
                astResult = t; }
  | STRING    { ASTNode * t = new ASTNode(STRING);
    		t->setPosition(@1.first_line, @1.first_column);
                @$ = @1;
                $$ = t;
                astResult = t; }
  | UNIT      { ASTNode * t = new ASTNode(UNIT);
    		t->setPosition(@1.first_line, @1.first_column);
                @$ = @1;
                $$ = t;
                astResult = t; }
  | TYPEID    { ASTNode * t = new ASTNode(TYPEID, $1);
    		t->setPosition(@1.first_line, @1.first_column);
                @$ = @1;
                $$ = t;
                astResult = t; }
  ;

literal:
    TRUE          { ASTNode * l = new ASTNode(TRUE);
    		    l->setPosition(@1.first_line, @1.first_column);
                    @$ = @1;
                    $$ = l;
                    astResult = l; }
  | FALSE         { ASTNode * l = new ASTNode(FALSE);
    		    l->setPosition(@1.first_line, @1.first_column);
                    @$ = @1;
                    $$ = l;
                    astResult = l; }
  | INTLITERAL    { ASTNode * l = new ASTNode(INTLITERAL, $1);
    		    l->setPosition(@1.first_line, @1.first_column);
                    @$ = @1;
                    $$ = l;
                    astResult = l; }
  | STRLITERAL    { ASTNode * l = new ASTNode(STRLITERAL, $1);
    		    l->setPosition(@1.first_line, @1.first_column);
                    @$ = @1;
                    $$ = l;
                    astResult = l; }
  ;

program:
    class            { ASTNode * p = new ASTNode("program");
    		       p->setPosition(@1.first_line, @1.first_column);
                       p->addChild($1);
                       @$ = @1;
                       $$ = p;
                       astResult = p; }
  | program class    { $1->addChild($2);
                       @$ = @1; @$.last_line = @2.last_line; @$.last_column = @2.last_column;
                       $$ = $1;
                       astResult = $1; }
  ;

class:
    CLASS TYPEID LBRACE class_body RBRACE                   { ASTNode * p = new ASTNode("object");
                                                              ASTNode * t = new ASTNode(TYPEID, $2);
    		                                              p->setPosition(@1.first_line, @1.first_column);
    		                                              t->setPosition(@2.first_line, @2.first_column);
                                                              $4->addChild(p); $4->addChild(t);
                                                              @$ = @1; @$.last_line = @5.last_line; @$.last_column = @5.last_column;
                                                              $$ = $4;
                                                              astResult = $4; }
  | CLASS TYPEID EXTENDS TYPEID LBRACE class_body RBRACE    { ASTNode * p = new ASTNode(TYPEID, $4);
                                                              ASTNode * t = new ASTNode(TYPEID, $2);
    		                                              p->setPosition(@1.first_line, @1.first_column);
    		                                              t->setPosition(@2.first_line, @2.first_column);
                                                              $6->addChild(p); $6->addChild(t);
                                                              @$ = @1; @$.last_line = @7.last_line; @$.last_column = @7.last_column;
                                                              $$ = $6;
                                                              astResult = $6; }
  ;
class_body:
    /*empty*/            { ASTNode * c = new ASTNode("class");
                           c->setPosition(yylloc.first_line, yylloc.first_column);
                           $$ = c;
                           astResult = c; }
  | class_body method    { $1->addChild($2);
                           @$ = @1; @$.last_line = @2.last_line; @$.last_column = @2.last_column;
                           astResult = $1; }
  | class_body field     { $1->addChild($2);
                           @$ = @1; @$.last_line = @2.last_line; @$.last_column = @2.last_column;
                           astResult = $1; }
  ;

field:
    OBJECTID COLON type SEMICOLON                { ASTNode * f = new ASTNode("field");
                                                   ASTNode * o = new ASTNode(OBJECTID, $1);

                                                    }
  | OBJECTID COLON type ASSIGN expr SEMICOLON    {  }
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