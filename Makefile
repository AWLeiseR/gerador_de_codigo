FLEX=flex
BISON=bison
CC=gcc
FLAGS=-g -Wall -lm -std=c99

PROGRAMA = gerador
LEXICO = lexico.l
SINTATICO = sintatico.y

$(PROGRAMA): $(LEXICO) $(SINTATICO) ast.h ast.c
	$(BISON) -d $(SINTATICO)
	$(FLEX) $(LEXICO)
	$(CC) -c *.c -I.  $(FLAGS)
	$(CC) *.o -o $(PROGRAMA)  $(FLAGS)

clean:
	rm -f *.yy.c
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.o
	rm -f *.exe
	rm -f $(PROGRAMA)