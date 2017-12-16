#include <ros/ros.h>
#include "ros/console.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float64.h"
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
  ros::Publisher pub2 = n.advertise<std_msgs::Float64>("huehue", 1);
  std_msgs::Bool mymsg;
  std_msgs::Float64 msg_ab;

  int detection = 0; int c1 =0 ;  int c2=0; int c3=0; int c4=0;
  double x1 = 0; double x2 = 0; double x3 = 0; double x4 = 0;
  double y1 = 0; double y2 = 0; double y3 = 0; double y4 = 0;
  double z1 = 0; double z2 = 0; double z3 = 0; double z4 = 0;
  double roll1 = 0; double pitch1 = 0; double yaw1 = 0;
  double roll2 = 0; double pitch2 = 0; double yaw2 = 0;
  double roll3 = 0; double pitch3 = 0; double yaw3 = 0;
  double roll4 = 0; double pitch4 = 0; double yaw4 = 0;
  double rollf = 0; double pitchf = 0; double yawf = 0;
  double X1 =0; double Y1 =0; double Z1=0;
  double X2 =0; double Y2 =0; double Z2=0;
  double X3 =0; double Y3 =0; double Z3=0;
  double X4 =0; double Y4 =0; double Z4=0;
  
  double life = 0.5;
  double x1_life = life;
  double x2_life = life;
  double x3_life = life;
  double x4_life = life;


  double seconds = 0;
  double seconds_prev = 0;
  double dt = 0;

  ros::Rate rate(60.0);
  while (n.ok()) {
    geometry_msgs::Twist error;

      // First tag
    tf::StampedTransform transform1;
    seconds = (double)ros::Time::now().toSec();
    dt = seconds - seconds_prev;



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
      c1=1;
      x1_life = life;
    }
    catch (tf::TransformException ex){
      c1= 0;
      //ROS_ERROR("%s",ex.what());
      // ros::Duration(1.0).sleep();
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
      c2=1;
      x2_life = life;
    }
    catch (tf::TransformException ex2){
      c2=0;
      //ROS_ERROR("%s",ex2.what());
      // ros::Duration(1.0).sleep();
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
      c3=1;
      x3_life = life;
    }
    catch (tf::TransformException ex3){
      c3=0;
      //ROS_ERROR("%s",ex3.what());
      // ros::Duration(1.0).sleep();
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
      c4=1;
      x4_life=life;
    }
    catch (tf::TransformException ex4){
      c4=0;
      //ROS_ERROR("%s",ex4.what());
      // ros::Duration(1.0).sleep();
    }


    x1_life -= dt;
    x2_life -= dt;
    x3_life -= dt;
    x4_life -= dt;
    ROS_INFO("dt: %f", dt);

    if(x1_life<=0){
      x1 =0;
      y1 =0;
      z1 =0;
      c1 =0;
      x1_life = life;
    }
    if(x2_life<=0){
      x2 =0;
      y2 =0;
      z2 =0;
      c2 =0;
      x2_life = life;
    }
    if(x3_life<=0){
      x3 =0;
      y3 =0;
      z3 =0;
      c3 =0;
      x3_life = life;
    }
    if(x4_life<=0){
      x4 =0;
      y4 =0;
      z4 =0;
      c4 =0;
      x4_life = life;
    }
        // Get the mean of all the position values
    transform_center.setOrigin(tf::Vector3(0,0,0));
    tf::Quaternion q;
    q.setRPY(0, 0, 0);
    transform_center.setRotation(q);

    if (c1==1){
      X1 = x1 ;
      Y1 = y1 -.45;
      Z1 = z1 -.33;
    } else {
      X1 = 0; 
      Y1 = 0;
      Z1 = 0;
      roll1 = 0;
      pitch1= 0;
      yaw1  = 0;
    }
    if (c2==1){
      X2 = x2 ;
      Y2 = y2 +.45;
      Z2 = z2 -.33;
    } else {
      X2 = 0; 
      Y2 = 0;
      Z2 = 0;
      roll2 = 0;
      pitch2= 0;
      yaw2  = 0;
    }
    if (c3==1){
      X3 = x3 ;
      Y3 = y3 +.45;
      Z3 = z3 +.33 ;
    } else {
      X3 = 0; 
      Y3 = 0;
      Z3 = 0;
      roll3 = 0;
      pitch3= 0;
      yaw3  = 0;
    }
    if (c4==1){
      X4 = x4 ;
      Y4 = y4 - .45;
      Z4 = z4 + .33;
    } else {
      X4 = 0; 
      Y4 = 0;
      Z4 = 0;
      roll4 = 0;
      pitch4= 0;
      yaw4  = 0;
    }


    // if(detection == 4){
    //  mymsg.data = true;
    //  pub1.publish(mymsg);
    //std::cout << "Window Center" << std::endl; 
    int avg =c1+c2+c3+c4;
    double X = (X1 + X2 + X3 + X4) / avg;
    //std::cout << "X: " << X << std::endl;

    double Y = (Y1 + Y2 + Y3 + Y4) / avg;
    //std::cout << "Y: " << Y << std::endl;

    double Z = (Z1 + Z2 + Z3 + Z4) / avg;
    //std::cout << "Z: " << Z << std::endl;

    double roll = (roll1 + roll2 + roll3 + roll4) / avg;
    //std::cout << "Roll: " << roll << std::endl;

    double pitch = (pitch1 + pitch2 + pitch3 + pitch4) / avg;
    //std::cout << "Pitch: " << pitch << std::endl;

    double yaw = (yaw1 + yaw2 + yaw3 + yaw4) / avg;
    
    //std::cout << "Yaw: " << yaw << std::endl;

    //std::cout << "  " << std::endl; 

    // MODULE DISTANCE CONDITION
    double A = (x4*x4) + (y4*y4);
    //std::cout << "A: " << A << std::endl;

    double B = (x3*x3) + (y3*y3);
    //std::cout << "B: " << B << std::endl;

    double AB = B - A;
    //std::cout << "AB: " << AB << std::endl;
    
    msg_ab.data = AB;
    pub2.publish(msg_ab);

    // if (AB <= 0.3 && AB >= -0.3) {
    //   ROS_INFO_STREAM("Centered");
    // } else { 
    //   if (AB > 0.3)
    //     ROS_INFO_STREAM("Move right");
    //   if (AB < -0.3)
    //     ROS_INFO_STREAM("Move left");
    // }

    //std::cout << "  " << std::endl;

        // Broadcaster
    transform_center.setOrigin(tf::Vector3(X, Y, Z));
    q.setRPY(roll, pitch, yaw);
    transform_center.setRotation(q);
    // previous_transform = transform_center;
    // }

    // if(detection < 4){
    //   transform_center = previous_transform;
    // }

    // std::cout << "Window Center" << std::endl; 
    // std::cout << "X: " << transform_center.getOrigin().x() << std::endl;
    // std::cout << "Y: " << transform_center.getOrigin().y() << std::endl;
    // std::cout << "Z: " << transform_center.getOrigin().z() << std::endl;
    // std::cout << "  " << std::endl; 


    // Send transform to visualize
    br.sendTransform(tf::StampedTransform(transform_center, ros::Time::now(), "/ardrone_base_link", "window"));

    tf::Quaternion q_final = transform_center.getRotation();
    tf::Matrix3x3 m_final(q_final);
    m_final.getRPY(rollf, pitchf, yawf);
    // yaw = (yaw/(3.14159265))*180.0;

    // Publish error between window frame and base link
    error.linear.x = transform_center.getOrigin().x();
    error.linear.y = transform_center.getOrigin().y();
    error.linear.z = transform_center.getOrigin().z();
    yawf = atan2(transform_center.getOrigin().x(), transform_center.getOrigin().y());
    error.angular.x = rollf;
    error.angular.y = pitchf;
    yawf = (yawf/(3.14159265))*180.0 - 90.0;
    //ROS_INFO("yaw: %f", yawf);
    error.angular.z = yawf;

    //ROS_INFO("roll: %f pitch: %f yaw: %f", rollf, pitchf, yawf);
    pub.publish(error);

    detection = 0;

    seconds_prev = seconds;

    ros::spinOnce();
    rate.sleep();
  }

}