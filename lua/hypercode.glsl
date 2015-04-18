struct VecR4 {
	float e1,e2,e3,e4;
};

struct Biv {
	float e14,e24,e34;
};

struct Rot {
	float s,e14,e24,e34;
};

struct Vec_Rot {
	float e1,e2,e3,e4,e124,e134,e234;
};


VecR4 r4( Vec4 v){
  return VecR4(v.r,v.g,v.b,v.a);
}

Rot gen( Biv b ){

  float dot = b.e1 * b.e1 + b.e2 * b.e2 + b.e3 * b.e3;
  float w = sqrt(dot);
  float cs = cos(w);
  float sn = -sin(w) / w; 
  
  return Rot(cs, b.e1 * sn, b.e2 * sb, b.e3 * sb); 
};


Vec_Rot v_gp_r(VecR4 a, Rot b){
  return Vec_Rot(
  a.e1 * b.s - a.e4 * b.e14,
  a.e2 * b.s - a.e4 * b.e24,
  a.e3 * b.s - a.e4 * b.e34,
  a.e1 * b.e24 - a.e2 * b.e14,
  a.e1 * b.e34 - a.e3 * b.e14,
  a.e1 * b.e14 + a.e2 * b.e24 + a.e3 * b.e34 + a.e4 * b.s,
  a.e2 * b.e34 - a.e3 * b.e24
  );
}

Rot reverse(Rot a){
  return Rot(a.s,-a.e14,-a.e24,-a.e34);
}

VecR4 vr_gp_r(Vec_Rot a, Rot b){
  return Vec4(
    a.e1 * b.s - a.e4 * b.e14 - a.e124 * b.e24 - a.e134 * b.e34,
    a.e2 * b.s - a.e4 * b.e24 + a.e124 * b.e14 - a.e234 * b.e34,
    a.e3 * b.s - a.e4 * b.e34 + a.e134 * b.e14 + a.e234 * b.e24,
    - a.e124 * b.e34 + a.e134 * b.e24 - a.e234 * b.e14,
  );
}

VecR4 spin(VecR4 a, Rot b){
  return vr_gp_b(v_gp_r(a,b), reverse(b) );
}

Vec3 project( VecR4 v, float dist ){
  float wt = 1 / (1 - v.e4);
  return Vec3( v.e1/wt, v.e2/wt, v.e3.wt );
}

Vec3 transform( Vec4 v, Vec3 biv){
  return project( spin( r4(v), gen( Biv(biv.r, biv.g, biv.b) ) );
}   
