
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 258,
     BOOL = 259,
     INT32 = 260,
     STRING = 261,
     UNIT = 262,
     TYPEID = 263,
     INTLITERAL = 264,
     STRLITERAL = 265,
     AND = 266,
     NOT = 267,
     CLASS = 268,
     DO = 269,
     ELSE = 270,
     EXTENDS = 271,
     FALSE = 272,
     IF = 273,
     IN = 274,
     ISNULL = 275,
     LET = 276,
     NEW = 277,
     THEN = 278,
     TRUE = 279,
     WHILE = 280,
     OBJECTID = 281,
     LBRACE = 282,
     RBRACE = 283,
     LPAR = 284,
     RPAR = 285,
     COLON = 286,
     SEMICOLON = 287,
     COMA = 288,
     PLUS = 289,
     MINUS = 290,
     TIMES = 291,
     DIV = 292,
     POW = 293,
     DOT = 294,
     EQUAL = 295,
     LOWER = 296,
     LOWEREQUAL = 297,
     ASSIGN = 298,
     UNKNOWN = 299
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 1 "parser/vsopc.y"

  int intValue;
  std::string *strValue;



/* Line 1676 of yacc.c  */
#line 103 "vsopc.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


