%token END		//  end-of-file

%token BOOL		//  native type : bool
%token INT32		//  native type : int32
%token STRING		//  native type : string
%token UNIT		//  native type : unit
%token TYPEID		//  user-define type

%token INTLITERAL
%token STRLITERAL

%token AND		//  and operator
%token NOT		//  not oprator

%token CLASS		//  user-define class
%token DO		//
%token ELSE		//
%token EXTENDS		//
%token FALSE		//
%token IF		//
%token IN		//
%token ISNULL		//
%token LET		//
%token NEW		//
%token THEN		//
%token TRUE		//
%token WHILE

%token OBJECTID         //  user-define object

%token LBRACE		//  {
%token RBRACE		//  }
%token LPAR		//  (
%token RPAR		//  )
%token COLON		//  :
%token SEMICOLON	//  ;
%token COMA		//  ,
%token PLUS		//  +
%token MINUS		//  -
%token TIMES		//  *
%token DIV		//  /
%token POW		//  ^
%token DOT		//  .
%token EQUAL		//  =
%token LOWER		//  <
%token LOWEREQUAL	//  <=
%token ASSIGN		//  <-



%%
prog :


%%


