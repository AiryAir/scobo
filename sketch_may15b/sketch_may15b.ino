#include <ros.h>
#include <geometry_msgs/Twist.h>
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


//void messageCb(const geometry_msgs::Twist& twist){
//  
//    demandx = twist.linear.x;
//    demandz = twist.angular.z;
//    
//    if(demandx >= 0){
//    digitalWrite(13,HIGH);
//    }
//    
//    else {
//    digitalWrite(13,LOW);
//    }
//    Serial.println(demandx);
//
//}

void poseCb(const turtlesim::Pose& pose_msg){

  x = pose_msg.x;
  y = pose_msg.y;
  theta = pose_msg.theta;
  linear = pose_msg.linear_velocity;
  angular = pose_msg.angular_velocity;
  
  if(linear != 0 && angular == 0){
    led = 1;
  }
  else if(linear == 0 && angular !=0){
    led = 2;
  }
  else if(linear == 0 && angular == 0){
    led = 3;
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
      digitalWrite(13,LOW);
      digitalWrite(12,LOW);
      break;
  }
    
  
} 
//ros::Subscriber<geometry_msgs::Twist> sub("/turtle1/cmd_vel", messageCb);
ros::Subscriber<turtlesim::Pose> sub("/turtle1/pose", poseCb);
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}
void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(50);
}
