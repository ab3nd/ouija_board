# Ouija Board
Hardware and software plans for a ouija board that is controlled by a computer as an IO device. 

Now that this is public, I thought I ought to write up a bit more of a description of what we had hoped to do, what ended up making it into the puzzle, lessons learned, and so forth. 

The initial idea of the puzzle was that players would play some sort of word game with Spirits From Beyond. The players would enter letters by sliding the Ouija board planchette around the board, and the machinery under the board would answer back. Since the easy way to drive the planchette from under the board was a magnet, we looked at a couple of different approaches for figuring out where the magnet was on the board. 

One approach was to cover the underside of the board with magnetic field visualization film, and then look at it with a camera. This has two main drawbacks, one of them being that field visualization film is expensive, and the other is that the camera has to have enough space to see the underside of the board, an unobstructed view (so the machinery has to move out of the way), and good light. This is annoying set of constraints to deal with. 

Magnets can also be sensed with Hall Effect sensors. One idea was to put a grid of sensors under the board, and interpolate the location of the planchette from the readings of multiple sensors, another option was to just have a sensor under each letter. Having a sensor under each letter means you can lose the planchetette if it's moved to some place that isn't a letter. Having a grid of sensors of sufficient resolution to detect the planchette at any location would mean we blew the budget building a PCB the size of a Ouija board, and then yours truly went nuts hand-soldering a bunch of SOT-23 parts (or I guess we contaminate someone's oven stenciling and reflowing them). So we didn't do that. 

The final approach was that the head of the machine that moves the planchette also had a ring of sensors. When it was moving the planchette, it would raise a magnet to engage the planchette, and then move to drag the planchette around. When it was sensing user input, it would lower the magnet, so it was only sensing the planchette magnet, and then move to keep the field over the sensor ring balanced. Since it was counting steps that it made to keep the magnet centered, the system would know what letter the sensor ring was under. This approach kept the sensor PCB small, reduced the number of parts needed, and could track the planchette anywhere. 

But wait, you say. When the puzzle was in operation, the players would say the word they wanted to input, and the system would then spell it and do some wiggling between yes and no. There was no actual use of the planchette as an input device!

Using the planchette as an input device had a number of problems. One was that the system could lose the planchette due to users moving it too fast, or picking it up and putting it back down somewhere else. We could have fixed that by having the system sweep the board, but that leads into the second problem, which is that there was no feedback. If the user spelled "obliges", but were closer to the 'h' than the 'i' and the system saw "oblhges", the user wouldn't have a way to see that it hadn't picked up their input properly. If it lost the planchette and was doing a sweep to find it again, the user would have no idea what was going on, and the system would be ignoring their input. 

Also, when people are properly using a Ouija board to communicate with Spirits from Beyond, they say their questions, and the spirits answer by pushing the planchette around (or it's the ideomotor effect, or Joe (that jerk) always tries to make it spell "dongs"). Since we didn't have a good way to contact players of the Game who have passed Beyond the Veil, we used baby monitors in the room with each Ouija board, and a person listening to the baby monitor. The person was SSHed into the Raspberry PI that was running each board, and used a little script to drive the planchette around. That was why the boards sometimes spelled out things like "NATO", to try to get people to spell things out with the NATO phonetic alphabet, in cases where the word they said was unclear. So while the players were speaking to Spirits from Beyond, the spirits still had their mortal shells, and were Beyond the room divider, rather than the Veil of Death.

### Actual Implementation Information

The gantry that moved the planchette was a cheapo CNC device skellington from AliExpress, which uses an Arduino and a couple of stepper motor drivers. Two of the motors drive one axis, the other axis only has one motor. They had the somewhat weird design decision that the Arduino is powered over USB, while the stepper drivers and motors have their own, independent power supply. This makes some sense, since it means that A) the Arduino doesn't need to be protected from noise from the stepper motors, and B) the parts that would normally do A are not needed, so the whole thing is cheaper for the Chinese factory it came from. 

The Arduino was controlled over a serial-over-USB link from a Raspberry Pi running stock Raspbian. A little python script presented a user interface for a person connected to the Pi via SSH, and took care of stuff like Mastermind solving, because humans are made of fallible meat, and so cannot be trusted to do Mastermind in their heads at 2AM. 

The Python for the UI and Mastermind solving is in user_console.py and mastermind.py. The control code for the Arduino is in ouija_control.ino. They communicate using a pretty simple text-based protocol. The incomplete code for following the planchette is in sensor_track.ino. It results in some very jerky movement, and it would probably be better to do some form of vector representation of the magnet displacement and a PID loop to drive that to zero when the magnet is well-centered, as well as velocity control to have it operate smoothly, but that's Left As An Exercise For The Reader. 

### Potentially Useful Things to Know

Aluminum is soft. If you want to cut it, it will smear like butter into hacksaws and abrasive wheels. The tool to use is just normal wood cutting implements, like miter saws and whatnot. 

Raspbian is pretty rough around the edges, I rant about it with a lot of swearing here:
http://gizmosmith.com/2019/08/28/raspbian-still-bad/
http://gizmosmith.com/2019/08/11/the-year-of-linux-on-the-desktop-is-ha-get-fucked/


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

These measurements are from my Ouija board, which is the older model. 

Width (outside of N to outside of Z) - 375mm
Height (top of YES/NO to bottom of GOODBYE) - 235mm

Board 295mm by 446mm

So the board size is essentially exactly the same, but the printing is a little different. The same size of plotter can work with either board, and the differences in text location can be handled by calibration. 


### Some CNC Devices to Use

A4 paper size is 210 mm × 297 mm (8.27 in × 11.7 in), so the plotters with an A4-sized area are a little too small for a traditional Ouija board. 
A3 paper is 297 mm × 420 mm (11.69 in × 16.54 in), so an A3 plotter would work. 

https://www.aliexpress.com/item/DIY-XY-Plotter-drawbot-pen-drawing-robot-machine-lettering-corexy-A4-A3-engraving-area-frame-plotter/32952725307.html?spm=2114.search0204.3.16.11db7b78WONW3n&ws_ab_test=searchweb0_0,searchweb201602_6_10065_10130_10068_10890_10547_319_10546_317_10548_10545_10696_453_10084_454_10083_10618_10307_537_536_10902_10059_10884_10887_321_322_10103,searchweb201603_16,ppcSwitch_0&algo_expid=b5854729-7ad1-4867-a162-900024d3b36f-2&algo_pvid=b5854729-7ad1-4867-a162-900024d3b36f

https://www.aliexpress.com/item/50-X-65cm-Laser-Engraving-Machine-Wood-Router-CNC-DIY-Laser-Engraver-Machine-For-Desktop-Cutting/32970303676.html?spm=2114.search0204.3.96.5e8f46b0ur8a8C&ws_ab_test=searchweb0_0,searchweb201602_6_10065_10130_10068_10890_10547_319_10546_317_10548_10545_10696_453_10084_454_10083_10618_10307_537_536_10902_10059_10884_10887_321_322_10103,searchweb201603_16,ppcSwitch_0&algo_expid=b95455ae-6d0d-4979-8a5e-edf8464dbc0f-14&algo_pvid=b95455ae-6d0d-4979-8a5e-edf8464dbc0f

Second one is huge, could be cut down to fit under board. Very tall, though. 

### Hardware Notes

AH49ENTR Hall effect sensors, SOT23-3 case

74hc4051 8ch analog demux

3 enable, 3 address = 24 hall effect sensors

### Magnet Grip

We replaced the planchette window/pointer with a cut piece of acrylic with a neodymium magnet in it, 5mm is about the max diameter for the magnet to still have it look good. 
