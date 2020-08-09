#include <iostream>
#include <cstdlib>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

float distance;

class Robot{
protected:

    ros::NodeHandle nh;

    ros::Publisher cmd_vel;
    ros::Subscriber sub;

    geometry_msgs::Twist msg;

    sensor_msgs::LaserScan scaned;

    

public:
    Robot(){
    
        sub = nh.subscribe("base_scan", 1000, &Robot::Callback, this );
        cmd_vel =  nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

    }

    ~Robot(void)
    {
    }

    void Callback(const sensor_msgs::LaserScanConstPtr& scan){

        /*scaned.header = scan ->header;
        scaned.angle_min = scan ->angle_min;
        scaned.angle_max = scan ->angle_max;
        scaned.angle_increment = scan ->angle_increment;
        scaned.time_increment = scan ->time_increment;
        scaned.scan_time = scan ->scan_time;
        scaned.range_min = scan ->range_min;
        scaned.range_max = scan ->range_max;
        scaned.ranges = scan ->ranges;
        scaned.intensities = scan ->intensities;*/

        distance = *std::min_element(scan->ranges.begin(), scan->ranges.end());

        if (distance > 0.2){
            cmd_vel.publish(msg);
        }
    }

    void run(){

        msg.linear.x = 1.0;        
        msg.angular.z = 0.0; 

        ROS_INFO("Min distance object %f", distance);

    }

};



int main(int argc, char **argv){

    ros::init(argc, argv, "basic_control");
    ros::NodeHandle n;

    Robot rob1;

    ros::Duration(5.0).sleep();


    ros::Rate loop_rate(5);
    while (ros::ok())
    {   

        rob1.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}