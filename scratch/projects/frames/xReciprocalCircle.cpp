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


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

#define INF Inf(1)

Pnt rframe(const Pnt& pa, const Pnt& pb){
  auto pair = pa ^ pb;
  VSR_PRECISION norm = pair.rnorm();
  VSR_PRECISION sqnorm = norm*norm; //this is just the wt() (self dot product)

  auto rpa = (pb)<=pair;
  return rpa / -sqnorm;
}

vector<Pnt> rframe(const Pnt& pa, const Pnt& pb, const Pnt& pc){
    vector<Pnt> recip;
    //1. Make a "tangent space" (here the subspace pseudoscalar is a direct OPNS circle)
    auto circle = pa ^ pb ^ pc;
    VSR_PRECISION norm = circle.rnorm();
    cout << norm << endl; 
    VSR_PRECISION sqnorm = norm*norm;

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
    VSR_PRECISION norm = sph.rnorm();
    cout << norm << endl; 
    VSR_PRECISION sqnorm = norm*norm;

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

  float time;
  float amt, amtA,amtB,amtC,vNum;
  bool bDrawRecip, bNormalize, bAreaNormal, bAreaWeight;



  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
      gui(amtA,"amtA",-1,1);
      gui(amtB,"amtB",-1,1);
      gui(amtC,"amtC",-1,1);
      gui(vNum,"num",1,100);
      gui(bDrawRecip, "drawRecip")(bNormalize,"normalize")(bAreaNormal,"areaNormal")(bAreaWeight,"areaWeight");

  }
 
    virtual void onDraw(){ 
        
      mouse = calcMouse3D();

      Pnt vertex = Round::null(amtB,amt,0);
      int num = vNum;
      vector<Pnt> nbr;
      for (int i=0; i<num;++i){
        float t = (float)i/num;
        nbr.push_back( Round::null( Vec::x.rot( Biv::xz * ( PI  + PIOVERTWO * t ) )  ));// + PIOVERTWO * t
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
        trp =  bNormalize ? Round::cen(trp) : trp;// /= ( bNormalize ? trp.norm() : 1);
        rp.push_back( trp );
        if(bDrawRecip) Draw( trp , 0,1,1,.4);//+ Inf(amtA)
      }


      Pnt sum;
      for (int i=0;i<num;i++){
        sum += rp[i];// / ( bAreaWeight ? rp[i][3] : 1);
      }

      cout << sum.rnorm() << endl;
      sum.print();
      sum = Round::cen(sum);
      sum.print();
     // Draw(sum,0,0,1,.2);
      DrawAt( Vec(sum), vertex,0,1,1);
      Draw(sum,1,1,0,.2);


  }
   

  
};



int main(){
                             
  MyApp app;

  app.start();

  return 0;

}

