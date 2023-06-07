#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <turtlesim/Pose.h>

ros::NodeHandle nh;


float linear = 0;
float angular = 0;
int led = 0;

int b = 13;
int g = 12;
int y = 11;
int r = 10;

void poseCb(const geometry_msgs::Twist& twist){
  
//  x = pose_msg.x;
//  y = pose_msg.y;
//  theta = pose_msg.theta;
//  linear = pose_msg.linear_velocity;
//  angular = pose_msg.angular_velocity;

  linear = twist.linear.x;
  angular = twist.angular.z;
  
  if(linear > 0 && angular == 0){
    led = 1;
  }
  else if(linear < 0 && angular == 0){
    led = 2;
  }
  else if(linear == 0 && angular > 0){
    led = 3;
  }
  else if(linear == 0 && angular < 0){
    led = 4;
  }
  else if(linear > 0 && angular < 0){
    led = 5;
  }
  else if(linear < 0 && angular > 0){
    led = 6;
  }
  else if(linear == 0 && angular == 0){
    led = 7;
  }

  switch(led){
    case 1:
      digitalWrite(b,HIGH);
      digitalWrite(g,LOW);
      
      digitalWrite(y,LOW);
      digitalWrite(r,LOW);
      break;
           
    case 2:
      digitalWrite(b,LOW);
      digitalWrite(g,HIGH);
      
      digitalWrite(y,LOW);
      digitalWrite(r,LOW);
      break;

    case 3:
      digitalWrite(b,LOW);
      digitalWrite(g,LOW);
      
      digitalWrite(y,HIGH);
      digitalWrite(r,LOW);
      break;

    case 4:
      digitalWrite(b,LOW);
      digitalWrite(g,LOW);
      
      digitalWrite(y,LOW);
      digitalWrite(r,HIGH);
      break;
      
    case 5:
      digitalWrite(b,HIGH);
      digitalWrite(g,LOW);
      
      digitalWrite(y,LOW);
      digitalWrite(r,HIGH);
      break;
      
    case 6:
      digitalWrite(b,LOW);
      digitalWrite(g,HIGH);
      
      digitalWrite(y,HIGH);
      digitalWrite(r,LOW);
      break;
      
    case 7:
      digitalWrite(b,LOW);
      digitalWrite(g,LOW);
      
      digitalWrite(y,LOW);
      digitalWrite(r,LOW);
      break;
  }
} 

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", poseCb);


void setup() {
  // put your setup code here, to run once:
  pinMode(b,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(y,OUTPUT);
  pinMode(r,OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
//  Serial.println("linear = ");
//  Serial.print(linear);
//  Serial.println("angular = ");
//  Serial.print(angular);
  delay(50);
}
