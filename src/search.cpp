#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "nav_msgs/Path.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/Joy.h"
#include <iostream>
double x_target = 0.0;
double y_target = 0.0;
double z_target = 0.1;

void suvcallback ( const std::msgs Bool)

int main(int argc, char **argv)
{
	ros::init(argc, argv, "search");
	ros::NodeHandle n;

	ros::Publisher read_pub = n.advertise<std_msgs::Empty>("ardrone/takeoff", 1000);
	ros::Publisher read_pub2 = n.advertise<std_msgs::Empty>("ardrone/land", 1000);
	ros::Publisher read_pub3 = n.advertise<geometry_msgs::Twist>("cmd_vel", 30);
	ros::Subscriber sub2 = n.subscribe("error", 1000, );
	std_msgs::Empty myMsg;
	int count = 0;
	geometry_msgs::Twist cmdMsg;

while(ros::ok()){
	seconds = (double)ros::Time::now().toSec();
while (seconds < 10)
	if ()
	cmdMsg.linear.z = 0.1;
	count++;
}
}