bison parser/vsopc.y -d
flex lexer/vsopc.l
g++ main.cpp lexer/lexer_header.cpp parser/parser_header.cpp -o vsopc
