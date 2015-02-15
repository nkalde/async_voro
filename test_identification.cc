#include "test_identification.h"

void test_avd(){
	std::cerr << "CASE 1" << std::endl;
	Automaton *a = new Automaton("bitmaps/map2.png");
	a->asynchronous_update(Automaton::avd);
	a->drawAutomaton();
	delete a;
	
	std::cerr << "CASE 2" << std::endl;
	a=new Automaton("bitmaps/map3.png");
	a->asynchronous_update(Automaton::avd);
	a->drawAutomaton();
	delete a;

	std::cerr << "CASE 3" << std::endl;
	a=new Automaton("bitmaps/map4.png");
	a->asynchronous_update(Automaton::avd);
	a->drawAutomaton();
	delete a;

	/*
	std::cerr << "CASE 4" << std::endl;
	a=new Automaton("bitmaps/map5.png");
	a->asynchronous_update(Automaton::avd_min);
	a->drawAutomaton();
	delete a;
	*/	
}


void test_lvd(){	
	std::cerr << "CASE 1" << std::endl;
	Automaton *a = new Automaton("bitmaps/map2.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;

	std::cerr << "CASE 2" << std::endl;
	a= new Automaton("bitmaps/map3.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;


	std::cerr << "CASE 3" << std::endl;
	a=new Automaton("bitmaps/map4.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;


	/*
	std::cerr << "CASE 4" << std::endl;
	a=new Automaton("bitmaps/map5.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;
	*/
}

int main(int argc, char *argv[]){
	//area
	//test_avd();

	//line
	test_lvd();

	return 0;
}
