/*
 * =====================================================================================
 *
 *       Filename:  xGLV.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/01/2014 16:57:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_GLVimpl.h"
#include "vsr_euclidean.h"
#include "vsr_group.h"
#include "vsr_hull.h"

using namespace vsr;

struct MyApp : App {  
  
  float time;  

  typedef NEVec<3> V;
  typedef NEBiv<3> Biv;

  vector< V > group;

  ConvexHull<3> hull;

  V tmp;



  MyApp(Window * win ) : App(win)
    //group(3,2)
    {
        scene.camera.pos( 0,0,5 ); 
        time = 0;

        
        //ROOT SYSTEM
        auto a = V::x.rot( Biv::xy * PIOVERTWO/4 );
        auto b = V::y.rot( Biv::xy * PIOVERTWO/4 );
        auto c = V::y.rot( Biv::yz * PIOVERTWO/4 );

        group = Root::System( a, b, c );
        cout << "GROUP SIZE: " << group.size() << endl;
        
        for (auto& i: group) i.print();
        
        //hull.calc(group); 
        hull.initialFace(group);
        hull.convexPass(group);
        hull.closeHoles(group);
  }

    
  /*-----------------------------------------------------------------------------
   *  INITIALIZE GUI
   *-----------------------------------------------------------------------------*/
    void initGui(){

    }
  

    /*-----------------------------------------------------------------------------
     *  ON DRAW
     *-----------------------------------------------------------------------------*/
    virtual void onDraw(){ 

        time +=.01;
       
   }
   
  
};




/*-----------------------------------------------------------------------------
 *  MAIN
 *-----------------------------------------------------------------------------*/
MyApp * app;

int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win ); 
  app -> initGui();
  
  glv << *app;
  Application::run();

  return 0;

}




