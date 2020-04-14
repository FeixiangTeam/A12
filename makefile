HOME=.
BIN=${HOME}/bin
INCL=${HOME}/incl
SRC=${HOME}/src
CC=g++-9

VPATH=${SRC}:${SRC}/GA

.cpp.o:
	${CC} -c $< -I ${INCL} -std=c++17 -Wall

GAexec: main.o data.o GA_init.o individual_init.o select.o mutation.o cross.o solve.o
	${CC} $^ -o ${BIN}/$@

bruteforce: main.o data.o bruteforce.o
	${CC} $^ -o ${BIN}/$@

test: test.o main.o data.o GA_init.o individual_init.o cross.o
	${CC} $^ -o ${BIN}/$@
	-rm -f *.o
	@echo ------------
	@${BIN}/$@
	@echo ------------
	-rm -f ${BIN}/$@

.PHONY: clean
clean:
	-rm -f *.o
