#include "voronav_node.h"

int unknown_occ = -1; //not identified
int absent_occ = 2; //absent data
int empty_occ = 0;
int dynamic_occ = 3;
int static_occ = 4;
int border_occ = 1;
Automaton *automaton =  new Automaton();

void occupancyCallback(const std_msgs::String::ConstPtr& msg)
{
	
	ROS_INFO("I heard: [%s]", msg->data.c_str());
	Json::Value data = msg->data;
	int cols = data.get("cols",-1).asInt();
	int rows = data.get("rows",-1).asInt();

	//read occupancy data and feed the automaton
	if (!(automaton->getLig()==rows and automaton->getLig()==cols)){//create automaton
		delete automaton;
		automaton = new Automaton(rows,cols);
	}

	Json::Value intArray_json = data["intArray"];
	for(int i=0;i<automaton->getLig();i++){
		for(int j=0;j<automaton->getCol();j++){
			int d_occ = intArray_json[i*cols+j].asInt();
			automaton->getCellIJ(rows,cols)->setSite(d_occ==dynamic_occ or d_occ==border_occ or d_occ==static_occ or d_occ==unknown_occ);	
		}
	}
}

std_msgs::String roadmapProcessing(){
	automaton->asynchronous_update(Automaton::cvd);
	std_msgs::String msg;
	ROS_INFO("%s", msg.data.c_str());
	return msg;
}

int main(int argc, char **argv){
//	/*
	//initialize node voroads
	ros::init(argc,argv,"vorads");
	//ros::NodeHandle n_pub;
	//ros::Publisher publisher_roadmap = n_pub.advertise<std_msgs::String>("/smarttiles/navigation_data",1);
	ros::NodeHandle n_sub;
	ros::Subscriber subscriber_roadmap = n_sub.subscribe("/smarttiles/occupancy_data",1,occupancyCallback);
	ros::Rate loop_rate(1);
	int count =0;
	while (ros::ok())
	{
		//publisher_roadmap.publish(roadmapProcessing());
		ros::spinOnce();
		loop_rate.sleep();
		++count;
	}
}
