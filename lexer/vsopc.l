%{
    #include "lexer/utils.h"
    #define YY_DECL extern "C" int yylex()

    int yycharno = 1;
    std::map<std::string, enum yytokentype> reservedId;
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
"//".*\n                  { yycharno = 1; }
<INITIAL,COMMENT>"(*"     { openedCommentsPos.emplace(yylineno, yycharno);
                            yycharno += yyleng;
                            BEGIN(COMMENT);
                          }
<COMMENT>"*)"             { yycharno += yyleng;
                            openedCommentsPos.pop();
                            if (openedCommentsPos.empty()) { BEGIN(INITIAL); }
                          }
<COMMENT>([^*(\n])+       { yycharno += yyleng; }
<COMMENT>\n               { yycharno = 1; }
<COMMENT><<EOF>>          { std::cout << "Lexical Error : end of file in comment" << std::endl;
                            return -1;
                          }

\x22                           { strStartPos = std::pair<int , int>(yycharno, yylineno);
                                 strBuffer = yytext;
                                 yycharno += yyleng;
                                 BEGIN(STR);
                               }
<STR>\x22                      { strBuffer += yytext;
                                 std::cout << "String Literral : " << strBuffer << std::endl;
                                 yycharno += yyleng;
                                 BEGIN(INITIAL);
                               }
<STR>\x5C\x0A([ '\x09'])*      { yycharno = yyleng - 2; }
<STR>\x5Cb                     { strBuffer += "\\x08";
                                 yycharno += yyleng;
                               }
<STR>\x5Ct                     { strBuffer += "\\x09";
                                 yycharno += yyleng;
                               }
<STR>\x5Cn                     { strBuffer += "\\x0A";
                                 yycharno += yyleng;
                               }
<STR>\x5Cr                     { strBuffer += "\\x0D";
                                 yycharno += yyleng;
                               }
<STR>\x5C\x22                  { strBuffer += 34;
                                 yycharno += yyleng;
                               }
<STR>\x5C\x5C                  { strBuffer += 92;
                                 yycharno += yyleng;
                               }
<STR>\x5Cx({hex-digit}{2})     { std::string s = yytext; s = s.erase(0,2);
                                 int intLit = dehexify(s);
                                 if (intLit == -1) {
                                   std::cout << "Lexical Error : invalid hexadecimal number" << std::endl;
                                   return -1;
                                 }
                                 strBuffer += intLit;
                                 yycharno += yyleng;
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
                                 yycharno += yyleng;
                               }

{bin-literal}                             { std::string s = yytext; s = s.erase(0,2);
                                            int intLit = debinify(s);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid binary number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                            yycharno += yyleng;
                                          }
{digit-literal}                           { int intLit = dedigify(yytext);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                            yycharno += yyleng;
                                          }
{hex-literal}                             { std::string s = yytext; s = s.erase(0,2);
                                            int intLit = dehexify(s);
                                            if (intLit == -1) {
                                              std::cout << "Lexical Error : invalid hexadecimal number" << std::endl;
                                              return -1;
                                            }
                                            std::cout << "Integer-literal : " << intLit << std::endl;
                                            yycharno += yyleng;
                                          }
{integer-literal}({objectId}|{typeId})    { std::cout << "Lexical Error : missing whitespace between integer and identifier" << std::endl;
                                            return -1;
                                          }

{typeId}    { std::cout << "Type-Identifier : " << yytext << std::endl;
              yycharno += yyleng;
            }

{objectId}    { std::map<std::string, enum yytokentype>::iterator isReserved;
                isReserved = reservedId.find(yytext);
                if (isReserved != reservedId.end()) {
                  std::cout << "Reversed Identifier : " << yytext << std::endl;
                } else {
                  std::cout << "Object-Identifier : " << yytext << std::endl;
                }
                yycharno += yyleng;
              }

"{"    { std::cout << "L Brace" << std::endl;
         yycharno += yyleng;
       }
"}"    { std::cout << "R Brace" << std::endl;
         yycharno += yyleng;
       }
"("    { std::cout << "L Par" << std::endl;
         yycharno += yyleng;
       }
")"    { std::cout << "R Par" << std::endl;
         yycharno += yyleng;
       }
":"    { std::cout << "Colon" << std::endl;
         yycharno += yyleng;
       }
";"    { std::cout << "Semi-Colon" << std::endl;
         yycharno += yyleng;
       }
","    { std::cout << "Coma" << std::endl;
         yycharno += yyleng;
       }
"+"    { std::cout << "Plus" << std::endl;
         yycharno += yyleng;
       }
"-"    { std::cout << "Minus" << std::endl;
         yycharno += yyleng;
       }
"*"    { std::cout << "Times" << std::endl;
         yycharno += yyleng;
       }
"/"    { std::cout << "Div" << std::endl;
         yycharno += yyleng;
       }
"^"    { std::cout << "Pow" << std::endl;
         yycharno += yyleng;
       }
"."    { std::cout << "Dot" << std::endl;
         yycharno += yyleng;
       }
"="    { std::cout << "Equal" << std::endl;
         yycharno += yyleng;
       }
"<"    { std::cout << "Lower" << std::endl;
         yycharno += yyleng;
       }
"<="   { std::cout << "Lower or Equal" << std::endl;
         yycharno += yyleng;
       }
"<-"   { std::cout << "Assign" << std::endl;
         yycharno += yyleng;
       }

{ignored-char}+    { yycharno += yyleng; }
{lf}+              { yycharno += 0; }
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