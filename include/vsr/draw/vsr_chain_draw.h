
#ifndef  vsr_chain_draw_INC
#define  vsr_chain_draw_INC

#include "gfx/gfx_mesh.h"
#include "gfx/gfx_print.h"

#include "vsr_cga3D_draw.h"
#include "form/vsr_chain.h"

#include <sstream>


namespace gfx{

  //using namespace vsr;

  using vsr::cga::Chain;
  using vsr::cga::Frame;
  using vsr::cga::Prismatic;
  using vsr::cga::Biv;
  using vsr::cga::Rot;
  using vsr::cga::Vec;
  using vsr::cga::Gen;
  
  void Draw(const Chain& c, bool bLoop = false, bool bFrame=true, float r=1.0, float g=1.0, float b=1.0, float a=1.0){
    
   if(bFrame){
    for (int i = 0; i<c.num(); ++i){
        float t=(float)i/c.num();
        DrawAt(c[i].z(), c[i].pos(), 0,0,t);
        DrawAt(c[i].y(), c[i].pos(), 0,1,t);
      //  DrawAt(c.joint(i).y() * .2, c[i].pos(), 0,1,0);
        //Draw( c[i].cxy(),0,1,0 );
    }
    }


    glColor4f(r,g,b,a);
    glBegin( bLoop ? GL_LINE_LOOP : GL_LINE_STRIP);
    for (int i = 0; i<c.num(); ++i){
        float t=(float)i/c.num();
        auto cvec = c[i].vec();
        auto nvec = c.link(i).vec() <= Vec::y; //for chains with R offset
        gfx::GL::vertex( cvec.begin() );
        gfx::GL::vertex( (cvec+c[i].y()*nvec).begin() ); 
    }
    glEnd();
  }


  void DrawR(const Frame& f, float r= 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
        
    static gfx::Mesh ma = gfx::Mesh::Cylinder( .5, 2, 4);

      //Frame f = r();
      Rot rot = f.rot();
    glPushMatrix();
      gfx::GL::translate( f.pos()[0], f.pos()[1], f.pos()[2] );// - 2 * f.scale() );
      gfx::GL::rotate( nga::Gen::aa( rot * Gen::rot( Biv::yz * PIOVERFOUR ) ).begin() );
      gfx::GL::scale( f.scale() );
      glColor4f(r,g,b,a); 
      ma.drawElements();
    glPopMatrix();

    glPushMatrix();
    
      gfx::GL::translate( f.pos()[0], f.pos()[1], f.pos()[2] );
      gfx::GL::rotate( nga::Gen::aa( rot ).begin() );
      gfx::GL::translate( 0, f.scale() * .5, 0 );
    
     // glPushMatrix();
        gfx::GL::scale( f.scale() * .5 );    
        glColor4f(g,r,b,a); 
        ma.drawElements(); 
     // glPopMatrix();
    
      /* gfx::GL::scale( 1, 10, 1); */    
      /* ma.drawElements(g,r,b,a); */ 
                 
    glPopMatrix();
  }


  
  void DrawR(const Chain& c, float r= 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
    for (int i = 0; i<c.num(); ++i){
        DrawR(c[i], r, g,b,a);
    }
  }

  void DrawR_(const Chain& c, float r= 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
    for (int i = 0; i<c.num(); ++i){
        Draw(c[i].cxy(), r, g,b,a);
        glColor4f(g,r,b,a);
        gfx::Glyph::Line(c[i].vec(), c[i].vec()+c[i].z()*.5);
    }

    /* glBegin(GL_TRIANGLES); */
    /* for (int i =0;i<c.num();i+=2){ */
    /*   int next = i<(c.num()-1)? i+1 : 0; */
    /*   int next2 = next<(c.num()-1)? next+1 : 0; */

    /*   gfx::GL::normal((c[i].y()^c[next].z()).dual() ); */
    /*   gfx::GL::vertex(c[i].vec()); */
    /*   gfx::GL::vertex(c[next].vec()); */
    /*   gfx::GL::vertex(c[next].vec() + c[next].z()); */
    /*   gfx::GL::normal((c[next].y()^c[next2].z()).dual() ); */
    /*   gfx::GL::vertex(c[next2].vec()); */
    /*   /1* gfx::GL::vertex(c[next].vec()); *1/ */
    /*   /1* gfx::GL::vertex(c[next].vec() - c[next].z()); *1/ */
    /* } */
    /* glEnd(); */
  }


  void Draw(const Prismatic& p){

  }


namespace tikz{

  string PrintPS(const Frame& f, int n, bool bx=true, bool by=true, bool bz=true){
    string c = name("c",n);
    stringstream os;
    auto v = f.vec();
    auto x = f.x(); auto y = f.y(); auto z = f.z();
    auto sx = name("cx",n); auto sy = name("cy",n); auto sz = name("cz",n);
    os << "\\coordinate"<<c<<" at "<<coord(v)<<";"<<endl;
    
    if (bx) os << "\\coordinate"<<sx<<" at "<<coord(x)<<";"<<endl;
    if (by) os << "\\coordinate"<<sy<<" at "<<coord(y)<<";"<<endl;
    if (bz) os << "\\coordinate"<<sz<<" at "<<coord(z)<<";"<<endl;
    
    if (bx) os << "\\draw [->]"<<c<<"--+"<<sx<<";"<<endl;
    if (by) os << "\\draw [->]"<<c<<"--+"<<sy<<";"<<endl;
    if (bz) os << "\\draw [->]"<<c<<"--+"<<sz<<";"<<endl;
    
    auto sxp = name("cxp",n); auto sxn = name("cxn",n);
    auto syp = name("cyp",n); auto syn = name("cyn",n);
    auto szp = name("czp",n); auto szn = name("czn",n);

    os << coord(sxp,x/4.0) << coord(sxn,-x/4.0);
    os << coord(syp,y/4.0) << coord(syn,-y/4.0);
    os << coord(szp,z/2.0) << coord(szn,-z/2.0);

    os << "\\draw[rounded corners=4pt]"<<c<<"+"<<sxp<<"--+"<<syp<<"--+"<<sxn<<"--+"<<syn<<"--cycle;"<<endl;
    //os << "\\draw[rounded corners=4pt]"<<c<<"+"<<szn<<"--+"<<sxp<<"--+"<<syp<<"--+"<<sxn<<"--+"<<syn<<"--cycle;"<<endl;
    //os << "\\draw[-]

    return os.str();
  }



  string PrintPS(const Chain& c){
    string s = begin();
    for (int i = 0;i<c.num();++i){
      int next = i<(c.num()-1)? i+1:0;
      s+=PrintPS(c[i],i,false,false,true);
      s+=draw(c[i].vec(), c[next].vec(),"-");
    }
    return s+ end();
  }
}//tikz::
  
}

#endif   /* ----- #ifndef vsr_chain_draw_INC  ----- */
