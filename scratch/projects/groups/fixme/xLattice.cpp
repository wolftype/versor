//#include "vsr_cga2D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_group.h"
#include "vsr_generic_draw.h"

//#include "vsr_cga3D_draw.h"

using namespace vsr;
//using namespace vsr::cga3D;


template<class V>
void Drw(const PointGroup2D<V>& p){
  for (auto i : p.ops ) Draw(i);
}

/* template<class V> */
/* void Drw(const Lattice<V>& l){ */
/*   for (auto i : l.vec) Draw(i); */
/* } */

struct MyApp : App {  
  
  float time;  

  float p,q,r;
  float ra,rb,rc,dist;
  bool bOrtho;
   
	MyApp(Window * win ) : App(win){
		scene.camera.pos( 0,0,10 ); 
    time = 0;
	}

    void initGui(){
      gui(p,"p",0,10)(q,"q",0,10)(r,"r",0,10); p = 2; q = 3, r=3;
      gui(ra)(rb)(rc)(dist,"dist",0,100);
      gui(bOrtho,"ortho");
    }
  
  	virtual void onDraw(){ 

        scene.camera.lens.ortho( bOrtho );
        time +=.01;

        static const int DIM = 4;

        NEBiv<DIM> b(0,0,0,time * ra,time* rb,time*rc);

        Lattice< DIM, NEVec<DIM> > lattice(p, q, r);

        for ( auto& i : lattice.vec ){
          i = i.rot(b);
        }

        for (int i = 0; i < DIM; ++i ){
          for (int j = 0; j < DIM; ++j ){
            for (int k = 0; k < DIM; ++k){
              for (int m = 0; m < DIM; ++m){
               auto v = lattice.at(i,j,k,m);
                for (int i = 0; i < lattice.Dim; ++i){
                  DrawAtB( Proj<DIM>::Call(dist, lattice.vec[i] ), v);//Proj<DIM>::Call(dist, v ) );
                }
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
	app = new MyApp( win ); 
  app -> initGui();
  
	
	glv << *app;

	Application::run();

	return 0;

}
