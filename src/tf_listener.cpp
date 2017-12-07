#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "tf_listener");

  ros::NodeHandle node;
  
  tf::TransformListener listener;

  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    //Calling try catch for listening from each tag
    // The tag names have to be set properly 
    try{
      listener.lookupTransform("/ardrone_base_frontcam", "/ar_marker_2",  
                               ros::Time(0), transform3);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    try{
      listener.lookupTransform("/ardrone_base_frontcam", "/ar_marker_0",  
                               ros::Time(0), transform1);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    try{
      listener.lookupTransform("/ardrone_base_frontcam", "/ar_marker_1",  
                               ros::Time(0), transform2);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    try{
      listener.lookupTransform("/ardrone_base_frontcam", "/ar_marker_4",  
                               ros::Time(0), transform4);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    //Finding translation by taking the mean directly
    tra = tf::Vector3(((msg->transform1.translation.x+msg->transform2.translation.x+msg->transform3.translation.x+msg->transform4.translation.x)/4), 
        ((msg->transform1.translation.y+msg->transform2.translation.y+msg->transform3.translation.y+msg->transform4.translation.y)/4), 
        ((msg->transform1.translation.z+msg->transform2.translation.z+msg->transform3.translation.z+msg->transform4.translation.z)/4));

    // Converting quaternion to ypr for each tf listened

    tf::Matrix3x3 Rotation;
    Rotation.setRotation(tf::Quaternion(msg->transform1.rotation.x,
    msg->transform1.rotation.y, msg->transform1.rotation.z, 
    msg->transform1.rotation.w));
  
    double roll1, pitch1, yaw1;
    Rotation.getEulerYPR(yaw1, pitch1, roll1); 

    tf::Matrix3x3 Rotation;
    Rotation.setRotation(tf::Quaternion(msg->transform2.rotation.x,
    msg->transform2.rotation.y, msg->transform2.rotation.z, 
    msg->transform2.rotation.w));
  
    double roll2, pitch2, yaw2;
    Rotation.getEulerYPR(yaw2, pitch2, roll2);

    tf::Matrix3x3 Rotation;
    Rotation.setRotation(tf::Quaternion(msg->transform3.rotation.x,
    msg->transform3.rotation.y, msg->transform3.rotation.z, 
    msg->transform3.rotation.w));
  
    double roll3, pitch3, yaw3;
    Rotation.getEulerYPR(yaw3, pitch3, roll3);

    tf::Matrix3x3 Rotation;
    Rotation.setRotation(tf::Quaternion(msg->transform4.rotation.x,
    msg->transform4.rotation.y, msg->transform4.rotation.z, 
    msg->transform4.rotation.w));
  
    double roll1, pitch1, yaw1;
    Rotation.getEulerYPR(yaw4, pitch4, roll4); 

    //Finding mean rotations to set to rotation matrix
    double yaw,roll,pitch;
    yaw= ((yaw1+yaw2+yaw3+yaw4)/4);
    pitch=((pitch1+pitch2+pitch3+pitch4)/4);
    roll=((roll1+roll2+roll3+roll4)/4);
    //Setting new rotation matrix
    tf::Matrix3x3 RotationYPR;
      RotationYPR.setEulerYPR(yaw,pitch,roll);

  //The broadcaster
  static tf::TransformBroadcaster br;
  tf::Transform brtransform;
  brtransform.setOrigin(tra);
  brtransform.setRotation(RotationYPR);
  std::string rect="Window center";
  // Unsure about last argument rect. Need to fix that mostly
  br.sendTransform(tf::StampedTransform(brtransform, ros::Time::now(), "/ardrone_base_frontcam", rect));
  ros::spinOnce();
  rate.sleep();
  }
  return 0;
};
