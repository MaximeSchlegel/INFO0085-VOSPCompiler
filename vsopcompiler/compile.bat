bison parser/vsopc.y -d
flex lexer/vsopc.l
g++ -g main.cpp lexer/utils.cpp checker/checker.cpp utils/ASTNode.cpp utils/graph.cpp -o vsopc
