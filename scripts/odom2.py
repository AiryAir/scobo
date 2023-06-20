import rospy
from std_msgs.msg import Int16
from nav_msgs.msg import Odometry

# Global variables to store tick counts
left_ticks = 0
right_ticks = 0

def left_callback(data):
    global left_ticks
    left_ticks = data.data

def right_callback(data):
    global right_ticks
    right_ticks = data.data

def publish_odom():
    rospy.init_node('odom_publisher', anonymous=True)
    left_sub = rospy.Subscriber('left_ticks', Int16, left_callback)
    right_sub = rospy.Subscriber('right_ticks', Int16, right_callback)
    odom_pub = rospy.Publisher('odom', Odometry, queue_size=10)
    rate = rospy.Rate(10)  # 10 Hz publishing rate

    while not rospy.is_shutdown():
        # Create an Odometry message and fill in the relevant data
        odom = Odometry()
        # Set the left and right wheel tick counts
        odom.pose.pose.position.x = left_ticks
        odom.pose.pose.position.y = right_ticks

        # Publish the Odometry message
        odom_pub.publish(odom)

        rate.sleep()

if _name_ == '_main_':
    try:
        publish_odom()
    except rospy.ROSInterruptException:
        pass
