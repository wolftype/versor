
#ifndef  vsr_chain_draw_INC
#define  vsr_chain_draw_INC

#include "vsr_chain.h"
#include "vsr_cga3D_draw.h"


namespace vsr{

  void Draw(const Chain& c, bool bLoop = false){
    for (int i = 0; i<c.num(); ++i){
        float t=(float)i/c.num();
        DrawAt(c[i].z(), c[i].pos(), 1-t,0,t);
    }

    glColor3f(0,1,0);
    glBegin( bLoop ? GL_LINE_LOOP : GL_LINE_STRIP);
    for (int i = 0; i<c.num(); ++i){
        float t=(float)i/c.num();
        glColor3f(0,t,1-t);
        gfx::GL::vertex( c[i].pos().begin() );
    }
    glEnd();
  }


  void DrawR(const Frame& f, float r= 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
        
    static Mesh ma = gfx::Mesh::Cylinder( .5, 2, 30);

      //Frame f = r();
      Rot rot = f.rot();
    glPushMatrix();
      gfx::GL::translate( f.pos()[0], f.pos()[1], f.pos()[2] );// - 2 * f.scale() );
      gfx::GL::rotate( Gen::aa( rot * Gen::rot( Biv::yz * PIOVERFOUR ) ).begin() );
      gfx::GL::scale( f.scale() ); 
      ma.drawElements(r,g,b,a);
    glPopMatrix();

    glPushMatrix();
    
      gfx::GL::translate( f.pos()[0], f.pos()[1], f.pos()[2] );
      gfx::GL::rotate( Gen::aa( rot ).begin() );
      gfx::GL::translate( 0, f.scale() * .5, 0 );
      gfx::GL::scale( f.scale() * .5 );    
      ma.drawElements(g,r,b,a); 
            
    glPopMatrix();
  }

  void DrawR(const Chain& c, float r= 1.0, float g = 1.0, float b = 1.0, float a = 1.0){
    for (int i = 0; i<c.num(); ++i){
        DrawR(c[i], r, g,b,a);
    }
  }

  void Draw(const Prismatic& p){

  }
}

#endif   /* ----- #ifndef vsr_chain_draw_INC  ----- */
