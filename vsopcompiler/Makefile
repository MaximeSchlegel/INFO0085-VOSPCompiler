all: install-tools vsopc

install-tools:
	flex lexer/vsopc.l
	bison -d parser/vsopc.y

vsopc: lex.yy.c vsopc.tab.h
	g++ main.cpp lexer/utils.cpp utils/ASTNode.cpp -o vsopc