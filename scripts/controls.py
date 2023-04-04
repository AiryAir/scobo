#!/usr/bin/env python3
# code for differential drive robot controlled by teleop_twist_keyboard

import rospy
from geometry_msgs.msg import Twist


d = 1/2         # distance between wheels
vl = 0          # linear velocity of left wheel
vr = 0          # linear velocity of right wheel

def move():
    a = 0
    rospy.init_node('scobo_med',anonymous=True)
    twist = Twist()
    pub = rospy.Publisher('/scobo/cmd_vel', Twist, queue_size=10)
    twist = Twist()

    while(1):
        a = a+1
        a=0
        rospy.spin()

if __name__ == '__main__':
    try:
        #Testing our function
        move()
        
    except rospy.KeyboardInterrupt: pass