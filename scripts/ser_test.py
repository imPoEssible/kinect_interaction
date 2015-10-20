#!/usr/bin/python
import roslib; roslib.load_manifest('walle')
import rospy
from std_msgs.msg import String
from std_msgs.msg import Char
from walle.msg import *
import random
import serial
import time

class SerialConnection:
	def __init__(self):
		self.addr = '/dev/ttyACM0'
		self.baud = 9600
		self.port = serial.Serial(self.addr, self.baud, timeout=2)

def gesture_callback(data):
	global message
	wave = data.wave
	hello = data.hello
	goodbye = data.goodbye
	#our message fields are all booleans so we can do easy if statements
	if wave:
		print "waved!"
		message = 1
	# if hello:
	# 	print "hello!"
	# 	message = 13
	# if goodbye:
	# 	print "goodbye"
	# 	message = 14
	# if not goodbye and not hello and not wave:
	# 	print "none work"
	# 	message = 0
	#print message


def position_callback(data):
	pass


def publisher():
	global message
	rospy.init_node("kinect_listener", anonymous = True)
	rospy.Subscriber("/detected_gestures", gestures, gesture_callback) #subscribing to detected gestures from detectfinger
	rospy.Subscriber("/point_location", pointerpos, position_callback) #subscribes to XYZ node from detectfinger

	ser = SerialConnection()

	#pub = rospy.Publisher('/emotion', Char) #publishes to emotion node, which the Arduino controlling WALL-E's eyes and ears listen to
	r = rospy.Rate(20)
	while not rospy.is_shutdown():
		if message != 0:
			print "sending message: ", message
			ser.port.write("1")
			time.sleep(2)
	r.sleep()

if __name__ == "__main__":
	print "emotions.py"
	message = 0
	publisher()