/*
 * =====================================================================================
 *
 *       Filename:  xUniversal.cpp
 *
 *    Description:  universal molecule (convex)
 *
 *        Version:  1.0
 *        Created:  12/09/2013 13:33:13
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or higher
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_cga3D_app.h"   
#include "vsr_fold.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, dx, dy, testAmt;

  virtual void setup(){
      bindGLV();

      gui(amt,"amt",-100,100)(testAmt,"testAmt",-100,100);
      gui(dx,"dx",-10,10)(dy,"dy",-10,10);
  }
 
 
  virtual void onDraw(){ 
        
      mouse = calcMouse3D();
   
      //CORNER POINTS OF QUADRILATERAL
      static Pnt pnt[4];
      static Field<Pnt> corner(2,2,1);

      pnt[0] = corner[0];//.set< 4 >( 0 ); pnt[0] = pnt[0].null() ;
      pnt[1] = corner[1];//.set< 4 >( 0 ); pnt[1] = pnt[1].null() ;
      pnt[2] = corner[3].trs(0,dy,0);//; pnt[2] = pnt[2].null() ;//t
      pnt[3] = corner[2].trs(dx,0,0);//; pnt[3] = pnt[3].null() ;//t.


      //LINES AND BISECTORS
      auto perim = Fold::Lines( pnt, 4 );
      for (auto i : perim ) Draw(i); 

      auto bisect = Fold::Bisect( perim );
      for (auto i : bisect ) Draw(i,0,1,0); 

      Pnt node[2];
      Pnt center[2];
      Pnt np[4];


      //Find Intersecting Points and Node Edges 
      int it = 0;
      for (int i = 0; i < bisect.size(); i+=2){
        int prev = i > 0 ? i - 1 : bisect.size() -1;
        int next = i < bisect.size() - 1 ? i + 1 : 0;
        auto pa = meet( bisect[i], bisect[next]);
        VT da = Vec( pa - pnt[i] ).wt();
        auto pb = meet( bisect[prev], bisect[i]);
        VT db = Vec( pb - pnt[i] ).wt();
        
        Pnt nodeA, nodeB;
        Pnt nodeC, nodeD;

        if (da < db ){
          nodeA = pa;
          nodeB = Fl::loc( perim[i], pa, false ).null();
          nodeC = pnt[next];
          nodeD = pnt[i];
       } else {
          nodeA = pb;
          nodeB = Fl::loc( perim[prev], pb, false ).null();
          nodeC = pnt[i];
          nodeD = pnt[prev];
       }
       // Draw(nodeA,1,1,0); Draw(nodeB,0,1,1);

        node[it] = nodeB;
        center[it] = nodeA;
        np[i] = nodeD;
        np[i+1] = nodeC;
        it++; 

        //Rigid2_ ra( nodeB, nodeA, nodeC);  
        //Draw( ra.orbit() );
      }

      //CONSTRAINT
       Rigid2_ ra( node[1], center[1], np[3] );

       Rigid2_ rb( np[2], center[1], node[1] );

       Rigid2_ rc( np[1], center[0], np[2] );

       Rigid3 rd( node[0], np[1], center[0], np[0], false );

       Rigid2_ re( np[0], center[0], np[3] );


      //MOTION
       Pnt pa = ra.orbit( amt * PIOVERTWO );
       
       rb.updateB( pa ); //Draw( rb.orbit() ); 
       Pnt pb = rb(); //rc.origin, ra.origin, amt); // interpolate nearness test

       rc.updateB( pb );  //Draw( rc.orbit() );
       Pnt pc = rc();// node[0], 1);//np[1], np[0], amt);

       rd.updateA( pc );
       Pnt pd = rd();

       Pair par = rd.possible();
       float s = Ro::size(par,false);

       cout << s << endl; 

       float val = 0.0; int numiterouter = 0;

        bool bSwitch = false;
        do {
          numiterouter++; 
          if (bSwitch) val +=.01;

          float nval = val * ( bSwitch ? -1.0 : 1.0 );
          bSwitch = !bSwitch;

          float iter = 0.0; int numiter = 0; bool bbSwitch = false;
          do {
           // cout <<"FIX" << endl; 
            numiter++;
            if (bbSwitch) iter += .01;
            
            float nnval = iter * (bbSwitch ? -1.0 : 1.0 );
            bbSwitch = !bbSwitch;
            
            pb = rb( np[1], node[1], amt + nval);//amt + ( iter  ) );//+ testAmt);
            rc.updateB( pb );
            pc = rc( np[1], node[0], amt + nnval );//amt + ( iter  ) );
            rd.updateA( pc );
            pd = rd();
            s = Ro::size( rd.possible(), false );

           } while  ( (s < 0) && (numiter < 200) ); 

           cout << "inner: " << numiter << endl;
      
      } while ( (s < 0) && (numiterouter < 200) ); 

      cout << "outer: " << numiterouter << endl;



      node[1] = pa; 
      np[2] = pb;
      np[1] = pc;
      node[0] = pd;
       
       Draw( pa, 0,1,0);
       Draw( pb, 0, 1,0);
       Draw( pc, 0,1,0);
       Draw( pd , 0, 1, 0);

        Draw(rb.circle() ); Draw(rc.circle());
        Draw (rd.possible() );

      glColor3f(0,1,0);
      glBegin(GL_TRIANGLES);
      
        TriNormal( np[0], node[0], center[0], true );
        TriNormal( node[0], np[1], center[0], true  );
        TriNormal( np[1], np[2], center[0], true  );
        TriNormal( np[2], center[1], center[0], true  );
        TriNormal( node[1], center[1], np[2], true  );
        TriNormal( np[3], center[1], node[1], true );
        TriNormal( np[3], np[0], center[1], true );
        TriNormal( np[0], center[0], center[1], true  );

      glEnd();

      glColor3f(0,1,1);
      glBegin( GL_LINES );
        GL::Tri( np[0], node[0], center[0] );
        GL::Tri( node[0], np[1], center[0] );
        GL::Tri( np[1], np[2], center[0] );
        GL::Tri( np[2], center[1], center[0] );
        GL::Tri( node[1], center[1], np[2] );
        GL::Tri( np[3], center[1], node[1]);
        GL::Tri( np[3], np[0], center[1]);
        GL::Tri( np[0], center[0], center[1] );
      glEnd();


  }
   

  
};



int main(){
                             
  MyApp app;
  app.start();
  
  return 0;

}
