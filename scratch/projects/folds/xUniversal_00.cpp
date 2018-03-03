/*
 * =====================================================================================
 *
 *       Filename:  xGusset.cpp
 *
 *    Description:  gusset molecule (p 361, Origami Design Secrets)
 *
 *        Version:  1.0
 *        Created:  11/26/2013 17:20:25
 *       Compiler:  gcc4.7 or higher or clang3.2 or higher
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_fold.h"
#include "form/vsr_fold_molecules.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,dx,dy;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(dx,"dx",-10,10)(dy,"dy",-10,10);

  }
 
     virtual void onDraw(){ 
        
      mouse = calcMouse3D();

      //CORNER POINTS OF QUADRILATERAL
      static Field<Pnt> corner(2,2,1);
      static Pnt pnt[4];

      pnt[0] = corner[0];
      pnt[1] = corner[1];
      pnt[2] = corner[3].trs(0,dy,0);
      pnt[3] = corner[2].trs(dx,0,0);      

      //LINES AND BISECTORS
      auto perim = Fold::Lines(pnt,4);      //<-- Edges
      for (auto i : perim ) Draw(i,0,1,0);
      auto bisect = Fold::Bisect(perim);    //<-- Bisectors
      for (auto i : bisect) Draw(i,0,0,1);

      //Nodes
      Pnt node[2];        //<-- points along edge
      Pnt center[2];      //<-- points in paper  
      Pnt np[4];          //<-- Transformed points
      


      //Find Intersecting Points and Node Edges 
      int it = 0;
      for (int i = 0; i < bisect.size(); i+=2){
        int prev = i > 0 ? i - 1 : bisect.size() -1;
        int next = i < bisect.size() - 1 ? i + 1 : 0;
        auto pa = Construct::meet( bisect[i], bisect[next]);
        VSR_PRECISION da = Vec( pa - pnt[i] ).wt();
        auto pb = Construct::meet( bisect[prev], bisect[i]);
        VSR_PRECISION db = Vec( pb - pnt[i] ).wt();
        
        Pnt nodeA, nodeB;
        Pnt nodeC, nodeD;

        if (da < db ){
          nodeA = pa;
          nodeB = Flat::loc( perim[i], pa, false ).null();
          nodeC = pnt[next];
          nodeD = pnt[i];
       } else {
          nodeA = pb;
          nodeB = Flat::loc( perim[prev], pb, false ).null();
          nodeC = pnt[i];
          nodeD = pnt[prev];
       }

        node[it] = nodeB;
        center[it] = nodeA;
        np[i] = nodeD;
        np[i+1] = nodeC;
        it++; 

      }


      Rigid2_ ra( node[1], np[3], center[1],true ); 
      // Rigid2_ rd( node[0], center[0], np[0] );
      Rigid3 rd(node[0], np[1], center[0], np[0], false);

      Rigid3 rb( np[2], node[1], center[0], center[1], false ); 
       //Rigid3 rc( np[1], center[0], center[1], node[0], false );   
      Rigid3 rc( np[1], np[2], center[0], center[1], false );   
      // Rigid3 rd( node[0], np[1], center[0], np[0], false ); 
       
       //MOTION
       Pnt pa = ra.orbit( amt * PIOVERTWO );
       //Pnt pd = rd.orbit( amt * PIOVERTWO );
       
       rb.updateA( pa ); 
       Pnt pb = rb(); 

      // rc.updateC( pd );
       rc.updateA(pb);
       Pnt pc = rc();

       rd.updateA(pc);
       Pnt pd = rd();

      node[1] = pa; 
      np[2] = pb;
      np[1] = pc;
      node[0] = pd;


      /*-----------------------------------------------------------------------------
       *  DRAW
       *-----------------------------------------------------------------------------*/
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


       /* cout << s << endl; */ 

       /* float val = 0.0; int numiterouter = 0; */

       /*  bool bSwitch = false; */
       /*  do { */
       /*    numiterouter++; */ 
       /*    if (bSwitch) val +=.01; */

       /*    float nval = val * ( bSwitch ? -1.0 : 1.0 ); */
       /*    bSwitch = !bSwitch; */

       /*    float iter = 0.0; int numiter = 0; bool bbSwitch = false; */
       /*    do { */
       /*      numiter++; */
       /*      if (bbSwitch) iter += .01; */
            
       /*      float nnval = iter * (bbSwitch ? -1.0 : 1.0 ); */
       /*      bbSwitch = !bbSwitch; */
            
       /*      pb = rb( np[1], node[1], amt + nval);//amt + ( iter  ) );//+ testAmt); */
       /*      rc.updateA( pb ); */
       /*      pc = rc( np[1], node[0], amt + nnval );//amt + ( iter  ) ); */
       /*      rd.updateA( pc ); */
       /*      pd = rd(); */
       /*      s = Ro::size( rd.possible(), false ); */

       /*     } while  ( (s < 0) && (numiter < 200) ); */ 

       /*     cout << "inner: " << numiter << endl; */
      
      /* } while ( (s < 0) && (numiterouter < 200) ); */ 

      /* cout << "outer: " << numiterouter << endl; */
