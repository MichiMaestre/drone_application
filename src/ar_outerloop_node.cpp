#include "ros/ros.h"
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

double seconds;
double startSeconds;

bool inAir = false;
double roll = 0.0;
double pitch = 0.0;
double yaw = 0.0;
double throttle = 0.0;
double takeoff = 0.0;
double land = 0.0;

void subCallback(const sensor_msgs::Joy& msg)
{
	
	roll = msg.axes[3];
	pitch = msg.axes[4];
	yaw = msg.axes[0];
	throttle = msg.axes[1];
	takeoff = msg.buttons[0];
	land = msg.buttons[1];


	ROS_INFO("x: %f y: %f z: %f t: %f takeoff: %f land: %f", pitch, roll,yaw,throttle, takeoff, land);


}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "ar_outerloop");
	ros::NodeHandle n;

	startSeconds = (double)ros::Time::now().toSec();

	
	ros::Subscriber sub = n.subscribe("joy", 1000, subCallback);
	ros::Publisher read_pub = n.advertise<std_msgs::Empty>("ardrone/takeoff", 1000);
	ros::Publisher read_pub2 = n.advertise<std_msgs::Empty>("ardrone/land", 1000);
	ros::Publisher read_pub3 = n.advertise<geometry_msgs::Twist>("cmd_vel", 30);
	std_msgs::Empty myMsg;
	geometry_msgs::Twist cmdMsg;
	ros::Rate loop_rate(30);
	while(ros::ok()){
		ros::spinOnce();
        seconds = (double)ros::Time::now().toSec() - startSeconds;

        //ROS_INFO("[%f]", seconds);

        // if(seconds>5 && seconds<1 &0& !inAir){
        // 	read_pub.publish(myMsg);
        // 	inAir = true;
        // }

        // if(seconds>10 && inAir){
        // 	read_pub2.publish(myMsg);
        // 	inAir = false;
        // }

        if(takeoff> 0.5){
        	read_pub.publish(myMsg);
        }
		
        if(land> 0.5){
        	read_pub2.publish(myMsg);
        }

        cmdMsg.linear.x = pitch;
        cmdMsg.linear.y = roll;
        cmdMsg.linear.z = throttle;

        cmdMsg.angular.z = yaw;

        read_pub3.publish(cmdMsg);


        
        loop_rate.sleep();

    }


    return 0;
}

