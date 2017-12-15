#include <ros/ros.h>
#include "ros/console.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include <iostream>

int main(int argc, char** argv){
  ros::init(argc, argv, "frame");

  ros::NodeHandle n;

  tf::TransformListener listener;
  static tf::TransformBroadcaster br;
  tf::Transform transform_center;
  tf::Transform previous_transform;

  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("error", 1000);
  ros::Publisher pub1 = n.advertise<std_msgs::Bool>("detect_flag", 1);
  std_msgs::Bool mymsg;

  int detection = 0;
  double x1 = 0; double x2 = 0; double x3 = 0; double x4 = 0;
  double y1 = 0; double y2 = 0; double y3 = 0; double y4 = 0;
  double z1 = 0; double z2 = 0; double z3 = 0; double z4 = 0;
  double roll1 = 0; double pitch1 = 0; double yaw1 = 0;
  double roll2 = 0; double pitch2 = 0; double yaw2 = 0;
  double roll3 = 0; double pitch3 = 0; double yaw3 = 0;
  double roll4 = 0; double pitch4 = 0; double yaw4 = 0;
  double rollf = 0; double pitchf = 0; double yawf = 0;


  ros::Rate rate(10.0);
  while (n.ok()) {
    geometry_msgs::Twist error;

      // First tag
    tf::StampedTransform transform1;

    try {
      listener.lookupTransform("/ardrone_base_link", "/ar_marker_0",  
       ros::Time(0), transform1);
      detection=detection+1;
      // Get the x,y,z and rpy of the transform
      x1 = transform1.getOrigin().x();
      y1 = transform1.getOrigin().y();
      z1 = transform1.getOrigin().z();
      tf::Quaternion q1 = transform1.getRotation();
      tf::Matrix3x3 m1(q1);
      m1.getRPY(roll1, pitch1, yaw1);
    }
    catch (tf::TransformException ex){
      //ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

        // Second tag
    tf::StampedTransform transform2;

    try {
      listener.lookupTransform("/ardrone_base_link", "/ar_marker_1",  
       ros::Time(0), transform2);
      detection=detection+1;
            // Get the x,y,z and rpy of the transform
      x2 = transform2.getOrigin().x();
      y2 = transform2.getOrigin().y();
      z2 = transform2.getOrigin().z();
      tf::Quaternion q2 = transform2.getRotation();
      tf::Matrix3x3 m2(q2);
      m2.getRPY(roll2, pitch2, yaw2);
    }
    catch (tf::TransformException ex2){
      //ROS_ERROR("%s",ex2.what());
      ros::Duration(1.0).sleep();
    }

        // Third tag
    tf::StampedTransform transform3;

    try {
      listener.lookupTransform("/ardrone_base_link", "/ar_marker_2",  
       ros::Time(0), transform3);
      detection=detection+1;
            // Get the x,y,z and rpy of the transform
      x3 = transform3.getOrigin().x();
      y3 = transform3.getOrigin().y();
      z3 = transform3.getOrigin().z();
      tf::Quaternion q3 = transform3.getRotation();
      tf::Matrix3x3 m3(q3);
      m3.getRPY(roll3, pitch3, yaw3);
    }
    catch (tf::TransformException ex3){
      //ROS_ERROR("%s",ex3.what());
      ros::Duration(1.0).sleep();
    }



        // std::cout << "X3: " << x3 << std::endl;

        // Fourth tag
    tf::StampedTransform transform4;

    try {
      listener.lookupTransform("/ardrone_base_link", "/ar_marker_3",  
       ros::Time(0), transform4);
      detection=detection+1;
            // Get the x,y,z and rpy of the transform
      x4 = transform4.getOrigin().x();
      y4 = transform4.getOrigin().y();
      z4 = transform4.getOrigin().z();
      tf::Quaternion q4 = transform4.getRotation();
      tf::Matrix3x3 m4(q4);
      m4.getRPY(roll4, pitch4, yaw4);
    }
    catch (tf::TransformException ex4){
      //ROS_ERROR("%s",ex4.what());
      ros::Duration(1.0).sleep();
    }

        // Get the mean of all the position values
    transform_center.setOrigin(tf::Vector3(0,0,0));
    tf::Quaternion q;
    q.setRPY(0, 0, 0);
    transform_center.setRotation(q);


    if(detection == 4){
      mymsg.data = true;
      pub1.publish(mymsg);
    //std::cout << "Window Center" << std::endl; 

    double X = (x1 + x2 + x3 + x4) / 4;
    //std::cout << "X: " << X << std::endl;

    double Y = (y1 + y2 + y3 + y4) / 4;
    //std::cout << "Y: " << Y << std::endl;

    double Z = (z1 + z2 + z3 + z4) / 4;
    //std::cout << "Z: " << Z << std::endl;

    double roll = (roll1 + roll2 + roll3 + roll4) / 4;
    //std::cout << "Roll: " << roll << std::endl;

    double pitch = (pitch1 + pitch2 + pitch3 + pitch4) / 4;
    //std::cout << "Pitch: " << pitch << std::endl;

    double yaw = (yaw1 + yaw2 + yaw3 + yaw4) / 4;
    
    //std::cout << "Yaw: " << yaw << std::endl;

    //std::cout << "  " << std::endl; 


        // Broadcaster
    transform_center.setOrigin(tf::Vector3(X, Y, Z));
    tf::Quaternion q;
    q.setRPY(roll, pitch, yaw);
    transform_center.setRotation(q);
    previous_transform = transform_center;
    }

    if(detection < 4){
      transform_center = previous_transform;
    }

    std::cout << "Window Center" << std::endl; 
    std::cout << "X: " << transform_center.getOrigin().x() << std::endl;
    std::cout << "Y: " << transform_center.getOrigin().y() << std::endl;
    std::cout << "Z: " << transform_center.getOrigin().z() << std::endl;
    std::cout << "  " << std::endl; 


    // Send transform to visualize
    br.sendTransform(tf::StampedTransform(transform_center, ros::Time::now(), "/ardrone_base_link", "window"));

    tf::Quaternion q_final = transform_center.getRotation();
    tf::Matrix3x3 m_final(q_final);
    m_final.getRPY(rollf, pitchf, yawf);
    yawf = (yawf/(3.14159265))*180.0;

    // Publish error between window frame and base link
    error.linear.x = transform_center.getOrigin().x();
    error.linear.y = transform_center.getOrigin().y();
    error.linear.z = transform_center.getOrigin().z();
    error.angular.x = rollf;
    error.angular.y = pitchf;
    error.angular.z = yawf;
    pub.publish(error);


        // THINK WHAT TO DO WHEN A MARKER IS NOT BEING DETECTED
        // FIRST GET WINDOW SIZE, ANY TIME THE 4 MARKERS GET DETECTED, DETECT AGAIN TO IMPROVE POSE
        // SAVE FRAME SO THAT WHEN GOING BLIND IT KEEPS GOING TOWARDS THAT POSE
    detection = 0;

    ros::spinOnce();
    rate.sleep();
  }

}