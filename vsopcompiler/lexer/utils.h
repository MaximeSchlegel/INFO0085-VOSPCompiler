#include <map>
#include <vector>
#include <stack>
#include <utility>
#include <string>
#include <iostream>
#include <math.h>
#include "../vsopc.tab.h"

extern int yylineno, yyleng, yycharno, prevTokenLength;
extern YYSTYPE yylval;

void yycharnoRefresh();
void yycharnoNewline();
int binToDigit (char c);
int digitToDigit (char c);
int hexToDigit (char c);
int debinify (std::string s);
int dedigify (std::string s);
int dehexify(char msc, char lsc);
int dehexify(std::string s);
int returnAndDisplayToken (int token, int lineno=yylineno, int charno=yycharno, bool display=true);
int returnAndDisplayError (int error, int lineno, int charno, bool display=true);
void displayTokens(std::vector<std::pair<int, std::pair<int, int>>> v);