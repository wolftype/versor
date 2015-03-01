/*
 * =====================================================================================
 *
 *       Filename:  xTangent.cpp
 *
 *    Description:  tangent field
 *
 *        Version:  1.0
 *        Created:  07/28/2014 19:48:23
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
#include "vsr_field.h"
#include "vsr_twist.h"
#include "vsr_knot.h"
#include "vsr_cga3D_frame.h"

using namespace vsr;
using namespace vsr::cga3D;


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt,amtB;

  bool bReset;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(bReset,"reset");
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

    //see p399 hestenes shape
    virtual void onDraw(){ 
        
      getMouse();

      static Frame control(0,0,5);
      Touch(interface, control);
      DrawAt( control.z(), control.pos(), 1,0,0);

      Frame frame(mouse[0], mouse[1], 0);
      
      Field<Vec> f(10,10,1);
      Field<Cir> fc(10,10,1);

      auto par = frame.ipz() * amt;
      auto dll = Twist::Along( frame.dly(), amt, amtB);

      auto kpar = TorusKnot(3,2).par() * amt;

      for(int i=0;i<f.num();++i){
        f[i] = f[i].unit();
        auto cir = Cir(Tnb(.1,0,0)).trs( f.grid(i) );

        double dist = 1.0 / (.1 + fabs( Ro::sqd(frame.pos(), f.grid(i)) ) );
      //  double dist = 1.0 / (.1 + fabs( ( f.grid(i) <= frame.dly()).wt() ) );

        auto boost = Gen::bst(par * dist );
        auto motor = Gen::mot(dll*dist);
        auto kboost = Gen::bst(kpar);
              
        auto fcir = cir.spin(motor);

        auto pseudo = Ro::car(cir);
        auto fpseudo = pseudo.spin(boost);

       // f.grid(i) = f.grid(i).spin(boost);
      //  Vec vec =  Ro::loc( f.grid(i) ) - f.grid(i);
        auto jacobian = !pseudo * fpseudo;
        
        fc[i] = fcir;
        auto wt = Ta::wt(fcir); 
        DrawAt( Ro::dir(fcir).copy<Biv>(), Ro::loc(fcir),fabs(wt),1,1-fabs(wt)); 

        /* auto np = Ro::loc( control.tz().reflect( fcir ) ); */
        /* Draw(np,0,1,1); */
     }

      

      //Goal: Find gradient of normals 
      // (direction of greatest change of curvature at each point on mesh)
      for (int i=1;i<f.w()-1;++i){
        for (int j=1;j<f.h()-1;++j){
          //vertices
          auto a = fc.at(i,j);
          auto b = fc.at(i+1,j);
          auto c = fc.at(i,j+1);
          auto d = fc.at(i-1,j);
          auto e = fc.at(i,j-1);

          //positions of vertices
          auto pa = Ro::loc(a);
          auto pb = Ro::loc(b);
          auto pc = Ro::loc(c);
          auto pd = Ro::loc(d);
          auto pe = Ro::loc(e);
 
          //normals at vertices
          auto da = Ro::dir(a).copy<Biv>().duale().unit();
          auto db = Ro::dir(b).copy<Biv>().duale().unit();
          auto dc = Ro::dir(c).copy<Biv>().duale().unit();
          auto dd = Ro::dir(d).copy<Biv>().duale().unit();
          auto de = Ro::dir(e).copy<Biv>().duale().unit();

          DrawAt(da, pa,0,1,1);

         //pseudoscalars of mesh
          auto psma = pa ^ pb ^ pc ^ Inf(1);
          auto psmb = pa ^ pc ^ pd ^ Inf(1);
          auto psmc = pa ^ pd ^ pe ^ Inf(1);
          auto psmd = pa ^ pe ^ pb ^ Inf(1);

          //sqnorms of psms
          auto mnormA = psma.rnorm();
          auto msqnormA = mnormA*mnormA;
          auto mnormB = psmb.rnorm();
          auto msqnormB = mnormB*mnormB;
          auto mnormC = psmc.rnorm();
          auto msqnormC = mnormC*mnormC;
          auto mnormD = psmd.rnorm();
          auto msqnormD = mnormD*mnormD;

          //recips of mesh
          auto mrcpA = ( (pb^pc^Inf(1))<=psma ) / msqnormA;
          auto mrcpB = ( (pc^pd^Inf(1))<=psmb ) / msqnormB;
          auto mrcpC = ( (pd^pe^Inf(1))<=psmc ) / msqnormC;
          auto mrcpD = ( (pe^pb^Inf(1))<=psmd ) / msqnormD;

          //directed content of mesh
          auto dca = ( Vec(pb - pa) ^ Vec(pc-pa) ) * .5;
          auto dcb = ( Vec(pc - pa) ^ Vec(pd-pa) ) * .5;
          auto dcc = ( Vec(pd - pa) ^ Vec(pe-pa) ) * .5;
          auto dcd = ( Vec(pe - pa) ^ Vec(pb-pa) ) * .5;

          //directed content of normals
          auto ndca = da ^ db ^ dc * (1.0/6.0);
          auto ndcb = da ^ dc ^ dd * (1.0/6.0);
          auto ndcc = da ^ dd ^ de * (1.0/6.0);
          auto ndcd = da ^ de ^ db * (1.0/6.0);

          //can now check if normals match . . .

          //pseudoscalars of normal field
          /* auto pseudoA = (db-da) ^ (dc-da); */
          /* auto pseudoB = (dc-da) ^ (dd-da); */
          /* auto pseudoC = (dd-da) ^ (de-da); */
          /* auto pseudoD = (de-da) ^ (db-da); */

          //alt pseudoscalars of normal field (tri)
          auto fpseudoA = da ^ db ^ dc;
          auto fpseudoB = da ^ dc ^ dd;
          auto fpseudoC = da ^ dd ^ de;
          auto fpseudoD = da ^ de ^ db;

          //recips of normal field
          /* auto rcpa = (db-dc)<=pseudoA; */  
          /* auto rcpb = (dc-dd)<=pseudoB; */
          /* auto rcpc = (dd-de)<=pseudoC; */
          /* auto rcpd = (de-db)<=pseudoD; */
         
          //alt recips of normal field (vecs)
          auto frcpa = (db^dc)<=fpseudoA;  
          auto frcpb = (dc^dd)<=fpseudoB;
          auto frcpc = (dd^de)<=fpseudoC;
          auto frcpd = (de^db)<=fpseudoD;

          //sqnorm of normal field 
          /* auto normA = pseudoA.norm(); */ 
          /* auto sqnormA = normA*normA; */
          /* auto normB = pseudoB.norm(); */        
          /* auto sqnormB = normB*normB; */
          /* auto normC = pseudoC.norm(); */        
          /* auto sqnormC = normC*normC; */
          /* auto normD = pseudoD.norm();// cout << normD << endl; */       
          /* auto sqnormD = normD*normD; */


          //alt sqnorm of normal field 
          auto fnormA = fpseudoA.norm(); 
          auto fsqnormA = fnormA*fnormA;
          auto fnormB = fpseudoB.norm();        
          auto fsqnormB = fnormB*fnormB;
          auto fnormC = fpseudoC.norm();        
          auto fsqnormC = fnormC*fnormC;
          auto fnormD = fpseudoD.norm(); cout << fnormD << endl;       
          auto fsqnormD = fnormD*fnormD;



          /* auto rcpa = (db^dc)<=pseudoA; */
          /* auto rcpb = (dc^dd)<=pseudoB; */
          /* auto rcpc = (dd^de)<=pseudoC; */
          /* auto rcpd = (de^da)<=pseudoD; */

          /* auto frcpa = (db^dc)<=pseudoA; */
          /* auto frcpb = (dc^dd)<=pseudoB; */
          /* auto frcpc = (dd^de)<=pseudoC; */
          /* auto frcpd = (de^da)<=pseudoD; */
 
 
          //normalize
          /* rcpa /= sqnormA; */        
          /* rcpb /= sqnormB; */        
          /* rcpc /= sqnormC; */
          /* rcpd /= sqnormD; */  
          
          /* rcpa /= normA; */
          /* rcpb /= normB; */
          /* rcpc /= normC; */
          /* rcpd /= normD; */

          //moments with respect to reciprocals
          /* auto projA = frcpa <= ndca; */  
          /* auto projB = frcpb <= ndcb; */  
          /* auto projC = frcpc <= ndcc; */  
          /* auto projD = frcpd <= ndcd; */  

          /* auto projA = dca.duale() <= frcpa.duale(); */
          /* auto projB = dcb.duale() <= frcpb.duale(); */
          /* auto projC = dcc.duale() <= frcpc.duale(); */ 
          /* auto projD = dcd.duale() <= frcpd.duale(); */
          auto projA = frcpa <= dca; 
          auto projB = frcpb <= dcb; 
          auto projC = frcpc <= dcc;  
          auto projD = frcpd <= dcd; 



         // projA.print();

          //gradient (reciprocal, whitney kform)
         // auto rcp = rcpa + rcpb + rcpc + rcpd;
          auto frcp = frcpa + frcpb + frcpc + frcpd;

          /* auto projA = dca.unit() * (rcp <= dca) ; */
          /* auto projB = dcb.unit() * (rcp <= dcb) ; */
          /* auto projC = dcc.unit() * (rcp <= dcc) ; */
          /* auto projD = dcd.unit() * (rcp <= dcd) ; */
          auto proj = projA + projB + projC + projD;                 

          auto shape = da ^ proj;

         // DrawAt(frcp, pa, 1,0,0);
         // DrawAt(proj,pa, 0,1,0);
          DrawAt(shape.duale(),pa, 0,1,0);

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
