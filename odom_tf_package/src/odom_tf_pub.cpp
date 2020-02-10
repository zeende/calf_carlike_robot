 #include "ros/ros.h"
 #include <tf/transform_broadcaster.h>
 #include <nav_msgs/Odometry.h>
 #include </home/max/catkin_ws/devel/include/serial_port/header.h>

    double x = 0.0;
    double y = 0.0;
    double th = 0.0;
//默认机器人的起始位置是odom参考系下的0点
    double vx = 0;
    double vy = 0;
    double vth = 0;
    double dt = 0.1;


void messageCallback(const serial_port::header::ConstPtr& msg)
{
    //ROS_INFO("I heard: [%d] [%d] [%d]", msg->num1, msg->num2, msg->num3);
    vx = double(msg->num1)/1000;
    vy = double(msg->num2)/100;
    vth = double(msg->num3)/1000;
    ROS_INFO("I heard: [%f] [%f] [%f]", vx, vy, vth);

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "odom_pub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("serial_data_odom", 1, messageCallback);
    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 1);
    tf::TransformBroadcaster odom_broadcaster;

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();


    ros::Rate r(10);//以1Hz的速率发布里程信息，
    while(n.ok())
    {
        ros::spinOnce();               // check for incoming messages
        current_time = ros::Time::now();

        //compute odometry in a typical way given the velocities of the robot
        double dt = (current_time - last_time).toSec();
        double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
        double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
        double delta_th = vth * dt;

        x += delta_x;
        y += delta_y;
        th += delta_th;
        //在这里，我们将根据我们设定的恒定速度更新我们的里程信息。
        
        //since all odometry is 6DOF we'll need a quaternion created from yaw
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
        //我们通常会尝试在我们的系统中使用所有消息的3D版本，以允许2D和3D组件在适当的情况下协同工作，
        //并将消息数量保持在最低限度。因此，有必要将我们的yaw(偏航值)变为四元数。 
        //tf提供的功能允许从yaw(偏航值)容易地创建四元数，并且可以方便地获取四元数的偏航值。
        
        //first, we'll publish the transform over tf
        geometry_msgs::TransformStamped odom_trans;
        odom_trans.header.stamp = current_time;
        odom_trans.header.frame_id = "odom";
        odom_trans.child_frame_id = "base_link";
        /*在这里，我们将创建一个TransformStamped消息，通过tf发送。
        我们要发布在current_time时刻的从"odom"坐标系到“base_link”坐标系的变换。
        因此，我们将相应地设置消息头和child_frame_id，
        确保使用“odom”作为父坐标系，将“base_link”作为子坐标系。*/

        odom_trans.transform.translation.x = x;
        odom_trans.transform.translation.y = y;
        odom_trans.transform.translation.z = 0.0;
        odom_trans.transform.rotation = odom_quat;

        //send the transform
        odom_broadcaster.sendTransform(odom_trans);
//将我们的里程数据中填入变换消息中，然后使用TransformBroadcaster发送变换。

        //next, we'll publish the odometry message over ROS
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id = "odom";
//还需要发布nav_msgs/Odometry消息，以便导航包可以从中获取速度信息。
//将消息的header设置为current_time和“odom”坐标系。
        //set the position
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;

        //set the velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;
        odom.twist.twist.linear.y = vy;
        odom.twist.twist.angular.z = vth;
//这将使用里程数据填充消息，并发送出去。
//我们将消息的child_frame_id设置为“base_link”坐标系，
//因为我们要发送速度信息到这个坐标系。

        //publish the message
        odom_pub.publish(odom);
        last_time = current_time;
        
        r.sleep();
        }
 // return 0;
}
