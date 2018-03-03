/*
 * =====================================================================================
 *
 *       Filename:  vsr_shapes.h
 *
 *    Description:  meshes made by leveraging cga3d
 *
 *        Version:  1.0
 *        Created:  12/05/2013 17:25:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */

#include "space/vsr_cga3D_op.h"
//#include "vsr_cga3D_funcs.h"

#include "gfx/gfx_mesh.h"

namespace vsr{ namespace cga {

  using gfx::Mesh;
  using gfx::Vec3f;

  struct Shape {


      /// currently returns vector<Vertex> maybe return a Mesh?
      static vector<Vertex> Sphere(float radius=1, int w=20, int h=20){
      
      
        vector<Vertex> v;
      
        for (int i = 0;i<w;++i){
          for (int j=0;j<h;++j){
            auto tu = (float)i/w * TWOPI;
            auto tv = -PIOVERTWO + (float)j/h * PI;
            auto rot = Gen::rot(tu,tv);    
            v.push_back(  Vec3f( Vec::x.spin(rot) * radius )  );         
          }
        }
      
        return v;
      }
      
      
      static vector<Vertex> Cylinder(int w=20, int h=20, float radius = 1.0, float height = 2.0){
      
      
        vector<Vertex> v;
      
        for (int i = 0;i<w;++i){
          for (int j=0;j<h;++j){
            auto tu = (float)i/w * PI;
            auto tv = -height/2.0 + (float)j/h * height;
            auto rot = Gen::rot( Biv::xz * tu);    
            v.push_back(  Vec3f( Vec::x.spin(rot) * radius ) + Vec3f(0,tv,0));         
          }
        }
      
        return v;
      }    
  
    /// A Torus Mesh
    static inline Mesh Torus( double rad = 1.0, int slices =20, int stacks=20){
        Mesh m;

        Cir cira = CXZ(.5);
        Cir cirb = CXZ(1);

          for (int j = 0; j < slices; ++j){
            VSR_PRECISION tu = PI * (VSR_PRECISION)j/slices;
            Pnt pa = Round::point(cirb, tu);
            for (int i = 0; i < stacks; ++i){   
               VSR_PRECISION tv = PI * (VSR_PRECISION)i/stacks;
               Pnt pb = Round::point( (pa <= cira).dual(), tv ); 
               m.add(pb);      
            }
        }

        //c --- d
        //|
        //a --- b
        for (int j = 0; j < slices; ++j){
          static bool color = false;
          color = !color;
          for (int i = 0; i < stacks; ++i){   
            int a = j * stacks + i;
            int b = (j < slices - 1) ?  a + stacks : i;
            int c = (i < stacks - 1 ) ? a + 1 : j * stacks;
            int d = (j < slices - 1 ) ? c + stacks : ( i < stacks -1 ) ? i + 1 : 0 ;
            
            m[a].Col.set(color,!color,1,1);
            m[b].Col.set(color,!color,1,1);
            m[c].Col.set(color,!color,1,1);
            m[d].Col.set(color,!color,1,1);
            m.add(a).add(b).add(c).add(d);
          }
        }



        m.mode(GL::TS);
        return m;
    }

    ///Skin Circles -- pass in an array or vector of circles, boolean bLoop specifies joining last and first
    template<typename T>
    static inline Mesh Skin( T cir, int num, bool bLoop = false, int res = 20){
  
        Mesh m;

        if (num==0) return m;
        
        for (int i = 0;  i < res; ++i){
            float t= 1.0 * i/res;
            float pt = TWOPI*t;
            for (int j = 0; j < num; ++j){
                float u = (float)j/num;
                Vec v = Round::point( cir[j], pt );
                m.add( v[0], v[1], v[2] );
                m[m.num()-1].Col.set( (1+sin(pt))/2.0,.2,(1+cos(pt))/2.0,1);
            }
        }

        //Calc Indices (FOR TRIANGLE STRIP)
        int a,b;
        for (int i = 0; i < res; ++i){
            if (i>0) m.add(i*num);
            for (int j = 0; j < num; ++j){
              a = i * num + j;            
              b = (i<res-1) ? a + num : j ;       
              m.add(a).add(b);               
            }       
            if (i<res-1) m.add(b);     
        }
        if (bLoop){
            for (int i =0; i<res; ++i){
              int a = i*num;
              int b = i*num+(num>0?(num-1):0);

              m.add(a).add(b);
            }
            m.add(0);
        }

        //Calc normals
        for (int i = 0; i < res; ++i){
            for (int j = 0; j < num; ++j){
                int a = i * num + j;       
                int b = (i<(res-1)) ? a + num : j;        //next col
                int c = (i>0)? a-num : (res-1)*num + j;   //prev col
                int d = (j<(num-1)) ? a+1 : a;            //next row
                int e = (j>0)?a-1 : a;                    //prev row
                Vec3f va = (m[b].Pos - m[c].Pos).cross( m[d].Pos - m[e].Pos).unit();
                m[a].Norm = va;          
            }
        }

        m.mode( GL::TS );
        m.store();
        return m;    

    }

    ///Skin Circles -- pass in an array or vector of circles
    template<typename T>
    static inline Mesh Skin2( T cir, int num, int res = 10){
  
        Mesh m;
        
        for (int i = 0;  i < res; ++i){
            double t= 1.0 * i/res;
            
            for (int j = 0; j < num; ++j){
                Vec v = Round::point( cir[j], TWOPI * t );
                m.add( v[0], v[1], v[2] );
            }
        }
        
        //Calc Indices (FOR TRIANGLE STRIP)
        int a,b,c,d;
        for (int i = 0; i < num-1; ++i){
            for (int j = 0; j < res; ++j){
            
                a = j * num + i;                //<-- current idx                        
                b = a + 1;                      //<-- next circle
                if (j>=res-1) { 
                    c = i + 1;
                    d = i;
                }
                else {
                    c = a + 1 + num ;           //<-- next on next circle
                    d = a + num ;               //<-- next on same circle
                }
                int idx[2] = {a,c};
                m.add(idx,2);
            }
            m.add(i).add(i+1+num);
        }                
 
         for (int i = 0; i < num-1; ++i){
            for (int j = 0; j < res; ++j){
            
                a = j * num + i;                //<-- current idx                        
                b = a + 1;                      //<-- next circle
                if (j>=res-1) { 
                    c = i + 1;
                    d = i;
                }
                else {
                    c = a + 1 + num ;           //<-- next on next circle
                    d = a + num ;               //<-- next on same circle
                }
            }
               Vec3f ta = m[b].Pos - m[a].Pos;
               Vec3f tb = m[d].Pos - m[a].Pos;
               Vec3f tc = ta.cross(tb);
               m[a].Norm = Vec3f(0,0,0);// tc.unit();
        }          
 //       //calc normals
 //       for (int i = 0; i < res; ++i){
 //           for (int j = 0; j < num-1; ++j){
 //               a = i * num + j;                //<-- current idx
 //               b = a + 1;                      //<-- same on next circle
 //               
 //               if (i>=res-1) { 
 //                   c = j + 1;
 //                   d = j;
 //               }
 //               else {
 //                   c = a + 1 + num ;           //<-- next on next circle
 //                   d = a + num ;               //<-- next on same circle
 //               }
 //               Vec3f ta = m[b].Pos - m[a].Pos;
 //               Vec3f tb = m[d].Pos - m[a].Pos;
 //               Vec3f tc = ta.cross(tb);
 //               m[a].Norm = tc.unit();
 //           }
 //       }
 //       
        m.mode( GL::TS );
        return m;      
    }
  
  };
} } //vsr::cga::
