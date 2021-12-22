#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "second_assignment/VelAdd.h"


//constants declaration
//size_t LengthOfArray = sizeof ranges / sizeof ranges[0];
float arrayToDivide[721];

//publisher declaration
ros::Publisher pub;
geometry_msgs::Twist my_vel;

//variable for adding speed by user's choice
float velAdd = 0.0;

//funtcion to retreive the minimum value from a list of value
float ObtainMinDistance(int min, int max, float arrayToDivide[]){

	float minVal = 500.0;
	
	for(int i = min; i <= max; i++){	
		if(arrayToDivide[i] < minVal)
			minVal = arrayToDivide[i];		
	}
	return minVal;
}

//function to turn left
void TurnLeft(){

	my_vel.linear.x = 0.5;
	my_vel.angular.z = 1.5;
}

//function to turn right
void TurnRight(){

	my_vel.linear.x = 0.5;
	my_vel.angular.z = -1.5;
}

//function to go straightt
void GoStraight(){

	my_vel.linear.x = 1.5 + velAdd;
	my_vel.angular.z = 0.0;
}

//Callback for cheching if we are close to walls and also where to turn
void MoveRobotCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{	
	
	//fill an array with the values from the array laser scan
	for(int i = 0; i <= 720; i++){
		
		arrayToDivide[i] = msg->ranges[i];		
	}	
	//Getting the min frontal distance
	float minFrontDist = ObtainMinDistance(310, 410, arrayToDivide);
	printf("minFrontDist is %f\n", minFrontDist);
	
	//Getting the min right distance
	float minRightDist = ObtainMinDistance(20, 120, arrayToDivide);
	printf("minRightDist is %f\n", minRightDist);
	
	//Getting the min left distance
	float minLeftDist = ObtainMinDistance(610, 710, arrayToDivide);
	printf("minLeftDist is %f\n", minLeftDist);
	
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
	
	//publish speed and rotation on topic cmd_vel	
	float velocity = my_vel.linear.x;
	float rotation = my_vel.angular.z;
	
	pub.publish(my_vel);
	
	//print on bash velocity, linear and angular
	printf("Linear velocity is: %f ;\tAngular velocity is: %f\n", velocity, rotation);
}

//Callback to retreive the value to add to velocity
bool ServerCallback(second_assignment::VelAdd::Request &req, second_assignment::VelAdd::Response &res)
{
	velAdd = velAdd + req.value;
	
	if(req.value == -1 or velAdd < -1.5){
		velAdd = -1.5; //in order to have velocity = 0, so the robot is stopped 
	}
	return true;
}

int main (int argc, char **argv)
{
	// Initialize the node, setup the NodeHandle for handling the communication with the ROS 
	//system  
	ros::init(argc, argv, "robot_controller");  
	ros::NodeHandle nh;
	
	// Define the subscriber to robot's position, in particular to retreive distance from walls  
	ros::Subscriber sub = nh.subscribe("/base_scan", 1, MoveRobotCallback); 
	
	//Define the publisher on cmd_vel	
	pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	
	//Define the service for taking the input speed variation
	ros::ServiceServer serverCc = nh.advertiseService("/veladd", ServerCallback);
	
	ros::spin();
	return 0;
}
