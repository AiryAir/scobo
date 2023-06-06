#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <turtlesim/Pose.h>

ros::NodeHandle nh;

float demandx=0;
float demandz=0;
float x = 0;
float x1 = 0;
float y = 0;
float theta = 0;
float linear = 0;
float angular = 0;
int led = 0;

void poseCb(const geometry_msgs::Twist& twist){
  
//  x = pose_msg.x;
//  y = pose_msg.y;
//  theta = pose_msg.theta;
//  linear = pose_msg.linear_velocity;
//  angular = pose_msg.angular_velocity;

  linear = twist.linear.x;
  angular = twist.angular.z;
  
  if(linear != 0 && angular == 0){
    led = 1;
  }
  else if(linear == 0 && angular !=0){
    led = 2;
  }
  else if(linear != 0 && angular !=0){
    led = 3;
  }
  else if(linear ==0 && angular ==0){
    led = 4;
  }

  switch(led){
    case 1:
      digitalWrite(13,HIGH);
      digitalWrite(12,LOW);
      break;
           
    case 2:
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      break;

    case 3:
      digitalWrite(13,HIGH);
      digitalWrite(12,HIGH);
      break;

    case 4:
      digitalWrite(13,LOW);
      digitalWrite(12,LOW);
      break;
  }
} 

ros::Subscriber<geometry_msgs::Twist> sub("turtle1/cmd_vel", poseCb);


void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(50);
}
