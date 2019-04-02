
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

module magnet_holder(){
   difference(){
      cylinder(r=4, h=10, $fn=200);
      translate([0,0,-3]){
         cylinder(r=2.5, h=10, $fn=200);
      }
   }

   difference(){
      union(){
         translate([-2.5, 0, 14]){
            translate([1.5, 0, 0]){
               rotate([0,90,0]){
                  cylinder(r=2.5, h=2, $fn=200);
               }
            }
         }

         translate([0,0,12]){
               cube([2,5,4], center=true);
         }
      }

      translate([-2.5, 0, 14]){
         rotate([0,90,0]){
            cylinder(r=0.5, h=5, $fn=200);
         }
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

//Put the servo in
//rotate([90,0,0]){
//   servo_with_horn();
//}

//Initial position for magnet holder
//translate([0, -(22.2+4.14+2.9+2.53), 45]){
//   rotate([0,180,90]){
//      magnet_holder();
//   }
//}

magnet_holder();

////guide tube for magnet holder
//difference(){
//   hull(){
//      translate([0, -(22.2+4.14+2.9+2.53), 30]){
//         cylinder(r=5, h=20, $fn=200);
//      }
//
//      //piece to connect guide tube to mount_surface
//      translate([-10, -(22.2+4.14+2.9+2.53+5+2), 30]){
//         cube([20,2,20]);
//      }
//   }
//
//   //cut the hole out of the middle for the tube
//   translate([0, -(22.2+4.14+2.9+2.53), 29.5]){
//            cylinder(r=4.2, h=21, $fn=200);
//   }
//}
//
////math for the servo mounts
//servo_front = 15.6 + 2.51;
//mount_surface_front = -(22.2+4.14+2.9+2.53+5+2);
//thickness = -(mount_surface_front+ servo_front);
//
////servo mounts
//difference(){
//   union(){
//      translate([-13,-servo_front-thickness,0]){
//         cube([13, thickness, 12.45]);
//      }
//
//      translate([22.6,-servo_front-thickness,0]){
//         cube([11, thickness, 12.45]);
//      }
//   }
//   translate([-11,-servo_front-(thickness+3),-0.5]){
//      cube([42, thickness, 13]);
//   }
//}
//
////piece to hold servo mounts to everything else
//difference(){
//   hull(){
//      translate([-10, -(22.2+4.14+2.9+2.53+5+2), 30]){
//         cube([20,2,20]);
//      }
//
//      translate([-10, -(22.2+4.14+2.9+2.53+5+2), -30]){
//         cube([20,2,20]);
//      }
//
//      translate([15, -(22.2+4.14+2.9+2.53+5+2), -4]){
//         cube([20,2,20]);
//      }
//   }
//
//   translate([12.45/2,-(22.2+4.14+2.9+2.53+4),12.45/2]){
//      rotate([90,0,0]){
//         cylinder(r=17,h=5,$fn=200);
//      }
//   }
//}
////Sums here are height of servo, horn, radius of cylinder, thickness of mount, etc. 
//translate([-20, -(22.2+4.14+2.9+2.53+5+2), 50])
//rotate([0,0,90]){
//   mount_surface();
//}
//
////Mount for the pcb
// translate([0, -(22.2+4.14+2.9+2.53), 30+20]){
//   difference(){
//      cylinder(r=17.809911, h=2, $fn=200);
//      translate([0,0,-0.5]){
//         cylinder(r=4.2, h=3, $fn=200);
//      }
//   }
//}

