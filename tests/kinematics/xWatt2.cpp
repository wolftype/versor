#include "vsr_cga3D.h" 
#include "vsr_GLVimpl.h"
#include "vsr_stat.h"

using namespace vsr;
using namespace vsr::cga3D;


struct Watt{

   Watt(float _distance=0, float _ra=0, float _rb=0, float _rc=0)
   : fitness(0), length(0), distance(_distance), ra(_ra), rb(_rb), rc(_rc){}
  
   float distance; ///< distance between points a and b
   float ra,rb;    ///< length of end rods in terms of distance
   float rc;       ///< length of connecting rod in terms of distance

   float fitness;  ///< fitness metric of linkage to accomplish desired path
   float length;///< max spread of path 

   vector<Point> vp, ap, bp;
   /// spit out a vector of points
   Watt& operator()(){

     vp.clear();
     ap.clear();
     bp.clear();
     fitness = 0;
     length = 0;

     Point pa = Ro::point(-distance/2,0,0);
     Point pb = Ro::point(distance/2,0,0);
     Dls sa = Ro::dls(pa,distance*ra);
     Dls sb = Ro::dls(pb,distance*rb);

     Cir ca = (sa^Dlp(0,0,1)).dual();
     auto cb = (sb^Dlp(0,0,1));
    
     //samples
     for (int i=0;i<50;++i){
        
        double theta = PI * i/50;
        auto tp = Ro::pnt_cir(ca, theta);
        Dls sc = Ro::dls(tp, rc*distance);
        
        //intersection
        auto meet = (sc ^ sb).dual();

        Vec tmid; Vec last;
        if (Ro::size(meet, false) >= 0){
          auto par = ( meet.dual() ^ Dlp(0,0,1) ).dual();
          auto tq = Ro::loc( Ro::split(par,true));
          auto mid = ( tp + ( (tq - tp ) *.5) ).null();
          vp.push_back(mid);
          ap.push_back(tp);
          bp.push_back(tq);
          //spread
          tmid += Vec(mid) - last;
          last = mid;

          //collinearity
         // auto pln = mid ^ LN(0,1,0);          
         // fitness += fabs(pln.rnorm());
        }
        Lin ln;
        if (!vp.empty()){
          ln = vp[0] ^ vp[ vp.size()-1] ^ Inf(1);
          for (auto& i : vp){
            auto pln = i ^ ln;
            fitness += fabs(pln.rnorm());
          }
        }
        length = fabs(tmid.norm());
     }

     return *this;
   }

   void draw( float r, float g, float b, bool bDrawAll=false){
     Point pa = Ro::point(-distance/2,0,0);
     Point pb = Ro::point(distance/2,0,0);
     

     for (int i=0;i<vp.size();++i){
      // glColor3f(.5,.1,.5);
       glColor3f(r,g,b);
       Glyph::Lines(vp,vp.size());
     }

    if(bDrawAll){
       Draw( (Ro::dls(pa,distance*ra)^Dlp(0,0,1)).dual(),0,0,1);
      Draw( (Ro::dls(pb,distance*rb)^Dlp(0,0,1)).dual(),0,0,1);

     if (!vp.empty() ){
       glColor3f(b,r,b);
       Glyph::Line(pa,ap[0]);
       Glyph::Line(ap[0],bp[0]);
       Glyph::Line(bp[0],pb);
     }
    }

   }
};


struct MyApp : App {    
   
  Pnt mouse;
  Lin ray; 
  
  float time;
  float maxfit, minlen,  numsamples;
  bool bDraw;

  MyApp(Window * win ) : App(win){
    scene.camera.pos( 0,0,10 ); 
  }
  
    void getMouse(){
      auto tv = interface.vd().ray; 
      Vec z (tv[0], tv[1], tv[2] );
      auto v= interface.mouse.projectMid; 
      ray = Ro::point(v[0],v[1],v[2] ) ^ z ^ Inf(1); 
      mouse = Ro::point( ray,  Ori(1) );  
   }   

  void initGui(){
    gui(maxfit,"maxfit",0,50); 
    gui(minlen,"minlen",0,1000); 
    gui(numsamples,"numsamples",0,1000); 
    gui(bDraw);

  }

    virtual void onDraw(){ 
        
    getMouse();

   // Rand::Seed(10);
    srand(10);
    
    time += .01;
    float minfit=1000;
    float maxlen=0;
    Watt minwatt, maxwatt;

    float distance = 5;
    float bestratio;
    vector<Vec> plot;
     for (int i=0; i<numsamples; ++i){
          float t = (float)i/numsamples;
          float ra = Rand::Num(2); 
          float rb = Rand::Num(2); 
          float rc = Rand::Num(2);
          Watt watt(distance, ra, rb, rc); 
          watt();
          if (bDraw) watt.draw(t,0,1-t);
           
           if (watt.fitness < minfit) { 
            if (watt.length>minlen) {    
             minwatt = watt; 
             minfit = watt.fitness; 
            }
          }
          
          
           if (watt.length > maxlen) { 
             if (watt.fitness < maxfit){
               maxwatt = watt; 
               maxlen = watt.length; 
             }
          }
          //if (watt.fitness 
     }

     minwatt().draw(1,0,0,true);
     maxwatt().draw(0,1,1,true);

    // cout << maxlen << endl;

     //Watt watt(distance, radius, radius, ratio);
     //watt();
     //watt.draw();

     /* for (auto& i : plot){ */
     /*   Draw( (Vec(5,5,0)+i).null(), 0,0,1); */
     /* } */
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

