#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "error_pose");

  ros::NodeHandle n;

  tf::TransformListener listener;

  ros::Rate rate(10.0);
  while (n.ok()) {
  		tf::StampedTransform window;

	    try {
	      listener.lookupTransform("/ardrone_base_link", "/window",  
	       ros::Time(0), window);
	      detection=detection+1;
	      // Get the x,y,z and rpy of the transform
	      x1 = window.getOrigin().x();
	      y1 = window.getOrigin().y();
	      z1 = window.getOrigin().z();

	      std::cout << "x: " << x1 << std::endl;
	      std::cout << "y: " << y1 << std::endl;
	      std::cout << "z: " << z1 << std::endl;
	      
	      tf::Quaternion q1 = transform1.getRotation();
	      tf::Matrix3x3 m1(q1);
	      m1.getRPY(roll1, pitch1, yaw1);
	    }
	    catch (tf::TransformException ex){
	      ROS_ERROR("%s",ex.what());
	      ros::Duration(1.0).sleep();
	    }
  }

