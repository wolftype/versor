
/*
 * =====================================================================================
 *
 *       Filename:  xTangent.cpp
 *
 *    Description:  tangent field take 2 (dn on edges not faces)
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
  float amt,amtB,radius,dcurve;

  bool bReset, bDist,bUseMotor,bSurfArea;


  Field<Sca> f;
  Field<Cir> fc;
  Field<Biv> fs;

  MyApp(Window * win, int num, float spacing = 1 ) : App(win),
  f(num,num,1,spacing), 
  fc(num,num,1,spacing), fs(num,num,1,spacing)
  
  {
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(dcurve,"dcurve",-10000,10000);
      gui(bReset,"reset")(bDist,"bDist")(bUseMotor,"bUseMotor")(bSurfArea,"bSurfArea");

      bSurfArea = 1; //use directed content of mesh itself
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
      mouse = Round::point( ray,  Ori(1) );  
  }

  void shape(){

      //Goal: Find shapes i.e. gradient of normals 
      // (direction of greatest change of curvature at each point on mesh)
      for (int i=1;i<fc.w()-1;++i){
        for (int j=1;j<fc.h()-1;++j){

          //tangents
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
          auto n = Ro::dir(a).copy<Biv>().duale().unit();

          auto nb = Ro::dir(b).copy<Biv>().duale().unit();
          auto nc = Ro::dir(c).copy<Biv>().duale().unit();
          auto nd = Ro::dir(d).copy<Biv>().duale().unit();
          auto ne = Ro::dir(e).copy<Biv>().duale().unit();

          //normal differences along edges
          auto fa = nb-n;
          auto fb = nc-n;
          auto fc = nd-n;
          auto fd = ne-n;

          //simplicial edges
          Vec ea = pb - pa;
          Vec eb = pc - pa;
          Vec ec = pd - pa;
          Vec ed = pe - pa;

          //simplicial psuedsocalars
          auto psa = !(ea^eb);
          auto psb = !(eb^ec);
          auto psc = !(ec^ed);
          auto psd = !(ed^ea);

          //simplicial recips;
          auto raa = eb<=psa; auto rab = -ed<=psd;
          auto rba = ec<=psb; auto rbb = -ea<=psb;
          auto rca = ed<=psc; auto rcb = -eb<=psc;
          auto rda = ea<=psd; auto rdb = -ec<=psd;

          //inner product of normal differences and recips
          float wa = ( (fa<=raa)+(fa<=rab) )[0]; 
          float wb = ( (fb<=rba)+(fb<=rbb) )[0]; 
          float wc = ( (fc<=rca)+(fc<=rcb) )[0]; 
          float wd = ( (fd<=rda)+(fd<=rdb) )[0]; 

          //sum = derivative
          auto dn = fa*wa + fb*wb + fc*wc + fd*wd;

          //psuedoscalars of mesh
          /* auto pssa = Vec(pa) ^ Vec(pb) ^ Vec(pc); */
          /* auto pssb = Vec(pa) ^ Vec(pc) ^ Vec(pd); */
          /* auto pssc = Vec(pa) ^ Vec(pd) ^ Vec(pe); */
          /* auto pssd = Vec(pa) ^ Vec(pe) ^ Vec(pb); */

          /* auto norma = pssa.norm(); */
          /* auto snorma = norma*norma; */
          /* auto normb = pssb.norm(); */
          /* auto snormb = normb*normb; */
          /* auto normc = pssc.norm(); */
          /* auto snormc = normc*normc; */
          /* auto normd = pssd.norm(); */
          /* auto snormd = normd*normd; */

          //weighted rcps
          /* rcpa /= snorma; */
          /* rcpb /= snormb; */
          /* rcpc /= snormc; */
          /* rcpd /= snormd; */

          //recip of mesh
          /* Vec rcpa = (Vec(pc) - Vec(pb)) <= !dca; */
          /* Vec rcpb = (Vec(pd) - Vec(pc)) <= !dcb; */
          /* Vec rcpc = (Vec(pe) - Vec(pd)) <= !dcc; */
          /* Vec rcpd = (Vec(pb) - Vec(pe)) <= !dcd; */

          /* DrawAt(rcpa, pa,1,1,0); */
          /* DrawAt(rcpb, pa,1,0,1); */
          /* DrawAt(rcpc, pa,0,1,1); */
          /* DrawAt(rcpd, pa,1,0,0); */


          /* //directed content of mesh */
          auto dca = Vec(pb-pa) ^ Vec(pa-pc) * .5;
          auto dcb = Vec(pc-pa) ^ Vec(pa-pd) * .5;
          auto dcc = Vec(pd-pa) ^ Vec(pa-pe) * .5;
          auto dcd = Vec(pe-pa) ^ Vec(pa-pb) * .5;

          //boundary normal differences biv (not used)
          auto bfa = (nb-n)^(nc-n)*.5;
          auto bfb = (nc-n)^(nd-n)*.5;
          auto bfc = (nd-n)^(ne-n)*.5;
          auto bfd = (ne-n)^(nb-n)*.5;

          //boundary normal differences
          fa = nc-nb;
          fb = nd-nc;
          fc = ne-nd;
          fd = nb-ne;

          /* //projection of moments of areas wrt boundary normal differences */
          Vec pfa = fa <= (bSurfArea ? dca : bfa);
          Vec pfb = fb <= (bSurfArea ? dcb : bfb);
          Vec pfc = fc <= (bSurfArea ? dcc : bfc);
          Vec pfd = fd <= (bSurfArea ? dcd : bfd);

          //dn
          dn = pfa+pfb+pfc+pfd;

          /* Vec pfa = rcpa <= fa.duale(); */
          /* Vec pfb = rcpb <= fb.duale(); */
          /* Vec pfc = rcpc <= fc.duale(); */
          /* Vec pfd = rcpd <= fd.duale(); */


