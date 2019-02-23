%{
    #include <map>
    #include <stack>
    #include <utility>
    #include <string>
    #include <iostream>
    #include <math.h>
    #include "vsopctest.tab.h"
    #define YY_DECL extern "C" int yylex()

    int binToDigit (char c) {
      if ('0' <= c && c <= '1') { return c - '0'; }
      return -1;
    }

    int digitToDigit (char c) {
      if ('0' <= c && c <= '9') { return c - '0'; }
      return -1;
    }

    int hexToDigit (char c) {
      if ('0' <= c && c <= '9') { return c - '0'; }
      if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
      if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
      return -1;
    }

    int debinify (std::string s) {
      int res = 0;
      for (int i=0; i < s.size(); i++) {
        int d = binToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 2, (double) s.size() - 1 - i);
      }
      return res;
    }

    int dedigify (std::string s) {
      int res = 0;
      for (int i=0; i < s.size(); i++) {
        int d = digitToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 10,(double) s.size() - 1 - i);
        }
      return res;
    }

    int dehexify(char msc, char lsc) {
      int msi = hexToDigit(msc), lsi = hexToDigit(lsc);
      if (msi == -1 or lsi == -1) { return -1; }
      return hexToDigit(msc) * 16 + hexToDigit(lsc);
    }

    int dehexify(std::string s) {
      int res = 0;
      for (int i=0; i < s.size(); i++) {
        int d = hexToDigit(s[i]);
        if (d == -1) { return -1; }
        res += d * pow((double) 16, (double) s.size() - 1 - i);
      }
      return res;
    }

    std::map<std::string, enum yytokentype> reservedId;
    int currentCharPos = 1;
    std::string strBuffer;
    std::pair<int,int> strStartPos;
    std::stack<std::pair<int, int>> openedCommentsPos;
%}

%option noyywrap
%option yylineno

%x COMMENT
%x STR

space           " "
tab             \x09
lf              \x0A
ff              \x0B
cr              \x0C
whitespace      {space}|{tab}
ignored-char    {whitespace}|{ff}|{cr}

bin-digit          0|1
digit              {bin-digit}|[2-9]
hex-digit          {digit}|[a-fA-F]
bin-literal        0b{bin-digit}+
digit-literal      {digit}+
hex-literal        0x{hex-digit}+
integer-literal    {digit-literal}|{hex-literal}|{bin-literal}

lowercase-letter    [a-z]
uppercase-letter    [A-Z]
letter              {lowercase-letter}|{uppercase-letter}

typeId      {uppercase-letter}({letter}|{digit}|_)*
objectId    {lowercase-letter}({letter}|{digit}|_)*


%%
"//".*\n                  { currentCharPos = 1; }
<INITIAL,COMMENT>"(*"     { openedCommentsPos.emplace(yylineno, currentCharPos);
                            std::cout << "Ship Comment " << currentCharPos << std::endl;
                            currentCharPos += yyleng;
                            BEGIN(COMMENT);
                          }
<COMMENT>"*)"             { currentCharPos += yyleng;
                            openedCommentsPos.pop();
                            if (openedCommentsPos.empty()) { BEGIN(INITIAL); }
                          }
