bison -d vsopc.y
flex vsopc.l
g++ lex.yy.c utils.cpp -o vsopc
