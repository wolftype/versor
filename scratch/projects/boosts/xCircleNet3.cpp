/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet3.cpp
 *
 *    Description:  circle net for blending surfaces
 *
 *        Version:  1.0
 *        Created:  07/07/2015 13:58:14
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

//#include "draw/vsr_graph_draw.h"

using namespace vsr;
using namespace vsr::cga;


/*-----------------------------------------------------------------------------
 *  DATA
 *-----------------------------------------------------------------------------*/

struct MyData{
  Vec pos;
  Vec normal;
};


template<> void Renderable<CyclideQuad>::DrawImmediate(const CyclideQuad& s){
  for (int i=0;i<4;++i){
     auto tx = -Round::dir(s.tframe[i].bitan[0].undual()).copy<Vec>().unit();
     auto ty = -Round::dir(s.tframe[i].bitan[1].undual()).copy<Vec>().unit();
     auto tz = -Round::dir(s.tframe[i].bitan[2].undual()).copy<Vec>().unit();

     DrawAt( tx, s.tframe[i].frame.pos(), 1,0,0);
     DrawAt( ty, s.tframe[i].frame.pos(), 0,1,0);
     DrawAt( tz, s.tframe[i].frame.pos(), 0,0,1);
  }
}

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
  float fnum,amt,amt2,amt3,xfu,xfv = 0;
  float posA,posB,posC,posD;

  bool bFlipX, bFlipY, bFlipTx, bFlipTy;
  

  Frame fa, fb;
  Frame net;
  
  Frame control;

  CyclideQuad cyclide;

  bool bDrawA,bDrawB,bDrawC,bDrawD,bDrawE,bDrawF,bDrawForward,bDrawNormal,bDrawEdges;

  HEGraph<MyData> graph;
  vector<MyData> mesh;
  
  int width = 10; 
  int height = 10;
  float linewidth;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
   // gui(fnum,"num",3,10);
    gui(amt,"spinxy",-100,1000)(amt2,"theta",-100,1000)(amt3,"height",-100,100);//(bReset,"bReset");
    gui(xfu,"orientation",-1000,1000);
    //gui(posA)(posB)(posC)(posD);
    //gui(bDrawForward,"bDrawForward");
    gui(bDrawA, "circles")(bDrawB, "surface")(bDrawC,"const_coord")(bDrawD, "edges")(bDrawE,"z");//(bDrawF)(bDrawNormal);
    gui(linewidth,"linewidth",.1,10);
   // gui(bDrawEdges,"bDrawEdges");
    
    control.rot( Biv::yz * PIOVERFOUR/2.0 + Biv::xz * .01 );

    objectController.attach(&control);
    objectController.attach(&fa);
    objectController.attach(&fb);

    fa.pos(0,3,0);
    fb.pos(0,0,0);
    fb.scale(2);

    mesh = vector<MyData>( (width+1)*(height+1));
    graph.UV( width+1,height+1,mesh);

    bDrawA = bDrawB = bDrawC = bDrawD = bDrawE = bDrawF = bDrawNormal = false;
    bDrawForward = 1;
    bFlipX = 1;
    xfu = xfv = 1;

    ps.bShadedOutput = false;
    fnum=4;

    amt3 = 3;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    glLineWidth(linewidth);

    auto sphA = fa.bound();
    auto sphB = fb.bound();
    

    Circle cir_top = (sphA ^ Dlp(0,1,0,amt3).rotate( Biv::xy * amt ) ).dual();
    

    auto pa = Construct::point(cir_top, PI);
    auto pb = Construct::point(cir_top, amt2);

   
    auto tan3a = Tangent::at( sphA.undual(), pa ).dual();
    auto tan3b = Tangent::at( sphA.undual(), pb ).dual();
    auto tan1a = Tangent::at( cir_top, pa );

    //tan3a.print();

    //plunge!
    auto pd = Construct::pointA( ((sphB^tan3a).dual() ^ sphB).dual() );
    auto pc = Construct::pointA( ((sphB^tan3b).dual() ^ sphB).dual() );


    Draw( sphB^tan3a, 1, 0, 1);
    Draw( sphB^tan3b, 1, 0, 1);
    Draw( pa ^ sphA ^ sphB, 0, 1, 0);

    
   Circle cir = pb ^ pa ^ pd;
   Draw(cir,.3,0,0);

   auto pc2 = Construct::pointA( (sphB^cir.dual()).dual() );

   Draw(pa,0,1,1,1);
   Draw(pb,0,1,1,1);
   Draw(pc,0,1,1,1);
   Draw(pd,0,1,1,1);
  
 
     //positions
    cyclide.pos(pa, pb, pc, pd);
 
   
   Rot trotA = Gen::ratio( Vec::z, -Round::dir(tan3a).copy<Vec>().unit()); 
   Rot trotB = Gen::ratio( Vec::x.spin(trotA), -Round::dir(tan1a).copy<Vec>().unit());
   Rot trotC = Gen::rot( ( Vec::x.spin(trotB*trotA) ^ Vec::y.spin(trotB*trotA) ) * xfu );

   cyclide.tframe[0].frame.rot() = trotC*trotB*trotA; 


    //calculate frames and log
    cyclide.frame();
    cyclide.log();

    Draw(cyclide);

    //evaluate circle edges
    if (bDrawA){

       for (int j=0;j<=10;++j){
         auto t = (float)j/10;
        
         auto cirU = cyclide.cirU(t);
         auto cirV = cyclide.cirV(t);

         Draw(cirU,1,0,.2);
         Draw(cirV,0,1,.2);
       }
    }

    //evaluate points
    if (bDrawB){

      for (int i =0;i<=width;i++){ 
       auto tu = (float)i/width;
       for (int j =0;j<=height;j++){ 
         auto tv = (float)j/height;
        
         auto np = cyclide.eval(tu,tv);
         auto tan = cyclide.apply( cyclide.tframe[0].frame.tz(), tu,tv);

         int idx = i*(height+1) + j;
         mesh[idx].pos = np;
         mesh[idx].normal = -Round::dir(tan).copy<Vec>().unit();
        // Draw(np,0,1,1);
       }
      }

 //    glBegin(GL_TRIANGLES);
 //    for (auto& i : graph.face()){
 //         auto& a = i->a();
 //         auto& b = i->b();
 //         auto& c = i->c(); 
 //         glColor4f(.2,1,.2,.7);
 //         GL::normal( a.normal.begin() );
 //         GL::vertex( a.pos.begin() );
 //         GL::normal( b.normal.begin() );
 //         GL::vertex( b.pos.begin() );
 //         GL::normal( c.normal.begin() );
 //         GL::vertex( c.pos.begin() );
 //    }
 //    glEnd();

     Draw(graph,.2,1,.2,.7);

    }

   for (int i=0;i<4;++i){

     auto tx = -Round::dir(cyclide.tframe[i].bitan[0].undual()).copy<Vec>().unit();
     auto ty = -Round::dir(cyclide.tframe[i].bitan[1].undual()).copy<Vec>().unit();
     auto tz = -Round::dir(cyclide.tframe[i].bitan[2].undual()).copy<Vec>().unit();
   
//     DrawAt( tx, cyclide.tframe[i].frame.pos(), 1, .2, 0);
//     DrawAt( ty, cyclide.tframe[i].frame.pos(), .2, 1, 0);
//     DrawAt( tz, cyclide.tframe[i].frame.pos(), .1, .2, 1);
// 
  }
 
    Draw(sphA,0,1,1,.3);
    Draw(sphB,0,1,1,.3);


  for (int i=0;i<4;++i){
     
     if (bDrawC){
       if (i&1) {
         Draw( cyclide.tframe[i].sphere[1], .1, 1, .3, .2);
       }
       else {
         Draw( cyclide.tframe[i].sphere[0], 1, .2, .2, .2);
       }
       Draw (cyclide.tframe[i].frame.pos(),.2,0,0);
     }

     if ( bDrawD){
       if (i&1) {
         Draw( cyclide.tframe[i].circle[0],1,0,0 );
       }
       else {
         Draw( cyclide.tframe[i].circle[1],0,1,0 );
       }
     }

     if (bDrawE){
       if (i&1){
        Draw( cyclide.tframe[i].sphere[2], 0,1,1,.2);
       } else{
        Draw( cyclide.tframe[i].sphere[2], .5,.5,1,.2);
       }
     }
     
   }



   }
 
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
