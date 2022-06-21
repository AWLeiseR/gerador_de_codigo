rm ast.o
rm funcoes.o
rm gerador
rm lex.yy.c
rm lex.yy.o
rm sintatico.tab.c
rm sintatico.tab.h
rm sintatico.tab.o
make
./gerador < triangulo.txt >texte.s
# valgrind --leak-check=full ./gerador < triangulo.txt >texte.s