/*
 * =====================================================================================
 *
 *       Filename:  xMeshImport.cpp
 *
 *    Description:  import obj file, turn into half-edge graph, manipulate with a bunch of knobs
 *
 *        Version:  1.0
 *        Created:  03/25/2015 12:04:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */


#include "vsr_app.h"   
#include "form/vsr_graph.h"
#include "form/vsr_field.h"
#include "form/vsr_knot.h"
#include "util/vsr_stat.h"

#include "gfx_file.h"
#include "gfx/gfx_map.h"

using namespace vsr;
using namespace vsr::cga;

//init overloade for Field of Frames (put this in somewhere, maybe in 
//vsr_cga3D_impl.h

template<>
void Field<Frame>::init(){
  for (int i=0;i<mNum;++i){
      mData[i].pos() = mPoint[i]; 
  }
}


/*-----------------------------------------------------------------------------
 *  DATA ABOUT A POINT (including distances to control points)
 *-----------------------------------------------------------------------------*/
struct MyData{
  Point point;             //Conformal Point
  Vec pos;                 //Position within lattice
  Dll dll;               //personal twist (unused)
  Par par;                 //personal pair;
  vector<float> distance;  //distances to control points;
};


/*-----------------------------------------------------------------------------
 *  NODE NETWORK INFORMATION
 *-----------------------------------------------------------------------------*/
// a node and links outward
struct NodeData{

  //points to a graph
  HEGraph<Vertex> * graph;
  
  int numLength=3;
  float timer=0;
  float randomoffset;// = Rand::Num();


  typedef HEGraph<Vertex>::Node Node;
  Node * node;
  Node * next;
  vector< Node * > chain;

  void clear(){
    chain.clear();
  }

  void seed( HEGraph<Vertex>& g ){
   node = &g.node( Rand::Num( g.node().size() ) );
   randomoffset = Rand::Num(.7,1.0);
    graph = &g;
    clear();
    extend();
  }

  void extend(){
    
    auto loop = node->edgeLoop();
    for (auto& i : loop ) chain.push_back(i->node);

    for (int i =0; i<numLength; ++i){
     loop = graph->edgeLoop( loop );
     for (auto& j : loop ) chain.push_back(j->node);       
    }

//    bool bFirst = true;
//    for (auto& j : loop ){
//      auto& edge = j;
//      chain.push_back(edge->node);
//      for (int i = 0; i < numLength; ++i){
//       if (edge->opp) edge = edge->opp->next;
//       else edge = edge->next;
//       chain.push_back(edge->node);
//     }
//     if (bFirst) { bFirst = false; next=chain.back(); }
//    }
    
  }

  float phase(float offset=0){
    return (1+sin(timer+offset))/2.0;
  }

  void step(float st){
    timer+=st*randomoffset;
    static int counter =0;
    if (phase() > .8) counter=1;
    if (phase() < .0001 && counter == 1) {
      counter = 0; 
      propagate();
    }
  }

  void propagate(){
    if (!chain.empty()) node = chain[ Rand::Num( chain.size() ) ];//.back();
    clear();
    extend();
  }

};



/*-----------------------------------------------------------------------------
 *  MAIN APPLICATION
 *-----------------------------------------------------------------------------*/
struct MyApp : App {
 
  MyApp() : App(1920,1080) {}

  File file;            //file writing output (gfx_file.h)

  //Some Variables
  float timer=0;        //timer (incremented by
  float dt;             //speed of time)
  
  bool bReset = false;  //reset mesh vertices

  //speed of model rotation about y axis
  float modelspinY;
  float modelspinX; //or x

  //general control geometry
  Frame frame;
  float framescale;              // size of frame
  float framexy,framexz,frameyz, framepos;//b,cirxy,cirxz;                // speed of rotation of control frame, and axis of circle
  vector<Point> controlpoint;   // control points around frame cxz 
  vector<Pair> controltangent;   // control tangents (null) around frame cxz 
  vector<Dll> controlline;   // control lines around frame cxz (y axis);
  int numcontrol;                // number of control points

  //transform selection: twist transform, knot transform, random twist
  bool bTwist, bKnot, bTangent, bTwistline, bTwistRandom;
  
  //twist parameters Amt of transformation, range of transformation,
  float twistAmt, twistRange;

  //knot parameters
  float theta,phi,mP,mQ = 0;
  float knotspeed; //speed of knot (is multipled by dt)
  
  //tangent parameters
  float tangentWt;

  //twistline parameters
  float twistWt, falloff;

  //control geometry for twisting
  Field<Frame> field;
  Field<Dll> dfield;

  //control geometry for orbiting
  TorusKnot tk;


  //mesh info
  vector<NodeData> nodeData;
  Mesh mesh;
  MBO * mbo;
  HEGraph<Vertex> graph;
  vector<MyData> data;

