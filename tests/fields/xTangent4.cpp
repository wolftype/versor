/*
 * =====================================================================================
 *
 *       Filename:  xTangent4.cpp
 *
 *    Description:  shape operator
 *
 *        Version:  1.0
 *        Created:  02/20/2015 12:21:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_field.h"
#include "vsr_twist.h"
#include "vsr_cga3D_frame.h"
#include "vsr_differential.h"

#include "vsr_cga3D_app.h"

using namespace vsr;
using namespace vsr::cga3D;



struct MyApp : App {    
   
  Pnt mouse;

  Frame control = Frame(0,0,5);

  float time;
  float wt, amt,amtB,radius,dcurve,xamt,rotamt;

  bool bReset,bDist,bUseMotor,bMean,bDrawShape,bDrawCotan,bDrawTangent,bDrawNormals,bDrawDiffNorm,bWeighNormals;
  bool bUseTangents, bUseDGNormal, bUseDGCurve, bDrawLines;

  Field<Sca> f,f2; //gaussian and mean
  Field<Cir> fc;   //tangents
  Field<Vec> fnd;  //normal diffs
  Field<Biv> fs;   //biv shapes (n^diff)
  Field<Vec> fn;   //normals

  MyApp(int num=10, float spacing = .2 ) : App(),
  f(num,num,1,spacing), f2(num,num,1,spacing),
  fc(num,num,1,spacing), fs(num,num,1,spacing),
  fn(num,num,1,spacing), fnd(num,num,1,spacing)
  {}

  void setup(){
    scene.camera.pos( 0,0,10 ); 
    time = 0;

    bindGLV();
    initGui();
    
    objectController.attach(&control);    
  }

  void initGui(){
      gui(xamt,"xamt",-100,100);
      gui(rotamt,"rotamt",-100,100);
      gui(wt,"wt",-100,100);
      gui(amt,"amt",-100,100);
      gui(amtB,"amtB",-100,100);
      gui(dcurve,"dcurve",-10000,10000);
      gui(bReset,"reset")(bDist,"bDist")(bUseMotor,"bUseMotor")(bMean,"bMean")(bDrawShape,"bDrawShape");
      gui(bDrawCotan,"bDrawCotan")(bUseDGNormal,"bUseDGNormal")(bWeighNormals,"bWeighNormals");
      gui(bUseDGCurve,"bUseDGCurve")(bDrawTangent,"bDrawTangent")(bDrawNormals,"bDrawNormals")(bDrawDiffNorm,"bDrawDiffNorm");
      gui(bUseTangents,"bUseTangents")(bDrawLines,"bDrawLines");
      wt = 1;
      amt=.3;
      bMean = 0; //use directed content of mesh itself
  }
  
  
  /*-----------------------------------------------------------------------------
   *  CALCULATE NORMALS
   *-----------------------------------------------------------------------------*/
  void normal(){

    //calculate normals from displaced tangents (does not work)
    if (bUseTangents) {
      for (int i=0;i<fc.num();++i){
        fn[i] = Ro::dir(fc[i]).copy<Biv>().duale().unit();
      }
    } else {

    for (int i=1;i<fn.w()-1;++i){
      for (int j=1;j<fn.h()-1;++j){
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

          //simplicial edges
          Vec ea = pb - pa;
          Vec eb = pc - pa;
          Vec ec = pd - pa;
          Vec ed = pe - pa;

          //simplicial edge lengths
          auto xa = ea.norm();
          auto xb = eb.norm();
          auto xc = ec.norm();
          auto xd = ed.norm();

          auto xxa = fabs( ea.wt());//
          auto xxb = fabs( eb.wt());//
          auto xxc = fabs( ec.wt());//
          auto xxd = fabs( ed.wt()); 

           //simplicial pseudsocalars
          auto psa = !(ea^eb);
          auto psb = !(eb^ec);
          auto psc = !(ec^ed);
          auto psd = !(ed^ea);

          //area weights
          auto bwa = .5*(ea^eb).rnorm();
          auto bwb = .5*(eb^ec).rnorm();
          auto bwc = .5*(ec^ed).rnorm();
          auto bwd = .5*(ed^ea).rnorm();
          auto area = bwa+bwb+bwc+bwd;

          //simplicial recips;
          auto raa = eb<=psa; auto rab = -ed<=psd;
          auto rba = ec<=psb; auto rbb = -ea<=psa;
          auto rca = ed<=psc; auto rcb = -eb<=psb;
          auto rda = ea<=psd; auto rdb = -ec<=psc;
                   
         //normal (last working) 
         //enacts operation all around with reciprocals.
         //and sums
         auto na =  (raa^rbb).duale();
         auto nb =  (rba^rcb).duale();
         auto nc =  (rca^rdb).duale();
         auto nd =  (rda^rab).duale();


           if (bWeighNormals){
            na *= 1.0/bwa;//(xxa);//xa);
            nb *= 1.0/bwb;//(xxb);//xb);
            nc *= 1.0/bwc;//(xxc);//xc);
            nd *= 1.0/bwd;//(xxd);//xd);
          }

          //ccwise define
          Vec orient = (ea^eb).duale();
          Vec mnorm = na + nb + nc + nd;
          float mean = mnorm.norm() ;
          if (!bUseDGCurve) f2.at(i,j) = mean;
          
          Vec norm = (mnorm).unit();
          if ( (norm <= orient)[0]  < 0 ) norm = -norm;
          fn.at(i,j) = norm;
          
      }
    }

      // Edge Cases
     for (int i=0;i<fn.w();++i){
      for (int j=0;j<fn.h();++j){
        
        if (i==0){
          fn.at(i,j) = fn.at(i+1,j);//Vec(-1,0,0);//.unit();
        } else if (i==fn.w()-1){
          fn.at(i,j) = fn.at(i-1,j);//Vec(1,0,0);
        }
        if (j==0) {
          fn.at(i,j) = fn.at(i,j+1);//Vec(0,-1,0);
        } else if (j==fn.h()-1){
          fn.at(i,j) = fn.at(i,j-1);//Vec(0,1,0);
        }

        if (i==0&&j==0){
          fn.at(i,j) = fn.at(i+1,j+1);
        } else if (i==0&&j==fn.h()-1){
          fn.at(i,j) = fn.at(i+1,j-1);
        } else if (i==fn.w()-1&&j==0){
          fn.at(i,j) = fn.at(i-1,j+1);
        } else if (i==fn.w()-1&&j==fn.h()-1){
          fn.at(i,j) = fn.at(i-1,j-1);
        }
      }
    }
    }
  }

  void shape(){

      //Goal: Find shape operators i.e. exterior differential of normals 
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

          //simplicial edges
          Vec ea = pb - pa;
          Vec eb = pc - pa;
          Vec ec = pd - pa;
          Vec ed = pe - pa;

          //norms
          auto xa = ea.norm();
          auto xb = eb.norm();
          auto xc = ec.norm();
          auto xd = ed.norm();

          //simplicial psuedoscalars
          auto psa = !(ea^eb);
          auto psb = !(eb^ec);
          auto psc = !(ec^ed);
          auto psd = !(ed^ea);
          
          //area
          auto aa = (ea^eb).rnorm();
          auto ab = (eb^ec).rnorm();
          auto ac = (ec^ed).rnorm();
          auto ad = (ed^ea).rnorm();
          auto area = (aa + ab + ac + ad);

          //simplicial recips;
          auto raa = eb<=psa; auto rab = -ed<=psd;
          auto rba = ec<=psb; auto rbb = -ea<=psa;
          auto rca = ed<=psc; auto rcb = -eb<=psb;
          auto rda = ea<=psd; auto rdb = -ec<=psc;

          //weighted
          /* raa *= xa; rab *= xa; */
          /* rba *= xb; rbb *= xb; */
          /* rca *= xc; rcb *= xc; */
          /* rda *= xd; rdb *= xd; */
           
          
          /*-----------------------------------------------------------------------------
           *  CALCULATE PARTIAL DERIVATIVES OF F wrt EDGES
           *-----------------------------------------------------------------------------*/
          //normals
          auto n = fn.at(i,j);
          auto nb = fn.at(i+1,j);
          auto nc = fn.at(i,j+1);
          auto nd = fn.at(i-1,j);
          auto ne = fn.at(i,j-1);
          
          //normal differences along valency edges
          auto fa = nb-n;
          auto fb = nc-n;
          auto fc = nd-n;
          auto fd = ne-n;

          //differences projected onto edges (partial derivatives of n)
          auto wa = fa<=ea; //auto wa2=fb<=ea; auto wa3=fd<=ea;
          auto wb = fb<=eb; //auto wb2=fc<=eb; auto wb3=fa<=eb;
          auto wc = fc<=ec; //auto wc2=fd<=ec; auto wc3=fb<=ec;
          auto wd = fd<=ed; //auto wd2=fa<=ed; auto wd3=fc<=ed;

          //vector derivative per simplex?
          /* Vec vda = raa*wa + rbb*wb; */
          /* Vec vdb = rba*wb + rcb*wc; */
          /* Vec vdc = rca*wc + rdb*wd; */
          /* Vec vdd = rda*wd + rab*wa; */

          /* //Riemann (and Guassian) */
          Biv ba = ( raa*(wa) ) ^ ( rbb*(wb) ) ;
          Biv bb = ( rba*(wb) ) ^ ( rcb*(wc) ) ; 
          Biv bc = ( rca*(wc) ) ^ ( rdb*(wd) ) ; 
          Biv bd = ( rda*(wd) ) ^ ( rab*(wa) ) ;

          //dN (Spur, Trace) as the derivative of shape
          auto dN = (raa+rab)*wa/xa + (rba+rbb)*wb/xb + (rca+rcb)*wc/xc + (rda+rdb)*wd/xd;

         // Biv sa = (n^(raa*(wa)) 
          //Shape Commutators Bivectors . . .
          /* Biv sca = (n^(raa*(wa)) % (n^(rbb*(wb)) / (ea^eb).rnorm() ; */
          /* Biv scb = (n^(rba*(wb)) % (n^(rcb*(wc)) / (eb^ec).rnorm() ; */ 
          /* Biv scc = (n^(rca*(wc)) % (n^(rdb*(wd)) / (ec^ed).rnorm() ; */ 
          /* Biv scd = (n^(rda*(wd)) % (n^(rab*(wa)) / (ed^ea).rnorm() ; */

 
          // build on an e^i basis
          /* Biv ba = ( raa*(fa<=raa) ) ^ ( rbb*(fb<=rbb) ) ; */
          /* Biv bb = ( rba*(fb<=rba) ) ^ ( rcb*(fc<=rcb) ) ; */ 
          /* Biv bc = ( rca*(fc<=rca) ) ^ ( rdb*(fd<=rdb) ) ; */ 
          /* Biv bd = ( rda*(fd<=rda) ) ^ ( rab*(fa<=rab) ) ; */


          //border
          /* Vec bba = (bra*bwa); */
          /* Vec bbb = (brb*bwb); */
          /* Vec bbc = (brc*bwc); */
          /* Vec bbd = (brd*bwd); */


          //Mean
          /* auto ma = (raa+rab)*wa / ea.norm(); */
          /* auto mb = (rba+rbb)*wb / eb.norm(); */
          /* auto mc = (rca+rcb)*wc / ec.norm(); */
          /* auto md = (rda+rdb)*wd / ed.norm(); */

          /* float wfa1 = ( (fa<=raa) )[0] ; float wfa2 = ( (fa<=rab) )[0] ; */ 
          /* float wfb1 = ( (fb<=rba) )[0] ; float wfb2 = ( (fb<=rbb) )[0] ; */ 
          /* float wfc1 = ( (fc<=rca) )[0] ; float wfc2 = ( (fc<=rcb) )[0] ; */ 
          /* float wfd1 = ( (fd<=rda) )[0] ; float wfd2 = ( (fd<=rdb) )[0] ; */

          /* auto bna = (fa*wfa1) ^ (fb*wfb2); */
          /* auto bnb = (fb*wfb1) ^ (fc*wfc2); */
          /* auto bnc = (fc*wfc1) ^ (fd*wfd2); */
          /* auto bnd = (fd*wfd1) ^ (fa*wfa2); */


          //curl wrt simplex
          /* auto ca = dn <= (raa^rba) * ea.norm(); */
          /* auto cb = dn <= (rba^rbb) * eb.norm(); */
          /* auto cc = dn <= (rca^rcb) * ec.norm(); */
          /* auto cd = dn <= (rda^rdb) * ed.norm(); */
         
          //Riemann Curvature
          auto R = ((ba+bb+bc+bd)/area)*wt;
          //border
          //auto BR =(bba+bbb+bbc+bbd)*wt;
          //Pseudoscalar
          auto I = n.duale();
          //Gaussian Curvature -R<=!I
          auto kg =-(R<=!I)[0];
          //Spur, Trace, etc
          auto spur = dN;//(wa*(raa+rab)+wb*(rba+rbb)+wc*(rca+rcb)+wd*(rda+rdb));
          //Mean Curvature
          auto mean = -(n<=spur)[0] * wt;
          
          if (!bUseDGCurve) f.at(i,j) = kg;
          //if (!bUseDGCurve) f2.at(i,j) = mean;//R.rnorm();
        }
      }
  }

  void spur(){

      for (int i=1;i<fc.w()-1;++i){
        for (int j=1;j<fc.h()-1;++j){

          //this shape
          auto sa = fs.at(i,j);

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

          auto n = fn.at(i,j);//Ro::dir(a).copy<Biv>().duale().unit();

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
          auto rba = ec<=psb; auto rbb = -ea<=psa;
          auto rca = ed<=psc; auto rcb = -eb<=psb;
          auto rda = ea<=psd; auto rdb = -ec<=psc;

          //shapes along boundary
          auto sb = fs.at(i+1,j);
          auto sc = fs.at(i,j+1);
          auto sd = fs.at(i-1,j);
          auto se = fs.at(i,j-1);

          //shape differencing 
          auto fa = sb - sa;
          auto fb = sc - sa;
          auto fc = sd - sa;
          auto fd = se - sa;

          //dot with recips
          /* auto wa = (raa<=fa)+(rab<=fa);// ).norm(); */ 
          /* auto wb = (rba<=fb)+(rbb<=fb);// ).norm();//[0]; */ 
          /* auto wc = (rca<=fc)+(rcb<=fc);// ).norm();//[0]; */ 
          /* auto wd = (rda<=fd)+(rdb<=fd);// ).norm();//[0]; */ 

          //derivatives
          /* auto da = ea<=fa; */
          /* auto db = eb<=fb; */
          /* auto dc = ec<=fc; */
          /* auto dd = ed<=fd; */

          //vec contractions
          auto da = ea<=fa;
          auto db = eb<=fb;
          auto dc = ec<=fc;
          auto dd = ed<=fd;


          auto wa = raa*(raa<=da)+rab*(rab<=da);// ).norm(); 
          auto wb = rba*(rba<=db)+rbb*(rbb<=db);// ).norm(); 
          auto wc = rca*(rca<=dc)+rcb*(rcb<=dc);// ).norm(); 
          auto wd = rda*(rda<=dd)+rdb*(rdb<=dd);// ).norm(); 

          /* auto wa = fa / ea.norm(); */
          /* auto wb = fb / eb.norm(); */
          /* auto wc = fc / ec.norm(); */
          /* auto wd = fd / ed.norm(); */

          //auto NN = (fa*wa.norm() ) + (fb*wb.norm()) + (fc*wc.norm()) + (fd*wd.norm());

          auto NN = wa+wb+wc+wd;
         // auto NN = da+db+dc+dd;
          auto N = NN;//fnd.at(i,j);//wa+wb+wc+wd;

          //mean curvature
          auto mean = -n <= N;//NN.duale();
          if (!bUseDGCurve) f2.at(i,j) = mean.norm();
 
        }
      }
  }

  
  /*-----------------------------------------------------------------------------
   *  Classical cotan method (for comparison)
   *-----------------------------------------------------------------------------*/
  void cotan(){
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

          //simplicial edges
          Vec ea = pb - pa;
          Vec eb = pc - pa;
          Vec ec = pd - pa;
          Vec ed = pe - pa;

          //angles
          float aa = acos( (eb.unit()<=ea.unit())[0] );
          float ab = acos( (ec.unit()<=eb.unit())[0] );
          float ac = acos( (ed.unit()<=ec.unit())[0] );
          float ad = acos( (ea.unit()<=ed.unit())[0] );

          float deficit = TWOPI - (aa+ab+ac+ad);

          //circumcenters
          auto cb = Ro::loc(pa^pb^pc);
          auto cc = Ro::loc(pa^pc^pd);
          auto cd = Ro::loc(pa^pd^pe);
          auto ca = Ro::loc(pa^pe^pb);

          //dual edges
          Vec da = cb - ca;
          Vec db = cc - cb;
          Vec dc = cd - cc;
          Vec dd = ca - cd;

          //weights
          auto wa = da.norm() / ea.norm();
          auto wb = db.norm() / eb.norm();
          auto wc = dc.norm() / ec.norm();
          auto wd = dd.norm() / ed.norm();

          //normals
          auto na = ea * wa;
          auto nb = eb * wb;
          auto nc = ec * wc;
          auto nd = ed * wd;

          auto area = (((ea^eb)+(eb^ec)+(ec^ed)+(ed^ea))/2.0).rnorm();
          auto mean = (na + nb + nc + nd) / area;

          Vec ccw = (ea^eb).duale();
          if ( (mean<=ccw)[0] < 0 )  mean = -mean;
          
          if(bUseDGNormal) {
            fn.at(i,j) = mean.unit();
          }
          if(bUseDGCurve){
           // cout << "dg curve" << endl;
            f.at(i,j) = Sca(deficit/area) * wt; 
            //cout << deficit << endl;
            f2.at(i,j) = Sca(mean.norm() * wt/ 2.0);
          }

          if (bDrawCotan) DrawAt( -mean, pa, 1,0,0);
        }
      }
  }

    //see p399 hestenes "shape of dg in gc"
    virtual void onDraw(){ 
                
      //Control Point
      mouse = calcMouse3D();
      Frame frame(mouse[0], mouse[1], .1);
      frame.scale( amtB );

      //Generators
      auto par = frame.ipz() * amt;
      auto dll = Twist::Along( frame.dly(), amt, amtB);


     // DrawAt( control.z(), control.pos(), 0,0,1);
      
      /* for(int i=0;i<fc.num();++i){ */
      /*     auto s = Ro::dls_pnt( fc.grid(i), amtB ); */
      /*     auto r = control.tz().reflect( s ); */
      /*     Draw(r.dual(),1,1,0); */
      /*    // DrawAt( Ro::dir( r.dual() ).copy<Biv>(), Ro::loc( r ), 1,1,0); */
      /* } */


      //initial transform of tangents
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

          double xdist = 1.0 / (.1 + fabs( (fc.grid(i) <= frame.dlx() ).wt()) );
          double ydist = 1.0 / (.1 + fabs( (fc.grid(i) <= frame.dly() ).wt()) );

          auto saddle = Gen::bst( par * xdist - par * ydist );
                
          auto fcir =  bUseMotor ? cir.spin(motor) : cir.spin(boost);     
          fc[i] = fcir;
          auto wt = Ta::wt(fcir); 

          if (bDrawTangent) DrawAt( Ro::dir(fcir).copy<Biv>().duale(), Ro::loc(fcir),fabs(wt),1,1-fabs(wt)); 

          //jacobian as !x * f(x) where x = dlp ??
          /* auto pseudo = Ro::car(cir); */
          /* auto fpseudo = pseudo.spin(motor); */
          /* auto jacobian = !pseudo * fpseudo; */
       }
     }
     

      //Shape Calculations
      normal(); cotan(); 
      shape(); //spur(); 

      if (bDrawNormals){
        for(int i=0;i<fn.w();++i){
          for(int j=0;j<fn.h();++j){
            DrawAt( fn.at(i,j), Ro::loc(fc.at(i,j)),1,0,0);
          }
        }
      }

      if (bDrawLines) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

     glBegin( GL_TRIANGLES );
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


        auto tsa = (( bMean ? f2 : f).at(i,j)[0] );//.rnorm() );
        auto tsb = (( bMean ? f2 : f).at(i+1,j)[0]);//.rnorm() );
        auto tsc = (( bMean ? f2 : f).at(i,j+1)[0]);//.rnorm() );
        auto tsd = (( bMean ? f2 : f).at(i+1,j+1)[0]);//.rnorm() );

        auto sa = fabs(tsa);
        auto sb = fabs(tsb);
        auto sc = fabs(tsc);
        auto sd = fabs(tsd);
  
        auto ba = tsa > 0;
        auto bb = tsb > 0;
        auto bc = tsc > 0;
        auto bd = tsd > 0;

        auto na = Ro::dir(a).dual().copy<Vec>().unit();
        auto nb = Ro::dir(b).dual().copy<Vec>().unit();
        auto nc = Ro::dir(c).dual().copy<Vec>().unit();
        auto nd = Ro::dir(d).dual().copy<Vec>().unit();

        glColor3f( sa,ba,1-sa);
        GL::normal(na.begin()); GL::vertex(pa.begin());
        glColor3f( sb,bb,1-sb);
        GL::normal(nb.begin()); GL::vertex(pb.begin());
        glColor3f( sc,bc,1-sc);
        GL::normal(nc.begin()); GL::vertex(pc.begin());
        glColor3f( sc,bc,1-sc);
        GL::normal(nc.begin()); GL::vertex(pc.begin());
        glColor3f( sb,bb,1-sb);
        GL::normal(nb.begin()); GL::vertex(pb.begin());
        glColor3f( sd,bd,1-sd);
        GL::normal(nd.begin()); GL::vertex(pd.begin());


        //Draw(Ro::loc(pa) ^ Ro::loc(pb), 1,.3,.2); 
        //Draw(Ro::loc(pa) ^ Ro::loc(pc), 1,.3,.2); 
      }
     }
    glEnd();

 
      if(bDrawShape){
        for (int i=1;i<fs.w()-1;++i){
          for (int j=1;j<fs.h()-1;++j){
              auto S = fs.at(i,j);
              DrawAt(S, Ro::loc( fc.at(i,j)), 0,1,0,S.rnorm());
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


