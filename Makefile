Toybox.exe:	in.html	Toybox.cpp	Toy.hpp	Types.hpp	Lexer.hpp	Parser.hpp
	g++ Toybox.cpp -o Toybox

run:	Toybox.exe
	./Toybox first.tb

clean:
	del *.exe out.html
