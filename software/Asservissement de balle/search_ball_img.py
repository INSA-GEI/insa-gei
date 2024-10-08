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
import timeit

bufferVal = 32

# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
#colorLower = (22, 765, 6)		# Min yellow
#colorUpper = (50, 255, 255)		# Max yellow

pts = deque(maxlen=bufferVal)
#imageName = "./Images/balle_jaune.png"
imageName = "./Images/IMG_20240927_111429.jpg"

def searchBall():
    # resize the frame, blur it, and convert it to the HSV
    # color space
    #frame = imutils.resize(frame, height=320)
    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    #print ("x=190,y=396 : ", hsv[396][190])
    #print ("x=218,y=404 : ", hsv[404][218])

    ###cv2.imshow("HSV", hsv)

    # construct a mask for the color "green", then perform
    # a series of dilations and erosions to remove any small
    # blobs left in the mask

    colorUpper = (25, 220, 230)		# Max yellow
    colorLower = (8, 160, 130)		# Min yellow

    mask = cv2.inRange(hsv, colorLower, colorUpper)
    ###maskImg = mask.astype(float)
    ###cv2.imshow("inRange", maskImg)

    mask = cv2.erode(mask, None, iterations=2)
    #cv2.imshow("Erode", mask)

    mask = cv2.dilate(mask, None, iterations=2)
    #cv2.imshow("Dilate", mask)

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
        #M = cv2.moments(c)
        #center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        print ("Found balle: X,Y: ", int(x), int(y))
        return ((x,y), radius)
    else:
        return None

        # only proceed if the radius meets a minimum size
        #if radius > 1:
        #    # draw the circle and centroid on the frame,
        #    # then update the list of tracked points
        #    cv2.circle(frame, (int(x), int(y)), int(radius),(0, 255, 255), 2)
        #    cv2.circle(frame, center, 5, (0, 0, 255), -1)

    ## update the points queue
    #pts.appendleft(center)

    # loop over the set of tracked points
    #for i in range(1, len(pts)):
    #    # if either of the tracked points are None, ignore
    #    # them
    #    if pts[i - 1] is None or pts[i] is None:
    #        continue

    #    # otherwise, compute the thickness of the line and
    #    # draw the connecting lines
    #    thickness = int(np.sqrt(bufferVal / float(i + 1)) * 2.5)
    #    cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

print (imageName)
frame = cv2.imread(imageName)
frame = imutils.resize(frame, width=320)
if frame is None:
	exit

print ("got image")

# keep looping
print ("it starts")

min_time = 1.0
max_time = 0.0
moy_time = 0.0

for i in range(100):
	start_time=time.time()
	((x,y), radius) = searchBall()
	end_time=time.time()
	elapsed_time = end_time-start_time

	if elapsed_time < min_time:
		min_time = elapsed_time

	if elapsed_time > max_time:
		max_time = elapsed_time

	moy_time = moy_time + elapsed_time

print("--- min %s seconds ---" % (min_time))
print("--- max %s seconds ---" % (max_time))
print("--- moy %s seconds ---" % (moy_time/100))
#timeit.timeit(searchBall,number=1)

# only proceed if the radius meets a minimum size
if radius > 1:
	# draw the circle and centroid on the frame,
        # then update the list of tracked points
        cv2.circle(frame, (int(x), int(y)), int(radius),(0, 255, 255), 2)
        #cv2.circle(frame, center, 5, (0, 0, 255), -1)

# show the frame to our screen
cv2.imshow("Frame", frame)
key = cv2.waitKey(1) & 0xFF

while key != ord("q"):
	key = cv2.waitKey(1) & 0xFF

# close all windows
cv2.destroyAllWindows()

