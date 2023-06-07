#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <turtlesim/Pose.h>

ros::NodeHandle nh;


float linear = 0;
float angular = 0;
int led = 0;

int m1a = 10;
int m1b = 11;
int m2a = 12;
int m2b = 9;

void poseCb(const geometry_msgs::Twist& twist){

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
    led = 7;
  }
  else if(linear < 0 && angular > 0){
    led = 7;
  }
  else if(linear == 0 && angular == 0){
    led = 7;
  }

//m1a

  switch(led){
    case 1:
      digitalWrite(m1a,HIGH);
      digitalWrite(m2a,HIGH);
      
      digitalWrite(m1b,LOW);
      digitalWrite(m2b,LOW);
      break;
           
    case 2:
      digitalWrite(m1a,LOW);
      digitalWrite(m2a,LOW);
      
      digitalWrite(m1b,HIGH);
      digitalWrite(m2b,HIGH);
      break;

    case 3:
      digitalWrite(m1a,LOW);
      digitalWrite(m1b,HIGH);
      
      digitalWrite(m2a,HIGH);
      digitalWrite(m2b,LOW);
      break;

    case 4:
      digitalWrite(m1a,HIGH);
      digitalWrite(m1b,LOW);
      
      digitalWrite(m2a,LOW);
      digitalWrite(m2b,HIGH);
      break;
      
    case 5:
      digitalWrite(m1a,LOW);
      digitalWrite(m1b,LOW);
      
      digitalWrite(m2a,LOW);
      digitalWrite(m2b,LOW);
      break;
      
    case 6:
      digitalWrite(m1a,LOW);
      digitalWrite(m1b,LOW);
      
      digitalWrite(m2a,LOW);
      digitalWrite(m2b,LOW);
      break;
      
    case 7:
      digitalWrite(m1a,LOW);
      digitalWrite(m1b,LOW);
      
      digitalWrite(m2a,LOW);
      digitalWrite(m2b,LOW);
      break;
  }
} 

ros::Subscriber<geometry_msgs::Twist> sub("/key_vel", poseCb);


void setup() {
  // put your setup code here, to run once:
  pinMode(m1a, OUTPUT);
  pinMode(m1b, OUTPUT);
  pinMode(m2a, OUTPUT);
  pinMode(m2b, OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(50);
}
