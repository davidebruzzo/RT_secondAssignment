Second Assignment - Research track
================================
I'm Davide Bruzzo and this is my solution for the second Reasearch Track I assignment. The request of the assignment was to write a c++ code for achieving this robot’s behaviour:

- constantly drive the robot around the Monza's circuit.
- staying within the track limits.
- interact with the user to increase and decrease speed, stop the robot and reset it to the starting grid.

# Table of contents
****************************
Here down below how it is organized this README.md :

1. [Introduction](#introduction)
2. [My solution](#solution)
3. [Flowchart of the code](#flowchart)
4. [Video demonstration](#video)
5. [Final considerations and conclusions](#issues)

## Introduction <a name="introduction"></a>
-----------------------------

#### *Installing and running* 
-----------------------------
 
The simulator requires a ROS installation on the machine. Once you have installed, you can build the project if it is in your ROS workspace with:

```bash
$ catkin_make
```
In order to launch ROS, type in the shell:
```bash
$ roscore &
```
It's the first thing you have to do! Now You are ready to launch every single node. You should open a new shell for each node you want to launch or at least split the shell you are working on.

In the first you have to run the world node:

```bash
$ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
This command will open the circuit environment.

Then you have to run the controller node:
```bash
$ rosrun second_assignment controller
```
By that command the robot starts driving, staying in the track limits.

If you want to increase or decrease speed, stop the robot or taking back to its grid position you have also to type:
```bash
$ rosrun second_assignment user_interface
```

# My solution <a name="solution"></a>
------------------------------------------------

#### *How I have organized the code* 

I tried to make the flow of code as clear as possibile by dividing it in three main functions :

- moveRobot()
- moveRobotTowardsSilverBlock()
- avoidWalls()

The ```moveRobot()``` function is the one that deals with calling the other two functions. This function is cycled in a while infinite loop. 
First of all, it checks if we are closer to a silver block, so close that there is nothing between robot and the silver block.

If this is the case, ```moveRobot()``` calls ```moveRobotTowardsSilverBlock()```. This is a function to align (by rotating) and drive towards the silver block. If Robot is so close that it is under a certain distance, it can grab the block, by using the Robot class function ```R.grab()```. Then the Robot rotates handling the block and releases it, by using the Robot class function ```R.release()``` on his back. Finally it moves a bit away and turns back to drive in counter-wise direction.  

If Robot is not near a silver block,  ```moveRobot()``` calls ```avoidWalls()``` that is a function that checks if Robot is closer to golden blocks (walls). ```avoidWalls()``` checks if Robot is in an angle and decides if it has to turn right or left, by comparing the distance from golden blocks in the two directions (right and left). The one that is bigger has more space to drive by, so this will be the side that has no wall.  ```avoidWalls()``` turns the Robot until we are far enough from walls.  
At the end, whith the two function called, or equally if there is enough space in front, the Robot can drive straight.  

***As well as we are in a while loop all this procedure is cycled costantly.***
