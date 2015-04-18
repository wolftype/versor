
/*-----------------------------------------------------------------------------
 *  General Conformal Transformation from one Circle to another
 *-----------------------------------------------------------------------------*/


#include "util/vsr_cga3D_app.h"  

struct MyApp : App {
      
  float amt;

  Circle ca, cb;
   
  virtual void setup(){
    
    bindGLV();
    gui(amt,"amt",-100,100);

    ca = cga::circle(-3,0,0);  
    cb = cga::circle(3,0,0);  
  }
  
  virtual void onAnimate(){
    
    ca = ca.rotate(Biv::xy * amt);
  }

  virtual void onDraw(){       
    
    //log of transformation from ca to cb
    auto ratio = gen::ratio(ca,cb);
    ratio.print();
    auto log = gen::log( ratio );
    //log[0].print();
    //log[1].print();
     
    int num = 100;
    for (int i = 0; i < num; ++i ){
      float t = 1.0 * i/num;
      //transformed circle
      auto nc = ca.spin( gen::con(log,t) );
      Draw ( nc, 1, t, 1-t  );
    
    }

    Draw(ca,1,0,1);
    Draw(ca.spin(ratio),1,0,1);
    //draw(cb,1,1,0);
  
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}  
