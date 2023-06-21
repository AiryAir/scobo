#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16, Header
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Quaternion,TransformStamped
import math
import tf

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

left_ticks = 0
right_ticks = 0

def left_callback(data):
    global left_ticks
    left_ticks = data.data

def right_callback(data):
    global right_ticks
    right_ticks = data.data

def encoder_ticks_callback():
    global prev_left_ticks, prev_right_ticks, x, y, theta, left_ticks, right_ticks

    # Calculate the change in encoder ticks
    delta_left_ticks = left_ticks - prev_left_ticks
    delta_right_ticks = right_ticks - prev_right_ticks

    # Update the previous encoder tick values
    prev_left_ticks = left_ticks
    prev_right_ticks = right_ticks

    # Calculate the linear and angular velocities
    linear_velocity = (delta_left_ticks + delta_right_ticks) * (2 * math.pi * WHEEL_RADIUS) / (2 * TICKS_PER_REV)
    angular_velocity = (delta_right_ticks - delta_left_ticks) * (2 * math.pi * WHEEL_RADIUS) / (WHEELBASE * TICKS_PER_REV)

    # Calculate the change in position and orientation
    delta_x = linear_velocity * math.cos(theta)
    delta_y = linear_velocity * math.sin(theta)
    delta_theta = angular_velocity

    # Update the position and orientation
    x += delta_x
    y += delta_y
    theta += delta_theta

    odom_quat = tf.transformations.quaternion_from_euler(0, 0, theta)

    # Publish the transform over tf
    odom_trans = TransformStamped()
    odom_trans.header.stamp = rospy.Time.now()
    odom_trans.header.frame_id = "odom"
    odom_trans.child_frame_id = "base_link"

    odom_trans.transform.translation.x = x
    odom_trans.transform.translation.y = y
    odom_trans.transform.translation.z = 0.0
    odom_trans.transform.rotation = Quaternion(*odom_quat)

    # Send the transform
    odom_broadcaster = tf.TransformBroadcaster()
    odom_broadcaster.sendTransformMessage(odom_trans)

    # Publish the odometry message over ROS
    odom = Odometry()
    odom.header.stamp = rospy.Time.now()
    odom.header.frame_id = "odom"

    # Set the position
    odom.pose.pose.position.x = x
    odom.pose.pose.position.y = y
    odom.pose.pose.position.z = 0.0
    odom.pose.pose.orientation = Quaternion(*odom_quat)

    # Set the velocity
    odom.child_frame_id = "base_link"
    odom.twist.twist.linear.x = linear_velocity
    odom.twist.twist.linear.y = 0.0
    odom.twist.twist.angular.z = angular_velocity

    # Publish the message
    odom_publisher.publish(odom)

    last_time = rospy.Time.now()

if __name__ == '__main__':
    try:
        rospy.init_node('odometry_publisher')
        rospy.Subscriber('/left_ticks', Int16, left_callback)
        rospy.Subscriber('/right_ticks', Int16, right_callback)

        odom_publisher = rospy.Publisher('odom', Odometry, queue_size=10)

        rate = rospy.Rate(10)  # 10 Hz publishing rate

        while not rospy.is_shutdown():
            encoder_ticks_callback()
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
