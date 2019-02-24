install-tools: parser/vsopc.y lexer/vsopc.l
	bison -d parser/vsopc.y
	flex lexer/vsopc.l

vsopc: lex.yy.c lexer.utils.cpp
	g++ lex.yy.c lexer/utils.cpp -o vsopc