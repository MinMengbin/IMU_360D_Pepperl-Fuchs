/*
 * IMU_360D.cpp
 *  This cpp file is used for reading linear acceleration (x, y , z),
 *  Angular rate (x, y, z),
 *  Created by Mengbin Min (Mike) based on the communication protocol
 *  from the datasheet of Inertial measurement unit IMU360D-F99-B20-V15
 *  https://www.pepperl-fuchs.com/global/en/classid_6422.htm?view=productdetails&prodid=81885
 *
 *  Oct 5, 2017
 *  Contact: prlatlab@gmail.com
 */

// header files for standard input and output, stardard libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ros/ros.h"// header file for ros

// header file for ros data type
#include "geometry_msgs/Vector3Stamped.h"
#include "std_msgs/Float32.h"  //Unsigned int 32 data type
#include "std_msgs/String.h"
#include "sensor_msgs/Imu.h"

// header file for CAN frame structure and sockaddr structure and socket com
#include <linux/can.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <net/if.h>  //header file for ifreq structure

#define g 9.81 // Acceleration of gravity

int main(int argc, char **argv)
{

    int s; // creat a handler of CAN communication
    int nbytes; // byte numbers of received and sent messages

    //define the structure used for CAN bus
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    // gyro data and accel data
    std_msgs::Float32 gyro_accel;
    sensor_msgs::Imu imu_msg;

    // variables used for converting received data to IMU ros topic
    char data[2] = {0, 0};
    signed int x;
    float y;

    int flag = 1; // flag for debuggin

    //set up ros
	ros::init(argc, argv, "IMU_360D");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<sensor_msgs::Imu>("data", 1000);
	ros::Rate loop_rate(100); // running frequency

	// CAN bus communication initialization.
	ROS_INFO("Trying to create CAN bus connection.");

	s = socket(PF_CAN, SOCK_RAW, CAN_RAW); // creat a handler of CAN communication

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    bind(s, (struct sockaddr *)&addr, sizeof(addr)); //binding s to socket
    ROS_INFO("CAN communication connected.");

    // Read inputs and publish acceleration and gyro data.
    while ( ros::ok()) {
    //while (flag) { // for debuging

          for(int k = 0; k < 8; k++){
				nbytes = read(s, &frame, sizeof(struct can_frame));
				//printf ("nbytes = %d\n", nbytes); // for debug

				if (nbytes < 0) {
						perror("can raw socket read fail");
						return 1;
				}

				if (nbytes < sizeof(struct can_frame)) {
						fprintf(stderr, "read: incomplete CAN frame\n");
						return 1;
				}

				// Rotation Rate data
				if ((frame.can_id & 0x00ffff00) == 0x00ff0100){
					//ROS_INFO("CAN ID = %08x",frame.can_id); //for debug
					// print acceleration data from data[0] to data[7]
					for (int i = 0; i < 3; i++) {
						data[1] = frame.data[i];
						data[0] = frame.data[i+1];
						switch (i){
						case (0):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/100;
								imu_msg.angular_velocity.x = y;
								ROS_INFO ("gyro[x] = %f",y);//for debug
								break;
						case (1):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/100;
								imu_msg.angular_velocity.y = y;
								ROS_INFO ("gyro[y] = %f",y);//for debug
								break;
						case (2):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/100;
								imu_msg.angular_velocity.z = y;
								ROS_INFO ("gyro[z] = %f",y);//for debug
								break;
						}

					}
				}

				// Linear Acceleration data
				if ((frame.can_id & 0x00ffff00) == 0x00ff0400){
					//ROS_INFO("CAN ID = %08x",frame.can_id); //for debug
					// print acceleration data from data[0] to data[7]
					for (int i = 0; i < 3; i++) {
						data[1] = frame.data[i];
						data[0] = frame.data[i+1];
						switch (i){
						case (0):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = (((float)x - 0)/1000) * g;
								imu_msg.linear_acceleration.x = y;
								ROS_INFO ("accel[x] = %f",y);//for debug
								break;
						case (1):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = (((float)x - 0)/1000) * g;
								imu_msg.linear_acceleration.y = y;
								ROS_INFO ("accel[y] = %f",y);//for debug
								break;
						case (2):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = (((float)x - 0)/1000) * g;
								imu_msg.linear_acceleration.z = y;
								ROS_INFO ("accel[z] = %f",y);//for debug
								break;
						}

					}
				}

				// Quaternion data
				if ((frame.can_id & 0x00ffff00) == 0x00ff0600){
					//ROS_INFO("CAN ID = %08x",frame.can_id); //for debug
					// print acceleration data from data[0] to data[7]
					for (int i = 0; i < 4; i++) {
						data[1] = frame.data[i];
						data[0] = frame.data[i+1];
						switch (i){
						case (0):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/1000;
								imu_msg.orientation.x = y;
								ROS_INFO ("Quat[x] = %f",y);//for debug
								break;
						case (1):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/1000;
								imu_msg.orientation.y = y;
								ROS_INFO ("Quat[y] = %f",y);//for debug
								break;
						case (2):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/1000;
								imu_msg.orientation.z = y;
								ROS_INFO ("Quat[z] = %f",y);//for debug
								break;
						case (3):
								//printf ("nbytes[%d] = %d\n",i, frame.data[i]);//for debug
								x = *data;
								y = ((float)x - 0)/1000;
								imu_msg.orientation.w = y;
								ROS_INFO ("Quat[w] = %f",y);//for debug
								break;
						}

					}
				}
          }
		     chatter_pub.publish(imu_msg);
		     flag = 0; // a flag for debugging
		}
	close(s);
	printf ("CAN communication disconnected\n");
    return 0;
}

