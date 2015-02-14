#include "test_identification.h"

void test_aid(){
	std::cerr << "CASE 1" << std::endl;
	Automaton *a = new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map2.png");
	a->asynchronous_update(Automaton::avd_min);
	a->drawAutomaton();
	delete a;
	
	std::cerr << "CASE 2" << std::endl;
	a=new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map3.png");
	a->asynchronous_update(Automaton::avd_min);
	a->drawAutomaton();
	delete a;

	std::cerr << "CASE 3" << std::endl;
	a=new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map4.png");
	a->asynchronous_update(Automaton::avd_min);
	a->drawAutomaton();
	delete a;

	/*
	std::cerr << "CASE 4" << std::endl;
	a=new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map5.png");
	a->asynchronous_update(Automaton::avd_min);
	a->drawAutomaton();
	delete a;
	*/	
}


void test_lid(){	
	std::cerr << "CASE 1" << std::endl;
	Automaton *a = new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map2.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;

	std::cerr << "CASE 2" << std::endl;
	a= new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map3.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;


	std::cerr << "CASE 3" << std::endl;
	a=new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map4.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;


	/*
	std::cerr << "CASE 4" << std::endl;
	a=new Automaton("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map5.png");
	a->asynchronous_update(Automaton::cvd);
	a->drawAutomaton();
	delete a;
	*/
}

int main(int argc, char *argv[]){
	//area
	//test_aid();

	//line
	test_lid();

	return 0;
}
