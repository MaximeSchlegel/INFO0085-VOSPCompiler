all: install-tools vsopc

install-tools:
	flex lexer/vsopc.l
	bison -d parser/vsopc.y

vsopc: lex.yy.c vsopc.tab.h
	g++ lex.yy.c lexer/utils.cpp -o vsopc