  //network info
  int numNodes, numChain, numEmanating; //unused
  float netspeed, netspeedtransmit;     //speed of pulsing net effect, and transmission of it (unused)



  //draw variables
  float farclip;                // far clipping plane (adjust if things disappear);
  float linewidth=5;
  bool bDrawFrame, bDrawField, bDrawWire, bDrawFill;
  bool bRender, bImmediate;     // output, immediate mode


  
  /*-----------------------------------------------------------------------------
   *  Setup Variables
   *-----------------------------------------------------------------------------*/
  void setup(){

    //Seed RNG
    Rand::Seed();
    
    //Bind Gui
    bindGLV();
    
    //Add Variables to GUI
    gui(dt,"dt",0,100);

    //general
    gui(modelspinY,"modelspinY",-100,100);
    gui(modelspinX,"modelspinX",-100,100);
    gui(linewidth,"linewidth",1,100);

    //network
    gui(netspeed,"netspeed",0,1000);
    gui(netspeedtransmit,"netspeedtransmit",0,1);
    gui(numNodes,"numNodes",0,1000);           // number of nodes
    gui(numEmanating,"numEmanating",0,1000);   // number of random extrusions per node
    gui(numChain,"numChain",0,1000);           // number of chain per node

    //deformation

    //frame control
    gui(framescale,"frame_scale",1,100);
    gui(framexy,"framexy",-100,100);
    gui(framexz,"framexz",-100,100);
    gui(frameyz,"frameyz",-100,100);
    gui(framepos,"framepos",-100,100);
    gui(numcontrol,"numcontrol",1,100);

    //reset
    gui(bReset,"bReset");

    //twist
    gui(bTwist,"twist");
    gui(bTwistRandom,"twist_random");
    gui(twistAmt,"twistAmt",-100,100);
    gui(twistRange,"twistRange",-100,100);

    gui(bTwistline,"twist_line");
    gui(twistWt,"twist_wt",-100,100);

    //tangents
    gui(bTangent,"tangent");
    gui(tangentWt, "tangent_wt",-1000,1000);
    gui(falloff,"falloff",-10,10);
    
    
    //knot
    gui(bKnot,"knot");
    gui(knotspeed,"knotspeed",-10,10);    
    gui(theta,"theta",-PI,PI);
    gui(phi,"phi",-PIOVERTWO,PIOVERTWO);
    gui(mP, "mP",0,200);
    gui(mQ, "mQ",0,200);

    //draw
    gui(farclip,"farclip",1,1000);
    gui(bDrawFrame, "bDrawFrame");
    gui(bDrawField, "bDrawField");
    gui(bDrawWire, "bDrawWire");
    gui(bDrawFill, "bDrawFill");
    gui(bRender,"bRender");
    gui(bImmediate,"bImmediate"); //immediate mode

    /*-----------------------------------------------------------------------------
     *  DEFAULTS
     *-----------------------------------------------------------------------------*/
    //general
    
    mSceneRenderer.immediate(true);  // set renderer to use programmable pipeline...
    farclip = 300;    
    linewidth=2;

    /*-----------------------------------------------------------------------------
     *  CAMERA
     *-----------------------------------------------------------------------------*/
    scene.camera.pos(0,0,20);
    mColor.set(0,0,0);

    
    //control
    framescale=10;
    numcontrol=10;
    frame.pos(0,2,0);

    falloff = .05;
    
    /*-----------------------------------------------------------------------------
     *  KNOT
     *-----------------------------------------------------------------------------*/
    mP=3; mQ=3;
    phi=-PIOVERTWO;

    
    /*-----------------------------------------------------------------------------
     *  LOAD MESH
     *-----------------------------------------------------------------------------*/
    mesh.mode(GL::T);
    //mesh.load( "resources/flowen/arco_dec_01.obj" );
    //mesh.load( "resources/flowen/hexa_dec_01.obj" );
    //mesh.load( "resources/flowen/oxa_tri_01.obj" );
    //mesh.load( "resources/flowen/moxi_dec_025.obj" );
    mesh.load("resources/obj/bunny.obj");
    
    mesh.scale(50); 
    mesh.store();
    //calc avg center and bounding box
    Vec3f v;  
    for (auto& i : mesh.vertex() ){
      v += i.Pos;
    }
    v *= 1.0/mesh.num();
    mesh.moveTo(-v);

    //bounds
    float maxX,maxY,maxZ,minX,minY,minZ; maxX=maxY=maxZ=0; minX=minY=minZ=10000;  
    for (auto& i : mesh.vertex() ){    
       
      if (i.Pos[0] > maxX) maxX = i.Pos[0];
      if (i.Pos[1] > maxY) maxY = i.Pos[1];
      if (i.Pos[2] > maxZ) maxZ = i.Pos[2];

      if (i.Pos[0] < minX) minX = i.Pos[0];
      if (i.Pos[1] < minY) minY = i.Pos[1];
      if (i.Pos[2] < minZ) minZ = i.Pos[2];
 
    }

    float rangeX = fabs(maxX-minX);
    float rangeY = fabs(maxY-minY);
    float rangeZ = fabs(maxZ-minZ);


    mbo = new MBO(mesh, GL::DYNAMIC);
 
    calcNormals();
    structure();

    
    /*-----------------------------------------------------------------------------
     *  FIELDS
     *-----------------------------------------------------------------------------*/
     field.resize(2,2,2,rangeX,rangeY,rangeZ); 
     dfield.resize(2,2,2,rangeX,rangeY,rangeZ); 

     for (int i=0;i<field.num();++i){
        field[i].pos() =field.grid(i);

        objectController.attach(&field[i]); //control
     }


     /*-----------------------------------------------------------------------------
      * GENERAL 
      *-----------------------------------------------------------------------------*/
      objectController.attach(&frame);
      frame.scale() = 15;
         
     /*-----------------------------------------------------------------------------
      *  Data Points
      *-----------------------------------------------------------------------------*/
      //cout << maxX << " " << minX << " " << rangeX << " " << rangeY << " " << rangeZ << endl;
      data = vector<MyData>(mesh.num());
      for (int i=0;i<mesh.num();++i){
        data[i].point = point(mesh[i].Pos[0], mesh[i].Pos[1], mesh[i].Pos[2]);
        //coordinate in hull
        data[i].pos[0] = fabs(mesh[i].Pos[0]-minX)/rangeX; 
        data[i].pos[1] = fabs(mesh[i].Pos[1]-minY)/rangeY; 
        data[i].pos[2] = fabs(mesh[i].Pos[2]-maxZ)/rangeZ; 
        //twist position start
        //data[i].dll = dfield.vol( data[i].pos );
        //if (data[i].pos[0] > .8) data[i].pos.print();
      }


  }

