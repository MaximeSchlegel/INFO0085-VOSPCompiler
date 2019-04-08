bison parser/vsopc.y -d
flex lexer/vsopc.l
g++ -g main.cpp lexer/utils.cpp utils/ASTNode.cpp -o vsopc