/*            DrawAt(Vec(pfa), pa,1,0,0,.5); */
/*            DrawAt(Vec(pfb), pa,1,1,0,.5); */
/*            DrawAt(Vec(pfc), pa,1,0,1,.5); */
/*            DrawAt(Vec(pfd), pa,0,1,1,.5); */

          //Shape
           auto shape = n^dn;
           fs.at(i,j) = shape;

          //  DrawAt( pf, pa, 1,1,1);
         //  DrawAt( Vec( Op::pj(pf,da.duale())) ,pa,0,0,1);

            //Gaussian
            auto kg = n <= dn;
            f.at(i,j) = kg;

            //!da.duale() <= ( pf <= da.duale() ).duale();
            //da <= Op::pj(da,pf);//( da <= da.duale() );//da <= pf.duale() );//Op::pj(da,pf);
         //   cout << "gauss: " << kg[0] << endl; 

        }
      }
  }

  void spur(){

      for (int i=1;i<fc.w()-1;++i){
        for (int j=1;j<fc.h()-1;++j){

          //tangent data
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

          auto da = Ro::dir(a).copy<Biv>().duale();

          //directed content of mesh
          auto dca = Vec(pb-pa) ^ Vec(pa-pc) * .5;
          auto dcb = Vec(pc-pa) ^ Vec(pa-pd) * .5;
          auto dcc = Vec(pd-pa) ^ Vec(pa-pe) * .5;
          auto dcd = Vec(pe-pa) ^ Vec(pa-pb) * .5;

          //this shape
          auto sa = fs.at(i,j);

          //shapes along boundary
          auto sb = fs.at(i+1,j);
          auto sc = fs.at(i,j+1);
          auto sd = fs.at(i-1,j);
          auto se = fs.at(i,j-1);

          //differencing along boundary
          auto dsa = sc - sb;
          auto dsb = sd - sc;
          auto dsc = se - sd;
          auto dsd = sb - se;
          auto ds = dsa + dsb + dsc + dsd;

          //commuting along boundary
          auto ca = sc % sb;
          auto cb = sd % sc;
          auto cc = se % sd;
          auto cd = sb % se;

          //moments
          auto mca = dca <= ca;
          auto mcb = dcb <= cb;
          auto mcc = dcc <= cc;
          auto mcd = dcd <= cd;

          auto msa = dca.duale() <= dsa;
          auto msb = dcb.duale() <= dsb;
          auto msc = dcc.duale() <= dsc;
          auto msd = dcd.duale() <= dsd;
          Vec N = msa + msb + msc + msd;

          //mean curvature
          auto mean = -da <= N;
          f.at(i,j) = mean;
 
          //Riemann curvature at pa
          auto com = mca + mcb + mcc + mcd;
   //       cout << "riemann " << com[0] << endl; 
//          auto com = ca + cb + cc + cd;

     //     DrawAt( sa, pa, 0, 1, 0 );
     //     DrawAt( N, pa, 1,0,0);
    //      DrawAt( ds, pa, 0, 0, 1);
      //    Draw( pa ^ pb );
      //    Draw( pa ^ pc );
        }
      }
  }

    //see p399 hestenes shape
    virtual void onDraw(){ 
        
      getMouse();

      Frame frame(mouse[0], mouse[1], .1);
      frame.scale( amtB );

      static Frame control(0,0,5);
      Touch(interface, control);
      DrawAt( control.z(), control.pos(), 0,0,1);
      for(int i=0;i<fc.num();++i){
          auto s = Ro::dls_pnt( fc.grid(i), amtB );
          auto r = control.tz().reflect( s );
          Draw(r.dual(),1,1,0);
         // DrawAt( Ro::dir( r.dual() ).copy<Biv>(), Ro::loc( r ), 1,1,0);
      }

      //generators
      TorusKnot tk(0,1);
      tk.HF.cir() = CXY(1);
      auto kpar = tk.par() * amt;
      auto par = frame.ipz() * amt;
      auto dll = Twist::Along( frame.dly(), amt, amtB);

      //initial transform
      if (bReset){
        for(int i=0;i<fc.num();++i){

          auto cir = Cir(Tnb(.1,0,0)).trs( fc.grid(i) );
          
          //distances
          double dist = 1.0 / (.1 + fabs( Ro::sqd(frame.pos(), fc.grid(i)) ) );
          double ldist = 1.0 / (.1 + fabs( ( fc.grid(i) <= frame.dly()).wt() ) );
          double cdist = 1.0 / (.1 + fabs( sqrt( (fc.grid(i) <= frame.cxy().dilate(.2).dual()).wt() ) ) );

          //transforms
          auto boost = Gen::bst(par * ( bDist ? dist : 1) );
          auto motor = Gen::mot(dll* ( bDist ? dist : ldist));
          auto kboost = Gen::bst(kpar* ( bDist ? dist : cdist));
                
          auto fcir =  bUseMotor ? cir.spin(motor) : cir.spin(kboost);     
          fc[i] = fcir;
          auto wt = Ta::wt(fcir); 
          DrawAt( Ro::dir(fcir).copy<Biv>(), Ro::loc(fcir),fabs(wt),1,1-fabs(wt)); 

          //jacobian
          /* auto pseudo = Ro::car(cir); */
          /* auto fpseudo = pseudo.spin(motor); */
          /* auto jacobian = !pseudo * fpseudo; */
       }
     }

     


      //Shape
      shape();// spur();
      for (int i=1;i<fs.w()-1;++i){
        for (int j=1;j<fs.h()-1;++j){
            DrawAt(fs.at(i,j), Ro::loc( fc.at(i,j)), 0,1,0);
        }
      }
 
     glBegin(GL_TRIANGLES);
     for (int i=0;i<fc.w()-1;++i){
      for (int j=0;j<fc.h()-1;++j){
        auto& a = fc.at(i,j);
        auto& b = fc.at(i+1,j);
        auto& c = fc.at(i,j+1);
        auto& d = fc.at(i+1,j+1);

        auto pa = Ro::loc(a);
        auto pb = Ro::loc(b);
        auto pc = Ro::loc(c);
        auto pd = Ro::loc(d);

        auto na = Ro::dir(a).dual().copy<Vec>().unit();
        auto nb = Ro::dir(b).dual().copy<Vec>().unit();
        auto nc = Ro::dir(c).dual().copy<Vec>().unit();
        auto nd = Ro::dir(d).dual().copy<Vec>().unit();

        auto sa = fabs( f.at(i,j)[0] );//.rnorm() );
        auto sb = fabs( f.at(i+1,j)[0]);//.rnorm() );
        auto sc = fabs( f.at(i,j+1)[0]);//.rnorm() );
        auto sd = fabs( f.at(i+1,j+1)[0]);//.rnorm() );

        glColor3f( sa,0,1-sa);
        GL::normal(na.begin()); GL::vertex(pa.begin());
        glColor3f( sb,0,1-sb);
        GL::normal(nb.begin()); GL::vertex(pb.begin());
        glColor3f( sc,0,1-sc);
        GL::normal(nc.begin()); GL::vertex(pc.begin());
        glColor3f( sc,0,1-sc);
        GL::normal(nc.begin()); GL::vertex(pc.begin());
        glColor3f( sb,0,1-sb);
        GL::normal(nb.begin()); GL::vertex(pb.begin());
        glColor3f( sd,0,1-sd);
        GL::normal(nd.begin()); GL::vertex(pd.begin());


        //Draw(Ro::loc(pa) ^ Ro::loc(pb), 1,.3,.2); 
        //Draw(Ro::loc(pa) ^ Ro::loc(pc), 1,.3,.2); 
      }
     }
    glEnd();


      if ( interface.keyboard.down ){
        if (interface.keyboard.code == 'e'){
            for (int i=1;i<fc.w()-1;++i){
              for (int j=1;j<fc.h()-1;++j){

                //tangent data
                auto& a = fc.at(i,j);
                auto& b = fc.at(i+1,j);
                auto& c = fc.at(i,j+1);
                auto& d = fc.at(i-1,j);
                auto& e = fc.at(i,j-1);

                //positions of vertices
                auto pa = Ro::loc(a);
                
                auto pb = Ro::loc(b);
                auto pc = Ro::loc(c);
                auto pd = Ro::loc(d);
                auto pe = Ro::loc(e);
  
                //this shapes
                auto sa = fs.at(i,j);
                //nbr
                auto sb = fs.at(i+1,j);
                auto sc = fs.at(i,j+1);
                auto sd = fs.at(i-1,j);
                auto se = fs.at(i,j-1);

                auto mb = f.at(i+1,j);
                auto mc = f.at(i,j+1);
                auto md = f.at(i-1,j);
                auto me = f.at(i,j-1);

               // auto cir = Ro::round( Ro::dls_pnt( pa, -1 ), sa );
                //auto dll = ( pa ^ sa.duale() ^ Inf(1) ).dual().runit()*sa.wt()*dcurve;
                
                Dll dll;
                Biv biv;


                /* if (i<fc.w()-2) dll += ( pb ^ sb.duale() ^ Inf(1) ).dual().runit()*sb.wt()*dcurve; */
                /* if (j<fc.h()-2) dll += ( pc ^ sc.duale() ^ Inf(1) ).dual().runit()*sc.wt()*dcurve; */
                /* if (i>1) dll += ( pd ^ sd.duale() ^ Inf(1) ).dual().runit()*sd.wt()*dcurve; */
                /* if (j>1) dll += ( pe ^ se.duale() ^ Inf(1) ).dual().runit()*se.wt()*dcurve; */

                auto csb = sa % sb; auto wb = csb.wt();
                auto csc = sa % sc; auto wc = csc.wt();
                auto csd = sa % sd; auto wd = csd.wt();
                auto cse = sa % se; auto we = cse.wt();

               // cout << wb << endl;

               // biv = (csb + csc + csd + cse) * dcurve;
                biv = sa * dcurve;


                if (i<fc.w()-2) dll += ( pb ^ csb.duale() ^ Inf(1) ).dual()*dcurve;//*mb;
                if (j<fc.h()-2) dll += ( pc ^ csc.duale() ^ Inf(1) ).dual()*dcurve;//*mc;
                if (i>1) dll += ( pd ^ csd.duale() ^ Inf(1) ).dual()*dcurve;//*md;
                if (j>1) dll += ( pe ^ cse.duale() ^ Inf(1) ).dual()*dcurve;//*me;



                //auto dll = ( Inf(1) <= (cir) ).runit();
                auto mot = Gen::mot(dll);
                auto bst = Gen::boost(dll);
               
                auto bst2 = Gen::bst( ( (b*wb)+(c*wc)+(d*wd)+(e*we) ) * dcurve );
                auto mot2 = Gen::mot( (pa ^ biv.duale()^Inf(1)).dual() );

               // cout << sa.wt() << endl; 

                a = a.spin( mot2 );
                /* c = c.spin( mot ); */
                /* d = d.spin( mot ); */
                /* e = e.spin( mot ); */

                DrawAt( Ro::dir(a).copy<Biv>().duale(), Ro::loc(a),1,0,0);
                /* DrawAt( Ro::dir(c).copy<Biv>(), Ro::loc(c),1,0,0); */
                /* DrawAt( Ro::dir(d).copy<Biv>(), Ro::loc(d),1,0,0); */
                /* DrawAt( Ro::dir(e).copy<Biv>(), Ro::loc(e),1,0,0); */
              
              }
            }
        }

        if (interface.keyboard.code == 'u'){
            for (int i=1;i<fc.w()-1;++i){
              for (int j=1;j<fc.h()-1;++j){
            
              }
            }
        }
      }




  }
   

  
};


