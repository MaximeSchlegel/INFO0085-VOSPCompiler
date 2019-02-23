%{
    #include <map>
    #include <string>
    #include <iostream>
    #include <math.h>
    #include "vsopc.tab.h"
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
    std::string strBuffer;
%}

%option noyywrap

space           " "
tab             \x09
lf              \x0A
ff              \x0B
cr              \x0C
whitespace      {space}|{tab}
ignored-char    {whitespace}|{lf}|{ff}|{cr}

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

typeId      {uppercase-letter}({letter}|{digit}|"_")*
objectId    {lowercase-letter}({letter}|{digit}|"_")*


%%
{bin-literal}                             { std::string s = yytext; s = s.erase(0,2);
                                            int intLit = debinify(s);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid binary number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                          }
{digit-literal}                           { int intLit = dedigify(yytext);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                          }
{hex-literal}                             { std::string s = yytext; s = s.erase(0,2);
                                            int intLit = dehexify(s);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid hexadecimal number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                          }
{integer-literal}({objectId}|{typeId})    { std::cout << "Lexical Error : Missing whitespace between integer and identifier" << std::endl;
                                            return -1;
                                          }

\x22    { std::string buffer = yytext;
          int c = yyinput();
          bool erase_space;
          while (c != '\x22') {
          switch (c) {
            case EOF: {
              std::cout << "Lexical Error : End of file in string" << std::endl;
              return -1;
            }
            case '\x0A': {
              std::cout << "Lexical Error : Feed line without \\ in string" << std::endl;
              return -1;
            }
            case '\x00': {
              std::cout << "Lexical Error : Null character in string" << std::endl;
              return -1;
            }
            case '\x5C': {
              int c1 = yyinput();
              erase_space = false;
              switch (c1) {
                case '\x0A': {
                  erase_space = true;
                  break;
                }
                case 'b': {
                  buffer += "\\x08";
                  break;
                }
                case 't': {
                  buffer += "\\x09";
                  break;
                }
                case 'n': {
                  buffer += "\\x0A";
                  break;
                }
                case 'r': {
                  buffer += "\\x0D";
                  break;
                }
                case '\x22': {
                  buffer += "\\x22";
                  break;
                }
                case '\x5C': {
                  buffer += "\\x5C";
                  break;
                }
                case 'x': {
                  char c2 = yyinput(); char c3 = yyinput();
                  int asciiChar = dehexify(c2, c3);
                  if (asciiChar == -1) {
                    std::cout << "Lexical Error : Invalid Ascii Character" << std::endl;
                    return -1;
                  }
                  if (asciiChar < 32 or asciiChar == 127 ) {
                    buffer += "\\x"; buffer += c2; buffer += c3;
                    std::cout << c2 << " ; "<< c3 << std::endl;
                  } else {
                    std::cout << asciiChar << std::endl;
                    buffer += asciiChar;
                  }
                  break;
                }
                default: { //Error backslash without a valid escape sequence
                  std::cout << "Lexical Error : \\ character in string" << std::endl;
                  return -1;
                }
              }
              break;
            }
            case ' ': {
              if (!erase_space) {
                buffer += c;
              }
              break;
            }
            case '\x09': {
              if (!erase_space) {
                buffer += c;
              }
              break;
            }
            default: {
              erase_space = false;
              buffer += c;
              break;
            }
          }
          c = yyinput();
        }
        buffer += c;
        std::cout << "String literral : " << buffer << std::endl;
      }

{typeId}    { std::cout << "Type-Identifier : " << yytext << std::endl; }

{objectId}    { std::map<std::string, enum yytokentype>::iterator isReserved;
                isReserved = reservedId.find(yytext);
                if (isReserved != reservedId.end()) {
                  std::cout << "Reversed Identifier : " << yytext << std::endl;
                } else {
                  std::cout << "Object-Identifier : " << yytext << std::endl;
                }
              }

"{"    { std::cout << "L Brace" << std::endl; }
"}"    { std::cout << "R Brace" << std::endl; }
"("    { std::cout << "L Par" << std::endl; }
")"    { std::cout << "R Par" << std::endl; }
":"    { std::cout << "Colon" << std::endl; }
";"    { std::cout << "Semi-Colon" << std::endl; }
","    { std::cout << "Coma" << std::endl; }
"+"    { std::cout << "Plus" << std::endl; }
"-"    { std::cout << "Minus" << std::endl; }
"*"    { std::cout << "Times" << std::endl; }
"/"    { std::cout << "Div" << std::endl; }
"^"    { std::cout << "Pow" << std::endl; }
"."    { std::cout << "Dot" << std::endl; }
"="    { std::cout << "Equal" << std::endl; }
"<"    { std::cout << "Lower" << std::endl; }
"<="   { std::cout << "Lower or Equal" << std::endl; }
"<-"   { std::cout << "Assign" << std::endl; }

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
              return -1;
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

{ignored-char}    {  }
<<EOF>>           { std::cout << "End of file" << std::endl; }


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