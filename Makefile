CC = gcc



YACCFILE = ModelChecker.y
LEXFILE = ModelChecker.l
OUTPUTFILE = mc
TREE = parseTree.c kripke.c Set.c model_checker.c



PARSERHEADER = y.tab.h
PARSERCFILE = y.tab.c 
FLEX = lex.yy.c

all : $(OUTPUTFILE)


$(OUTPUTFILE) : $(PARSERCFILE) $(FLEX) $(TREE)
	mkdir -p bin
	$(CC) $(CFLAGS)  -o $@ $^


$(PARSERCFILE) $(PARSERHEADER) : $(YACCFILE)
	yacc -d $(YACCFILE)


$(FLEX) : $(LEXFILE)
	flex $(LEXFILE)

run : $(OUTPUTFILE)
	./$(OUTPUTFILE)

clean :
	rm -f $(OUTPUTFILE) $(PARSERHEADER) $(PARSERCFILE) $(FLEX)