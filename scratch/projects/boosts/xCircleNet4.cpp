  /*
 * =====================================================================================
 *
 *       Filename:  xCircleNet4.cpp
 *
 *    Description:  circle net for continuously blending surfaces
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
   void Renderable<vsr::HEGraph<MyData>,0>::DrawImmediate( const vsr::HEGraph<MyData>& graph){
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



struct MeshRender{

  static void Draw(const vector<MyData>& mesh, int w, int h, bool bNormals =false){

    bool bColor = false;
    glBegin(GL_QUADS);
    for (int i =0;i<w-1;++i){
      for (int j =0;j<h-1; ++j){
          int a = i*h+j;
          int b = a+h;
          int c = b+1;
          int d = a+1;
          
          glColor4f(bColor*.8,bColor,bColor,.5);
          GL::normal(mesh[a].normal.begin());
          GL::vertex(mesh[a].pos.begin());
          GL::vertex(mesh[b].pos.begin());
          GL::vertex(mesh[c].pos.begin());
          GL::vertex(mesh[d].pos.begin());    
          bColor = !bColor;
      }
      bColor = !bColor;
    }
    glEnd();

    if (bNormals){
      glColor3f(0,1,0);
      glBegin(GL_LINES);
      for (int i=0;i<mesh.size();++i){
       //DrawBAt( mesh[i].normal, mesh[i].pos, 0, 1, 0 );
       GL::vertex((mesh[i].pos).begin()) ;
       GL::vertex((mesh[i].pos+mesh[i].normal).begin()) ;
      }
      glEnd();
    }
  }
};


struct CyclideDraw{
  HEGraph<MyData> graph;
  vector<MyData> mesh;

  int width = 0;
  int height = 0;

  bool bDrawCyclide = true;
  bool bDrawPlunge = false;
  bool bDrawSurface = true;
  bool bDrawXYSurface = true;
  bool bDrawYZSurface = false;
  bool bDrawXZSurface = false;
  bool bDrawNormals = false;

  void init(int w, int h){
    width =w ; height =h;
    mesh = vector<MyData>( (width+1)*(height+1));
    graph.UV( width+1,height+1,mesh); 
  }

   void draw(const CyclideQuad& cyclide, int surf =0){

     if (width==0) init(20,20);

      if (bDrawCyclide) Draw(cyclide);
      if (bDrawPlunge) {
        Draw( cyclide.tframe[2].sphere[2].dual() ^ cyclide.tframe[0].tan[2], 1, 0, 1 );
        Draw( cyclide.tframe[2].sphere[2].dual() ^ cyclide.tframe[1].tan[2], 1, 0, 1 );
        Draw( cyclide.tframe[1].sphere[2].dual() ^ cyclide.tframe[0].tan[2], 1, 0, 1 );
        Draw( cyclide.tframe[1].sphere[2].dual() ^ cyclide.tframe[3].tan[2], 1, 0, 1 );
      }
     
      if (bDrawSurface){
      for (int i =0;i<=width;i++){ 
       auto tu = (float)i/width;
       for (int j =0;j<=height;j++){ 
         auto tv = (float)j/height;
        
         Point np;
         Pair tan;
         switch(surf){
          case 0:
            np = cyclide.eval(tu,tv);
            tan = cyclide.apply( cyclide.tframe[0].tan[2], tu,tv);
            break;
          case 1:
             np = cyclide.evalXZ(tu,tv);
             tan = cyclide.applyXZ( cyclide.tframe[0].tan[2], tu,tv);
             break;
          case 2:
             np = cyclide.evalYZ(tu,tv);
             tan = cyclide.applyYZ( cyclide.tframe[0].tan[2], tu,tv);
             break;
         }

         int idx = i*(height+1) + j;
         mesh[idx].pos = np;
         mesh[idx].normal = -Round::dir(tan).copy<Vec>().unit();
       }
      }
     }

    // Draw(graph,.2,1,.2,.7);   
        
     MeshRender::Draw(mesh,width+1,height+1,bDrawNormals);
   }
};



/*-----------------------------------------------------------------------------
 *  APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : App {
 
  CyclideDraw cyclideDraw;
  //Some Variables
  bool bReset = false;
  float fnum,amt,amt2,amt3,xfu,xfv = 0;
  float posA,posB,posC,posD;

  float camt2, camt3,alpha;

  bool bFlipX, bFlipY, bFlipTx, bFlipTy;
  

  Frame fa, fb, fc, fd, fe, ff;
  Frame net;
  
  Frame control;

  CyclideQuad cyclide, cyclide2,cyclide3;

  bool bDrawA,bDrawB,bDrawC,bDrawD,bDrawE,bDrawF,bDrawForward,bDrawNormal,bDrawEdges,bDrawCircleNet;
  bool bDrawCyclide, bDrawPlunge, bDrawSpheres,bDrawSpheresAround,bDrawSurface,bSmooth,bDrawNormals;

  HEGraph<MyData> graph;
  vector<MyData> mesh;

  HEGraph<MyData> graph2;
  vector<MyData> mesh2;

  HEGraph<MyData> graph3;
  vector<MyData> mesh3;

  float flatness;
  int width = 10; 
  int height = 10;
  float linewidth;
  float lightX, lightY, lightZ ;
 
  bool bAddLeft,bAddFront,bAddBottom,bAddTop;

  bool bFlipLeft, bFlipFront, bFlipBottom, bFlipTop,bFlipLog;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){

    ///Bind Gui
    bindGLV();
    gui(amt,"spinxy",-100,1000)(amt2,"theta",-100,1000)(amt3,"height",-100,100);//(bReset,"bReset");
    gui(xfu,"orientation",-1000,1000);
    gui(flatness,"flatness",-20,20);
    gui(camt2, "camt2",-PI,PI);
    gui(camt3, "camt3",-PI,PI);

    gui(bAddLeft,"bAddLeft"); gui(bFlipLeft,"bFlipLeft");
    gui(bAddFront,"bAddFront");  gui(bFlipFront,"bFlipFront");
    gui(bAddBottom,"bAddBottom");  gui(bFlipBottom,"bFlipBottom");
    gui(bAddTop,"bAddTop");  gui(bFlipTop,"bFlipTop");
    gui(bFlipLog,"bFlipLog");

    gui(bDrawA, "circles")(bDrawB, "surface")(bDrawC,"const_coord")(bDrawD, "edges")(bDrawE,"z");//(bDrawF)(bDrawNormal);
    gui(bDrawCircleNet,"bDrawCircleNet");
    gui(bDrawCyclide,"bDrawCyclide");
    gui(bDrawPlunge,"bDrawPlunge");
    gui(bDrawSpheres,"bDrawSpheres");
    gui(bDrawSurface,"bDrawSurface");
    gui(bDrawSpheresAround,"bDrawSpheresAround");
    gui(bSmooth,"bSmooth");
    gui(bDrawNormals,"bDrawNormals");
    
    gui(linewidth,"linewidth",.1,10);
    gui(ps.bShadedOutput, "bshaded"); 
    gui(ps.bSortOutput, "bsorted"); 
    gui(ps.bOccludeOutput, "bOcclude"); 
    gui(ps.bOffsetOutput, "bOffset"); 
    gui(alpha,"alpha");
    gui(lightX, "lightX",-10,10);
    gui(lightY, "lightY",-10,10);
    gui(lightZ, "lightZ",-10,10);
    
    control.rot( Biv::yz * PIOVERFOUR/2.0 + Biv::xz * .01 );

    objectController.attach(&control);
    objectController.attach(&fa);
    objectController.attach(&fb);
    objectController.attach(&fc);
    objectController.attach(&fd);
    objectController.attach(&fe);
    objectController.attach(&ff);

    fa.pos(0,3,0);
    fb.pos(0,0,0);
    fc.pos(2,3,0);
    fd.pos(-2,3,0);
    fe.pos(0,6,0);
    ff.pos(0,-3,0);
    fb.scale(2);
    

    bDrawA = bDrawB = bDrawC = bDrawD = bDrawE = bDrawF = bDrawNormal = false;
    bDrawForward = 1;
    bFlipX = 1;
    xfu = 0;
    xfv = 1;

    fnum=4;
    
    amt = 0;
    amt3 = 3;

    bDrawSurface = bDrawSpheres = true;
    alpha = 1;
    lightZ = 5;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){


    GL::lightPos( lightX, lightY, lightZ );
    
    glLineWidth(linewidth);
    gl2psLineWidth(linewidth);
    gl2psPointSize(5);

    auto ptA =  Construct::point( fa.bound(), Vec::z ); Draw(ptA,1,0,0);
    auto ptB =  Construct::point( fb.bound(), Vec::z );
    auto sphA = fa.bound().spin( Gen::bst( Pair(Tnv(0,0,1)).trs(ptA) * flatness ) );
    auto sphB = fb.bound().spin( Gen::bst(  Pair(Tnv(0,0,1)).trs(ptB) * flatness ) ) ;

   // sphA = fa.dxy();
   // sphB = fb.dxy();
    
    auto sphC = fc.bound();
    auto sphD = fd.bound();
    auto sphE = fe.bound();
    auto sphF = ff.bound();
    
    Circle cir_top = (sphA ^ Dlp(0,1,0,amt3).rotate( Biv::xy * amt ) ).dual();
    
    auto pa = Construct::point(cir_top, PI);
    auto pb = Construct::point(cir_top, amt2);

    pa = Construct::point( -1,3,0);
    pb = Construct::point( 1,3,0);


    //Contact Element at pa and pb on SphA
    Contact ca(pa,sphA);
    Contact cb(pb,sphA); 

    //Contact Element connecting cb and ca to SphB
    Contact cc(cb,sphB);
    Contact cd(ca,sphB);
   
    //Orientation of Base Patch
    Rot trotA = Gen::ratio( Vec::z, -Round::dir(ca.tnv).copy<Vec>().unit()); 
    Rot trotB = Gen::ratio( Vec::x.spin(trotA), Vec( Op::project( Vec( cd.point-ca.point), Biv::xy.spin(trotA)) ).unit() );// -Round::dir(tan1a).copy<Vec>().unit());
    Rot trotC = Gen::rot( ( Vec::x.spin(trotB*trotA) ^ Vec::y.spin(trotB*trotA) ) * xfu );

    //default positions
    cyclide.pos(ca.point, cb.point, cc.point, cd.point); 
    cyclide.tframe[0].frame.rot() = trotC*trotB*trotA; 
    cyclide.frame();
    cyclide.log();

    //Add Base Cyclide to Circular Net
    CircularNet net(cyclide);

    //add spheres to net idx, side b, whether it is a corner? (false if smooth)
    if (bSmooth){
        net.addAt(sphC, 0, 1, false);
        if (bAddBottom)  {
          net.addAt(sphD, 0, 3, false);
        }
        if (bAddTop) {
          net.addAt(sphE, 0, 0, false);
          int n = net.size()-1;
          net.addAt(sphC, n, 1, false);
          if (bAddBottom) net.addAt(sphD, n, 3, false);
        }
        if (bAddFront){
           net.addAt(sphF, 0, 2, false); 
           int n = net.size()-1;
           net.addAt(sphC, n, 1, false);
           if (bAddBottom) net.addAt(sphD, n, 3, false);
        }
    
    } else {
    
     //add sphC to left edge
     net.addAt( sphC, 0, 1, true);

     //take its bitangent and find plunge into sphD location
     auto sphTop = (net[1][1].bitan[1] ^ Round::location(sphD)).undual();
     
     if (bAddTop) {
       net.addAt( sphTop, 0, 0, true,bFlipTop);

     
       //record some circles to calculate fourth corner    
       auto cirA =  net[0][0].frame.pos() ^ net[0][3].frame.pos() ^ net[2][0].frame.pos();
       auto cirB =  net[0][3].frame.pos() ^ net[0][2].frame.pos() ^ net[1][2].frame.pos();
       auto cirC =  net[2][0].frame.pos() ^ net[2][1].frame.pos() ^ net[1][2].frame.pos();

       auto par = (cirA.dual() ^ Round::surround(cirB) ).dual();
       auto p4a = Construct::pointA( par );
       auto p4b = Construct::pointB( par );
       auto p4 = FERROR( (p4a<=net[0][3].frame.pos())[0] ) ? p4b : p4a;

       auto sphLeft = ( net[2][0].bitan[0] ^ p4).undual();
       auto sphBottom = ( net[1][2].bitan[1] ^ p4 ).undual();
       auto sphFront = ( net[1][2].bitan[0] ^ p4 ).undual();

       if (bAddLeft) net.addAt(sphLeft, 0, 3, true);
       if (bAddBottom)  net.addAt(sphBottom, 0, 2, true);
       if (bAddFront) net.addAt(sphFront, 2, 0, true,bFlipFront);

       //Draw these sphere coordinates
       if (bDrawSpheresAround){
          if (bAddLeft) Draw( sphLeft, 0, 1, 1, alpha);
          if (bAddBottom) Draw( sphBottom,0,1,1,alpha);
          if (bAddTop) Draw( sphTop,0,1,1,alpha);
          if (bAddFront) Draw( sphFront,0,1,1,alpha);
       }
  
       if (bDrawCircleNet){
        Draw(cirA,0,1,0);
        Draw(cirB,0,1,0);
        Draw(cirC,0,1,0);
        Draw( p4,1,0,0);
       }
 
     }
   
     
    }

    cyclideDraw.bDrawPlunge = bDrawPlunge;
    cyclideDraw.bDrawCyclide = bDrawCyclide;
    cyclideDraw.bDrawSurface = bDrawSurface;
    cyclideDraw.bDrawNormals = bDrawNormals;

    for (auto& i : net.mCyclide){
      cyclideDraw.draw(i);
    }

 //   if (net.mCyclide.size() > 3 ){
 //     for (int i =0;i<=20;i++){ 
 //      auto tu = (float)i/20;
 //      for (int j =0;j<=20;j++){ 
 //        auto tv = (float)j/20;
 //        auto p1 = net[1].eval(tu,tv);
 //        auto tan3 = net[3].apply(net[3][0].tan[2], tu, 1-tv);
 //        auto k = p1 ^ tan3 ;
 //        Draw(k,1,.2,tv);
 //      }
 //     }
 //   }

if (net.mCyclide.size() > 3 ){

//    for (int i =0;i<=10;++i){
//      auto tu = (float)i/10;
//      auto sph1u = net[0].apply( net[0][0].sphere[2], tu, 0);
//      auto sph2u = net[3].apply( net[3][0].sphere[2], 1-tu, 0);
//      auto zlogu = Gen::log( (sph2u/sph1u).runit(), bFlipLog) * -.5;
//
//
//      for (int j=0;j<=10;++j){
//        auto tv = (float)j/10;
//        auto sph1v = net[0].apply( net[0][3].sphere[2], 0, tv);
//        auto sph2v = net[3].apply( net[3][3].sphere[2], 0, tv);
//        auto zlogv = Gen::log( (sph2v/sph1v).runit() ) * -.5;
//
//       // auto bst = Gen::bst(zlogu
//        auto p = net[0].eval(tu,tv);
//      //  auto zlog = Gen::log((sph2/sph1).runit()) * .5;
//       for (int k=0;k<=10;++k){
//           auto tw = (float)k/10;
//           //auto zbst = net[1].xfv(tw);
//         //  if (i==0 || i==10 || j==0 || j==10){
//           auto np = Round::location(p.spin( Gen::bst(zlogu*tw) ) );
//           Draw (np,1,0,0);
//         //  }  
//       }
//      }
//      if (i==0 || i==10 ) {
//        Draw(sph2u, 0, 1,0, .1);
//        Draw(sph1u, 1, 1,0, .1);
//        }
//    }
  
}

  
    if (bDrawSpheres){
     Draw(sphA,0,1,1,alpha);
     Draw(sphB,0,1,1,alpha);
     Draw(sphC,0,1,1,alpha);
     //Draw(Round::locasphD,0,1,1,.3);

     if (bSmooth){
     if (bAddBottom) Draw(sphD,0,1,1,alpha);
     if (bAddTop) Draw(sphE,0,1,1,alpha);
     if (bAddFront)  Draw(sphF,0,1,1,alpha);
     }
    }
 
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


   for (int i=0;i<4;++i){

     auto tx = -Round::dir(cyclide.tframe[i].bitan[0].undual()).copy<Vec>().unit();
     auto ty = -Round::dir(cyclide.tframe[i].bitan[1].undual()).copy<Vec>().unit();
     auto tz = -Round::dir(cyclide.tframe[i].bitan[2].undual()).copy<Vec>().unit();
   
  }
 

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
