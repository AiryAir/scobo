//DEPRECATED DO NOT USE

#include <ros.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle nh;

float demandx=0;
float demandz=0;

void messageCb(const geometry_msgs::Twist& twist){
  
    demandx = twist.linear.x;
    demandz = twist.angular.z;
    
    if(demandx >= 0){
    digitalWrite(13,HIGH);
    }
    
    else {
    digitalWrite(13,LOW);
    }

    
}

ros::Subscriber<geometry_msgs::Twist> sub("/turtle1/cmd_vel", messageCb);

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(500);
}
