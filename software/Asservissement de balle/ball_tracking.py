#!/usr/bin/python3
# 
# # USAGE
# python ball_tracking.py --video ball_tracking_example.mp4
# python ball_tracking.py

# import the necessary packages
from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time

bufferVal = 32

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
	help="path to the (optional) video file")
ap.add_argument("-i", "--image",
	help="path to the (optional) image file")
args = vars(ap.parse_args())

# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
#colorLower = (22, 765, 6)		# Min yellow
#colorUpper = (50, 255, 255)		# Max yellow

colorUpper = (10, 230, 230)		# Max yellow

colorLower = (25, 170, 150)		# Min yellow

pts = deque(maxlen=bufferVal)

# if a video path was not supplied, grab the reference
# to the webcam
if not args.get("video", False) and not args.get("image", False):
	vs = VideoStream(src=0).start()

# otherwise, grab a reference to the video file
elif not args.get("image", False):
	vs = cv2.VideoCapture(args["video"])
else:
	print (args["image"])
	frame = cv2.imread(args["image"])
	if frame.all() == None:
		exit
		
	print ("got image")

# allow the camera or video file to warm up
time.sleep(2.0)

# keep looping
run = True

while run:
	print ("it starts")

	if not args.get("image", False):
		# grab the current frame
		frame = vs.read()

	# handle the frame from VideoCapture or VideoStream
	frame = frame[1] if args.get("video", False) else frame

	# if we are viewing a video and we did not grab a frame,
	# then we have reached the end of the video
	if frame is None:
		break

	# resize the frame, blur it, and convert it to the HSV
	# color space
	#frame = imutils.resize(frame, width=600)
	blurred = cv2.GaussianBlur(frame, (11, 11), 0)
	hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

	print ("x=190,y=396 : ", hsv[396][190])
	print ("x=218,y=404 : ", hsv[404][218])

	cv2.circle(hsv, (int(190), int(396)), int(5),(0, 0, 0), 2)
	cv2.imshow("HSV", hsv)

	# construct a mask for the color "green", then perform
	# a series of dilations and erosions to remove any small
	# blobs left in the mask
	mask = cv2.inRange(hsv, colorLower, colorUpper)
	mask = cv2.erode(mask, None, iterations=2)
	mask = cv2.dilate(mask, None, iterations=2)

	# find contours in the mask and initialize the current
	# (x, y) center of the ball
	cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
	#print ("cnts = ", cnts)

	cnts = imutils.grab_contours(cnts)
	center = None

	#print ("cnts = ", cnts)

	# only proceed if at least one contour was found
	if len(cnts) > 0:
		# find the largest contour in the mask, then use
		# it to compute the minimum enclosing circle and
		# centroid
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

		# only proceed if the radius meets a minimum size
		if radius > 1:
			# draw the circle and centroid on the frame,
			# then update the list of tracked points
			cv2.circle(frame, (int(x), int(y)), int(radius),
				(0, 255, 255), 2)
			cv2.circle(frame, center, 5, (0, 0, 255), -1)

	# update the points queue
	pts.appendleft(center)

	# loop over the set of tracked points
	for i in range(1, len(pts)):
		# if either of the tracked points are None, ignore
		# them
		if pts[i - 1] is None or pts[i] is None:
			continue

		# otherwise, compute the thickness of the line and
		# draw the connecting lines
		thickness = int(np.sqrt(bufferVal / float(i + 1)) * 2.5)
		cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

	# show the frame to our screen
	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF

	# if the 'q' key is pressed, stop the loop
	if key == ord("q"):
		break

	if args.get("image", False):
		run =False

# if we are not using a video file, stop the camera video stream
if not args.get("video", False) and not args.get("image", False):
	vs.stop()

# otherwise, release the camera
elif not args.get("image", False):
	vs.release()

key = cv2.waitKey(1) & 0xFF
while key != ord("q"):
	key = cv2.waitKey(1) & 0xFF

# close all windows
cv2.destroyAllWindows()
