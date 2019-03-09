%{
    #include "../utils/parser_header.h"
%}
%language "c++"
%defines

%token <Symbol> ERROR				// lexical error token

%token <Symbol> END "end-of-file"		//

%token <Symbol> BOOL				//
%token <Symbol> INT32				//
%token <Symbol> STRING				//
%token <Symbol> UNIT				//
%token <Symbol>  TYPEID		//  user-defined type

%token <Symbol> TRUE				//
%token <Symbol> FALSE				//
%token <Symbol> INTLITERAL		//
%token <Symbol> STRLITERAL		//

%token <Symbol> AND				//
%token <Symbol> NOT				//

%token <Symbol> CLASS				//
%token <Symbol> DO				//
%token <Symbol> ELSE				//
%token <Symbol> EXTENDS				//
%token <Symbol> IF				//
%token <Symbol> IN				//
%token <Symbol> ISNULL				//
%token <Symbol> LET				//
%token <Symbol> NEW				//
%token <Symbol> THEN				//
%token <Symbol> WHILE				//
%token <Symbol> OBJECTID      			//  user-defined object

%token <Symbol> LBRACE				//  {
%token <Symbol> RBRACE				//  }
%token <Symbol> LPAR				//  (
%token <Symbol> RPAR				//  )
%token <Symbol> COLON				//  :
%token <Symbol> SEMICOLON			//  ;
%token <Symbol> COMMA				//  ,
%token <Symbol> PLUS				//  +
%token <Symbol> MINUS				//  -
%token <Symbol> TIMES				//  *
%token <Symbol> DIV				//  /
%token <Symbol> POW				//  ^
%token <Symbol> DOT				//  .
%token <Symbol> EQUAL				//  =
%token <Symbol> LOWER				//  <
%token <Symbol> LOWEREQUAL			//  <=
%token <Symbol> ASSIGN				//  <-

%start program


%%
type:
    BOOL
  | INT32
  | STRING
  | UNIT
  | TYPEID

literal:
    TRUE
  | FALSE
  | INTLITERAL
  | STRLITERAL

program:
    class
  | program class

class:
    CLASS TYPEID LBRACE class_body RBRACE
  | CLASS TYPEID EXTENDS TYPEID LBRACE class_body RBRACE

class_body:
    /*empty*/
  | class_body method
  | class_body field
  ;

method:
    OBJECTID LPAR formals RPAR COLON type LBRACE block RBRACE

formals:
    formal
  | formals COMMA formal

formal:
    OBJECTID COLON type

block:		//put the L/RBARCE in the parent rule
    expr
  | block SEMICOLON expr

field:
    OBJECTID COLON type SEMICOLON
  | OBJECTID COLON type ASSIGN expr SEMICOLON

expr:
    IF expr THEN expr
  | IF expr THEN expr ELSE expr
  | WHILE expr DO expr
  | LET OBJECTID COLON type IN expr
  | LET OBJECTID COLON type ASSIGN expr IN expr
  | OBJECTID ASSIGN expr
  | NOT expr
  | expr AND expr
  | expr EQUAL expr
  | expr LOWER expr
  | expr LOWEREQUAL expr
  | expr PLUS expr
  | expr MINUS expr
  | expr TIMES expr
  | expr DIV expr
  | expr POW expr
  | MINUS expr
  | ISNULL expr
  | OBJECTID LPAR RPAR
  | OBJECTID LPAR args RPAR
  | expr DOT OBJECTID LPAR RPAR
  | expr DOT OBJECTID LPAR args RPAR
  | NEW TYPEID
  | OBJECTID
  | literal
  | LPAR RPAR
  | LPAR expr RPAR
  | LBRACE block RBRACE

args:		// if use duplicate the rule with /*empty*/
    expr
  | args COMMA expr



%%