#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int16.h>


// IN1 IN2 Wheeled 
// Pin Definitions
const int leftEn = 9;           // Purple
const int rightEn = 10;          // Red
const int leftBackward = 5;      // Blue
const int leftForward = 6;       // Green IN1
const int rightForward = 8;     // Yellow
const int rightBackward = 7;    // Orange
#define ENC_IN_LEFT_A 3
#define ENC_IN_RIGHT_A 2

#define ENC_IN_LEFT_B 11
#define ENC_IN_RIGHT_B 4

// True = Forward; False = Reverse
boolean Direction_left = true;
boolean Direction_right = true;
 
// Minumum and maximum values for 16-bit integers
const int encoder_minimum = -16192;
const int encoder_maximum = 16192;

int pwm_max = 200;
int pwm_ctrl = 0;
int pwm_min = 180;
int high_pwm = 200;
int low_pwm = 180;
int mapped_speed_forward;
int mapped_speed_backward;
int mapped_speed_right;
int mapped_speed_left;

ros::NodeHandle nh;

// 100ms interval for measurements
const int interval = 100;
long previousMillis = 0;
long currentMillis = 0;


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
  } else if (linear > 0.0 && angular < 0.0) {
    forwardright();
  } else if (linear > 0.0 && angular > 0.0) {
    forwardleft();
  } else if (linear < 0.0 && angular < 0.0) {
    backwardright();
  } else if (linear < 0.0 && angular > 0.0) {
    backwardleft();
  } else {
    stop();
  }
}

ros::Subscriber<geometry_msgs::Twist> cmdVelSub("/cmd_vel", &cmdVelCallback);

std_msgs::Int16 right_wheel_tick_count;
ros::Publisher rightPub("right_ticks", &right_wheel_tick_count);
 
std_msgs::Int16 left_wheel_tick_count;
ros::Publisher leftPub("left_ticks", &left_wheel_tick_count);

// Increment the number of ticks
void right_wheel_tick() {
   
  // Read the value for the encoder for the right wheel
  int val = digitalRead(ENC_IN_RIGHT_B);
 
  if(val == LOW) {
    Direction_right = false; // Reverse
  }
  else {
    Direction_right = true; // Forward
  }
   
  if (Direction_right) {
     
    if (right_wheel_tick_count.data == encoder_maximum) {
      right_wheel_tick_count.data = 0;
    }
    else {
      right_wheel_tick_count.data++;  
    }    
  }
  else {
    if (right_wheel_tick_count.data == encoder_minimum) {
      right_wheel_tick_count.data = 0;
    }
    else {
      right_wheel_tick_count.data--;  
    }   
  }
}
 
// Increment the number of ticks
void left_wheel_tick() {
   
  // Read the value for the encoder for the left wheel
  int val = digitalRead(ENC_IN_LEFT_B);
 
  if(val == LOW) {
    Direction_left = true; // Reverse
  }
  else {
    Direction_left = false; // Forward
  }
   
  if (Direction_left) {
    if (left_wheel_tick_count.data == encoder_maximum) {
      left_wheel_tick_count.data = 0;
    }
    else {
      left_wheel_tick_count.data++;  
    }  
  }
  else {
    if (left_wheel_tick_count.data == encoder_minimum) {
      left_wheel_tick_count.data = 0;
    }
    else {
      left_wheel_tick_count.data--;  
    }   
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
  analogWrite(leftEn, mapped_speed_forward-pwm_ctrl);
  analogWrite(rightEn, mapped_speed_forward);
  digitalWrite(leftForward, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightBackward, LOW);
}

void backward() {
  Serial.println("Backward");
  analogWrite(leftEn, mapped_speed_forward-pwm_ctrl);
  analogWrite(rightEn, mapped_speed_backward);
  digitalWrite(leftForward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightBackward, HIGH);
}

void left() {
  Serial.println("Left");
  analogWrite(leftEn, mapped_speed_forward-pwm_ctrl);
  analogWrite(rightEn, mapped_speed_left);
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
}

void right() {
  Serial.println("Right");
  analogWrite(leftEn, mapped_speed_forward-pwm_ctrl);
  analogWrite(rightEn, mapped_speed_right);
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
}

void forwardright() {
  Serial.println("Forward Right");
  analogWrite(leftEn, high_pwm-pwm_ctrl);
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
  analogWrite(leftEn, high_pwm-pwm_ctrl);
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


void setup() {
   
  // Set pin states of the encoder
  pinMode(ENC_IN_LEFT_A , INPUT_PULLUP);
  pinMode(ENC_IN_LEFT_B , INPUT);
  pinMode(ENC_IN_RIGHT_A , INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B , INPUT);
  pinMode(leftEn, OUTPUT);
  pinMode(rightEn, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);
  // Every time the pin goes high, this is a tick
  attachInterrupt(digitalPinToInterrupt(ENC_IN_LEFT_A), left_wheel_tick, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_tick, RISING);
 
  // ROS Setup
  nh.getHardware();
  nh.initNode();
  nh.advertise(rightPub);
  nh.advertise(leftPub);
  nh.subscribe(cmdVelSub);


}

void loop() {
  
  currentMillis = millis();
 
  // If 100ms have passed, print the number of ticks
  if (currentMillis - previousMillis > interval) {
     
    previousMillis = currentMillis;
    rightPub.publish( &right_wheel_tick_count );
    leftPub.publish( &left_wheel_tick_count );
    nh.spinOnce();
  }
  nh.spinOnce();
}
