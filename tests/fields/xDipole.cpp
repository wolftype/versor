#include "vsr_cga3D.h"  
#include "vsr_cga3D_app.h"

#include "vsr_field.h"
#include "vsr_twist.h" 

#include "gfx/gfx_texture.h"
#include "gfx/gfx_fbo.h" 

using namespace vsr;


struct MyApp : App {  
  
  float time;   
  
  unsigned char * pixels; 

     Field<Vec> f; 
     Field<Vec> ortho; 
    
  int numDipoles;
    Par * par;
    Pnt * pos;  
  
  int numPotentials;
  Mesh * potentialsA;  
    Mesh * potentialsB;  
  int pRes;  
  
  float step; 
  float amt, amtPot, amtPos;
  float speed;  
  float period, pitch; 
  
  bool bReset, bDrawDll, bSave;   
  
  
  
  //ALL THIS IS THE GL PIPELINE STUFF FOR ADVANCED RENDERING.
  MBO * rect;   
  Pipe texpipe; 
  Pipe texalpha;   
     
  FBO * fboA;
  Texture * textureA;    
  Texture * textureB;
  
  float traceAmt; 
  
  MyApp() : App(), 
  f(40,50,1), 
  ortho(40,50,1), 
  numPotentials(100), 
  numDipoles(5),
  pRes(200),
  step(.5),
  amt(1.),
  speed(.005),
  bSave(0)
  {
     
    pos = new Pnt[numDipoles];
    par = new Par[numDipoles];
   
    potentialsA = new Mesh[numPotentials]; 
    potentialsB = new Mesh[numPotentials]; 
    
    for (int i = 0; i < numPotentials; ++i){ 
      float t = 1.0 * i/numPotentials;
      potentialsA[i] = Mesh::Contour(pRes).color(0,1,t ).mode(GL::LS); 
      potentialsB[i] = Mesh::Contour(pRes).color(0,1,t ).mode(GL::LS); 
    }   
    
    initDipoles();
    
  } 
  
  void initDipoles(){
    for (int i = 0; i < numDipoles; ++i){  
      float t= TWOPI * i/numDipoles;   
      pos[i] = Ro::point( Vec( .5 * cos(t) * f.tw(), sin(t) * f.th()/2.0, 0)); 
      par[i] = Par(Tnv(0,1,0)).trs( pos[i] ); 
    }
  }           
  
  virtual void setup(){
    bindGLV();

    gui(step,"step",0,10);
    // gui(pRes, "res",10,1000);   
    gui(amt, "amt", 0,1000);
    gui(amtPot, "amtPot", 0, 1000);   
    gui(amtPos, "amtPos", 0, 1000); 
    gui(speed, "speed", 0,1000);   
    gui(pitch, "pitch", 0,1000); 
    gui(bReset)(bDrawDll,"drawdll")(bSave,"bSave"); 
    gui(traceAmt, "trace",0,10);
  }

