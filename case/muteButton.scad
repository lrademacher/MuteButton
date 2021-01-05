// digispark:
// 25x19x4
$fn=32;

t = 2;
h1 = 33 -t;
h2 = 38.5 -t;
l = 60;
w = 40;

button_dia = 23.8;
led_dia = 5;

screw_hole_dia = 3;
screw_hole_dist = 3;
screw_hole_len = 6;

pcb_screw_wall_dist = 3;
pcb_screw_dist = 14;

round_r = 5;

module prism(l, w, h){
   polyhedron(
           points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
           faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]
           );
   }
   
module rounding(r, width) {
    translate([0,-r/2,-r/2])
    difference() {
        cube([width,r,r]);
        translate([0,r,r])
        rotate([0,90,0])
        cylinder(d=r, h=width);
    }
}
   
module case() { 
difference() {
    union() {
        difference() {
            union() {
                cube([w,l,h1]);
                translate([0,0,h1])
                       prism(w,l,  h2-h1);
            }       
            translate([w / 2, button_dia/2 + t + 1 + 5, 0])
                cylinder(h=t, d=button_dia);
            translate([w / 2, 45, 0])
                cylinder(h=t, d=led_dia);
            rounding(round_r, w);
            
            translate([w,l,0])
                rotate([0,0,180])
                    rounding(round_r, w);
            translate([w,0,0])
                rotate([0,0,90])
                    rounding(round_r, l);
            translate([0,l,0])
                rotate([0,0,-90])
                    rounding(round_r, l);
            
            translate([t,t,t])
                cube([w-2*t,l-2*t,h2]);
            
            translate([w/2-10/2,l-t,h2-(5+t)])
                cube([10, t, 5 + t]);
            }
            
        translate([0,0,h1-screw_hole_len]){
            cube([2*screw_hole_dist, 2*screw_hole_dist, screw_hole_len]);
            translate([0,2*screw_hole_dist,0])
            rotate([180,0,0])
            prism(2*screw_hole_dist,2*screw_hole_dist, 4);
        }
        translate([w-2*screw_hole_dist,0,h1-screw_hole_len]){
            cube([2*screw_hole_dist, 2*screw_hole_dist, screw_hole_len]);
            translate([0,2*screw_hole_dist,0])
            rotate([180,0,0])
            prism(2*screw_hole_dist,2*screw_hole_dist, 4);
        }
        translate([0,l-2*screw_hole_dist,h2-screw_hole_len-.6]){
            cube([2*screw_hole_dist, 2*screw_hole_dist, screw_hole_len]);
            translate([2*screw_hole_dist,0,0])
            rotate([180,0,180])
            prism(2*screw_hole_dist,2*screw_hole_dist, 4);
        }
        translate([w-2*screw_hole_dist,l-2*screw_hole_dist,h2-screw_hole_len-.6]){
            cube([2*screw_hole_dist, 2*screw_hole_dist, screw_hole_len]);
                        translate([2*screw_hole_dist,0,0])
            rotate([180,0,180])
            prism(2*screw_hole_dist,2*screw_hole_dist, 4);
        }
    }
    actual_screw_hole_len = screw_hole_len + 3;
    translate([screw_hole_dist,screw_hole_dist,h1-actual_screw_hole_len+.4])
        cylinder(d=screw_hole_dia, h=actual_screw_hole_len + 3);
    
    translate([w-screw_hole_dist,screw_hole_dist,h1-actual_screw_hole_len+.4])
        cylinder(d=screw_hole_dia, h=actual_screw_hole_len);
        
    translate([screw_hole_dist,l-screw_hole_dist,h2-actual_screw_hole_len+.4])
        cylinder(d=screw_hole_dia, h=actual_screw_hole_len);
        
    translate([w-screw_hole_dist,l-screw_hole_dist,h2-actual_screw_hole_len+.4])
        cylinder(d=screw_hole_dia, h=actual_screw_hole_len);
}
}

module lid() {
    l2 = sqrt(pow(l, 2) + pow(h2-h1, 2));
    
    difference(){
        cube([w,l2,t]);
        
        translate([screw_hole_dist,screw_hole_dist,0])
        cylinder(d1=screw_hole_dia, d2=screw_hole_dia*2, h=t);
        
        translate([w-screw_hole_dist,screw_hole_dist,0])
        cylinder(d1=screw_hole_dia, d2=screw_hole_dia*2, h=t);
        
        translate([screw_hole_dist,l2-screw_hole_dist,0])
        cylinder(d1=screw_hole_dia, d2=screw_hole_dia*2, h=t);
        
        translate([w-screw_hole_dist,l2-screw_hole_dist,0])
        cylinder(d1=screw_hole_dia, d2=screw_hole_dia*2, h=t);
        
        translate([w / 2 + pcb_screw_dist / 2, l2 - t - screw_hole_dia / 2 - pcb_screw_wall_dist, 0])
        cylinder(d1=screw_hole_dia, d2=screw_hole_dia*2, h=t);
        
        translate([w / 2 - pcb_screw_dist / 2, l2 - t - screw_hole_dia / 2 - pcb_screw_wall_dist, 0])
        cylinder(d1=screw_hole_dia, d2=screw_hole_dia*2, h=t);
    }
}

//case();

/*translate([0,0,h1])
rotate([atan((h2-h1)/l),0,0])*/
lid();

