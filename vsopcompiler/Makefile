all: install-tools vsopc

install-tools:
	flex lexer/vsopc.l
	bison -d parser/vsopc.y

vsopc: lex.yy.c vsopc.tab.h
	g++ main.cpp lexer/utils.cpp checker/checker.cpp utils/ASTNode.cpp utils/symbolTable.cpp -o vsopc