/*
 * =====================================================================================
 *
 *       Filename:  xGradient.cpp
 *
 *    Description:  Geometric Calculus . . . an ordinary gradient 
 *
 *        Version:  1.0
 *        Created:  11/22/2013 16:41:36
 *       Revision:  none
 *       Compiler:  gcc4.7 or clang3.2 or greater
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr_app.h"  
#include "form/vsr_differential.h" 

using namespace vsr;
using namespace vsr::cga;

template<class T>
Vec orthoGradient(const T& a, const T& b, const T& c){

  auto nx = b-a;
  auto ny = c-a;

  return (Vec::x * nx + Vec::y * ny ); 

}

struct MyApp : App {    
   
  Pnt mouse;
  Lin ray;

  float time;
  float amt;

  //A Field of scalar values
  Field<Scalar> scalar_field; 
  //A Vector Field
  Field<Vec> vector_field;

  void setup(){
    bindGLV();
    gui(amt,"amt",-100,100);
    scalar_field.resize(10,10,1);
    vector_field.resize(10,10,1);
  }
  
  void onDraw(){ 
        
     mouse = calcMouse3D();

     for ( int i=0; i < scalar_field.w(); i+=1 ) {
       float tu = amt * (float)i/scalar_field.w();
       
       for (int j=0; j < scalar_field.h(); j+=1 ){          
          float tv = amt * (float)j/scalar_field.h();
          
          scalar_field.at(i,j) = Sca(tu * tv);
       
       }
     }

     for (int i =0;i<vector_field.w(); ++i){
        for (int j = 0;j<vector_field.h(); ++j){
            int ni =  (i < vector_field.w()-1) ? i+1 : 0;
            int nj = (j < vector_field.h()-1) ? j+1 : 0;
            vector_field.at(i,j) = orthoGradient(scalar_field.at(i,j), scalar_field.at(ni,j), scalar_field.at(i,nj) );
        }
     }
     
     Draw(vector_field);
    
  }
   
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
