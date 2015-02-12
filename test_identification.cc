#include "test_identification.h"

void test_aid(){
	std::cerr << "CASE 1" << std::endl;
	Automaton *a;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map2.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::aid_min);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;

	std::cerr << "CASE 2" << std::endl;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map3.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::aid_min);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;


	std::cerr << "CASE 3" << std::endl;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map4.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::aid_min);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;


	std::cerr << "CASE 4" << std::endl;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map5.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::aid_min);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;
}


void test_lid(){	
	std::cerr << "CASE 1" << std::endl;
	Automaton *a;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map2.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::consensus_id);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;

	std::cerr << "CASE 2" << std::endl;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map3.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::consensus_id);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;


	std::cerr << "CASE 3" << std::endl;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map4.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::consensus_id);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;


	std::cerr << "CASE 4" << std::endl;
	a=map("/home/parallels/ros_local/catkin_ws/src/voroads/src/bitmaps/map5.png");
	a->printAutomaton(Cell::print_site);
	a->asynchronous_update(Automaton::consensus_id);
	a->printAutomaton(Cell::print_identifier_comp);
	delete a;
}

//create first map
Automaton* map(std::string filename){
	cv::Mat img = cv::imread(filename,cv::IMREAD_GRAYSCALE);
	std::cerr << "rowsxcols" << img.rows << "x" << img.cols;
	Automaton *a = new Automaton(img.rows,img.cols);
	for(int row = 0; row < img.rows; ++row) {
		for(int col = 0; col < img.cols; ++col) {
			Cell *c = a->getCellIJ(row,col);
			//(*p++);//points to each pixel value in turn assuming a CV_8UC1 greyscale image
			c->setSite(img.at<uchar>(row,col)<100);
		}
	}
	return a;
}


int main(int argc, char *argv[]){
	//area
	test_aid();
	
	//line
	test_lid();

	return 0;
}
