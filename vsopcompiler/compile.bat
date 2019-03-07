bison parser/vsopc.yy -d
flex lexer/vsopc.l
g++ main.cpp lexer/utils.cpp -o vsopc