  virtual void onDraw(){ 

  glLineWidth(3);
  
  time += speed /10.0;
  
  static Dll dll = DLN(0,0, 1);
  Touch(interface, dll);
  if (bDrawDll) {
    Draw(dll);
    for (int i = 0; i < numDipoles; ++i){
      Draw( pos[i] );
    }
  }
  if (bReset) {
   for (int i = 0; i < numDipoles; ++i){
    float t= TWOPI * i/numDipoles;                         
    pos[i] = Ro::point( Vec( .5 * cos(time + t) * f.tw(), sin(time+t) * f.th()/2.0, 0));
    pos[i] = pos[i].mot(  dll * t * time * PI  );                                                          
    par[i] = Par(Tnv(0,1,0)).trs( pos[i] );  
  }   
  }
  else {
  
     for (int i = 0; i < numDipoles; ++i){   
      Par tpar;
     
        for (int j = 0; j < numDipoles; ++j){  
        if (i != j){
        VT dist = Ro::sqd( pos[i], pos[j] ) + 1.;  
        tpar += par[j] * (amtPos/dist);    
        }
      } 
    
      pos[i] = Ro::loc( pos[i].bst( tpar ) );
  
    }  
  }    
    
  for (int i = 0; i< f.num();++i){ 
    static Par tpar; 
    tpar.reset();
    
    for (int j = 0; j < numDipoles; ++j){ 
      VT dist = Ro::sqd( f.grid(i), pos[j] ) + 1.; 
      tpar +=  par[j] *  (amt /(dist) );
    }

    Pnt np = Ro::loc( f.grid(i).bst( tpar ) );
    f[i] = Vec(np) - Vec( f.grid(i) );
    ortho[i] = f[i].rot( Biv::xy * PIOVERFOUR );  
  }          
  
  glBegin(GL_LINES); 
  for (int i = 0; i< f.num(); ++i) {
    VT aamt = fabs( ( f[i] <= Vec::x )[0] );
    glColor4f(1, 0, 0, (.7*aamt));
    GL::vertex( f.grid(i) );
    GL::vertex( Vec(f.grid(i)) + f[i] ) ;  
    
    glColor4f(0, .2, 1, 1-(.5*aamt)); 
    GL::vertex( ortho.grid(i) );  
    GL::vertex( Vec(ortho.grid(i)) + ortho[i] );   
  } 
  glEnd();   
  
  
  Biv br = Biv::xy * PIOVERTWO ;

    for (int i = 0; i < numPotentials; ++i){  
      
    // potentialsA[i] = Mesh::Contour(pRes).color(0,1,t * .4).mode(GL::LS); 
    // potentialsB[i] = Mesh::Contour(pRes).color(0,1,t * .4).mode(GL::LS);
  
    float theta = (TWOPI * i/numPotentials);
    float ta = cos( time ) * ortho.tw()/2.0;
    float tb = sin( theta ) * ortho.th()/2.0;
     
    Vec nva(0, tb, 0);  //starting points
    Vec nvb(0, tb, 0);   
    
    for (int j = 0; j < pRes; ++j){
      potentialsA[i][j].Pos.set ( nva[0], nva[1], nva[2] );
      potentialsB[i][j].Pos.set ( nvb[0], nvb[1], nvb[2] );
            
      // Par tparA, tparB; 
      // Pnt pva = Point( nva );
      // Pnt pvb = Point ( nvb );
      //        for (int j = 0; j < numDipoles; ++j){ 
      //   VT distA = Ro::sqd( pva, pos[j] ) + 1.;  
      //   VT distB = Ro::sqd( pvb, pos[j] ) + 1.;  
      //   tparA +=  par[j] *  (amtPot /(distA) );  
      //   tparB +=  par[j] *  (amtPot /(distB) );
      //         }               

       // nva = nva + ( Vec( Ro::loc( pva.bst( tparA) ) ) - nva).rot( br );  
       // nvb = nvb + ( Vec( Ro::loc( pvb.bst(-tparB) ) ) - nvb).rot( br );  
       nva -= ortho.euler2d( nva ) * step;
       nvb += ortho.euler2d( nvb ) * step;  
    } 
    
    potentialsA[i].drawElements(); 
    potentialsB[i].drawElements();
  }  
  
  }   

  //We can add a bit of the previously drawn frame  
  void addTrace(){
    texalpha.bind();
     // float nt = traceAmt;
        texalpha.program -> uniform("alpha", traceAmt); 

        textureB -> bind();      
        texalpha.line( *rect );
      textureB -> unbind();  

    texalpha.unbind();  
  }
                 

//  void preset(){ 
//   glDisable(GL_LIGHTING);
//    glEnable(GL_DEPTH_TEST);
//      glDepthFunc(GL_LESS);
//      
//      glEnable(GL_BLEND);
//      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//      
//      glPointSize(10);
//      glEnable(GL_LINE_SMOOTH);
//      
//      //frontBackDiff();
//      
//      //  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//      
//      GLushort gs = 1024 + 512 + 256 + 128 + 64 + 32 + 16 + 8 + 4 + 2;
//      glLineStipple(1,gs);
// }                

