#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <turtlesim/Pose.h>

ros::NodeHandle nh;


float linear = 0;
float angular = 0;
int led = 0;

int in1 = 10;
int in2 = 11;
int in3 = 12;
int in4 = 9;

int h = 140;
int h1 = 125;
int l = 100;

void poseCb(const geometry_msgs::Twist& twist){

  linear = twist.linear.x;
  angular = twist.angular.z;

  // lp = linear*180/0.22
  // ap = angular*180/2.84

  //forward full
  if(linear > 0 && angular == 0){
    led = 1;
  }
  //reverse full
  else if(linear < 0 && angular == 0){
    led = 2;
  }
  // only right
  else if(linear == 0 && angular > 0){
    led = 3;
  }

  // only left
  else if(linear == 0 && angular < 0){
    led = 4;
  }

  // forward right
  else if(linear > 0 && angular < 0){
    led = 5;
  }

  // forward left
  else if(linear > 0 && angular > 0){
    led = 6;
  }

  //reverse right
  else if(linear < 0 && angular < 0){
    led = 7;
  }

  //reverse left
  else if(linear < 0 && angular > 0){
    led = 8;
  }

  //STOP
  else if(linear == 0 && angular == 0){
    led = 9;
  }

//in1

  switch(led){

    // forward L, R
    case 1:
      analogWrite(in1,h);
      analogWrite(in3,h1);
      
      analogWrite(in2,0);
      analogWrite(in4,0);
      break;

    // reverse L,R
    case 2:
      analogWrite(in1,0);
      analogWrite(in3,0);
      
      analogWrite(in2,h);
      analogWrite(in4,h);
      break;

    // inplace right turn
    case 3:
      analogWrite(in1,0);
      analogWrite(in3,h1);
      
      analogWrite(in2,0);
      analogWrite(in4,0);
      break;

    // inplace left turn
    case 4:
      analogWrite(in1,h);
      analogWrite(in3,0);
      
      analogWrite(in2,0);
      analogWrite(in4,0);
      break;

      
    // forward right
    case 5:
      analogWrite(in1,h);
      analogWrite(in3,l);
      
      analogWrite(in2,0);
      analogWrite(in4,0);
      break;

    // forward left
    case 6:
      analogWrite(in1,l);
      analogWrite(in3,h1);
      
      analogWrite(in2,0);
      analogWrite(in4,0);
      break;

    // reverse right
    case 7:
      analogWrite(in1,0);
      analogWrite(in3,0);
      
      analogWrite(in2,l);
      analogWrite(in4,h);
      break;

   // reverse left
   case 8:
      analogWrite(in1,0);
      analogWrite(in3,0);
      
      analogWrite(in2,h);
      analogWrite(in4,l);
      break;

    // STOP
   case 9:
      analogWrite(in1,0);
      analogWrite(in3,0);
      
      analogWrite(in2,0);
      analogWrite(in4,0);  }
} 

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", poseCb);


void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(50);
}
