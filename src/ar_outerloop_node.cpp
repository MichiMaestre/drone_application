#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "nav_msgs/Path.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/Joy.h"
#include <iostream>

//double seconds;
//double startSeconds;

/*
* XBox Controller Variables
*/
bool inAir = false;
double roll = 0.0;
double pitch = 0.0;
double yaw = 0.0;
double throttle = 0.0;
double takeoff = 0.0;
double land = 0.0;
double autonomous_switch = 0.0;

/*
 * PD controller tuning constants
 */
double kp1 = 0.08;
double kp2 = 0.008;
double kp3 = 0.03;
double kp_yaw = 0.03;

double kd1 = 0.05;
double kd2 = 0.005;
double kd3 = 0.005;
double kd_yaw = 0.01;

double ep1 = 0;
double ep2 = 0;
double ep3 = 0;
double eyaw = 0;

double ep1_prev = 0;
double ep2_prev = 0;
double ep3_prev = 0;
double eyaw_prev = 0;

double ed1 = 0;
double ed2 = 0;
double ed3 = 0;
double edyaw = 0;

double ades1 = 0;
double ades2 = 0;
double ades3 = 0;
double ades_yaw = 0;

/*
 * Navigation target, not used as errors are reported at the moment
 */
double x_target = 2.5;
double y_target = 0.0;
double z_target = 0.0;

double seconds = 0;
double seconds_prev = 0;
double dt = 0;

bool autonomous = false;



void subCallback(const sensor_msgs::Joy& msg)
{
	
	roll = msg.axes[3];
	pitch = msg.axes[4];
	yaw = msg.axes[0];
	throttle = msg.axes[1];
	takeoff = msg.buttons[0];
	land = msg.buttons[1];
	autonomous_switch = msg.buttons[2];



	//ROS_INFO("x: %f y: %f z: %f t: %f takeoff: %f land: %f", pitch, roll,yaw,throttle, takeoff, land);


}

void subCallback2(const geometry_msgs::Twist& msg)
{
	ep1 = msg.linear.x - x_target;
	//ep2 = msg.linear.y;
	ep3 = msg.linear.z;
	eyaw = msg.angular.z;
	
}

void subCallback3(const std_msgs::Float64& msg)
{
	ep2 = msg.data;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "ar_outerloop");
	ros::NodeHandle n;

	seconds_prev = (double)ros::Time::now().toSec();

	
	ros::Subscriber sub = n.subscribe("joy", 1000, subCallback);
	ros::Subscriber sub2 = n.subscribe("error", 1000, subCallback2);
	ros::Subscriber sub3 = n.subscribe("huehue", 1000, subCallback3);
	ros::Publisher read_pub = n.advertise<std_msgs::Empty>("ardrone/takeoff", 1000);
	ros::Publisher read_pub2 = n.advertise<std_msgs::Empty>("ardrone/land", 1000);
	ros::Publisher read_pub3 = n.advertise<geometry_msgs::Twist>("cmd_vel", 30);
	std_msgs::Empty myMsg;
	geometry_msgs::Twist cmdMsg;
	ros::Rate loop_rate(30);
	
	//setup time
	seconds_prev = (double)ros::Time::now().toSec();
	
	while(ros::ok()){
		ros::spinOnce();
		seconds = (double)ros::Time::now().toSec();
		dt = seconds - seconds_prev;
		
		//send takeoff and land commands
		if(takeoff> 0.5){
			read_pub.publish(myMsg);
		}
		
		if(land> 0.5){
			read_pub2.publish(myMsg);
		}
		if(autonomous_switch>0.5){
			autonomous = true;
		}else{
			autonomous = false;
		}
		//ROS_INFO("autonomous_switch %f", autonomous_switch);
		
		
		if(dt!=0) {
			
			ed1 = (ep1-ep1_prev)/dt;
			ed2 = (ep2-ep2_prev)/dt;
			ed3 = (ep3-ep3_prev)/dt;
			edyaw = (eyaw - eyaw_prev)/dt;
			
			ep1_prev = ep1;
			ep2_prev = ep2;
			ep3_prev = ep3;
			eyaw_prev = eyaw;
			
			ades1 = kp1*ep1 + kd1*ed1;  
			ades2 = kp2*ep2 + kd2*ed2;
			ades3 = kp3*ep3 + kd3*ed3;
			ades_yaw = kp_yaw*eyaw + kd_yaw*edyaw;

			seconds_prev = seconds;


		}


		if(!autonomous){ 
			//send xbox commands
			//ROS_INFO_STREAM("NOPE");
			cmdMsg.linear.x = pitch;
			cmdMsg.linear.y = roll;
			cmdMsg.linear.z = throttle;
			cmdMsg.angular.z = yaw;
			read_pub3.publish(cmdMsg);
			ROS_INFO("NApitch: %f roll: %f throttle: %f yaw: %f",ades1,ades2,ades3,ades_yaw);


		}else{
			cmdMsg.linear.x = pitch;
			cmdMsg.linear.y = roll;
			cmdMsg.linear.z = throttle;
			// cmdMsg.linear.x = ades1;
			// cmdMsg.linear.y = ades2;
			// cmdMsg.linear.z = ades3;
			cmdMsg.angular.z = ades_yaw;
			read_pub3.publish(cmdMsg);
			ROS_INFO("Apitch: %f roll: %f throttle: %f yaw: %f",ades1,ades2,ades3,ades_yaw);

			// cmdMsg.linear.x = -ades3;
			// cmdMsg.linear.y = -ades1;
			// cmdMsg.linear.z = ades2;
			// cmdMsg.angular.z = -ades_yaw;
			//read_pub3.publish(cmdMsg);
		}
		
		loop_rate.sleep();

	}


	return 0;
}