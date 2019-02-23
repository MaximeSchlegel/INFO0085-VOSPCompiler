#include <map>
#include <stack>
#include <utility>
#include <string>
#include <iostream>
#include <math.h>
#include "vsopc.tab.h"

int binToDigit (char c);
int digitToDigit (char c);
int hexToDigit (char c);
int debinify (std::string s);
int dedigify (std::string s);
int dehexify(char msc, char lsc);
int dehexify(std::string s);
void displayToken (int token, int lineno, int charno);