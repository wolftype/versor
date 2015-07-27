/*
 * =====================================================================================
 *
 *       Filename:  xMiuriOri6.cpp
 *
 *    Description:  MiuriOri fold Rigid Networks
 *
 *        Version:  1.0
 *        Created:  07/20/2015 18:45:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_field.h"
#include "form/vsr_fold.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,theta;


  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100)(theta,"theta",-TWOPI,TWOPI);

  }

    virtual void onDraw(){ 
        
      mouse = calcMouse3D();

      //grid
      int w = 5; int h =5;
      bool bFlip = true;
      Field<Pnt> f(w,h,1);

      //offset
      for(int i=0;i<w;i++){
        for(int j=0;j<h;j+=2){
          int idx = f.idx(i,j);
          float tu = (float)i/w;
          float tv = (float)j/h;
          f[idx] = (f[idx] + Vec(( -amt + (2*amt * tu) ) * (bFlip ? -1 : 1),0,0)).null();
        }
        bFlip = !bFlip;
      }

      //vertical constraints
      vector<Rig3> rigid(w*h);

      // set initial positions
      // rigid[f.idx(0,0)].setResult( f.at(0,0) );
      // rigid[f.idx(1,0)].setResult( f.at(1,0) );
     //  rigid[f.idx(2,0)].setResult( f.at(2,0) );


      for (int i=0;i<f.num();++i){
          rigid[i].setResult( f[i] );
      }

       rigid[f.idx(0,0)].bCalc = false;
       rigid[f.idx(1,0)].bCalc = false;
       rigid[f.idx(2,0)].bCalc = false;

      for (int j=1;j<h;++j){
        
        auto left = f.at(0,j); 
        auto mid = f.at(1,j);
        auto right = f.at(2,j);

        int bl = f.idx(0,j-1);
        int bm = f.idx(1,j-1);
        int br = f.idx(2,j-1);

        int tl = f.idx(0,j);
        int tm = f.idx(1,j);
        int tr = f.idx(2,j);
        
        //middle, left, right
        rigid[tm].set(mid,   &rigid[bl], &rigid[bm], &rigid[br], false, false );
        rigid[tl].set(left,  &rigid[bl], &rigid[bm], &rigid[tm], false, true );
        rigid[tr].set(right, &rigid[tm], &rigid[bm], &rigid[br], false, true );
          
      }

      //horizontal constraints
      bool bSwitch = false;
      for (int i=3;i<w;++i){
          int j = 0;
          auto left = f.at(i,j+2);
          auto mid = f.at(i,j+1);
          auto right = f.at(i,j);
          
          int tl = f.idx(i,j+2);
          int tm = f.idx(i,j+1);
          int tr = f.idx(i,j);

          int bl = f.idx(i-1,j+2);
          int bm = f.idx(i-1,j+1);
          int br = f.idx(i-1,j);

          
          rigid[tm].set(mid,   &rigid[bl], &rigid[bm], &rigid[br], bSwitch, false);
          rigid[tl].set(left,  &rigid[bl], &rigid[bm], &rigid[tm], bSwitch, true );
          rigid[tr].set(right, &rigid[tm], &rigid[bm], &rigid[br], bSwitch, true ); 

       
         for (int j=3;j<h;++j){
            int a = f.idx(i-1,j);
            int b = f.idx(i-1,j-1);
            int c = f.idx(i,j-1);
            int idx = f.idx(i,j);
            auto target = f.at(i,j);
            rigid[idx].set(target, &rigid[a],&rigid[b],&rigid[c], false,true);
         }
    
          bSwitch = !bSwitch;
        }
//
//        //the corner we will move
//       // Rig2 r( f.at(2,0), f.at(1,0), f.at(1,1));
//        
        f.at(2,0) = Constrain::Double( Constrain::Distance(f.at(1,0),f.at(2,0)), Constrain::Distance(f.at(1,1),f.at(2,0)), theta);
//        
//        rigid[ f.idx(2,0) ].setResult( f.at(2,0) );
//        
//
//        /*-----------------------------------------------------------------------------
//         *  DRAW
//         *-----------------------------------------------------------------------------*/


        Draw(f);

        for (int i=0;i<(w*h);++i) {
          cout << i << endl;
          Draw(rigid[i].eval(),1,0,0);
        }

 //     glColor3f(.3,.3,.3);
 //     glBegin(GL_TRIANGLES);
 //     for (int i=0;i<w;++i){
 //       for (int j=0;j<h;++j){
 //         int idxa = f.idx(i,j);
 //         int idxb = f.idx(i,j+1);
 //         int idxc = f.idx(i+1,j+1);
 //         int idxd = f.idx(i+1,j);

 //         auto a = rigid[idxa].result;
 //         auto b = rigid[idxb].result;
 //         auto c = rigid[idxc].result;
 //         auto d = rigid[idxd].result;

 //         TriNormal( a, b, c );
 //         TriNormal( c, d, a);

 //       }
 //     }
 //     glEnd();
    
  }
   

  
};


int main(){
                             
  MyApp app;
  app.start();
  
  return 0;

}
