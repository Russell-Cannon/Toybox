.NOTPARALLEL:
Toybox.exe:	Toybox.o	Toy.o	Parser.o	Lexer.o	SymbolTable.o	Types.h	
	g++ -g -o Toybox Toybox.cpp Toy.o Parser.o Lexer.o SymbolTable.o

Toy.o:	Toy.h	Toy.cpp
	g++ -g -c Toy.cpp

Parser.o:	Parser.h	Parser.cpp	Lexer.h	Toy.h	SymbolTable.h	Types.h
	g++ -g -c Parser.cpp

Lexer.o:	Lexer.h	Lexer.cpp	Types.h
	g++ -g -c Lexer.cpp

SymbolTable.o: SymbolTable.h	SymbolTable.cpp
	g++ -g -c SymbolTable.cpp

run:	Toybox.exe
	./Toybox $(ARGS)

clean:
	del *.exe *.o out.html