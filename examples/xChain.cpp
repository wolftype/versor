#include "vsr_app.h"
#include "util/vsr_stat.h"

#include "form/vsr_chain.h" 

using namespace vsr;
using namespace vsr::cga;
using namespace glv;  

const int numChain = 10;  ///< number of kinematic chains
const int numLink = 100;   ///< number of links per chain


struct MyApp : App { 
   
  Chain chain[numChain];  ///< n kinematic chains 

  bool bRandom,bUpdate; 
  float amt;

  Dls dls = Construct::sphere(3,1,0);
  
  void setup() {

    immediate(false);
    bindGLV();
    initGui();
                
    for (int j = 0; j < numChain; ++j){
      float t = PI - ( PI * 1.0 * (j+1)/numChain);           
      chain[j].alloc(numLink);
      chain[j].link(0).pos() = Point( cos(t), sin(t), 0 ); 
      chain[j].fk();
      for (int i = 0; i < chain[j].num(); ++i){
        Frame& f = chain[j].joint(i);
        Pnt& p = chain[j][i].pos();
        objectController.attach( &f, &p);
      }
    } 

    scene.camera.pos( 0,0,15);
    
    Rand::Seed();
  }
  
  void initGui(){
    gui(bRandom,"random")(bUpdate,"bUpdate")(amt, "amt",0,1); 
    bRandom = true;
    amt = .1;
  }

  void onDraw(){

    static float time = 0;
    time += .01;
    float rad = 5.0;
    light = Vec3f(sin(time)*rad, cos(time)*rad, 1);

    for (int j = 0; j < numChain; ++j ){
      
      for (int i = 0; i < chain[j].num(); ++i ){
        
        draw( chain[j][i] );
        draw( chain[j][i].bound(), Rand::Uniform(), Rand::Uniform(), Rand::Uniform(),.3,bUpdate);  
         
        chain[j].joint(i).step();
      }   
    
      //forward kinematics 
      chain[j].fk(); 
      
     }                
    
    //if random button is clicked . . .
    if (bRandom){
       
      //pick a random joint on a random chain and rotate a random amount
      int chainIdx = Rand::Int( numChain-1 );
      int jointIdx = Rand::Int( numLink-1 );

      Biv randomBiv = Biv( Rand::Uniform(), Rand::Uniform(), Rand::Uniform() ) * ( Rand::Boolean() ? -amt : amt );

      //db() method is the differential of the bivector (plane of rotation)
      chain[chainIdx].joint(jointIdx).db() += randomBiv;      
    }
    
    //text("hit 'r' and 'g' and drag with mouse to rotate and translate joints");    
                   
  }
};
                        

int main(){
                          
  
  MyApp app;
  app.start();
  
  return 0;
  
}


