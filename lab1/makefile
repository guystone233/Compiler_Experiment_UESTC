CFLAGS = -O0 -g -lfl

all: lex.yy.c
	gcc -o scan $(CFLAGS) lex.yy.c
lex.yy.c : auto_lex.l
	flex auto_lex.l

clean:
	$(RM) *.o *.exe lex.yy.c