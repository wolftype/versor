/*
 * =====================================================================================
 *
 *       Filename:  xCircleNet.cpp
 *
 *    Description:  circle net (a net of contact elements)
 *
 *        Version:  1.0
 *        Created:  06/16/2015 19:31:58
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

using namespace vsr;
using namespace vsr::cga;


/*-----------------------------------------------------------------------------
 *  DATA
 *-----------------------------------------------------------------------------*/

struct MyData{
  
  Vec pos;
  Vec normal;
};

/*-----------------------------------------------------------------------------
 *  APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt,xfu,xfv = 0;
  float posA,posB,posC,posD;

  bool bFlipX, bFlipY, bFlipTx, bFlipTy;
  
  Frame net;
  Frame fa,fb,fc,fd;

  bool bDrawA,bDrawB,bDrawC,bDrawD,bDrawE,bDrawF,bDrawForward,bDrawNormal,bDrawEdges;

  HEGraph<MyData> graph;
  vector<MyData> mesh;
  
  int width = 10;
  int height = 10;
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");
    gui(xfu,"xfu",-100,100);
    gui(xfv,"xfv",-100,100);
    gui(posA)(posB)(posC)(posD);
    gui(bFlipX,"flipX")(bFlipY,"flipY")(bFlipTx,"bFlipTx")(bFlipTy,"bFlipTy")(bDrawForward,"bDrawForward");
    gui(bDrawA)(bDrawB)(bDrawC)(bDrawD)(bDrawE)(bDrawF)(bDrawNormal);
    gui(bDrawEdges,"bDrawEdges");
    gui(ps.bShadedOutput, "shaded output");
    
    fa.rot( Biv::yz * PIOVERFOUR/2.0 + Biv::xz * .01 );

    objectController.attach(&fa);

    amt = 5;
    posA = .75 ; posB = .5 ; posC = .25 ; posD = 0;

    mesh = vector<MyData>( (width+1)*(height+1));
    graph.UV( (width+1),height+1,mesh);


    bDrawA = bDrawB = bDrawC = bDrawD = bDrawE = bDrawF = bDrawNormal = false;
    bDrawForward = 1;
    bFlipX = 1;
    xfu = xfv = 1;
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    
    net.scale(amt);

    //GREEN NET
    Draw(net.cxy(),.2,0,0);

    fa.pos() = Construct::point( net.cxy(), posA*TWOPI);
    fb.pos() = Construct::point( net.cxy(), posB*TWOPI);
    fc.pos() = Construct::point( net.cxy(), posC*TWOPI);
    fd.pos() = Construct::point( net.cxy(), posD*TWOPI);

//    auto ta = fa.cxy(); 
//    auto tb = fb.cxy(); 
//    auto tc = fc.cxy(); 
//    auto td = fd.cxy(); 

//    Draw(ta);
//    Draw(tb);
//    Draw(tc);
//    Draw(td);

    auto ra = fa.tz();

    //theorem 3 of bobenko and huhnen-venedy -- only need one frame
    //b and f tangents, a and c positions
    auto saz =(ra ^ fb.pos());
    auto sa =(fa.ty() ^ fb.pos() );
    
    auto rbz = Tangent::at( saz, fb.pos() );
    auto rb = Tangent::at( sa, fb.pos() );
    
    auto rotA = Gen::ratio( Vec::z, Round::dir(rbz).copy<Vec>().unit() );
    auto rotB = Gen::ratio( Vec::y.spin(rotA), Round::dir(rb).copy<Vec>().unit() );
    fb.rot() = rotB * rotA;
    
    auto sbz = (rbz ^ fc.pos() );
    auto sb =( fb.tx(-1) ^ fc.pos());

    auto rcz= Tangent::at( sbz, fc.pos() );
    auto rc = Tangent::at( sb, fc.pos() ); 
 
    rotA = Gen::ratio( Vec::z, -Round::dir(rcz).copy<Vec>().unit() );
    rotB = Gen::ratio( Vec::x.spin(rotA), Round::dir(rc).copy<Vec>().unit() );
    fc.rot() = rotB * rotA;
    
    auto scz = (rcz^fd.pos());
    auto sc =( fc.ty() ^ fd.pos());

    auto rdz = Tangent::at( scz,fd.pos() );
    auto rd = Tangent::at( sc, fd.pos() );

    rotA = Gen::ratio( Vec::z, Round::dir(rdz).copy<Vec>().unit() );
    rotB = Gen::ratio( Vec::y.spin(rotA), Round::dir(rd).copy<Vec>().unit() );
    fd.rot() = rotB * rotA;
    
    auto sd =( fd.tx(-1) ^ fa.pos());


    sa = sa.runit();
    sb = sb.runit();
    sc = sc.runit();
    sd = sd.runit();

    //Frames
//    Draw(fa);
//    Draw(fb);
//    Draw(fc);
//    Draw(fd);

    
    auto sphAx = (fa.tx().dual() ^ fb.pos()).dual();
    auto sphCx = (fd.tx( bFlipTx ? 1 : -1 ).dual() ^ fc.pos()).dual();

    auto sphDy = (fa.ty().dual() ^ fd.pos()).dual();
    auto sphBy = (fb.ty( bFlipTy ? 1 : -1 ).dual() ^ fc.pos()).dual();

    //bisection
    Dlp bisectA = fb.pos() - fa.pos();
    Dlp bisectB = fb.pos() - fc.pos();
    Dlp bisectC = fc.pos() - fd.pos();
    Dlp bisectD = fd.pos() - fa.pos();

    auto midpointA = Construct::pointB(( bisectA ^ sa.dual()).dual());
    auto midpointB = Construct::pointB(( bisectB ^ sb.dual()).dual());
    auto midpointC = Construct::pointA(( bisectC ^ sc.dual()).dual());
    auto midpointD = Construct::pointB(( bisectD ^ sd.dual()).dual());

  //  Draw( midpointA, 1,1,0);
  //  Draw( midpointB, 1,1,0);
  //  Draw( midpointC, 1,1,0);
  //  Draw( midpointD, 1,1,0);

    //contact
    auto midcontactA = Tangent::at( sphAx.dual(), midpointA ).dual() ^ midpointC;
    auto midcontactB = Tangent::at( sphBy.dual(), midpointB ).dual() ^ midpointD;

 //   Draw(midcontactA,1,0,1);
 //   Draw(midcontactB,1,0,1);

    //sphere contact
    auto midsphereA = (Tangent::at( sa, midpointA).dual() ^ midpointC).dual();
    auto midsphereB = (Tangent::at( sb, midpointB).dual() ^ midpointD).dual();

    //is corner inside or outside principle spheres on opposite edges
    auto dotX =  ( fa.pos() <= sphCx )[0];
    auto dotY =  ( fa.pos() <= sphBy )[0];
    cout << "dot a:" << dotX << endl;
    cout << "dot b:" << dotY << endl;

   auto logAx = Gen::log( Gen::ratio( sphAx, sphCx, bFlipX ) );
   auto logBy = Gen::log( Gen::ratio( sphDy, sphBy, bFlipY ) );

   //auto halfLogAx1 = Gen::log( Gen::ratio(sphAx, Dls( Round::carrier( logAx.dual() ).dual() ), bFlipX));
   auto halfLogAx1 = Gen::log( Gen::ratio(sphAx, Dls(sphCx-sphAx), bFlipX));
   auto halfLogAx2 = Gen::log( Gen::ratio(Dls( Round::carrier( logAx.dual() ).dual()), sphCx, bFlipX));

   auto pairX = logAx.runit();
   auto pairY = logBy.runit();

  // Draw(pairX.dual(),0,1,0);
   auto thetaX = acos( (fa.x() <= fd.x()) [0]);
   auto thetaY = acos( (fa.y() <= fb.y()) [0]);

    cout << "theta x " << thetaX << endl;
    cout << "theta y " << thetaY << endl;

  //new midpoint method
//   auto logAx = Gen::log( Gen::ratio( sphAx, midsphereB ) );
//   auto logBy = Gen::log( Gen::ratio( sphDy, midsphereA ) );

    //along rotation axis
 //   auto xa = (sphAx ^ logBy).dual();
 //   Draw(xa,1,1,1);
    //along rotation axis
 //   auto xc = (sphCx ^ logBy).dual();
 //   Draw(xc,1,1,1);


 //  auto logT = Gen::log( Gen::ratio(sphAx, sphCx).unit() * Gen::ratio(sphDy,sphBy).unit() );


   auto sphAy = (fa.ty().dual() ^ fb.pos()).dual();
   auto sphCy = (fc.ty().dual() ^ fd.pos()).dual();

   auto sphAz = (fa.tz().dual() ^ fb.pos()).dual();
   auto sphCz = (fc.tz().dual() ^ fd.pos()).dual();

   auto sphBx = (fb.tx().dual() ^ fc.pos()).dual();
   auto sphDx = (fd.tx().dual() ^ fa.pos()).dual();

   auto sphBz = (fb.tz().dual() ^ fc.pos()).dual();
   auto sphDz = (fd.tz().dual() ^ fa.pos()).dual();


   auto logAz = Gen::log( Gen::ratio( sphAz, sphCz ) );
   auto logBz = Gen::log( Gen::ratio( sphDz, sphBz ) );


   auto logAy = Gen::log( Gen::ratio( sphAy, sphCy ) );
   auto logBx = Gen::log( Gen::ratio( sphDx, sphBx ) );
    
   cout << "wt a: " << logAx.wt() << " " << logAx.rnorm() << endl;
   cout << "wt b: " << logBy.wt() << " " << logBy.rnorm() << endl;

//  auto dirX = Round::dir(logAx.dual()).copy<Biv>();
//  cout << "sign a1: " << Op::sign( dirX, fa.yz() ) << endl;
//  cout << "sign a2: " << Op::sign( dirX, fb.yz() ) << endl;
//  cout << "sign a3: " << Op::sign( dirX, fc.yz() ) << endl;
//  cout << "sign a4: " << Op::sign( dirX, fd.yz() ) << endl;
//
//  auto dirY = Round::dir(logBy.dual()).copy<Biv>();
//  cout << "sign b1: " << Op::sign( dirY, fa.xz() ) << endl;
//  cout << "sign b2: " << Op::sign( dirY, fb.xz() ) << endl;
//  cout << "sign b3: " << Op::sign( dirY, fc.xz() ) << endl;
//  cout << "sign b4: " << Op::sign( dirY, fd.xz() ) << endl;
  
   cout << "size a: " << Round::size(logAx,true) << endl;
   cout << "size b: " << Round::size(logBy,true) << endl;
    


    //COMMUTATOR
    auto db = Pair( logAz % logBz );
    db.print();

    //I velocity
    auto dIx = Biv( fa.xy() % logAz );
    dIx.print();



//   Draw( Round::surround ( logA.dual() ) ^ sphA.dual() ^ sphC.dual(),1,1,0 );

   auto sconfA = Gen::bst( logAz  * -xfu);
   auto sconfB = Gen::bst( logBz * -xfv);
   auto sconfC = Gen::bst( logAx * -xfu );
   auto sconfD = Gen::bst( logBy * -xfv );
   auto sconfE = Gen::bst( logAy * -xfu );
   auto sconfF = Gen::bst( logBx * -xfv );
   
   auto tsa = sphAz.spin( sconfA );
   auto tsb = sphDz.spin( sconfB );
   auto tsc = sphAx.spin( sconfC );
   auto tsd = sphDy.spin( sconfD );
   auto tse = sphAy.spin( sconfE );
   auto tsf = sphDx.spin( sconfF );


//    if (bFlipX){
//      auto tmpCir = logAx.dual();
//      auto dir = Round::dir(tmpCir).copy<Biv>();
//      logAx = Round::produce( Round::surround( tmpCir ), dir ).dual();
//    }
   // logAx = logAx.runit();
   // logBy = logBx.runit();

 //   auto bisectX = Round::carrier( sa ).dual().unit() - Round::carrier( sc ).dual().unit();
 //   auto bisectY = Round::carrier( sd ).dual().unit() - Round::carrier( sb ).dual().unit();

        
    for (int i=0;i<=width;++i){
      float tu=  xfu * (float)i/width;
      auto tbstA = Gen::bst( logAx * -tu );
      
      //one dir
      auto tcirA = sa.spin(tbstA);
      auto tpnt = Round::loc( fa.pos().spin(tbstA) );

      //another
      auto tbstA2 = Gen::bst( pairX * thetaX * -tu );
      auto tcirA2 = sa.spin(tbstA2);
      auto tpnt2 =  Round::loc( fa.pos().spin(tbstA2) );

      if (bDrawForward){
     //   Draw(tpnt);
     //   Draw(tcirA,0,1,1);

      } else{
     //   Draw(tcirA2,0,1,1);
     //   Draw(tpnt2);
      }
   
      for (int j=0;j<=height;++j){
        float tv=  xfv * (float)j/height;
        auto tbstB = Gen::bst( logBy * -tv );
        auto tcirB = sd.spin(tbstB);
        auto tpnt = Round::loc( fa.pos().spin(tbstB) );
        
        // Draw(tcirB,0,1,1);
        // Draw(tpnt);
        auto nbst = tbstA * tbstB;//Gen::bst( logAx * -tu + logBy * -tv);
        auto mesh_pnt = Round::loc( fa.pos().spin( nbst) );// tbstA * tbstB) );
        auto tanA =  Round::dir( Tangent::at( tcirA, mesh_pnt ) ).copy<Vec>().unit();
        auto tanB = Round::dir( Tangent::at( tcirB, mesh_pnt ) ).copy<Vec>().unit();

        auto normal_vec = -Round::dir( fa.tz().spin( nbst ) ).copy<Vec>().unit();
        auto normal_biv = tanA ^ tanB;
        
        int idx = i * (height+1) + j;
        mesh[idx].pos = mesh_pnt;
        mesh[idx].normal = normal_vec;//normal_biv.duale();


      //  DrawAt( tanA, mesh[idx].pos, 1, 0, 0 );
      //  DrawAt( tanB, mesh[idx].pos, 0, 0, 1 );
       if (bDrawNormal) DrawAt( mesh[idx].normal, mesh[idx].pos,0,.4,0 );
       // normal.push_back( normal_biv.duale() );

      // auto tbst = Gen::bst( logAx * tu ) * Gen::bst( logBy * tv );
      // auto tbst2 = Gen::con( logT, tu, tv );
      // auto tpnt = Round::loc( fa.pos().spin(tbst2) );
      // Draw(tpnt);
     }
 
    }


    //Circle EDGES
//    if (bDrawEdges){
//    Draw(sa,.2,.2,.2);//.4,1,.6);
//    Draw(sc,.2,.2,.2);//.4,1,.6);
//    Draw(sb,.2,.2,.2);//.8,1,.2);
//    Draw(sd,.2,.2,.2);//.8,1,.2); 
//    }
//
//   glBegin(GL_TRIANGLES);
//   for (auto& i : graph.face()){
//        auto& a = i->a();
//        auto& b = i->b();
//        auto& c = i->c(); 
//        glColor4f(0,1,1,1);
//        GL::normal( a.normal.begin() );
//        GL::vertex( a.pos.begin() );
//        GL::normal( b.normal.begin() );
//        GL::vertex( b.pos.begin() );
//        GL::normal( c.normal.begin() );
//        GL::vertex( c.pos.begin() );
//   }
//   glEnd();
//
//
//
//    DrawAt( fa.x(), fa.pos(),1,0,0);
//    DrawAt( -fb.x(), fb.pos(),1,0,0);
//    DrawAt( fc.x(), fc.pos(),1,0,0);
//    DrawAt( -fd.x(), fd.pos(),1,0,0);
//
//    DrawAt( fa.y(), fa.pos(),0,1,0);
//    DrawAt( fb.y(), fb.pos(),0,1,0);
//    DrawAt( fc.y(), fc.pos(),0,1,0);
//    DrawAt( fd.y(), fd.pos(),0,1,0);
//
//    DrawAt( fa.z(), fa.pos(),0,0,1);
//    DrawAt( fb.z(), fb.pos(),0,0,1);
//    DrawAt( fc.z(), fc.pos(),0,0,1);
//    DrawAt( fd.z(), fd.pos(),0,0,1);



   if (bDrawA) { 
      Draw( logAz.dual(),1,0,0 );
      Draw(sphAz,1,0,0,.2);
      Draw(tsa,1,0,0,.2);
   }
   
   if (bDrawB){
      Draw(logBz.dual(),0,1,0);
      Draw(sphDz,0,1,0,.2);
      Draw(tsb,0,1,0,.2);
   }

   if (bDrawC) {
     Draw(logAx.dual(),0, .5, 1);
      Draw(sphAx,0,.5,1,.2);
      Draw(tsc,0,.5,1,.2);
      Draw( (sphAx ^ tsc).dual() );
   }

   if (bDrawD) {
     Draw(logBy.dual(),.2,.1,.1);

     Draw(sphDy,.2,.1,.1,.2);
     Draw(tsd,.2,.1,1,.2);
     Draw((sphDy ^ tsd).dual() );
   }

   if (bDrawE) {
     Draw(logAy.dual(),0,.5,1);
      Draw(sphAy,0,.5,1,.2);
     Draw(tse,0,.5,1,.2);
   }

   if (bDrawF) {
     Draw(logBx.dual(),.2,.1,1);  
      Draw(sphDx,.2,.1,1,.2);
     Draw(tsf,.2,.1,1,.2);
   }

   }
 
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
