# Ouija Board
Hardware and software plans for a ouija board that is controlled by a computer as an IO device

### Measurements of Ouija board

Letters - 30mm tall by 14mm (I) - 38mm (W)

"NO" - 20mm tall by 35mm wide

"YES" - 20mm tall by 45mm wide

"GOOD BYE" - 20mm by 170mm wide

Closest approach to board edge - 25mm (bottom of "GOOD BYE")

Board thickness - 3.5mm

So the maximum diameter of the sense ring is 25mm + 20mm/2 = 35mm, to be able to have the center of the actuated planchette window over the centerline of the letters closest to the edge of the board. 

The minimum diameter of the sense ring would be set by the size of the letters, and appears to be around 30-35mm, so the sense ring should be a PCB with a diameter no larger than 35mm and not much smaller than 35mm either. 

### Hardware Notes

AH49ENTR Hall effect sensors, SOT23-3 case

74hc4051 8ch analog demux

3 enable, 3 address = 24 hall effect sensors
