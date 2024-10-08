#!/usr/bin/python3

import cv2
import time
import threading
import imutils
from collections import deque
#from imutils.video import VideoStream
import numpy as np

global semFrame
global semBall

bufferVal = 32

def GetFrame():
    global frame
    img_counter = 0
    start_time = time.time()

    print ("Start Capture")

    while True:
        ret, frame = cam.read()
        if not ret:
            print("failed to grab frame")
            print("Frame=", frame)
            print("ret=", ret)  
            break

        semFrame.set()
        img_counter+=1
    
        if (time.time() - start_time) > 1:
            print ("FPS: ", img_counter)
            img_counter =0
            start_time = time.time()

def ShowFrame():
    print ("Start Sending frames")

    while True:
        #time.sleep(1)
        if 'frame' in globals():
            if semBall.is_set():
                semBall.clear()
                print ("Found ball: X,Y: ", int(x), int(y))
                #cv2.imshow("Retour camera", cv2.resize(frame, (160,120)))
                cv2.circle(frame, (int(x), int(y)), int(radius),(0, 255, 255), 2)
                cv2.imshow("Retour camera", frame)

        k = cv2.waitKey(int(1000/30)) # Necessary for window to popup

def SearchBall():
    global hsv
    global mask
    global blurred
    global cnts

    global x
    global y
    global radius

    pts = deque(maxlen=bufferVal)

    print ("Start searching ball")

    while True:
        semFrame.wait(None) # Wait for a new frame to check
        semFrame.clear()

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
            
            semBall.set()
            #print ("Found balle: X,Y: ", int(x), int(y))
            #return ((x,y), radius)
        #else:
            #return None

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

print ("Start config")
cam = cv2.VideoCapture(cv2.CAP_V4L2)
cam.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))

width=320
height=240
fps=90

cam.set(cv2.CAP_PROP_FRAME_WIDTH, width)
cam.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
if fps != -1:
	cam.set(cv2.CAP_PROP_FPS, fps)

print ("Config is:\n\tWidth = {0}\n\tHeight = {1}\n\tFPS = {2}",width, height, fps)

semFrame = threading.Event()
semBall = threading.Event()

threadGetFrame = threading.Thread(target=GetFrame)
threadShowFrame = threading.Thread(target=ShowFrame)
threadSearchBall = threading.Thread(target=SearchBall)

threadShowFrame.start()
threadGetFrame.start()
threadSearchBall.start()

threadShowFrame.join()
threadGetFrame.join()
threadSearchBall.join()

cam.release()
