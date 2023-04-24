#!/usr/bin/env python3

import rospy
import os
import Tkinter as tk

dir1 = 7
pwm1 = 36
dir2 = 38
pwm2 = 15

#dir1, dir2 low for forward
#dir1, dir2 high for reverse
#dir1 is right, dir2 is left
#pwm1, pwm2 high for motors on/off

#pwm1 is right side motors
#pwm2 is left side motors

def init():
    gpio.setmode(gpio.BOARD)
    gpio.setup(7, gpio.OUT)
    gpio.setup(36, gpio.OUT)
    gpio.setup(38, gpio.OUT)
    gpio.setup(15, gpio.OUT)
    gpio.setup(16, gpio.OUT)
    
def turnon():
    gpio.output(16,True)
    
def turnoff():
    gpio.output(16,False)

def forward(tf):
    
    gpio.output(dir1, False)
    gpio.output(pwm1,True)
    gpio.output(dir2, False)
    gpio.output(pwm2,True)
    time.sleep(tf)
    gpio.cleanup()

def reverse(tf):
    
    gpio.output(dir1, True)
    gpio.output(pwm1,50)
    gpio.output(dir2, True)
    gpio.output(pwm2,50)
    time.sleep(tf)
    gpio.cleanup()

def left_turn(tf):
    
    gpio.output(dir2, True)
    gpio.output(pwm2,True)
    gpio.output(dir1, False)
    gpio.output(pwm1,True)
    time.sleep(tf)
    gpio.cleanup()

def right_turn(tf):
    
    gpio.output(dir2, False)
    gpio.output(pwm2,True)
    gpio.output(dir1, True)
    gpio.output(pwm1,True)
    time.sleep(tf)
    gpio.cleanup()

def key_input(event):
    init()
    print ("Key:", event.char)
    key_press = event.char
    sleep_time = 0.03

    if key_press.lower() == 'w':
        forward(sleep_time)
    elif key_press.lower() == 's':
        reverse(sleep_time)
    elif key_press.lower() == 'd':
        left_turn(sleep_time)
    elif key_press.lower() == 'a':
        right_turn(sleep_time)
    elif key_press.lower() == 'o':
        turnon()
    elif key_press.lower() == 'p':
        turnoff()
    else:
        print("Invalid key stroke")
        gpio.output(dir1,False)
        gpio.output(pwm1,False)
        gpio.output(dir2,False)
        gpio.output(pwm2,False)
    
command = tk.Tk()
command.bind('<KeyPress>', key_input)
command.mainloop()
