#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <turtlesim/Pose.h>

ros::NodeHandle nh;


float linear = 0;
float angular = 0;
int mov = 0;

// l298n pins
int in1 = 10;
int in2 = 11;
int in3 = 12;
int in4 = 9;

// high low (unused)
int h = 255;
int l = 100 ;

float lp = 0;
float rp = 0;

float lwheel = 0;
float rwheel = 0;

void poseCb(const geometry_msgs::Twist& twist){

  linear = twist.linear.x;
  angular = twist.angular.z;

  // individual wheel velocities
  // R = 2V - wL/2R
  // L = 2V + wL/2R
  rwheel = (2*linear) - (angular*30)/(2*5);
  lwheel = (2*linear) + (angular*30)/(2*5);

  rwheel = ceil(rwheel*1000)/1000;
  lwheel = ceil(lwheel*1000)/1000;

  // pwm value for left and right wheels
  rp = abs(round(rwheel*250/0.44));
  lp = abs(round(lwheel*250/0.44));


  // forward, inplace R,L, forward R,L
  if(linear >= 0 && angular >= 0){
    mov = 1;
  }

  //reverse, back right, back left
  else if(linear <= 0 && angular <= 0){
    mov = 2;
  }
  else if(linear <= 0 && angular >= 0){
    mov = 3;
  }
  else if(linear >= 0 && angular <= 0){
    mov = 4;
  }

  switch(mov){

    // forward,L,R & inplace L,R
    case 1:
      analogWrite(in1,lp);
      analogWrite(in3,rp);
      
      analogWrite(in2,0);
      analogWrite(in4,0);
      break;

    // reverse L,R
    case 2:
      analogWrite(in1,0);
      analogWrite(in3,0);
      
      analogWrite(in2,lp);
      analogWrite(in4,rp);
      break;

    // default stop
    default:
      analogWrite(in1,0);
      analogWrite(in3,0);

      analogWrite(in2,0);
      analogWrite(in4,0);

      }
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
