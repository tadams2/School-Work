# You have to comment the proper lines below according to the
# scanner and parser generators available in your system

LEX = lex 
#LEX = flex 

# We assume that your C-compiler is called cc

CC = cc

all: scanner

# 'make' will generate the scanner execuable

scanner: lex.yy.c
	$(CC) -o scanner lex.yy.c -lfl

## this is the rule to generate the file lex.yy.c from our file scanner.l

lex.yy.c: scanner.l
	$(LEX) scanner.l

## 'make clean' removes the generated files

clean:	
	rm -f lex.yy.c scanner 
