P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.save
	cp LexicalAnalyzer.save LexicalAnalyzer.o

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

run : P2.out test.ss
	./P2.out test.ss

clean : 
	rm *.o P2.out *.dbg *.lst *.p2 *.p1

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.save SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamRP2
	mkdir TeamRP2
	cp Project2.cpp TeamRP2
	cp LexicalAnalyzer.h TeamRP2
	cp LexicalAnalyzer.save TeamRP2
	cp SyntacticalAnalyzer.h TeamRP2
	cp SyntacticalAnalyzer.cpp TeamRP2
	cp makefile TeamRP2
	cp README.txt TeamRP2
	tar cfvz TeamRP2.tgz TeamRP2
	cp TeamRP2.tgz ~tiawatts/cs460drop
