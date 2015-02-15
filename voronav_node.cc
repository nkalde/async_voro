#include "voronav_node.h"

int unknown_occ = -1; //not identified
int absent_occ = 2; //absent data
int empty_occ = 0;
int dynamic_occ = 3;
int static_occ = 4;
int border_occ = 1;
Automaton *automaton;

void occupancyCallback(const std_msgs::String::ConstPtr& msg)
{

	ROS_INFO("OCCUPANCY DATA CALLBACK");
	//ROS_INFO("I heard: [%s]", msg->data.c_str());
	Json::Value data;
	Json::Reader reader;
	if (! reader.parse(msg->data,data,false) ){
		ROS_INFO("JSON PARSING FAILED");
	}
	std::string class_name = data.get("__class__","none").asString();
	int rows = data.get("rows",-1).asInt();
	int cols = data.get("cols",-1).asInt();
	long int timestamp = data.get("timestamp",-1).asInt64();//pb
	Json::Value intArray = data["intArray"];

	if (intArray.isArray()){
		//read occupancy data and feed the automaton
		if (automaton == NULL){//create automaton
			automaton = new Automaton(rows,cols);
		}

		for(int i=0;i<automaton->getLig();i++){
			for(int j=0;j<automaton->getCol();j++){

				int d_occ = intArray[i*cols+j].asInt();
				automaton->getCellIJ(i,j)->setSite(d_occ==dynamic_occ or d_occ==border_occ or d_occ==static_occ or d_occ==unknown_occ);	
			}
		}
	}
}

std_msgs::String roadmapProcessing(){

	ROS_INFO("ROADMAP PROCESSING");
	if (automaton != NULL){
		automaton->asynchronous_update(Automaton::cvd);
		automaton->drawAutomaton();
	}
	std_msgs::String msg;
	return msg;
}

int main(int argc, char **argv){
	//initialize node voroads
	ros::init(argc,argv,"vorads");
	ros::NodeHandle n_pub;
	
	//roadmap publisher
	ros::Publisher publisher_roadmap = n_pub.advertise<std_msgs::String>("/smarttiles/navigation_data",1);
	ros::NodeHandle n_sub;
	
	//occupancy data subscriber
	ros::Subscriber subscriber_roadmap = n_sub.subscribe("/smarttiles/occupancy_data",1,occupancyCallback);
	ros::Rate loop_rate(10);
	
	while (ros::ok())
	{
		roadmapProcessing();
		//publisher_roadmap.publish(roadmapProcessing());
		ros::spinOnce();
		loop_rate.sleep();
	}
}
