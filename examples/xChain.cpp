#include "vsr_cga3D.h"

#include "vsr_GLVimpl.h" 
#include "vsr_chain.h"
#include "vsr_stat.h"


using namespace vsr;
using namespace glv;  

struct MyApp : App {
  
    Chain chain[5];  

  bool bRandom; 
  float amt;
  
  MyApp(Window * win) : App(win)
  {            
      //chain = new Chain[5];
    for (int j = 0; j < 5; ++j){
      float t = PI - ( PI * 1.0 * (j+1)/5);           
      chain[j].alloc(4);
      chain[j].link(0).pos() = Point( cos(t), sin(t), 0 ); 
      //chain[j].links();
        
      chain[j].fk();
    //    chain[j].joints(1);
      
    } 
    
    Rand::Seed();
    
  }
  
  virtual void initGui(){
         
    gui(bRandom,"random")(amt, "amt",0,1); 
    bRandom = false;
  }

  void onDraw(){
         
    

    for (int j = 0; j < 5; ++j ){
      
      for (int i = 0; i < chain[j].num(); ++i ){
        Draw( chain[j][i] );
        Draw( chain[j][i].bound(), 1,0,0,.2);  
        Touch( interface, chain[j].joint(i), chain[j][i] );  
         
         chain[j].joint(i).step();
      }   
     
      for (int i = 0; i < chain[j].num()-1; ++i){
        gfx::Glyph::Line( chain[j][i].pos(), chain[j][i+1].pos() );
     
      }
       
      
      chain[j].fk(); 
      
     }                
    
    if (bRandom){
       //pick a random joint on a random chain and rotate a random amount
      int chainIdx = Rand::Int( 4 );
      int jointIdx = Rand::Int( 3 );
      
      chain[chainIdx].joint(jointIdx).db() += Biv( Rand::Uniform(), Rand::Uniform(), Rand::Uniform() ) * ( Rand::Boolean() ? -amt : amt );
      
    }
    
    text("hit 'r' and 'g' and drag with mouse to rotate and translate joints");    
                   
  }
};
                        
MyApp * myApp;

int main(){
                          
  
  GLV glv(0,0);  
                
  Window * win = new Window(800,500,"Versor",&glv);    
                          
  myApp = new MyApp(win);
  myApp -> initGui();
  
  glv << *myApp;

  Application::run();
  
  return 0;
  
}