MyApp * app;


int main(){
                             
  GLV glv(0,0);  

  Window * win = new Window(500,500,"Versor",&glv);    
  app = new MyApp( win,30,.3 ); 
  app -> initGui();
  
  
  glv << *app;

  Application::run();

  return 0;

}

          //directed content of normals
          /* auto ndca = da ^ db * (1.0/2.0); */
          /* auto ndcb = da ^ dc * (1.0/2.0); */
          /* auto ndcc = da ^ dd * (1.0/2.0); */
          /* auto ndcd = da ^ de * (1.0/2.0); */

        //pseudoscalars of normal field
          /* auto pseudoA = (db-da) ^ (dc-da); */
          /* auto pseudoB = (dc-da) ^ (dd-da); */
          /* auto pseudoC = (dd-da) ^ (de-da); */
          /* auto pseudoD = (de-da) ^ (db-da); */

          //alt pseudoscalars of normal field (tri)
          /* auto fpseudoA = da ^ db ^ dc; */
          /* auto fpseudoB = da ^ dc ^ dd; */
          /* auto fpseudoC = da ^ dd ^ de; */
          /* auto fpseudoD = da ^ de ^ db; */

          /* //sqnorm of normal field */ 
          /* auto fnormA = fpseudoA.norm(); */ 
          /* auto fsqnormA = fnormA*fnormA; */
          /* auto fnormB = fpseudoB.norm(); */        
          /* auto fsqnormB = fnormB*fnormB; */
          /* auto fnormC = fpseudoC.norm(); */        
          /* auto fsqnormC = fnormC*fnormC; */
          /* auto fnormD = fpseudoD.norm();// cout << fnormD << endl; */       
          /* auto fsqnormD = fnormD*fnormD; */


          /* //recips of normal field */
          /* auto ra = (db^dc)<=fpseudoA; */  
          /* auto rb = (dc^dd)<=fpseudoB; */
          /* auto rc = (dd^de)<=fpseudoC; */
          /* auto rd = (de^db)<=fpseudoD; */

          /* ra /= fnormA; */
          /* rb /= fnormB; */
          /* rc /= fnormC; */
          /* rd /= fnormD; */
         
          //alt recips of normal field (vecs)
          /* auto frcpa = (db)<=fpseudoA; */  
          /* auto frcpb = (dc)<=fpseudoB; */
          /* auto frcpc = (dd)<=fpseudoC; */
          /* auto frcpd = (de)<=fpseudoD; */

          /* auto frcpa = db-da; */  
          /* auto frcpb = dc-da; */
          /* auto frcpc = dd-da; */
          /* auto frcpd = de-da; */
