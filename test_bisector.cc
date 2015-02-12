#include "test_bisector.h"

void bis_case_1(Automaton * a){
    Cell * s1 = a->getCellIJ(6,1);
    Cell * s2 = a->getCellIJ(2,5);
    s1->setSite(true);
    s2->setSite(true);
}

void bis_case_2(Automaton * a){
    Cell * s1 = a->getCellIJ(6,1);
    Cell * s2 = a->getCellIJ(1,6);
    s1->setSite(true);
    s2->setSite(true);

}

void bis_case_3(Automaton * a){
    Cell *s1= a->getCellIJ(6,2);
    Cell *s2= a->getCellIJ(0,4);
    s1->setSite(true);
    s2->setSite(true);
}

void bis_case_4(Automaton * a){
    Cell *s1 = a->getCellIJ(6,2);
    Cell *s2 = a->getCellIJ(0,5);
    s1->setSite(true);
    s2->setSite(true);
}

void bis_case_5(Automaton * a){
    Cell *s1 = a->getCellIJ(6,2);
    Cell *s2 = a->getCellIJ(1,4);
    s1->setSite(true);
    s2->setSite(true);
}

void bis_case_6(Automaton * a){
    Cell *s1 = a->getCellIJ(6,3);
    Cell *s2 = a->getCellIJ(1,4);
    s1->setSite(true);
    s2->setSite(true);
}

int main(int argc, char *argv[]){
    std::cerr << "CASE 1" << std::endl;
    Automaton *a = new Automaton(8,8);
    bis_case_1(a);
    a->printAutomaton(Cell::print_site);
    a->asynchronous_update(Automaton::pr);
    a->asynchronous_update(Automaton::bis_thick);
    a->asynchronous_update(Automaton::bis_thin);
    a->printAutomaton(Cell::print_bisector);
    delete a;

    std::cerr << "CASE 2" << std::endl;
    a = new Automaton(8,8);
    bis_case_2(a);
    a->printAutomaton(Cell::print_site);
    a->asynchronous_update(Automaton::pr);
    a->asynchronous_update(Automaton::bis_thick);
    a->asynchronous_update(Automaton::bis_thin);
    a->printAutomaton(Cell::print_bisector);
    delete a;

    std::cerr << "CASE 3" << std::endl;
    a = new Automaton(8,8);
    bis_case_3(a);
    a->printAutomaton(Cell::print_site);
    a->asynchronous_update(Automaton::pr);
    a->asynchronous_update(Automaton::bis_thick);
    a->asynchronous_update(Automaton::bis_thin);
    a->printAutomaton(Cell::print_bisector);
    delete a;

    std::cerr << "CASE 4" << std::endl;
    a = new Automaton(8,8);
    bis_case_4(a);
    a->printAutomaton(Cell::print_site);
    a->asynchronous_update(Automaton::pr);
    a->asynchronous_update(Automaton::bis_thick);
    a->asynchronous_update(Automaton::bis_thin);
    a->printAutomaton(Cell::print_bisector);
    delete a;

    std::cerr << "CASE 5" << std::endl;
    a = new Automaton(8,8);
    bis_case_5(a);
    a->printAutomaton(Cell::print_site);
    a->asynchronous_update(Automaton::pr);
    a->asynchronous_update(Automaton::bis_thick);
    a->asynchronous_update(Automaton::bis_thin);
    a->printAutomaton(Cell::print_bisector);
    delete a;

    std::cerr << "CASE 6" << std::endl;
    a = new Automaton(8,8);
    bis_case_6(a);
    a->printAutomaton(Cell::print_site);
    a->asynchronous_update(Automaton::pr);
    a->asynchronous_update(Automaton::bis_thick);
    a->asynchronous_update(Automaton::bis_thin);
    a->printAutomaton(Cell::print_bisector);
    delete a;

    return 0;
}
