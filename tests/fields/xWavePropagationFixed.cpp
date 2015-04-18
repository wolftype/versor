/*
 * =====================================================================================
 *
 *       Filename:  xWavePropagationFixed.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/02/2014 21:54:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


//#include "vsr_generic_op.h"  
#include "vsr_cga2D_types.h"
#include "vsr_cga2D_draw.h" 

#include "vsr_GLVimpl.h"

using namespace vsr;
using namespace vsr::cga2D;


struct Wavefront {

    Point cir;
    Point pnt;

    bool exists;

    bool isRef[4]   = {0,0,0,0};
    bool hasRef[4]  = {0,0,0,0};
    bool calcRef[4] = {1,1,1,1};

    Wavefront( const Point& p = Ro::null(0,0), float rad = .2) : 
    pnt(p), cir( Ro::dls(p, rad) )
    {}

   // Wavefront reflection[4];

    ~Wavefront(){}
    
    void propagate(float amt = 1.1){
      float rad = Ro::size(cir,true);
      cout << rad << endl; 
      cir = cir.dilate(pnt, log(amt)/rad);
    }

    bool sideOf( const Dll& dll ){
       auto p = Ro::cen( cir );
       auto s = p <= dll;
       return ( s[0] > 0 );
    }

    void draw(){
      Draw(cir,1,0,0);
    }

};


struct Boundary {

    Line top, bottom, left, right;
    Pair ptop, pbottom, pleft, pright;
    Point stop, sbottom, sleft, sright;

    vector<DualLine> vline; 
    vector<Pair> vpair;
    vector<Point> vpoint;

    
    Boundary( float w, float h) {
      Point tl = Vec(-w/2.0,-h/2.0).null(); 
      Point tr = Vec(w/2.0,-h/2.0).null(); 
      Point bl = Vec(-w/2.0,h/2.0).null(); 
      Point br = Vec(w/2.0,h/2.0).null(); 

      init(tl,tr,bl,br);
    }

    void init( const Point& tl, const Point& tr, const Point& bl, const Point& br){
        top = tl^tr^Inf(1);
        right = tr^br^Inf(1);
        bottom = br^bl^Inf(1);
        left = bl^tl^Inf(1);

        ptop = tl ^ tr;
        pright = tr ^ br;
        pbottom = br ^ bl;
        pleft = bl ^ tl;

        stop = Ro::sur( ptop );
        sright = Ro::sur( pright);
        sbottom = Ro::sur( pbottom);
        sleft = Ro::sur( pleft );

        vline.push_back(top.dual());
        vline.push_back(right.dual());
        vline.push_back(bottom.dual());
        vline.push_back(left.dual());

        vpair.push_back(ptop);
        vpair.push_back(pright);
        vpair.push_back(pbottom);
        vpair.push_back(pleft);

        vpoint.push_back(stop);
        vpoint.push_back(sright);
        vpoint.push_back(sbottom);
        vpoint.push_back(sleft);

    }
    
    void draw(){
      Draw(top,0,1,0); 
      Draw(bottom,0,1,0);
      Draw(right,0,1,0);
      Draw(left,0,1,0);
    }
    
};

struct WaveField {

  Wavefront waves[130];
  Boundary box;

  WaveField() : box(8,8) {}

  virtual void update(){

    cout << "NUM WAVES: " << waves.size() << endl; 

    int numWaves = waves.size();

    for (int i=0;i<numWaves;++i){//(auto& i : waves){

      Wavefront& w = *waves[i];
      
      for (int j=0;j<4;++j){
       //<t_úX> cout << i.calcRef[j] << endl; 
        if (w.calcRef[j]){
          
          auto meet = (w.cir ^ box.vline[j]).dual(); 
          
          if ( Ro::size(meet,true) < .02 ) {
            //cout << Ro::size(meet,true) << endl;

            //Draw(meet,0,0,1);    
            
            auto split = Ro::split(meet);
            auto tc = split[0];
            auto sa = tc <= box.vpoint[j];  
            auto sb = split[1] <= box.vpoint[j];   

            if (sa[0] <= 0){

              //NEW WAVE
              Wavefront * nw = new Wavefront(tc);
              for (auto& dll : box.vline){
                nw -> calcRef[j] = nw->sideOf(dll);
              }
              nw -> calcRef[j] = 0;

              
              w.calcRef[j] = 0;
              w.hasRef[j] = 1;
              waves.push_back( nw );
              w.reflection[j] = nw;
              
           }
          }
        }
     
        if (w.hasRef[j]) {
            w.reflection[j] -> cir = w.cir.re( box.vline[j] );
        }
      }
    }
  }

  void draw(){
     
      if (!waves.empty()) waves[0] -> propagate();
      for (auto& i : waves){
        i->draw();
      }

      box.draw();

  }

};

struct MyApp : App {    
   
  Point mouse;

  float time;
  float amt;

 // Boundary box;

  WaveField wavefield;

  MyApp(Window * win ) : App(win)
  {
    scene.camera.pos( 0,0,10 ); 
    time = 0;
  }

  void initGui(){
      gui(amt,"amt",-100,100);
      amt = 50;
  }
  
  void getMouse(){
    auto tm = interface.mouse.projectMid;     
    mouse = Round::null( tm[0] * amt, tm[1] * amt  ); 
   // mouse.print();
  }

  virtual void update(){    
    wavefield.update();
  }


   virtual void onDraw(){         
      getMouse();

      update();

      if (interface.mouse.isDown) {
          interface.mouse.isDown = false;
          for (auto& i : wavefield.waves) delete i;
          wavefield.waves.clear();
          Wavefront * wf = new Wavefront(mouse);
          wavefield.waves.push_back( wf );
      }

      wavefield.draw();      
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
