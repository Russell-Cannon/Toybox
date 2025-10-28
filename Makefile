ToyBox.exe:	postamble.txt	preamble.txt	Toy.hpp	Types.hpp	Lexer.hpp	Parser.hpp
	g++ ToyBox.cpp -o ToyBox

run:	ToyBox.exe
	./ToyBox first.tb

clean:
	del *.exe *.html

do:	ToyBox.exe	run
