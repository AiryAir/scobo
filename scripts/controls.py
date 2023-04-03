#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist


d = 1/2         # distance between wheels
vl = 0          # linear velocity of left wheel
vr = 0          # linear velocity of right wheel

vel = Twist()   # velocity message

def message_callback(vel):
    vl = vel.linear.x - vel.angular.z*d/2
    vr = vel.linear.x + vel.angular.z*d/2