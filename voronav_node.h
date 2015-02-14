#ifndef VORONAV_NODE_H
#define VORONAV_NODE_H
#include <json/json.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
<<<<<<< HEAD
#include "automaton.h"
=======
>>>>>>> e74e0856d75fb4d472780cad8f314d304bb10339
#include <sstream>

//feed occupancy data to the automaton
void occupancyCallback(const std_msgs::String::ConstPtr& msg);

//update automaton to compute roadmap
std_msgs::String roadmapProcessing();

#endif // VORONAV_NODE_H
