Toybox.exe:	Toybox.o	Toy.o	Parser.o	Lexer.o	Types.h	
	g++ -g -o Toybox Toybox.cpp Toy.o Parser.o Lexer.o 

Toy.o:	Toy.h	Toy.cpp
	g++ -g -c Toy.cpp

Parser.o:	Parser.h	Parser.cpp	Lexer.h	Toy.h	Types.h	SymbolTable.h
	g++ -g -c Parser.cpp

Lexer.o:	Lexer.h	Lexer.cpp	Types.h
	g++ -g -c Lexer.cpp


run:	Toybox.exe
	./Toybox

clean:
	del *.exe *.o out.html