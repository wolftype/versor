/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet2.cpp
 *
 *    Description:  clean up of version 1
 *
 *        Version:  1.0
 *        Created:  06/29/2015 12:37:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_graph.h"  
#include "form/vsr_cyclide.h"

#include "draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;


/*-----------------------------------------------------------------------------
 *  Surface DATA
 *-----------------------------------------------------------------------------*/

struct MyData{
  Vec pos;
  Vec normal;
};


/*-----------------------------------------------------------------------------
 *  Surface DRAW METHOD (immediate mode)
 *-----------------------------------------------------------------------------*/
   template<> 
   void Renderable<vsr::HEGraph<MyData>>::DrawImmediate( const vsr::HEGraph<MyData>& graph){
     glBegin(GL_TRIANGLES);
     for (auto& i : graph.face()){
          auto& a = i->a();
          auto& b = i->b();
          auto& c = i->c(); 
          //glColor4f(.2,1,.2,.7);
          GL::normal( a.normal.begin() );
          GL::vertex( a.pos.begin() );
          GL::normal( b.normal.begin() );
          GL::vertex( b.pos.begin() );
          GL::normal( c.normal.begin() );
          GL::vertex( c.pos.begin() );
     }
     glEnd();
  }

/*-----------------------------------------------------------------------------
 *  APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float fnum,amt,amt2,xfu,xfv, amtW = 0;
  float posA,posB,posC,posD;

  bool bFlipX, bFlipY, bFlipTx, bFlipTy;
  
  Frame net;
  
  Frame control;

  CyclideQuad cyclide;

  bool bDrawA,bDrawB,bDrawC,bDrawD,bDrawE,bDrawF,bDrawForward,bDrawNormal,bDrawEdges,bDrawOrtho3;

  HEGraph<MyData> graph;
  vector<MyData> mesh;
  
  int width = 10; 
  int height = 10;


  float linewidth,offset;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(fnum,"num",3,10);
    gui(amt,"amt",-100,1000)(amt2,"amt2",-100,1000)(bReset,"bReset");
    gui(xfu,"xfu",-1000,1000);
    gui(xfv,"xfv",-1000,1000);
    gui(amtW,"amtW",-1000,1000);
    gui(posA)(posB)(posC)(posD);
    gui(bDrawForward,"bDrawForward");
    gui(bDrawA, "coordinate curves")(bDrawB, "surface")(bDrawC,"coordinate surface")(bDrawD,"edges")(bDrawE,"z")(bDrawF)(bDrawNormal);
    gui(bDrawEdges,"bDrawEdges");
    gui(bDrawOrtho3,"bDrawOrtho3");
    gui(offset,"offset",-10,10);
    gui(linewidth, "linewidth",0,100);
    
    control.rot( Biv::yz * PIOVERFOUR);

    objectController.attach(&control);

    amt = 2;
   // posA = .75 ; posB = .5 ; posC = .25 ; posD = 0;
    posA = posB = posC = posD = 0;

    mesh = vector<MyData>( (width+1)*(height+1));
    graph.UV( (width+1),height+1,mesh);


    bDrawA = bDrawB = bDrawC = bDrawD = bDrawE = bDrawF = bDrawNormal = false;
    bDrawForward = 1;
    bFlipX = 1;
    xfu = xfv = 1;

    ps.bShadedOutput = false;
    fnum=4;

    mColor.set(.8,.8,.8);
  }

  void reset(){
    control.rot(Biv::yz * PIOVERFOUR);
  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

   // if (bReset){ reset(); }

    glLineWidth(linewidth);
    //red NET
    net.scale(amt);
    Draw(net.cxy(),1,0,0);

    cyclide.tframe[0].frame = control;

    //positions
    float pos[] = {posA, posB, posC, posD };
    for (int i=0;i<4;++i){
      cyclide.tframe[i].frame.pos() = Round::point( net.cxy(), ((float)i/4 + pos[i] )*TWOPI);
    }

    //calculate frames and log
    cyclide.frame();
    cyclide.log();
    cyclide.logZ(); //z direction log


    //evaluate and draw circle edges
    if (bDrawA){
       for (int j=0;j<=10;++j){
         auto t = (float)j/10;
        
         auto cirU = cyclide.cirU(t);
         auto cirV = cyclide.cirV(t);

         Draw(cirU,1,0,.2);
         Draw(cirV,0,1,.2);
       }
    }

    //evaluate points of mesh
    if (bDrawB){

      for (int i =0;i<=10;i++){ 
       auto tu = (float)i/10;
       for (int j =0;j<=10;j++){ 
         auto tv = (float)j/10;
        
         //point
         auto np = cyclide.eval(tu,tv);
         //tangent
         auto norm = cyclide.evalNormal(tu,tv);
         //offset
         auto tan = Gen::bst( cyclide.tframe[3].tan[2] * offset );
         auto tp = cyclide.tframe[0].frame.pos().spin ( tan );
         auto offset = Round::location( cyclide.apply(tp,tu,tv) );
         
         mesh[i*(height+1)+j].pos = np;
         mesh[i*(height+1)+j].normal = norm;

         Draw(np,0,1,1);
         Draw(offset,1,0,1);

        
         if (bDrawOrtho3){

             if (j==0){
              auto tc = cyclide.apply( cyclide.tframe[0].sphere[2], tu, tv );
              Draw(tc,0,1,1,.2);
             }
         }
       }
      }
    }

  for (int i=0;i<4;++i){

     auto tx = -Round::dir(cyclide.tframe[i].bitan[0].undual()).copy<Vec>().unit();
     auto ty = -Round::dir(cyclide.tframe[i].bitan[1].undual()).copy<Vec>().unit();
     auto tz = -Round::dir(cyclide.tframe[i].bitan[2].undual()).copy<Vec>().unit();

     DrawAt( tx, cyclide.tframe[i].frame.pos(), 1, .2, 0);
     DrawAt( ty, cyclide.tframe[i].frame.pos(), .2, 1, 0);
     DrawAt( tz, cyclide.tframe[i].frame.pos(), .1, .2, 1);
     
     //SPHERE SURFACES
     if (bDrawC){
       if (i&1) {
         Draw( cyclide.tframe[i].sphere[1], .1, 1, .3, .2);
       }
       else {
         Draw( cyclide.tframe[i].sphere[0], 1, .2, .2, .2);
       }
       Draw (cyclide.tframe[i].frame.pos(),.2,0,0);
     }

     //CIRCLE EDGES
     if ( bDrawD){
       if (i&1) {
         Draw( cyclide.tframe[i].circle[0],1,0,0 );
       }
       else {
         Draw( cyclide.tframe[i].circle[1],0,1,0 );
       }
     }

     //SPHERE SURFACE Z
     if (bDrawE){
       if (i&1){
        Draw( cyclide.tframe[i].sphere[2], 0,1,1,.2);
       } else{
        Draw( cyclide.tframe[i].sphere[2], .5,.5,1,.2);
       }
     }
     

   }

   if (bDrawB){
     Draw(graph,.2,.7,.2,.4);
   }


    if (bDrawF){

 //     auto ratU = Gen::ratio( tframe[0].sphere[2].dual(), tframe[2].sphere[2].dual() );
 //     auto ratV = Gen::ratio( tframe[3].sphere[2].dual(), tframe[1].sphere[2].dual() );
 //     auto logU = Gen::log( ratU  );
 //     auto logV = Gen::log( ratV  );

 //     auto curvature = Pair(logU % logV);
 //     auto k = tan.dual() <= curvature;
 //     k.print();

 //     Draw( logU.dual(),0,1,1);
 //     Draw( logV.dual(),.2,.5,1);
 //     Draw( tframe[0].sphere[2], 0, 1, 1, .2);
 //     Draw( tframe[2].sphere[2], 0, 1, 1, .2);
 //     Draw( logU.dual(),0,1,1);
 //     Draw( logV.dual(),.2,.5,1);   
 //     Draw( tframe[1].sphere[2], .2, .5, 1, .2);
 //     Draw( tframe[3].sphere[2], .2, .5, 1, .2);


    }   

   }
 
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
