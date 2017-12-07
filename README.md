# Quadrotor project - Going through a window

* First terminal:

`roslaunch drone_application launch_drone.launch`


* Second terminal:

`roslaunch drone_application visual2.launch`

* Third terminal:

`rosrun drone_application listener`

This will publish to Twist messages to topic /error that can be taken by the PID controller.
