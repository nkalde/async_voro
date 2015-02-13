CC = g++
CCFLAGS = -std=c++11 `pkg-config --cflags opencv2`
LIBS = `pkg-config --libs opencv2`

all: test_cell test_bisector test_identification

test_identification: test_identification.o cell.o automaton.o
	${CC} ${CCFLAGS} test_identification.o cell.o automaton.o -o test_identification $(LIBS)

test_bisector: test_bisector.o cell.o automaton.o
	${CC} ${CCFLAGS} test_bisector.o cell.o automaton.o -o test_bisector ${LIBS}

test_cell: test_cell.o cell.o automaton.o
	${CC} ${CCFLAGS} test_cell.o cell.o automaton.o -o test_cell ${LIBS}

test_identification.o:
	${CC} ${CCFLAGS} -c test_identification.cc
	
test_bisector.o: test_bisector.cc
	${CC} ${CCFLAGS} -c test_bisector.cc

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
