HOME = .
BIN = ${HOME}/bin
INCL = ${HOME}/incl
SRC = ${HOME}/src
ANALYSIS = ${HOME}/analysis
CC = g++-9
CFLAG = -std=c++17 -Wall -O2

VPATH = ${SRC}:${SRC}/GA

.cpp.o:
	${CC} -c $< -I ${INCL} ${CFLAG}

GA: main.o data.o GA_init.o solve.o individual_init.o select.o cross.o mutation.o
	${CC} $^ -o ${BIN}/$@ ${CFLAG}

GA_v2: main.o data.o GA_init.o solve.o individual_init_randomly.o select.o cross.o mutation.o
	${CC} $^ -o ${BIN}/$@ ${CFLAG}

bruteforce: main.o data.o bruteforce.o
	${CC} $^ -o ${BIN}/$@ ${CFLAG}

analysis: bruteforce GA GA_v2
	@python3 ${ANALYSIS}/analysis.py data.json ${ANALYSIS}/bruteforce.csv ${BIN}/bruteforce 1
	@python3 ${ANALYSIS}/analysis.py data.json ${ANALYSIS}/GA.csv ${BIN}/GA 10
	@python3 ${ANALYSIS}/analysis.py data.json ${ANALYSIS}/GA_v2.csv ${BIN}/GA_v2 10

test: test.o
	${CC} $^ -o ${BIN}/$@
	-rm -f *.o
	@echo ------------
	@${BIN}/$@
	@echo ------------
	-rm -f ${BIN}/$@

.PHONY: clean
clean:
	-rm -f *.o