     virtual void onDraw3D(GLV& glv){   
        
    gfx::GL::enablePreset();

    interface.glv().getViewData(&glv);  
    //interface().getViewData(&glv);    
    // 
    // //cout << interface.vd().w << " " << interface.vd().h << endl; 
    scene.resize( interface.vd().w, interface.vd().h );  
    //        
    //Write into textureA using textureB 
    fboA -> attach( *textureA, GL::COLOR );  
    fboA -> bind();               
    // 
        glViewport(0,0,textureA->width(),textureA->height() ); 
      glClearColor(0,0,0,1);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //  
    

      addTrace();  
       renderA(); 
        
      if (bSave) save(); 
    // 
    fboA -> unbind();  
    // 
    // //Switch to full Viewport and Bind Texture to Rect 
    glViewport(0,0,scene.camera.lens.width(),scene.camera.lens.height());     
    // 
    texpipe.bind();  
    // 
       setFrameBuf();
    // 
       textureA -> bind();   
    //   
        texpipe.line( *rect );  
    //   
       textureA -> unbind();     
    // 
     texpipe.unbind();   
    // 
    swapTextures();   

  } 
  
  virtual void initGL(){
    
    printf("INITGL\n");    
     
    traceAmt = .5;
    
    texalpha.program = new ShaderProgram( ClipSpaceVert, TFragAlpha, 0 );
    texalpha.bindAll();   
  
    string Vert = AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert; 
    string Frag = DefaultFrag;
  
    pipe.program = new ShaderProgram(Vert,Frag,0);
    pipe.bindAll();                               
    
    float tw = width();//scene.camera.lens.width(); 
    float th = height();//scene.camera.lens.height();
    
    printf("%f, %f\n", tw, th);                
    
    fboA = new FBO();
    textureA = new Texture( tw, th );
    textureB = new Texture( tw, th );
    texpipe.program = new ShaderProgram( ClipSpaceVert, FOGL+FXAA, 0 );//TFrag,0);// 
    texpipe.bindAll();
     fboA -> attach(*textureA, GL::COLOR);  
  
    rect = new MBO( Mesh::Rect( 2.0, 2.0 ).color(0,0,0,1.0) ); 
    
    pixels = new unsigned char[ int(tw * th * 4) ];   
  }  
  
  void swapTextures(){
    Texture * tmp = textureA; textureA = textureB; textureB = tmp;
  }
  
  void setFrameBuf(){
    texpipe.program -> uniform("frameBufSize", scene.camera.lens.width(), scene.camera.lens.height() );
  } 
  
  // virtual bool onKeyDown(GLV& glv){
  //   App :: onKeyDown(glv); 
  //   
  //   switch( glv.keyboard().code){
  //     
  //     case 'p':
  //       save();
  //   }
  // } 
     
  void save(){ 
    static int fnum = -1; fnum++;  
    string dire = "~/Documents/vsr_output/render/";
    stringstream name; name << "test_" << fnum;
    stringstream os; os << dire << name.str() << ".ppm";   
    string filename = os.str();
    int tw = textureA->width();
    int th = textureA->height();
    glReadPixels(0,0,tw,th ,GL_RGBA, GL::UBYTE, pixels); 
        
    //FILE * dir = popen("mkdir output", "r");

    ofstream frame(filename.c_str(),ios::binary);
    frame << "P6\n" <<  tw << " " << th << "\n255\n";
    for (int j=th-1; j>=0; j--) {
      for (int i=0; i < tw; i++) {
        //gfx::Color c( pixels[idx],pixels[idx+1],pixels[idx+1] )
        frame.write((char*)(pixels + tw*4*j+4*i), 3);
        //frame << pixels[idx] << pixels[idx+1] << pixesl[idx+2];
      }
      }
    frame.close(); 
                           
    //stringstream cmd; cmd << "convert " << os.str() << " " << dire << name.str() << ".png" << " && rm " << os.str();
    //FILE * convert = popen(cmd.str().c_str(), "r");  
    
  }
};


int main(){
                             
  MyApp app;
  app.start();
  
  return 0;

}
