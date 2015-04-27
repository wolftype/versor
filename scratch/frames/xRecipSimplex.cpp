/*
 * =====================================================================================
 *
 *       Filename:  xRecipSimplex.cpp
 *
 *    Description:  simple simplicial reciprocal
 *
 *        Version:  1.0
 *        Created:  10/09/2014 17:15:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

#include "vsr_print.h"
#include <sstream>

using std::stringstream;

using namespace vsr;
using namespace vsr::cga3D;


void printPsNode(const Vec& v, string name=""){
   cout << "node(" << name << ") at (" << v[0] << "," << v[1] << "," << v[2] << "){};" << endl; 
};


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,amt2,num,theta,growamt;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amt2,"amt2",-100,100);
      gui(num,"num",1,100);
      gui(theta,"theta",0,TWOPI);
      gui(growamt,"growamt",0,TWOPI);
      gui(bReset,"reset");
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();


      int n = num;

      
    for (int j=0;j<3;++j){
    
      vector<Vec> v;

      float grow = 1 + growamt*(float)j/3;
      for (int i = 0;i < n; ++i){
        double t = theta*(float)i/n;
        v.push_back( Vec(cos(t)*amt*grow,sin(t)*amt*grow,amt2) );
      }

      for (int i = 0;i < n; ++i){
        //recips
        int prev = i>0?i-1:n-1;
        int next = i<(n-1)?i+1:0;
        Biv bprev = v[prev] ^ v[i];
        Biv bnext = v[i] ^ v[next];
        Vec ra = -(v[prev] <= !bprev);
        Vec rb = (v[next] <= !bnext);
        
        Draw(v[i]);
        DrawAt(ra, v[i]/2.0, 1,0,0); DrawAt(rb, v[i]/2.0, 0,1,0);
        
        stringstream osa, osb; osa << "va" << i; osb <<"vb"<<i;
       // cout << tikz::coord(osa.str(), ra) << endl;       
        //cout << tikz::coord(osa.str(), rb) << endl;  
        
        cout << tikz::draw(Vec(0,0,0),v[i]) << endl;     
        cout << tikz::draw(v[i]/2.0,ra, "red") << endl;     
        //cout << tikz::draw(Vec(0,0,0),v[i]) << endl;     
       // printPsNode(ra,osa.str());
       // printPsNode(rb,osb.str());
        //ra.print();      
        //rb.print();


        /* //should be 1 */
        /* cout << 1 << endl; */
        /* (ra <= v[i] ).print(); */
        /* (rb <= v[i] ).print(); */

        /* //should be 0 */
        /* cout << 0 << endl; */
        /* (ra <= v[prev] ).print(); */
        /* (rb <= v[next] ).print(); */
      }

    }


  }
   

  
};


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
