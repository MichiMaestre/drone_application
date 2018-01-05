# Quadrotor project - Going through a window

First connect to the drone wifi, then follow the next steps:

* First terminal:

`roslaunch drone_application launch_drone.launch`

This will start the drone and will open Rviz. Add TF and a camera for visualization with topic ardrone/frontcam/image_raw. If the tags are visible, they will be seen in the visualization.
It also runs the ar_track_alvar package afor AR tag detection and the PID controller node, which will wait for the listener node (explained below) to run.

* Second terminal:

`rosrun drone_application listener`

This will broadcast a transform with the window center which will be the goal target for the PID controller. There are a large number of PID variables for each direction and hence required a lot of tuning.


Successful Demo:

<a href="https://imgflip.com/gif/2177ph"><img src="https://i.imgflip.com/2177ph.gif" title="made at imgflip.com"/></a>