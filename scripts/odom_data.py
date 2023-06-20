#!/usr/bin/env python

import rospy
from std_msgs.msg import Header
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Quaternion, Twist

# Constants for wheelbase, ticks per revolution, and wheel radius
WHEELBASE = 0.5  # Example value, replace with your actual wheelbase
TICKS_PER_REV = 360  # Example value, replace with your actual ticks per revolution
WHEEL_RADIUS = 0.1  # Example value, replace with your actual wheel radius

# Variables to store previous encoder tick values
prev_left_ticks = 0
prev_right_ticks = 0

# Variables to store current position and orientation
x = 0.0
y = 0.0
theta = 0.0

def encoder_ticks_callback(left_ticks, right_ticks):
    global prev_left_ticks, prev_right_ticks, x, y, theta

    # Calculate the change in encoder ticks
    delta_left_ticks = left_ticks - prev_left_ticks
    delta_right_ticks = right_ticks - prev_right_ticks

    # Update the previous encoder tick values
    prev_left_ticks = left_ticks
    prev_right_ticks = right_ticks

    # Calculate the linear and angular velocities
    linear_velocity = (delta_left_ticks + delta_right_ticks) * (2 * 3.14159 * WHEEL_RADIUS) / (2 * TICKS_PER_REV)
    angular_velocity = (delta_right_ticks - delta_left_ticks) * (2 * 3.14159 * WHEEL_RADIUS) / (WHEELBASE * TICKS_PER_REV)

    # Calculate the change in position and orientation
    delta_x = linear_velocity * cos(theta)
    delta_y = linear_velocity * sin(theta)
    delta_theta = angular_velocity

    # Update the position and orientation
    x += delta_x
    y += delta_y
    theta += delta_theta

    # Create and publish the odometry message
    header = Header()
    header.stamp = rospy.Time.now()
    header.frame_id = "odom"

    pose = Point(x, y, 0.0)
    orientation = Quaternion(0.0, 0.0, sin(theta / 2), cos(theta / 2))

    twist = Twist()
    twist.linear.x = linear_velocity
    twist.angular.z = angular_velocity

    odom = Odometry(header, "base_link", pose, orientation, twist)

    odom_publisher.publish(odom)

if __name__ == '__main__':
    rospy.init_node('odometry_publisher')

    # Replace "left_ticks_topic" and "right_ticks_topic" with your actual encoder tick topics
    rospy.Subscriber('left_ticks_topic', int, encoder_ticks_callback)
    rospy.Subscriber('right_ticks_topic', int, encoder_ticks_callback)

    odom_publisher = rospy.Publisher('odom', Odometry, queue_size=10)

    rospy.spin()
