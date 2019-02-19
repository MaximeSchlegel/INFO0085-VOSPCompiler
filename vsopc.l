%{
    #include <map>
    #include <string>
    #include <iostream>
    #include "vsopc.tab.h"
    #define YY_DECL extern "C" int yylex()

    std::map<std::string, enum yytokentype> reservedId;
%}

%option noyywrap


tab         "\x9"
lf          "\x10"
ff          "\x12"
cr          "\x13"
whitespace  " "|{tab}|{lf}|{ff}|{cr}

bin-digit   0|1
digit       {bin-digit}|[2-9]
hex-digit   {digit}|[a-fA-F]
integer-literal     {digit}+|"0x"{hex-digit}+|"0b"{bin-digit}+

lowercase-letter    [a-z]
uppercase-letter    [A-Z]
letter              {lowercase-letter}|{uppercase-letter}

regular-char        [^\n\0EOF]
escape-sequence     "b"|"t"|"n"|"r"|'"'|"\"|"x"{hex-digit}+
escape-char         "\"{escape-char}


typeId              {uppercase-letter}({letter}|{digit}|"_")*
objectId            {lowercase-letter}({letter}|{digit}|"_")*


%%
EOF             {std::cout << "End-of-file" << std::endl; }
{whitespace}    {  }



{integer-literal}                           { std::cout << "Integer-literal : " << yytext << std::endl; }
{integer-literal}({objectId}|{typeId})      { std::cout << "Lexical Error : Missing whitespace between integer and identifier" << std::endl; }


{typeId}        { std::cout << "Type-Identifier : " << yytext << std::endl; }

{objectId}      { std::map<std::string, enum yytokentype>::iterator isReserved;
                  isReserved = reservedId.find(yytext);
                  if (isReserved != reservedId.end()) {
                    std::cout << "Reversed Identifier : " << yytext << std::endl;
                  }
                  std::cout << "Object-Identifier : " << yytext << std::endl;
               }

"{"     { std::cout << "L Brace" << std::endl; }
"}"     { std::cout << "R Brace" << std::endl; }
"("     { std::cout << "L Par" << std::endl; }
")"     { std::cout << "R Par" << std::endl; }
":"     { std::cout << "Colon" << std::endl; }
";"     { std::cout << "Semi-Colon" << std::endl; }
","     { std::cout << "Coma" << std::endl; }
"+"     { std::cout << "Plus" << std::endl; }
"-"     { std::cout << "Minus" << std::endl; }
"*"     { std::cout << "Times" << std::endl; }
"/"     { std::cout << "Div" << std::endl; }
"^"     { std::cout << "Pow" << std::endl; }
"."     { std::cout << "Dot" << std::endl; }
"="     { std::cout << "Equal" << std::endl; }
"<"     { std::cout << "Lower" << std::endl; }
"<="    { std::cout << "Lower or Equal" << std::endl; }
"<-"    { std::cout << "Assign" << std::endl; }

"//"    { std::cout << "Skip Line Comment" << std::endl;
          int c = yyinput();
          while (c != '\n' && c != EOF) {
            c = yyinput();
          }
          if (c == EOF) {
            unput(EOF);
          }
        }
"(*"    { std::cout << "Skip Long Comment" << std::endl;
          int opened = 1;
          while(opened != 0) {
            int c1 = yyinput();
            if (c1 == EOF) {
              std::cout << "Error : End-Of-File while scanning Comment" << std::endl;
              exit(-1);
            }
            if (c1 == '*') {
              int c2 = yyinput();
              if (c2 == ')') {
                opened--;
              } else {
                unput(c2);
              }
            }
            if (c1 == '(') {
              int c2 = yyinput();
              if (c2 == '*') {
                opened++;
              } else {
                unput(c2);
              }
            }
          }
        }


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