module magnet_holder(){
   //Body with hole
   difference(){
      cylinder(r=4, h=10, $fn=200);
      translate([0,0,-3]){
         cylinder(r=2.55, h=10, $fn=200);
      }
   }

   //Tab with a hole in it
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
            cylinder(r=0.6, h=5, $fn=200);
         }
      }
   }
}

magnet_holder();