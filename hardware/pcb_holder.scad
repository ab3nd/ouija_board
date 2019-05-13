
module servo(){
   //Tower Pro Micro Servo
   color([0,0.368,1]){
      cube([22.6, 12.45, 22.3]);
      translate([-(32.38-22.6)/2, 0, 15.6]){
         cube([32.38, 12.45, 2.51]);
      }
      translate([12.45/2, 12.45/2, 22.2]){
         cylinder(h=4.14, r=12.45/2);
      }
      translate([12.45, 12.45/2, 22.2]){
         cylinder(h=4.15, r=2.5);
      }
   }

   color([0.6, 0.6, 0.6]){
      translate([12.45/2, 12.45/2, 22.2+4.14]){
         cylinder(h=2.9,r=2.29);
      }
   }
}

module servo_horn(){
   color([0.6, 0.6, 0.6]){
      difference(){
         union(){
            cylinder(r=7.8/2, h=4, $fn=200);
            //long bar
            hull(){
               cylinder(r=7.8/2, h=1.54);
               translate([14.10, 0, 0]){
                  cylinder(r=2, h=1.54, $fn=200);
               }
               translate([-14.10, 0, 0]){
                  cylinder(r=2, h=1.54, $fn=200);
               }
            }
            //short bar
            translate([-3.75/2, -13.45/2, 0]){
               cube([3.75, 13.45, 1.54]);
            }
            translate([0, -13.45/2, 0]){ 
               cylinder(h=1.54, r=3.75/2, $fn=200);
            }
            translate([0, 13.45/2, 0]){ 
               cylinder(h=1.54, r=3.75/2, $fn=200);
            }
         }
         
         //central hole
         translate([0,0,-0.5]){
            cylinder(r=1, h=5, $fn=200);
         }
         //servo shaft
         translate([0,0,4-2.53]){
            cylinder(r=4.61/2, h=2.54, $fn=200);
         }
         //arm holes
         //Not implmented yet
      }   
   }
}

module pcb(){
   color([0,0.5,0]){
      cylinder(r=17.809911, h=1, $fn=200);
   }
}

module servo_with_horn(){
   servo();

   translate([12.45/2, 12.45/2, 22.2+4.14+2.9+2.53]){
      rotate([180,0,0]){
         servo_horn();
      }
   }
}

module mount_surface(){
   rotate([0,90,0]){
      difference(){
         //Mount panel
         translate([0,-10,0]){
            cube([80,20,2]);
         }
         // Cutouts for mounting
         union(){
            translate([0,0,-1]){
               translate([10, 0, 0]){
                  cylinder(h=8, r=5, $fn=200);
               }

               translate([25.1+2, 0, 0]){
                  cylinder(h=8, r=2, $fn=200);
               }

               translate([25.1+4+10.78, 0, 0]){
                  cylinder(h=8, r=2, $fn=200);
                  translate([0,-2, 0]){
                     cube([30,4,8]);
                  }
               }

               translate([25.1+4+10.78 +30, 0, 0]){
                  cylinder(h=8, r=2, $fn=200);
               }
            }
         }
      }
   }
}

module guide_tube(){
   //guide tube for magnet holder
   difference(){
      hull(){
         translate([0, 0, 30]){
            cylinder(r=5, h=20, $fn=200);
         }

         //piece to connect guide tube to mount_surface
         translate([-10, -(5+2), 30]){
            cube([20,2,20]);
         }
      }

      //cut the hole out of the middle for the tube
      translate([0, 0, 29.5]){
               cylinder(r=4.2, h=21, $fn=200);
      }
   }
}

difference()
{
   union(){
      translate([0,7,10]){
         guide_tube();
      }

      translate([20,0,50]){
         rotate([0,0,90]){
            mount_surface();
         }
      }

      //Mount for the pcb
       translate([0, 7, 60]){
         difference(){
            cylinder(r=17.809911, h=2, $fn=200);
            translate([0,0,-0.5]){
               cylinder(r=4.2, h=3, $fn=200);
            }
         }
      }

      //servo supports
      difference(){
         translate([-13, 0, -1.5]){
            cube([47, 21, 15]);
         }

         translate([-10, 0, -2]){
            cube([40, 18, 16]);
         }

         translate([0,1,-2]){
               cube([22.8, 21, 16]);
         }   
      }

      //cubes to stick everything together
      translate([-13,0,-1.5]){
         cube([26,2,62]);
      }

      translate([30,0,-30]){
         cube([4,2,80]);
      }
   }

   //Cube on the back to cut off part of the disk
   translate([-20,-15,55]){
      cube([40,15,10]);
   }

   //hole for servo screw
   translate([6,2.5,6]){
      rotate([90,0,0]){
         cylinder(r=5, h=5, $fn=80);
      }
   }
}
