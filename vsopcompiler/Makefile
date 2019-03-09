all: install-tools vsopc

install-tools:
	flex -+ lexer/vsopc.ll
	bison parser/vsopc.yy -d

vsopc: lex.yy.cc vsopc.tab.hh
	g++ main.cpp lex.yy.cc lexer/header_lexer.cpp vsopc.tab.cc parser/header_parser.cpp -o vsopc
