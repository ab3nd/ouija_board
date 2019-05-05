#!/usr/bin/python

import math
import numpy as np

#Rotation of the first sensor from 0 degress

offset = 45.0/2.0

sensor_count = 8
delta_theta = 360.0/float(sensor_count)

for angle np.linspace(0, 360, sensor_count):
	print (angle + offset)

