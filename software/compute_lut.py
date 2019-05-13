#!/usr/bin/python

import math

#Rotation of the first sensor from 0 degress

offset = 45.0/2.0

sensor_count = 8
delta_theta = 360.0/float(sensor_count)

# Sensor address to PCB silk to ordering (starting at 000)
# 000	U1	0
# 001	U2	1
# 010	U3	2
# 011	U4	7
# 100	U9	3
# 101	U8	6
# 110	U7	4
# 111	U6	5

cos_lut_vals = []
sin_lut_vals = []
for angle in range(sensor_count):
	degrees = ((angle*delta_theta)+offset)
	cos_lut_vals.append(str(round(math.cos(degrees), 5)))
	sin_lut_vals.append(str(round(math.sin(degrees), 5)))

sin_lut = "float sin_lut[8] = {" + ", ".join(sin_lut_vals) + "}"
cos_lut = "float cos_lut[8] = {" + ", ".join(cos_lut_vals) + "}"
	
print(sin_lut)
print(cos_lut)