/*
 * =====================================================================================
 *
 *       Filename:  xElectret.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2013 12:34:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */



#include "vsr_cga3D.h"   
#include "vsr_app.h"

using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {    
   
  float amt;

  void setup(){
      bindGLV();
      gui(amt,"amt",-100,100);
  }
  
 virtual void onDraw(){ 
        
   Field<Vec> field(10,10,10);
   Field<Flp> electric(5,5,5,2);

   bool bSource = true;
   for (int j = 0; j < electric.num(); ++j){
     bSource = !bSource;
     float val = amt * ( bSource ? -1 : 1 ) ;
     electric[j] = electric.grid(j) ^ Inf( val );
   }

   
   auto gen = calcMouse3D() ^ Inf(-.1);              //<-- A "Flat Point"
   auto source_dilation = Gen::bst( gen );           //<-- Exponential of Flat Point creates a DILATION

   for (int i = 0; i < field.num(); ++i){
     Flp flp;
     for (int j = 0; j < electric.num(); ++j){
       float dist = Round::sqd( field.grid(i), electric.grid(j) );
       flp += electric[j] * ( 1.0 / (dist + .1) );
     }
     auto s = Gen::bst( flp );
     Point np = field.grid(i).spin( s );
     np = Round::location( np );/// np[3];
     field[i] = np - field.grid(i);
   }

     Draw(field);
    
  }
   

  
};




int main(){
                             
  MyApp app;
  app.start();
  
  return 0;

}
