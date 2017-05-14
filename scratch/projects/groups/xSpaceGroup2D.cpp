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


#include "vsr_app.h"   
#include "form/vsr_fold.h"
#include "form/vsr_group.h"
//#include "form/vsr_simplex.h"
#include "form/vsr_field.h"

using namespace vsr;
using namespace vsr::cga;

//template<int N>
//void Drw( const Simplex<N>& s, float dist=1.0, bool bOrtho=false){
//
//  using Vec = NEVec<3>;
//
//  auto proj = s.project(dist);
//
//  glNormal3f(0,0,1);
//  glColor3f(1,0,0);
//  for (auto& i : s.edges){
//    gfx::Glyph::Line( proj[i.a], proj[i.b] );
//  }
//
//  glColor4f(0,0,1,.2);
//  for (auto& i : s.faces){
//    gfx::Glyph::Triangle( proj[i.a], proj[i.b], proj[i.c] );
//  }
//
//}

struct MyApp : App {    
   
  Point mouse;
  Line ray;

  float time;
  float amt ,pNum, ratio, div, numDraw, iter_width, iter_height;
  bool bPin, bGlideA, bGlideB,bReset,bSnap,bDrawMotif;

  Point pa = PT(0,0,0);
  Point pb = PT(1,0,0);
  Point pc = PT(0,1,0);
  Point pd = PT(0,-1,0);


  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(bReset,"reset");
      gui(bDrawMotif,"bDrawMotif");
      gui(bSnap,"snap");
      gui(pNum,"pnum",1,6);
      gui(ratio,"ratio",0,2);
      gui(div,"div",1,10);
      gui(bPin,"pin");          ///< pin or spin
      gui(bGlideA,"ga");        ///< glide
      gui(bGlideB, "gb");
      gui(iter_width,"iter_width",0,200);
      gui(iter_height,"iter_height",0,200);
      amt = .5;
      ratio = 1.0;
      bPin = true;
      pNum = 2;
      iter_width = iter_height = 1;

      objectController.attach(&pa);
      objectController.attach(&pb);
      objectController.attach(&pc);
      objectController.attach(&pd);

      mColor.set(1,1,1);
  }
  

    virtual void onDraw(){ 
        
      mouse = calcMouse3D();
      time++;

      //symmetry number | w/h ratio | reflect or spin | face divisor | 
      SpaceGroup2D<Vec> sg(pNum,ratio,bPin,(int)div,bGlideA,bGlideB);


      // Pair them up
      vector<Pair> motif;
      motif.push_back(pa^pb);
      motif.push_back(pa^pc); 
      motif.push_back(pa^pd);

      // let's make a bunch of them
      auto pgroup = sg.apply(motif,1,1);
      auto res = sg.hang(pgroup,iter_width,iter_height,
          [](float tx, float ty, const Pair &pair)
          {
            return pair.rotate( Biv::xy * TWOPI * tx);
          });

      for (auto& i : res){
        Draw(i,0,0,0);
      }

      if (bDrawMotif){
        Draw( Round::dls(pa,.1),0,0,0,.5  );
        Draw( Round::dls(pb,.1),0,0,0,.5  );
        Draw( Round::dls(pc,.1),0,0,0,.5 );
        Draw( Round::dls(pd,.1),0,0,0,.5 );
      }
      
      // Ok, for next time we generate bunch of copies of motif to find near points ..
      // ... which snap together if snapping is enabled
      auto single = sg.apply(motif,3,3); 
      if (bSnap){                
        for (auto& i : single){
          auto sa =  Round::split(i,true);
          auto sb =  Round::split(i,false);

          if ( Round::sqd(sa, pa) < .03 ) 
            pa = Round::null( pa + (sa-pa) *.05);
          else if ( Round::sqd(sb, pa) < .03 )
            pa = Round::null( pa + (sb-pa)*.05);
          if ( Round::sqd(sa, pb) < .03   )
            pb = Round::null( pb + (sa-pb) *.05);
          else if ( Round::sqd(sb, pb) < .03 )
            pb = Round::null( pb + (sb-pb)*.05);
          if ( Round::sqd(sa, pc) < .03 )
            pc = Round::null( pc + (sa-pc) *.05);
          else if ( Round::sqd(sb, pc) < .03 )
            pc = Round::null( pc + (sb-pc)*.05);
          if ( Round::sqd(sa, pd) < .03 )
            pd = Round::null( pd + (sa-pd) *.05);
          else if ( Round::sqd(sb, pd) < .03 )
            pd = Round::null( pd + (sb-pd)*.05);

        }
      }

      // ... Or even maybe Reset the points 
      if(bReset){
        pa = PT(0,0,0);
        pb = PT(1,0,0);
        pc = PT(0,1,0);
        pd = PT(0,-1,0);
      }
      

//      cout << "num mirror ops: " << sg.ops.size() << endl; 
//      cout << "num spin ops: " << sg.sops.size() << endl; 
//      cout << "num glide ops: " << sg.gops.size() << endl; 
//      cout << "num res: " << single.size() << endl;
      
  }
   

  
};



int main(){
  MyApp app;
  app.start();
  return 0;                             
}
