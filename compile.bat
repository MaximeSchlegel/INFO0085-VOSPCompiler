bison -d vsopc.y
flex vsopc.l
gcc -o vsopc lex.yy.c