  void calcNormals(){
    auto& tmesh = mbo->mesh;
    for (int i = 0;i<tmesh.numIdx();i+=3 ){
       auto& a = tmesh.at(i);
       auto& b = tmesh.at(i+1);
       auto& c = tmesh.at(i+2);
       
       auto norm = (b.Pos-a.Pos).cross(c.Pos-a.Pos).unit();
       a.Norm = b.Norm = c.Norm = norm; 
    }
 
  }

  void structure(){
    
    auto& tmesh = mbo->mesh;
    //turn triangularized mesh into half-edge structure
    //would be could save this out and load it back . . .
    for (int i =0;i<tmesh.num();++i){
      graph.addNode( tmesh[i] );
    }
    
    //add facets
    for (int i = 0;i<tmesh.numIdx();i+=3 ){
       graph.fillFace( tmesh.idx(i), tmesh.idx(i+1), tmesh.idx(i+2) ); 
    }

    for (auto& i : graph.edge()){
        if (!i->bVisited){
          i->bVisited=true;
          for (auto& j : graph.edge()){
            if (!j->bVisited){
             if ( i->isOpp(*j) ){
                i->seal(*j);
                j->bVisited=true;
                break;
              }
             }
          }
        }
     }

     graph.reset();

     nodeData = vector<NodeData>(200);
     for (auto& i : nodeData) i.seed(graph);
  }

  
  /*-----------------------------------------------------------------------------
   *  Distance to planes of field
   *-----------------------------------------------------------------------------*/
  void calcDistance(){
    
  }


