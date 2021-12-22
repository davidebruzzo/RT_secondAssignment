#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_srvs/Empty.h"
#include "second_assignment/VelAdd.h"

//global variable definition
char input;
float change_vel;

//global empty definition
std_srvs::Empty reset;

//srv definition
second_assignment::VelAdd add;

//client definition
ros::ServiceClient clientUiCc;

void UiCallback(const sensor_msgs::LaserScan::ConstPtr &msg)
{
	//showing the menù message
	printf("You can:\n\tType '+' to increase speed\n\tType '-' to decrease speed\n\tType 's' to stop the robot\n\tType 'r' to reset robot from the starting line of circuit\nInsert: ");
	//taking user input
	scanf(" %c", &input);
	
	switch(input)
	{
		case '+':
			//increase speed
			change_vel = +0.5;
			ROS_INFO("INCREASED SPEED!");
			break;
		case '-':
			//decrease speed
			change_vel = -0.5;
			ROS_INFO("DECREASED SPEED!");
			break;
		
		case 'R':
		case 'r':
			//reset called
			ros::service::call("/reset_positions", reset);
			change_vel = 0;
			ROS_INFO("RESET!");
			break;
			
		case 'S':
		case 's':
			//stop called
			change_vel = -1;
			ROS_INFO("STOP!");
			break;
			
		default:
			//if the user type an incorrect value
			change_vel = 0;
			break;
	}
	
	//filling the field value with the user choice
	add.request.value = change_vel;
	
	//waiting for the client to exists
	clientUiCc.waitForExistence();
	clientUiCc.call(add);
}


int main(int argc, char **argv)
{
	// Initialize the node, setup the NodeHandle for handling the communication with the ROS 
	//system  
	ros::init(argc, argv, "user_interface");  
	ros::NodeHandle n1;
	
	//printf to show only once welcome message
	printf("\t-----Welcome in the user interface!-----\n\n" );
	
	//defining the subscriber, it has to be called every time robot checks distances
	ros::Subscriber sub = n1.subscribe("/base_scan", 1, UiCallback);
	
	//client initialization
	clientUiCc = n1.serviceClient<second_assignment::VelAdd>("/veladd");	
	
	ros::spin();
	return 0;

}
