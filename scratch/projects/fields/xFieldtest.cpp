/*
 * =====================================================================================
 *
 *       Filename:  xFieldtest.cpp
 *
 *    Description:  sanity checks
 *
 *        Version:  1.0
 *        Created:  02/19/2014 14:25:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_cga3D_app.h"

#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time =0;
  
  virtual void onDraw(){ 
        
      mouse = calcMouse3D();
      time +=.01;
      auto pnt = Ro::point( sin(time)*5, cos(time) * 5, 0);
    
      static Field<Vec> vf(20,10,1);
      //static Field<Vec> ortho(20,10,1);

      Par par;
      for (int i = 0; i < vf.num(); ++i){
        float dist = Ro::sqd( vf.grid(i), pnt ) + .1;
        par = Par(Tnv(0,1,0)).trs(pnt) * 1./dist;
        Vec nv = Ro::loc( vf.grid(i).bst(par*amt) ) - vf.grid(i);
        vf[i] = nv;
      }

      Draw(pnt);

      Draw(vf);
      //Draw(ortho);

  }
   

  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
