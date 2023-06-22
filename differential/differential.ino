#include <ros.h>
#include <geometry_msgs/Twist.h>

// Pin Definitions
const int leftEn = 9;           // Purple
const int rightEn = 10;          // Red
const int leftBackward = 3;      // Blue
const int leftForward = 2;       // Green
const int rightForward = 4;     // Yellow
const int rightBackward = 5;    // Orange

int pwm_max = 255;
int pwm_min = 25;
int high_pwm = 41;
int low_pwm = 32;
int mapped_speed_forward;
int mapped_speed_backward;
int mapped_speed_right;
int mapped_speed_left;
ros::NodeHandle nh;

void cmdVelCallback(const geometry_msgs::Twist& cmdVel) {
  float linear = cmdVel.linear.x;
  float angular = cmdVel.angular.z;

  if (linear == 0.0 && angular == 0.0) {
    stop();
  } else if (linear > 0.0 && angular == 0.0) {
    float scaledlinearforward = (linear - 0) * (pwm_max - pwm_min) / (0.26 - 0) + pwm_min;
    mapped_speed_forward = round(scaledlinearforward);
    forward();
  } else if (linear < 0.0 && angular == 0.0) {
    float scaledlinearbackward = ((linear - (-0.26)) * (pwm_min - pwm_max) / (0 - (-0.26))) + pwm_max;
    mapped_speed_backward = round(scaledlinearbackward);
    backward();
  } else if (linear == 0.0 && angular > 0.0) {
    float scaledangularleft = ((angular * 100) / 1.82) * (pwm_max - pwm_min) / 100 + pwm_min;
    mapped_speed_left = round(scaledangularleft);
    left();
  } else if (linear == 0.0 && angular < 0.0) {
    float scaledangularright = ((angular - (-1.82)) * (pwm_min - pwm_max) / (0 - (-1.82))) + pwm_max;
    mapped_speed_right = round(scaledangularright);
    right();
  } else {
    stop();
  }
}

void stop() {
  Serial.println("Stop");
  analogWrite(leftEn, 0);
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, HIGH);
  analogWrite(rightEn, 0);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, HIGH);
}

void forward() {
  Serial.println("Forward");
  analogWrite(leftEn, mapped_speed_forward);
  analogWrite(rightEn, mapped_speed_forward);
  digitalWrite(leftForward, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightBackward, LOW);
}

void backward() {
  Serial.println("Backward");
  analogWrite(leftEn, mapped_speed_backward);
  analogWrite(rightEn, mapped_speed_backward);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightBackward, HIGH);
}

void left() {
  Serial.println("Left");
  analogWrite(leftEn, mapped_speed_left);
  analogWrite(rightEn, mapped_speed_left);
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
}

void right() {
  Serial.println("Right");
  analogWrite(leftEn, mapped_speed_right);
  analogWrite(rightEn, mapped_speed_right);
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, LOW);
}
/*
void forwardright() {
  Serial.println("Forward Right");
  analogWrite(leftEn, high_pwm);
  analogWrite(rightEn, low_pwm);
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
}

void forwardleft() {
  Serial.println("Forward Left");
  analogWrite(leftEn, low_pwm);
  analogWrite(rightEn, high_pwm);
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
}

void backwardright() {
  Serial.println("Backward Right");
  analogWrite(leftEn, high_pwm);
  analogWrite(rightEn, low_pwm);
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
}

void backwardleft() {
  Serial.println("Backward Left");
  analogWrite(leftEn, low_pwm);
  analogWrite(rightEn, high_pwm);
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
}
else if (linear > 0.0 && angular < 0.0) {
    forwardright();
  } else if (linear > 0.0 && angular > 0.0) {
    forwardleft();
  } else if (linear < 0.0 && angular < 0.0) {
    backwardright();
  } else if (linear < 0.0 && angular > 0.0) {
    backwardleft();
  }
*/
ros::Subscriber<geometry_msgs::Twist> cmdVelSub("/cmd_vel", &cmdVelCallback);

void setup() {
  nh.initNode();
  nh.subscribe(cmdVelSub);

  pinMode(leftEn, OUTPUT);
  pinMode(rightEn, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);
}

void loop() {
  nh.spinOnce();
  delay(10);
}
