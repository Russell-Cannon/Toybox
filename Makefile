Toybox:	Toybox.o	Toy.o	Types.h	Lexer.h	Parser.h
	g++ -g -o Toybox Toybox.cpp Toy.o

# Toybox.o:	Toybox.cpp	Parser.h
# 	g++ -g -c Toybox.cpp

# Parser.o:	Parser.cpp	Parser.h	Lexer.h	Toy.h	Types.h
# 	g++ -g -c Parser.cpp

# Lexer.o:	Lexer.h
# 	g++ -g -c Lexer.cpp

Toy.o:	Toy.h	Toy.cpp
	g++ -g -c Toy.cpp

# Types.o:	Types.h
# 	g++ -g -c 

run:	Toybox
	./Toybox first.tb

clean:
	del *.exe *.o out.html
