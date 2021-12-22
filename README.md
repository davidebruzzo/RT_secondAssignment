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
2. [Code explanation](#code)
3. [Flowchart](#flowchart)
4. [Final considerations and conclusions](#issues)

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
$ rosrun rqt_graph rqt_graph
```

# Code explanation<a name="code"></a>
------------------------------------------------
### - *Scheme* 
------------------------------------------------
This is the schema of the project and the interaction between the nodes. ROS can genearte this graph by the command:
```bash
$ rosrun second_assignment user_interface
```
<p align="center">
<img src="https://github.com/davidebruzzo/RT_secondAssignment/blob/main/images/rosgraph.png" width="700" />
<p>


### - *World node/The circuit* 
------------------------------------------------

The first node to launch is the one that shows up the circuit environment, in this case the circuit is the 'Autodromo di Monza', you can have a look at it in the world directory:

<p align="center">
<img src="https://github.com/davidebruzzo/RT_secondAssignment/blob/main/world/tracciato.png" width="500" />
<p>

 This node publishes on the topic ```/base_scan``` the values of the robot's scan, and subscribes to the topic  ```/cmd_vel ```
 
 ### - *Controller node*
 ------------------------------------------------
 
 The controller node takes care of driving the robot around the track, detecting track limits and staying on track. It subscribes to the ```/base_scan``` topic to retreive distances. 
 Distances are stored in an array of 721 elements. 
 Since the robot has a 180 degrees view, by a simple proportion we can divide this array in different sections that represent different angles of view. 
 Every time ```/base_scan``` fill the array of distances, the controller node calls the callback function ```MoveRobotCallback()``` that divides in this way the distances array:
 
 ```c++
  float minFrontDist = ObtainMinDistance(310, 410, arrayToDivide);
	printf("minFrontDist is %f\n", minFrontDist);
	
	//Getting the min right distance
	float minRightDist = ObtainMinDistance(20, 120, arrayToDivide);
	printf("minRightDist is %f\n", minRightDist);
	
	//Getting the min left distance
	float minLeftDist = ObtainMinDistance(610, 710, arrayToDivide);
	printf("minLeftDist is %f\n", minLeftDist);
 ```
 where ```ObtainMinDistance``` is a simple function to obtain the minum value in the range specified of distances.
 Then the logic is very simple, if the robot is near a line of the circuit it has to rotate a bit, this is done as it follows:
 ```c++
  //check where to turn, right or left, or going straight	
	if(minFrontDist < 1.5){	//if we are closer to a wall we have to turn
		
		if(minRightDist < minLeftDist){ //we have to turn left since it is the direction with more space available
			TurnLeft();
			printf("\nTurning left a bit..");			
			
		}
		else{	//otherwise we have to turn right since it is the direction with more space available
			TurnRight();
			printf("\nTurning right a bit..");
		}	
	}
	else{//if we are far by walls we can go straight
		GoStraight();
		printf("\nGoing straight!!");
	}
 ```
 When the robot is turning it slows down in order to turn avoiding walls, and then it can goes straight using the starting value of speed and also an accelaration which has to be added. This is managed by a custom service between user_interface and the controller that implements the server of this service. 
 As last thing, the controller publishes the velocity on ```/cmd_vel ``` topic.
 
 #### - *VelAdd service*

 This service links the user_interface and the controller node in order to send to the controller node the acceleration (+0.5) or the deceleration(-0.5) chosen by the user.
 The user can also choose to stop the robot or reset to the starting position through ```/reset_position``` service.
 
 ### - *User interface node*
 ------------------------------------------------
 
 This node shows a menu in the shell, where there are shown different possibilities for the user:
 
 - ```+``` to increase linear velocity
 - ```-``` to decrease linear velocity
 - ```r/R``` to reset robot position
 - ```s/s``` to stop the robot
 
 If the user types a value of them, it sends through the custom service the user choice. In the user_interface it is created the *client* side of this service.
 
 # Flowchart<a name="flowchart"></a>
 
 <p align="center">
<img src="https://github.com/davidebruzzo/RT_secondAssignment/blob/main/images/sec_ass.drawio.png" width="600" />
<p>

 # Final considerations and conclusions<a name="#issues"></a>
------------------------------------------------
Dealing with ROS was very challenging because I had to learn how processes communicates each other as long as it is a new 'operative system'. 
The simple logic under the robot's behavior helped me also to focus on the interaction between different nodes. 
This doesn't means however that robot capacity of driving and turning can't be improved. In fact a possible improvement, is that the turning can be smoother if we are not so close to the circuit lines.
Another possible improvement can be the file launcher.launch that occupies of launching every node in different consoles.
***As well as we are in a while loop all this procedure is cycled costantly.***
