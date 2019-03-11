%union {
  int intValue;
  std::string *strValue;
}

%token ERROR                    // lexical error token

%token END "end-of-file"	    //

%token BOOL			            //
%token INT32			        //
%token STRING			        //
%token UNIT			            //
%token <strValue> TYPEID	    //  user-defined type

%token TRUE			            //
%token FALSE			        //
%token <intValue> INTLITERAL	//
%token <strValue> STRLITERAL	//

%token AND			            //
%token NOT			            //

%token CLASS			        //
%token DO			            //
%token ELSE			            //
%token EXTENDS			        //
%token IF			            //
%token IN			            //
%token ISNULL			        //
%token LET			            //
%token NEW			            //
%token THEN			            //
%token WHILE			        //
%token OBJECTID         	    //  user-defined object

%token LBRACE			        //  {
%token RBRACE			        //  }
%token LPAR			            //  (
%token RPAR			            //  )
%token COLON			        //  :
%token SEMICOLON		        //  ;
%token COMMA			        //  ,
%token PLUS			            //  +
%token MINUS			        //  -
%token TIMES			        //  *
%token DIV			            //  /
%token POW			            //  ^
%token DOT			            //  .
%token EQUAL			        //  =
%token LOWER			        //  <
%token LOWEREQUAL		        //  <=
%token ASSIGN			        //  <-

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