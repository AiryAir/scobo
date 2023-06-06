#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

ros::NodeHandle nh;

int led = 0;
int red1 = 13;
int red2 = 8;
int blue = 6;
int green = 12;

void poseCb(const turtlesim::Pose& pose_msg){
  
  float x = pose_msg.x;
  float y = pose_msg.y;
  float theta = pose_msg.theta;
  float linear = pose_msg.linear_velocity;
  float angular = pose_msg.angular_velocity;

  if(linear == 0 && angular == 0){
    led = 1;
  }
  else if(linear > 0 && angular == 0){
    led = 2;
  }
  else if(linear < 0 && angular == 0){
    led = 3;
  }
  else if(linear == 0 && angular > 0){
    led = 4;
  }
  else if(linear == 0 && angular < 0){
    led = 5;
  }

  switch(led){
    case 1:
      digitalWrite(red1,LOW);
      digitalWrite(red2,LOW);
      digitalWrite(blue,LOW);
      digitalWrite(green,LOW);
      break;
           
    case 2:
      digitalWrite(red1,LOW);
      digitalWrite(red2,LOW);
      digitalWrite(blue,LOW);
      digitalWrite(green,HIGH);
      break;

    case 3:
      digitalWrite(red1,HIGH);
      digitalWrite(red2,LOW);
      digitalWrite(blue,LOW);
      digitalWrite(green,LOW);
      break;
      
    case 4:
      digitalWrite(red1,LOW);
      digitalWrite(red2,LOW);
      digitalWrite(blue,HIGH);
      digitalWrite(green,LOW);

    case 5:
      digitalWrite(red1,LOW);
      digitalWrite(red2,HIGH);
      digitalWrite(blue,LOW);
      digitalWrite(green,LOW);
  }
    
  
} 

//ros::Subscriber<geometry_msgs::Twist> sub("/turtle1/cmd_vel", messageCb);
ros::Subscriber<turtlesim::Pose> sub("/turtle1/pose", poseCb);


void setup() {
  // put your setup code here, to run once:
  pinMode(red1,OUTPUT);
  pinMode(red2,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(50);
}
