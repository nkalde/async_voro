#include "automaton.h"

int main(int argc, char *argv[]){
	//default constructor
	Cell *c0 = new Cell();
    //c0->printCell();
    //c0->printPatternsMap();
	delete c0;

	//other constructor
    Cell *c1 = new Cell(0,0,0);
    //c1->printCell();
	delete c1;


	//default constructor
    Automaton *a0 = new Automaton();
    //a0->printAutomaton(1);
    delete a0;


	//default constructor
	Automaton *a1 = new Automaton(5,5);
    a1->printAutomaton(0);
    a1->printAutomaton(1);
    a1->printAutomaton(2);
    a1->printAutomaton(3);
    a1->printAutomaton(4);
	a1->printAutomaton(5);
    delete a1;
	return 0;
}
