/*
 * =====================================================================================
 *
 *       Filename:  xSpaceGroup2D.cpp
 *
 *    Description:  wallpaper groups
 *
 *        Version:  1.0
 *        Created:  07/10/2014 15:51:24
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
#include "vsr_fold.h"
#include "vsr_group.h"
#include "vsr_simplex.h"
#include "vsr_field.h"

using namespace vsr;
using namespace vsr::cga3D;

template<int N>
void Drw( const Simplex<N>& s, float dist=1.0, bool bOrtho=false){

  using Vec = NEVec<3>;

  auto proj = s.project(dist);

  glNormal3f(0,0,1);
  glColor3f(1,0,0);
  for (auto& i : s.edges){
    gfx::Glyph::Line( proj[i.a], proj[i.b] );
  }

  glColor4f(0,0,1,.2);
  for (auto& i : s.faces){
    gfx::Glyph::Triangle( proj[i.a], proj[i.b], proj[i.c] );
  }

}

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,pNum,ratio,div,numDraw,iter;
  bool bPin, bGlideA, bGlideB,bReset,bSnap;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
      gui(bSnap,"snap");
      gui(pNum,"pnum",1,6);
      gui(ratio,"ratio",0,2);
      gui(div,"div",1,10);
      gui(bPin,"pin");          ///< pin or spin
      gui(bGlideA,"ga");        ///< glide
      gui(bGlideB, "gb");
      gui(iter,"iter",0,200);
      amt = .5;
      ratio = 1.0;
      bPin = true;
      pNum = 2;
      iter = 1;
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
      time++;

      //Template on motif type
      SpaceGroup2D<Vec> sg(pNum,ratio,bPin,(int)div,bGlideA,bGlideB);

      /* int n=0; */
      /* for (auto& i : sg.ops){ */
      /*   float t = (float)n/sg.ops.size(); */
      /*   Draw(i,t,0,1-t); */
      /*   n++; */
      /* } */
      
      static Point pa = PT(0,0,0);
      static Point pb = PT(1,0,0);
      static Point pc = PT(0,1,0);
      static Point pd = PT(0,-1,0);

      Touch(interface,pa);
      Touch(interface,pb);
      Touch(interface,pc);
      Touch(interface,pd);
      
      vector<Pair> motif;
      motif.push_back(pa^pb); motif.push_back(pa^pc); motif.push_back(pa^pd);// motif.push_back

      auto single = sg.apply(motif,3,3); 
      if (bSnap){                
        for (auto& i : single){
          auto sa =  Ro::split(i,true);
          auto sb =  Ro::split(i,false);

          if ( Ro::sqd(sa, pa) < .03 ) pa = Ro::null( pa + (sa-pa) *.05);
          else if ( Ro::sqd(sb, pa) < .03 ) pa = Ro::null( pa + (sb-pa)*.05);
          if ( Ro::sqd(sa, pb) < .03   ) pb = Ro::null( pb + (sa-pb) *.05);
          else if ( Ro::sqd(sb, pb) < .03 ) pb = Ro::null( pb + (sb-pb)*.05);
          if ( Ro::sqd(sa, pc) < .03 ) pc = Ro::null( pc + (sa-pc) *.05);
          else if ( Ro::sqd(sb, pc) < .03 ) pc = Ro::null( pc + (sb-pc)*.05);
          if ( Ro::sqd(sa, pd) < .03 ) pd = Ro::null( pd + (sa-pd) *.05);
          else if ( Ro::sqd(sb, pd) < .03 ) pd = Ro::null( pd + (sb-pd)*.05);

        }
      }
      auto res = sg.apply(motif,iter,iter);

      for (auto& i : res){
        Draw(i,1,0,0);
      }

      Draw( Ro::dls(pa,.1),0,0,1,.5  );
      Draw( Ro::dls(pb,.1),0,0,1,.5  );
      Draw( Ro::dls(pc,.1),0,0,1,.5 );
      Draw( Ro::dls(pd,.1),0,0,1,.5 );

      if(bReset){
        pa = PT(0,0,0);
        pb = PT(1,0,0);
        pc = PT(0,1,0);
        pd = PT(0,-1,0);
      }

      cout << "num mirror ops: " << sg.ops.size() << endl; 
      cout << "num spin ops: " << sg.sops.size() << endl; 
      cout << "num glide ops: " << sg.gops.size() << endl; 
      cout << "num res: " << single.size() << endl;
      
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
