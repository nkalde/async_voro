all: test_cell
CCFLAGS = -std=c++11
CC = g++

test_cell: test_cell.o cell.o automaton.o
	${CC} ${CCFLAGS} test_cell.o cell.o automaton.o -o test_cell

test_cell.o: test_cell.cc
	${CC} ${CCFLAGS} -c test_cell.cc

cell.o: cell.cc
	${CC} ${CCFLAGS} -c cell.cc

automaton.o: automaton.cc
	${CC} ${CCFLAGS} -c automaton.cc

cells.o: cells.cc
	${CC} -c cells.cc

clean:
	rm *o test_cell
