bison parser/vsopc.y -d
flex lexer/vsopc.l
g++ main.cpp vsopc.tab.cc -o vsopc
