bison -d vsopc.y
flex vsopc.l
g++ -o vsopc lex.yy.c
