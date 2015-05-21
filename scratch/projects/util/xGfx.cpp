/*
 * =====================================================================================
 *
 *       Filename:  gfx.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/30/2015 13:53:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#include "gfx/gfx_app.h"
#include "gfx/util/glut_window.hpp"

using namespace gfx;

struct MyApp : public GFXApp<GlutContext> {

 virtual void setup(){}
 virtual void onDraw(){}
 
 virtual void onFrame(){

    static float amt =0;
    amt +=.1;
    clear();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glPushMatrix();
    //glFrustum(-1.0,1.0,-1.0,1.0,.1,100);
    gluPerspective(60,1,.1,100 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   // glPushMatrix();
    
    glTranslatef(0,0,-1);
    
    glRotatef(amt,0,1,0);
    glRotatef(amt,1,1,0);

    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glEnd();
    
    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3f(0,0,-1);
    glVertex3f(0,0,1);
    glEnd();

   // glMatrixMode(GL_PROJECTION);
   // glPopMatrix();

    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();


 }
  
};



int main(){

  MyApp app;

  app.start();
  
  return 0;
}


