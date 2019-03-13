# Ouija Board
Hardware and software plans for a ouija board that is controlled by a computer as an IO device

### Measurements of Ouija board

Letters - 30mm tall by 14mm (I) - 38mm (W)

"NO" - 20mm tall by 35mm wide

"YES" - 20mm tall by 45mm wide

"GOOD BYE" - 20mm by 170mm wide

Closest approach to board edge - 25mm (bottom of "GOOD BYE")

Board thickness - 3.5mm

So the maximum radius of the sense ring is 25mm + 20mm/2 = 35mm, to be able to have the center of the actuated planchette window over the centerline of the letters closest to the edge of the board. 

The minimum diameter of the sense ring would be set by the size of the letters, and appears to be around 30-35mm, so a radius of 15-17mm. 

So the radius should be at minimum 15-17mm, and at maxium 35mm, bigger is easier to build (more PCB space), but 17mm can probably fit a ring. 

Active area is a box bounded by "YES/NO" at the top, "GOOD BYE" at the bottom, and the edges of the alphabet at the sides.
These measurements are from Sarah's board, which is the 1972 Parker/Berverly board, whereas those above are from my board, a 1960s Parker/Salem board. 

Width (outside of N to outside of Z) - 360mm
Height (top of YES/NO to bottom of GOODBYE) - 230mm

Board 295mm x 445mm (Conveniently, just inside the cutting area of my laser cutter)

A4 paper size is 210 mm × 297 mm (8.27 in × 11.7 in), so the plotters with an A4-sized area are a little too small for a traditional Ouija board. 
A3 paper is 297 mm × 420 mm (11.69 in × 16.54 in), so an A3 plotter would work. 

https://www.aliexpress.com/item/DIY-XY-Plotter-drawbot-pen-drawing-robot-machine-lettering-corexy-A4-A3-engraving-area-frame-plotter/32952725307.html?spm=2114.search0204.3.16.11db7b78WONW3n&ws_ab_test=searchweb0_0,searchweb201602_6_10065_10130_10068_10890_10547_319_10546_317_10548_10545_10696_453_10084_454_10083_10618_10307_537_536_10902_10059_10884_10887_321_322_10103,searchweb201603_16,ppcSwitch_0&algo_expid=b5854729-7ad1-4867-a162-900024d3b36f-2&algo_pvid=b5854729-7ad1-4867-a162-900024d3b36f

https://www.aliexpress.com/item/50-X-65cm-Laser-Engraving-Machine-Wood-Router-CNC-DIY-Laser-Engraver-Machine-For-Desktop-Cutting/32970303676.html?spm=2114.search0204.3.96.5e8f46b0ur8a8C&ws_ab_test=searchweb0_0,searchweb201602_6_10065_10130_10068_10890_10547_319_10546_317_10548_10545_10696_453_10084_454_10083_10618_10307_537_536_10902_10059_10884_10887_321_322_10103,searchweb201603_16,ppcSwitch_0&algo_expid=b95455ae-6d0d-4979-8a5e-edf8464dbc0f-14&algo_pvid=b95455ae-6d0d-4979-8a5e-edf8464dbc0f

Second one is huge, could be cut down to fit under board. Very tall, though. 




### Hardware Notes

AH49ENTR Hall effect sensors, SOT23-3 case

74hc4051 8ch analog demux

3 enable, 3 address = 24 hall effect sensors
