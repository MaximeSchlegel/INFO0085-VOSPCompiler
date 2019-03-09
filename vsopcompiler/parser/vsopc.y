%{
    #include "utils/parser_header.h"
%}
%language "c++"
%defines

%token <Symbol> ERROR				// lexical error token

%token <Symbol> END "end-of-file"		//

%token <Symbol> BOOL INT32 STRING UNIT		//
%token <Symbol>  TYPEID				//  user-defined type

%token <Symbol> TRUE FALSE 			//
%token <Symbol> INTLITERAL STRLITERAL		//

%token <Symbol> AND NOT				//

%token <Symbol> CLASS DO ELSE EXTENDS IF IN	//
%token <Symbol> ISNULL LET NEW THEN WHILE	//
%token <Symbol> OBJECTID      			//  user-defined object

%token <Symbol> LBRACE RBRACE LPAR RPAR		//  { } ( )
%token <Symbol> COLON SEMICOLON COMMA		//  : ; ,
%token <Symbol> PLUS MINUS TIMES DIV POW DOT	//  + - * / ^ .
%token <Symbol> EQUAL LOWER LOWEREQUAL		//  = < <=
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