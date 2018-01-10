#include<iostream>
#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<control_speed/velocity_control.h>
#include<sensor_msgs/JointState.h>
#include<trajectory_msgs/JointTrajectory.h>
#include<trajectory_msgs/JointTrajectoryPoint.h>
#include<std_msgs/Bool.h>
#include<math.h>

using namespace std;
control_speed::velocity_control vel_out;

ros::Subscriber Vel;
ros::Subscriber Vel_fb;
ros::Publisher pub;
trajectory_msgs::JointTrajectory j;
trajectory_msgs::JointTrajectoryPoint p1;
double joint_pos_global;
double angle_degree;

 void fb_callback(const sensor_msgs::JointState & msg)
 {

     joint_pos_global = (double)msg.velocity[4];

//    cout<<"fb_vel"<<" "<<(double)msg.position[0]*180/3.14<<" "<<(double)msg.position[1]*180/3.14<<" "<<(double)msg.position[2]*180/3.14<<" "<<(double)msg.position[3]*180/3.14<<" "<<(double)msg.position[4]*180/3.14<<" "<<(double)msg.position[5]*180/3.14<<endl;
//    cout<<"fb_vel"<<" "<<(double)msg.velocity[0]*180/3.14<<" "<<(double)msg.velocity[1]*180/3.14<<" "<<(double)msg.velocity[2]*180/3.14<<" "<<(double)msg.velocity[3]*180/3.14<<" "<<(double)msg.velocity[4]*180/3.14<<" "<<(double)msg.velocity[5]*180/3.14<<endl;
//     cout<<"velocity"<<" "<<joint_pos_global<<endl;
//     angle_degree = joint_pos_global*180/3.14;
 }


void velCallback(const control_speed::velocity_control &msg)
{
//    cout<< "Inside callback"<<endl;

    vel_out.vel0.data=msg.vel0.data;
    vel_out.vel1.data=msg.vel1.data;
    vel_out.vel2.data=msg.vel2.data;
    vel_out.vel3.data=msg.vel3.data;
    vel_out.vel4.data=msg.vel4.data;
    vel_out.vel5.data=msg.vel5.data;

    cout<<"vel"<<vel_out.vel0.data*180/3.14<<" "<<vel_out.vel1.data*180/3.14<<" "<<vel_out.vel2.data*180/3.14<<" "<<vel_out.vel3.data*180/3.14<<" "<<vel_out.vel4.data*180/3.14<<" "<<vel_out.vel5.data*180/3.14<<endl;

    p1.velocities.clear();
    j.points.clear();

//    p1.velocities.push_back((float)vel_out.vel0.data);
//    p1.velocities.push_back((float)vel_out.vel1.data);
//    p1.velocities.push_back((float)vel_out.vel2.data);
//    p1.velocities.push_back((float)vel_out.vel3.data);
//    p1.velocities.push_back((float)vel_out.vel4.data);
//    p1.velocities.push_back((float)vel_out.vel5.data);

    p1.velocities.push_back((float)vel_out.vel0.data);
    p1.velocities.push_back((float)vel_out.vel1.data);
    p1.velocities.push_back((float)vel_out.vel2.data);
    p1.velocities.push_back((float)vel_out.vel3.data);
    p1.velocities.push_back((float)vel_out.vel4.data);
    p1.velocities.push_back((float)vel_out.vel5.data);


    p1.time_from_start = ros::Duration(1);
    j.points.push_back(p1);
    pub.publish(j);

}


int main( int argc, char **argv)
{


    ros::init(argc,argv, "velocity_control");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);

    Vel_fb = nh.subscribe("/joint_states",10,fb_callback);


    pub = nh.advertise<trajectory_msgs::JointTrajectory>("/ur_driver/joint_speed", 10);
    j.header.stamp = ros::Time::now();


    j.joint_names.push_back("shoulder_pan_joint");
    j.joint_names.push_back("shoulder_lift_joint");
    j.joint_names.push_back("elbow_joint");
    j.joint_names.push_back("wrist_1_joint");
    j.joint_names.push_back("wrist_2_joint");
    j.joint_names.push_back("wrist_3_joint");


        Vel = nh.subscribe("/calc_velocity", 100,velCallback);
        ros::spin();

 //       ros::spinOnce();
        loop_rate.sleep();

    return 0;

}


