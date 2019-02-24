bison -d parser/vsopc.y
flex lexer/vsopc.l
g++ lex.yy.c lexer/utils.cpp -o vsopc
