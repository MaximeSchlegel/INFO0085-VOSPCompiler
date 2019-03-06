bison -d parser/vsopc.y
flex lexer/vsopc.l
g++ main.cpp lexer/utils.cpp -o vsopc
