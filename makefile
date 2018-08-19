
build:main

main:component.h  enums.h  inputstream.h  instructionRunner.h  PL0.cpp  README.md  syntax.h
	g++ PL0.cpp -o main
	cat try.txt

run:main
	./main

clean:
	-rm main