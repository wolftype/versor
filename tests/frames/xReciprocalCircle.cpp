/*
 * =====================================================================================
 *
 *       Filename:  xReciprocalCircle.cpp
 *
 *    Description:  tests
 *
 *        Version:  1.0
 *        Created:  04/09/2014 18:38:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

/*
 * =====================================================================================
 *
 *       Filename:  xReciprocal.cpp
 *
 *    Description:  reciprocal frames for facet / ray intersection
 *
 *    implementation of an algorithm from "Covarient Approach to Geometry using Geometric Algebra", A. Lasenby, J. Lasenby, R. Wareham, p52)
 *
 *        Version:  1.0
 *        Created:  11/22/2013 13:33:48
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or greater (C++11)
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  pretty awesome
 *
 * =====================================================================================
 */


#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga3D;

#define INF Inf(1)

Pnt rframe(const Pnt& pa, const Pnt& pb){
  auto pair = pa ^ pb;
  VT norm = pair.rnorm();
  VT sqnorm = norm*norm; //this is just the wt() (self dot product)

  auto rpa = (pb)<=pair;
  return rpa / -sqnorm;
}

vector<Pnt> rframe(const Pnt& pa, const Pnt& pb, const Pnt& pc){
    vector<Pnt> recip;
    //1. Make a "tangent space" (here the subspace pseudoscalar is a direct OPNS circle)
    auto circle = pa ^ pb ^ pc;
    VT norm = circle.rnorm();
    cout << norm << endl; 
    VT sqnorm = norm*norm;

     //2. A reciprocal frame is built from contraction of the the wedge of others with the plane pseudoscalar . . .
    auto rpa = ( ( pb ^ pc  ) <= circle );
    auto rpb = ( ( pc ^ pa  ) <= circle );
    auto rpc = ( ( pa ^ pb  ) <= circle );

    //AND Normalized?
    rpa /= (norm);
    rpb /= (norm);
    rpc /= (norm);
    
    recip.push_back(rpa);
    recip.push_back(rpb);
    recip.push_back(rpc);

    return recip;
}

vector<Pnt> rframe(const Pnt& pa, const Pnt& pb, const Pnt& pc, const Pnt& pd){
    vector<Pnt> recip;
    //1. Make a "tangent space" (here the subspace pseudoscalar is a direct OPNS sphere)
    auto sph = pa ^ pb ^ pc ^ pd;
    VT norm = sph.rnorm();
    cout << norm << endl; 
    VT sqnorm = norm*norm;

     //2. A reciprocal frame is built from contraction of the the wedge of others with the plane pseudoscalar . . .
    auto rpa = ( ( pb ^ pc ^ pd  ) <= sph );
    auto rpb = ( ( pc ^ pd ^ pa  ) <= sph );
    auto rpc = ( ( pa ^ pb ^ pc ) <= sph );

    //AND Normalized?
    rpa /= (-sqnorm);
    rpb /= (-sqnorm);
    rpc /= (-sqnorm);
    
    recip.push_back(rpa);
    recip.push_back(rpb);
    recip.push_back(rpc);

    return recip;
}


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt, amtA,amtB,amtC,vNum;
  bool bDrawRecip, bNormalize, bAreaNormal, bAreaWeight;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      gui(amtA,"amtA",-1,1);
      gui(amtB,"amtB",-1,1);
      gui(amtC,"amtC",-1,1);
      gui(vNum,"num",1,100);
      gui(bDrawRecip, "drawRecip")(bNormalize,"normalize")(bAreaNormal,"areaNormal")(bAreaWeight,"areaWeight");

  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto tm = interface.mouse.projectMid;
      ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ INF; 
      mouse = Round::point( ray,  Ori(1) );  
  }

    virtual void onDraw(){ 
        
      getMouse();

      Pnt vertex = Ro::null(amtB,amt,0);
      int num = vNum;
      vector<Pnt> nbr;
      for (int i=0; i<num;++i){
        float t = (float)i/num;
        nbr.push_back( Ro::null( Vec::x.rot( Biv::xz * ( PI  + PIOVERTWO * t ) )  ));// + PIOVERTWO * t
        Draw(nbr[i],1,0,0);
      }

      vector<Pnt> rp;
      for (int i=0; i<num; i++){
        int next = i<(num-1) ? i+1 : 0;
        glColor3f(1,0,0);
        gfx::Glyph::Line( vertex, nbr[i] );
        gfx::Glyph::Line( nbr[i], nbr[next] ); 

       // auto trp = rframe( nbr[i], vertex ) + Inf(amtC) ;//,nbr[next])[0];
       // auto trp = rframe(vertex, nbr[i], nbr[next] )[0] + Inf(amtC);
        auto trp = rframe( vertex, nbr[i] );//, nbr[next] )[0] ;
        trp =  bNormalize ? Ro::cen(trp) : trp;// /= ( bNormalize ? trp.norm() : 1);
        rp.push_back( trp );
        if(bDrawRecip) Draw( trp , 0,1,1,.4);//+ Inf(amtA)
      }


      Pnt sum;
      for (int i=0;i<num;i++){
        sum += rp[i];// / ( bAreaWeight ? rp[i][3] : 1);
      }

      cout << sum.rnorm() << endl;
      sum.vprint();
      sum = Ro::cen(sum);
      sum.vprint();
     // Draw(sum,0,0,1,.2);
      DrawAt( Vec(sum), vertex,0,1,1);
      Draw(sum,1,1,0,.2);


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