<COMMENT>([^*(\n])+       { currentCharPos += yyleng; }
<COMMENT>\n               { currentCharPos = 1; }
<COMMENT><<EOF>>          { std::cout << "Lexical Error : end of file in comment" << std::endl;
                            return -1;
                          }

\x22                           { strStartPos = std::pair<int , int>(currentCharPos, yylineno);
                                 strBuffer = yytext;
                                 currentCharPos += yyleng;
                                 BEGIN(STR);
                               }
<STR>\x22                      { strBuffer += yytext;
                                 std::cout << "String Literral : " << strBuffer << std::endl;
                                 currentCharPos += yyleng;
                                 BEGIN(INITIAL);
                               }
<STR>\x5C\x0A([ '\x09'])*      { currentCharPos = yyleng - 2; }
<STR>\x5Cb                     { strBuffer += "\\x08";
                                 currentCharPos += yyleng;
                               }
<STR>\x5Ct                     { strBuffer += "\\x09";
                                 currentCharPos += yyleng;
                               }
<STR>\x5Cn                     { strBuffer += "\\x0A";
                                 currentCharPos += yyleng;
                               }
<STR>\x5Cr                     { strBuffer += "\\x0D";
                                 currentCharPos += yyleng;
                               }
<STR>\x5C\x22                  { strBuffer += 34;
                                 currentCharPos += yyleng;
                               }
<STR>\x5C\x5C                  { strBuffer += 92;
                                 currentCharPos += yyleng;
                               }
<STR>\x5Cx({hex-digit}{2})     { std::string s = yytext; s = s.erase(0,2);
                                 int intLit = dehexify(s);
                                 if (intLit == -1) {
                                   std::cout << "Lexical Error : invalid hexadecimal number" << std::endl;
                                   return -1;
                                 }
                                 strBuffer += intLit;
                                 currentCharPos += yyleng;
                               }
<STR>\x5C                      { std::cout << "Lexical Error : invalid escape sequence" << std::endl;
                                 return -1;
                               }
<STR>\x00                      { std::cout << "Lexical Error : null character in string" << std::endl;
                                 return -1;
                               }
<STR>\x0A                      { std::cout << "Lexical Error : feed line in string" << std::endl;
                                 return -1;
                               }
<STR><<EOF>>                   { std::cout << "Lexical Error : end of file in string" << std::endl;
                                 return -1;
                               }
<STR>.                         { strBuffer += yytext;
                                 currentCharPos += yyleng;
                               }

{bin-literal}                             { std::string s = yytext; s = s.erase(0,2);
                                            int intLit = debinify(s);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid binary number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                            currentCharPos += yyleng;
                                          }
{digit-literal}                           { int intLit = dedigify(yytext);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                            currentCharPos += yyleng;
                                          }
{hex-literal}                             { std::string s = yytext; s = s.erase(0,2);
                                            int intLit = dehexify(s);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid hexadecimal number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                            currentCharPos += yyleng;
                                          }
{integer-literal}({objectId}|{typeId})    { std::cout << "Lexical Error : missing whitespace between integer and identifier" << std::endl;
                                            return -1;
                                          }

{typeId}    { std::cout << "Type-Identifier : " << yytext << std::endl;
              currentCharPos += yyleng;
            }

{objectId}    { std::map<std::string, enum yytokentype>::iterator isReserved;
                isReserved = reservedId.find(yytext);
                if (isReserved != reservedId.end()) {
                  std::cout << "Reversed Identifier : " << yytext << std::endl;
                } else {
                  std::cout << "Object-Identifier : " << yytext << std::endl;
                }
                currentCharPos += yyleng;
              }

"{"    { std::cout << "L Brace" << std::endl;
         currentCharPos += yyleng;
       }
"}"    { std::cout << "R Brace" << std::endl;
         currentCharPos += yyleng;
       }
"("    { std::cout << "L Par" << std::endl;
         currentCharPos += yyleng;
       }
")"    { std::cout << "R Par" << std::endl;
         currentCharPos += yyleng;
       }
":"    { std::cout << "Colon" << std::endl;
         currentCharPos += yyleng;
       }
";"    { std::cout << "Semi-Colon" << std::endl;
         currentCharPos += yyleng;
       }
","    { std::cout << "Coma" << std::endl;
         currentCharPos += yyleng;
       }
"+"    { std::cout << "Plus" << std::endl;
         currentCharPos += yyleng;
       }
"-"    { std::cout << "Minus" << std::endl;
         currentCharPos += yyleng;
       }
"*"    { std::cout << "Times" << std::endl;
         currentCharPos += yyleng;
       }
"/"    { std::cout << "Div" << std::endl;
         currentCharPos += yyleng
       }
"^"    { std::cout << "Pow" << std::endl;
         currentCharPos += yyleng;
       }
"."    { std::cout << "Dot" << std::endl;
         currentCharPos += yyleng;
       }
"="    { std::cout << "Equal" << std::endl;
         currentCharPos += yyleng;
       }
"<"    { std::cout << "Lower" << std::endl;
         currentCharPos += yyleng;
       }
"<="   { std::cout << "Lower or Equal" << std::endl;
         currentCharPos += yyleng;
       }
"<-"   { std::cout << "Assign" << std::endl;
         currentCharPos += yyleng;
       }

{ignored-char}+    { currentCharPos += yyleng; }
{lf}+              { currentCharPos += 0; }
<<EOF>>            { std::cout << "End of file" << std::endl; }


%%
int main() {
    reservedId.emplace("bool", BOOL);
    reservedId.emplace("int32", INT32);
    reservedId.emplace("string", STRING);
    reservedId.emplace("unit", UNIT);
    reservedId.emplace("and", AND);
    reservedId.emplace("not", NOT);
    reservedId.emplace("class", CLASS);
    reservedId.emplace("do", DO);
    reservedId.emplace("else", ELSE);
    reservedId.emplace("extends", EXTENDS);
    reservedId.emplace("false", FALSE);
    reservedId.emplace("if", IF);
    reservedId.emplace("in", IN);
    reservedId.emplace("isnull", ISNULL);
    reservedId.emplace("let", LET);
    reservedId.emplace("new", NEW);
    reservedId.emplace("then", THEN);
    reservedId.emplace("true", TRUE);
    reservedId.emplace("while", WHILE);

    yylex();
    return 0;
}