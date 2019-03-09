all: install-tools vsopc

install-tools:
	bison parser/vsopc.yy -d
	flex -+ lexer/vsopc.l

vsopc: lex.yy.c vsopc.tab.h
	g++ lex.yy.c lexer/utils.cpp -o vsopc