  void onAnimate(){

    auto& tmesh = mbo->mesh;

    //global
    timer += dt;    
    mSceneRenderer.immediate(bImmediate);

    scene.model.dBiv = Vec3f(modelspinX,modelspinY,0);
    scene.camera.lens.far() = farclip;

    //general control
    frame.db() = Biv(framexy,framexz,frameyz);
    frame.scale() = framescale;
    //frame.pos() = PAO.trs(Vec::x * framepos * sin(timer));
    frame.step();
    controlpoint  = vector<Point>(numcontrol);
    controltangent = vector<Pair>(numcontrol);
    controlline = vector<Dll>(numcontrol);
    for (int i=0;i<numcontrol;++i){
      float t = (float)i/numcontrol;
      auto vec = frame.x().rot( frame.xy() * PI * t);
      controlpoint[i] =  Round::split( Round::produce( frame.bound(), vec ), true).null();//frame.x().rot( //Round::pnt_cir( frame.cxy(), acos((frame.y() <= Vec::y)[0]) + t*TWOPI );
      controltangent[i] = Par(vec.copy<Tnv>()).trs( controlpoint[i] ); 
      controlline[i] = frame.dlz().trs(controlpoint[i]);
    }
    

    //twist
    if(bTwistRandom) {
      field[ Rand::Num(field.num()) ].db() = Biv( Rand::Num(twistAmt), Rand::Num(twistAmt), Rand::Num(twistAmt) ); 
      for (int i=0;i<field.num();++i){
        field[i].step();
      }
    }else{
     for (int i=0;i<field.num();++i){
        float t = (float)i/field.num();
        field[i].orient( Vec(0,0,0), twistAmt + twistRange * sin(timer+t*PIOVERTWO), field[i].pos()[0] < 0 );        
     }
    }

    for(int i=0;i<field.num();++i) dfield[i] = field[i].dll();


    //Knot
     tk.P = mP; tk.Q=mQ;
     tk.HF.vec() = Vec::x.spin( Gen::rot( theta,phi) );
     tk.HF.cir() = frame.cxz();



    if (bKnot){
      auto bst = tk.bst(knotspeed*dt);
      for(int i=0;i<mesh.num();++i){
        data[i].point = Round::loc( data[i].point.spin(bst) );
        mbo->mesh[i].Pos = data[i].point;
      }
    }

    if (bTwist){
     for(int i=0;i<mbo->mesh.num();++i){
       auto twist = Gen::mot( dfield.vol( data[i].pos ) );
       mbo->mesh[i].Pos = PAO.spin(twist);
     }
    }

    if (bTangent){
      //foreach foreach
      for(auto& i: data) {
        i.distance = vector<float>(numcontrol);
        i.par = Par();
        for(int j=0;j<numcontrol;++j){
          i.distance[j] = 1.0 / (falloff + Round::dist(i.point,controlpoint[j]) ); 
          i.par += controltangent[j] * i.distance[j];
        }
      }
      for (int i=0; i<tmesh.num();++i){
        tmesh[i].Pos = data[i].point.spin( Gen::bst(data[i].par*tangentWt) );
      }
    }

    if (bTwistline){
      //foreach foreach
      for(auto& i: data) {
        i.distance = vector<float>(numcontrol);
        i.dll = Dll();
        for(int j=0;j<numcontrol;++j){
          i.distance[j] = 1.0 / (falloff + Round::dist(i.point,controlpoint[j]) ); 
          i.dll += controlline[j] * i.distance[j];
        }
      }
      for (int i=0; i<tmesh.num();++i){
        tmesh[i].Pos = data[i].point.spin( Gen::mot(data[i].dll*twistWt) );
      }
 
    }

      calcNormals();

    if (bReset){
      mbo->mesh.reset();
      for (int i=0;i<mesh.num();++i){
        data[i].point = Round::null(mesh[i].Pos[0], mesh[i].Pos[1], mesh[i].Pos[2]);
      }
    }
 
    //coloring   
    for (auto& i : mbo->mesh.vertex() ){
      i.Col = Vec4f(0,1,.3,1);
    }

    //network
    for (auto& i : nodeData) {
      i.step(netspeed);
    }

    for (auto& i : nodeData ){
      i.node->data().Col += Vec4f(1*i.phase(),1*i.phase(),1*i.phase(),0);
      for (int j=0;j<i.chain.size();++j ){
        float v = (float)j/i.chain.size();
        float ph = i.phase() * i.phase(v*PIOVERTWO);
        i.chain[j]->data().Col += Vec4f( v*ph/4.0, 0, v*ph, 0 );
      }
    }

    for (auto& i : graph.edge() ){
      if (i->opp==NULL) i->a().Col = Vec4f(1,0,0,1);
    }

    mbo->update();
    
    


  }

  /*-----------------------------------------------------------------------------
   *  Draw Routines 
   *-----------------------------------------------------------------------------*/
  void onDraw(){
    

    //MESH OR SOLID RENDER
    if (bDrawWire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glLineWidth(linewidth);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(*mbo);

    if( bDrawField) {
      auto v = data::map<Dll>(10,10,10,[&](float x,float y,float z){ return dfield.vol(x,y,z); } );
      for (auto& i : v) Draw(Frame(i));

      for (auto& i : data){
      
        Draw(Frame( dfield.vol( i.pos) ) );
      }

    }
    if (bDrawFrame){
      Draw(frame);
      Draw(frame.cxz());
      for (auto& i : controlpoint) Draw(i);
      for (auto& i : controltangent) Draw(i);
    }

    //OUTPUT
    if (bRender){
      
     // file.write( windowData().width(), windowData().height(), "/Volumes/Medico/vsr_tmp_output/render/wire/", "wire");
     // file.writeAlpha( windowData().width(), windowData().height(), "/Volumes/Medico/vsr_tmp_output/render/wire_alpha/", "wire_alpha");
    
    }

  }

  void onResize(int w, int h){
    App::onResize(w,h);
    file.resize(w,h); 
  }
  
};


int main(){
                             
  MyApp app;
  app.start();

  return 0;

}
