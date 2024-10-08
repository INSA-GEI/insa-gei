#!/usr/bin/python3

import cv2
import time

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
cv2.namedWindow("test")

print ("Start Capture")

img_counter = 0

start_time = time.time()

while True:
    ret, frame = cam.read()
    if not ret:
        print("failed to grab frame")
        print("Frame=", frame)
        print("ret=", ret)  
        break

    frame = cv2.resize(frame, (160,120))
    
    if (img_counter % 9) == 0 : # 15 fps over ethernet
        cv2.imshow("test", frame)

    img_counter+=1
    
    if (time.time() - start_time) > 1:
        print ("FPS: ", img_counter)
        img_counter =0
        start_time = time.time()

    k = cv2.waitKey(1)
    if k%256 == 27:
        # ESC pressed
        print("Escape hit, closing...")
        break
    elif k%256 == 32:
        # SPACE pressed
        img_name = "opencv_frame_{}.png".format(img_counter)
        cv2.imwrite(img_name, frame)
        print("{} written!".format(img_name))
        img_counter += 1

cam.release()
