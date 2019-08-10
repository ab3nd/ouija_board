
$fn = 60;

module microswitch(){
    sw_hole_d = 3.3;
    sw_hole_r = sw_hole_d/2;
    sw_hole_h = 4 + sw_hole_r+0.4;
    sw_hole_offset = 9.7 + sw_hole_r+0.4;
    sw_hole_sep = 25.35;

    difference(){
        //Body of the microwitch
        color("grey"){
            cube([49.2, 17.3, 20.9]);
        }

        //Mount holes
        translate([sw_hole_offset, -1, sw_hole_h]){ 
            rotate([-90,0,0]){
                cylinder(r = sw_hole_r, h = 25);
                translate([sw_hole_sep, 0, 0]){
                    cylinder(r=sw_hole_r, h = 25);
                }
            }
        }
    }
}

module holder(){
    difference(){
        translate([5, -12, 0]){ 
            color("blue"){
                minkowski(){
                    cube([40, 30, 18]);
                    sphere(r=3);
                }
            }
        }
        
        //measured from the strut
        groove_w = 6.2;
        translate([25.5, 10, -10]){
            translate([(13.5+groove_w)/2, 0, 0]){
                cylinder(r=groove_w/2, h = 40);
            }
            translate([-(13.5+groove_w)/2, 0, 0]){
                cylinder(r=groove_w/2, h = 40);
           }
        }
        
        //Delete the bottom face
        translate([0,-20,-7.9999]){
            cube([50, 50, 8]);
        }
    }
}

difference(){
    holder();
    rotate([90, 0, 0]){
        microswitch();
    }
}


