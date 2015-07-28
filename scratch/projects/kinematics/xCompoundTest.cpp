/*
 * =====================================================================================
 *
 *       Filename:  xCompoundTest.cpp
 *
 *    Description:  fix a bug
 *
 *        Version:  1.0
 *        Created:  04/22/2015 15:02:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   

using namespace vsr;
using namespace vsr::cga;

struct MyApp : App {
 
  //Some Variables
  bool bReset = false;
  float amt = 0;


  vector<Frame> mFrame = vector<Frame>(4);
  vector<Frame> mJoint = vector<Frame>(4);

   vector<Frame> mLink =  vector<Frame>(4);
  
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){
    ///Bind Gui
    bindGLV();
    ///Add Variables to GUI
    gui(amt,"amt",-100,100)(bReset,"bReset");



    
  }


  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){

    mFrame[0].rot() = Gen::rot( Biv::xy * amt );

    for (int i = 1; i < mFrame.size(); ++i){
      float t= (float)i/mFrame.size();
      mFrame[i] = mFrame[i-1].mot() * Gen::trs(0,1,0) * Gen::rot( Biv::xz * PIOVERTWO * t ) * Gen::rot (Biv::xy * t/2.0);
    }
  
    int mNum = mFrame.size();
    
    Rot ry(1);
   // Vec ty = Vec::y;
  
    for (int i=0; i<mNum;++i){
     int next = i < (mNum-1) ? i + 1 : 0;
     auto ylength =  (mFrame[next].vec() - mFrame[i].vec()).norm(); 
     auto theta = acos( (mFrame[i].z()<=mFrame[next].z())[0] );
     mLink[i].rot() = Gen::rotor( Biv::xz * theta * .5 );
     mLink[i].pos() = Vec(0, ylength, 0).null(); 
    }

    //NOTE: adjusted seems to work better (projecting onto plane)
    for (int i = 0; i < mNum; ++i){
      
      int next = i < (mNum-1) ? i + 1 : 0;            
      auto target = (mFrame[next].vec() - mFrame[i].vec() ).unit();         //target direction
      auto ty = Vec::y.spin( ry );
      auto theta = Gen::iphi( Gen::ratio(ty,target) );//acos( (ty<=target)[0] );//
      auto xyRot = Gen::rotor( Biv::xy * theta/2.0);

      auto tmpRot = Gen::ratio( Vec::y, target );                           //how to get there from origin
      tmpRot = !ry * tmpRot;
      Vec correctedTarget = Vec( Op::project( Vec::y.spin( tmpRot ), Biv::xy ) ).unit();  //project onto xy axis of local link
      
      auto adjustedRot = Gen::ratio( Vec::y, correctedTarget );
      
     // auto linkRot = Gen::ratio(Vec::y,  mLink[i].vec().unit() );           //what link position does...
     // auto adjustedRot = !ry * tmpRot;//!linkRot * tmpRot * !ry;//target.spin( !ry * !linkRot );                  //...compensate for that
//      DrawAt( Vec::y.spin(adjustedRot), mFrame[i].vec(),0,0,1);
     
     // Vec y = Vec::y.spin(ry);                                              // spin Y by current rotation
      mJoint[i].rot() = adjustedRot;//Gen::ratio( Vec::y, adjustedTarget);//Gen::rot( Biv::xy * acos( ( dv <= y )[0] )/2.0  ); //set rotation
      ry = ry * mJoint[i].rot() * mLink[i].rot();                  //compound

    }

    Frame tFrame[4];
    tFrame[0] = mFrame[0];
    tFrame[0].rot() = mJoint[0].rot();
    Draw(tFrame[0]);
    for (int i=1;i<mNum;++i){

      tFrame[i] = tFrame[i-1].mot() * mLink[i-1].mot() * mJoint[i].mot();
     // else tFrame[i] = tFrame[i-1].mot() * mLink[i-1].mot();
      if (bReset) Draw(mFrame[i]);
      else Draw(tFrame[i]);
    }



  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
