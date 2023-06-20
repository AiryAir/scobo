#include <ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <Encoder.h>

// ROS Node and Publishers
ros::NodeHandle nh;
nav_msgs::Odometry odom_msg;
ros::Publisher odom_pub("odom", &odom_msg);

// Encoder Pins
#define LEFT_ENCODER_A 2
#define LEFT_ENCODER_B 3
#define RIGHT_ENCODER_A 6
#define RIGHT_ENCODER_B 7

// Encoder Objects
Encoder left_encoder(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder right_encoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

// Constants
const float TICKS_PER_REVOLUTION = 360.0; // Number of ticks per revolution
const float WHEEL_RADIUS = 0.05; // Wheel radius in meters
const float BASE_WIDTH = 0.4; // Distance between the two wheels in meters

// Variables
float x = 0.0; // x position in meters
float y = 0.0; // y position in meters
float theta = 0.0; // orientation in radians

unsigned long prev_time = 0;
float prev_left_ticks = 0.0;
float prev_right_ticks = 0.0;

void setup()
{
  // Initialize ROS
  nh.initNode();
  nh.advertise(odom_pub);
}

void loop()
{
  // Read encoder ticks
  long left_ticks = left_encoder.read();
  long right_ticks = right_encoder.read();

  // Calculate time difference
  unsigned long current_time = millis();
  float dt = (current_time - prev_time) / 1000.0; // convert to seconds

  // Calculate wheel displacements
  float delta_left_ticks = (left_ticks - prev_left_ticks) / TICKS_PER_REVOLUTION;
  float delta_right_ticks = (right_ticks - prev_right_ticks) / TICKS_PER_REVOLUTION;
  float left_distance = 2 * PI * WHEEL_RADIUS * delta_left_ticks;
  float right_distance = 2 * PI * WHEEL_RADIUS * delta_right_ticks;

  // Calculate linear and angular velocities
  float linear_velocity = (left_distance + right_distance) / (2 * dt);
  float angular_velocity = (right_distance - left_distance) / (BASE_WIDTH * dt);

  // Update position and orientation
  float delta_theta = angular_velocity * dt;
  float delta_x = linear_velocity * cos(theta + delta_theta / 2) * dt;
  float delta_y = linear_velocity * sin(theta + delta_theta / 2) * dt;
  x += delta_x;
  y += delta_y;
  theta += delta_theta;

  // Publish odometry message
  odom_msg.header.stamp = nh.now();
  odom_msg.pose.pose.position.x = x;
  odom_msg.pose.pose.position.y = y;
  odom_msg.pose.pose.orientation.z = sin(theta / 2);
  odom_msg.pose.pose.orientation.w = cos(theta / 2);
  odom_pub.publish(&odom_msg);

  // Update previous values
  prev_time = current_time;
  prev_left_ticks = left_ticks;
  prev_right_ticks = right_ticks;

  // Handle ROS communication
  nh.spinOnce();

  // Delay for stability (if needed)
  delay(10);
}
