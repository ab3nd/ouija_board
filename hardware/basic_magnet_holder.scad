
$fn=60;

module mount_surface(){
    difference(){
        union(){
            angle_length = 50;
            angle_height = 20;
            thickness = 3;
            //Flanges on the sides
            hull(){
                translate([0, 7, 0]){
                    cube([angle_length, thickness, thickness]);
                    cube([thickness, thickness, angle_height]);
                }
            }
            hull(){
                translate([0,-10, 0]){
                    cube([angle_length, thickness, thickness]);
                    cube([thickness, thickness, angle_height]);
                }
            }
            
            //Mount panel
            translate([0,-10,0]){
                cube([80,20,thickness]);
            }
            
            //Magnet
            translate([0,20,10]){
                rotate([0, 90, 0]){
                    difference(){
                        cylinder(r=5, h=14);

                    }
                }
            }
            

            
            //Rectangle connector
            translate([0,10,8.5]){
                cube([14,7,3]);
            }
            
            //Flat end connector
            hull(){
                translate([0,20,10]){
                    rotate([0, 90, 0]){
                        cylinder(r=5, h=3);
                    }
                }
                translate([0,-10, 0]){
                    cube([3, 20, angle_height]);
                }
            }
        }
        
        
        // Cutouts for mounting
        union(){
            translate([0,0,-1]){
                translate([10, 0, 0]){
                    cylinder(h=8, r=5);
                }
                
                translate([25.1+2, 0, 0]){
                    cylinder(h=8, r=5);
                }
                   
                translate([10,-5, 0]){
                    cube([17,10,8]);
                }

                translate([25.1+4+10.78, 0, 0]){
                    cylinder(h=8, r=2.5);
                    translate([0,-2.5, 0]){
                        cube([30,5,8]);
                    }
                }
                
                translate([25.1+4+10.78 +30, 0, 0]){
                    cylinder(h=8, r=2.5);
                }
            }

            //Magnet hole
            translate([0,20,10]){
                rotate([0, 90, 0]){
                    translate([0,0,-0.5]){
                        cylinder(r=2.62, h=15);
                    }
                }
            }               
         }
      }
  }




mount_surface();

