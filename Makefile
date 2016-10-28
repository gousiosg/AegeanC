OBJ=yyparse.o yylex.o tree.o symbol.o codegen.o
YFLAGS=-vd
CFLAGS=-g -O3 -pipe

acc: $(OBJ)
	cc -o acc $(OBJ)

yyparse.o: yyparse.y

yylex.o: yylex.l

tree.o: tree.c

symbol.o: symbol.c

codegen.o:codegen.c

clean: 
	rm $(OBJ) y.tab.h y.output acc

install: acc
		cd $(HOME)
		mkdir $(HOME)/Aegean\ C
		cd $(HOME)/Aegean\ C
		mkdir $(HOME)/Aegean\ C/src
		mkdir $(HOME)/Aegean\ C/doc
		cd  $(CUR)
		cp codegen.c symbol.c tree.c tree.h yylex.l yyparse.y $(HOME)/Aegean\ C/src/
		cp acc.sh acc  $(HOME)/Aegean\ C/
		cp documentation.doc sample_program.ac readme.txt  $(HOME)/Aegean\ C/doc/
		cd $(HOME)/Aegean\ C
		less readme.txt 

	  


