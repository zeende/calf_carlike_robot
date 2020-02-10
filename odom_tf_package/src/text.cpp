 #include "ros/ros.h"
 #include <tf/transform_broadcaster.h>
 #include <nav_msgs/Odometry.h>
 #include </home/max/catkin_ws/devel/include/serial_port/header.h>
    
    double vx = 0;
    double vy = 0;
    double vth = 0;


 void messageCallback(const serial_port::header::ConstPtr& msg)
{
    //ROS_INFO("I heard: [%d] [%d] [%d]", msg->num1, msg->num2, msg->num3);
    vx = double(msg->num1)/100;
    vy = double(msg->num2)/100;
    vth = double(msg->num3)/100;
    ROS_INFO("I heard: [%f] [%f] [%f]", vx, vy, vth);

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odom_pub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("serial_data_odom", 5, messageCallback);
    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 1);
    tf::TransformBroadcaster odom_broadcaster;
    ros::Rate r(1.0);//以1Hz的速率发布里程信息，
    while(n.ok())
    {
        ros::spinOnce();
        //odom_pub.publish(odom);  
        r.sleep();            
    }
    

}