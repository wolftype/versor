// SURFACE PATCH

#include "gfx_gl.h"
#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_rigid.h>
#include <vsr/util/vsr_draw_util.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;

// void DrawT(const TFrame_ &tf, bool bFlip = false) {
//   Draw(tf.t[0] * (bFlip ? -1.0 : 1.0), 1, 0, 0);
//   Draw(tf.t[1] * (bFlip ? -1.0 : 1.0), 0, 1, 0);
//   Draw(tf.t[2] * (bFlip ? -1.0 : 1.0), 0, 0, 1);
// }
//
// struct TPatch {
//   TFrame_ tf[2][2];
// }





struct MyApp : App {

    Frame mFrameA;
    Frame mFrameB;
    Frame mFrameC;

    float mCurvatureA;
    float mCurvatureB;

    float mVelocity;

    bool bUseRelSense;
    bool bRelApproach;

    bool bDrawCoordinateSurfaces;
    bool bDrawCoordinateSurfaceU;
    bool bDrawConstructedFrame;
    bool bDrawTransformation;
    bool bDrawPointV;

    void onSetup(){
        mFrameA.set(-2,0,0);
        mFrameB.set(2,0,0);
        mFrameC.set(2,-2,0);
        objectController.attach(&mFrameA);
        objectController.attach(&mFrameB);
        objectController.attach(&mFrameC);
        mVelocity = 1;
    }

    void onDrawGui() {
        gui(mCurvatureA, "CurvatureA", -1000, 1000);
        gui(mCurvatureB, "CurvatureB", -1000, 1000);
        gui(mVelocity, "Velocity", 0, 10);
        gui(bUseRelSense, "Use Relative Sense");
        gui(bRelApproach, "Relative Approach");
        gui(bDrawCoordinateSurfaces, "Draw Coordinate Surfaces");
        gui(bDrawConstructedFrame, "Draw Constructed Frame");
        gui(bDrawCoordinateSurfaceU, "Draw Coordinate Surface U");
        gui(bDrawTransformation, "Draw Transformation");
        gui(bDrawPointV, "Draw Point V");
    }

    void onDraw(){
        GL::lightsOff();

        static float tval = 0;
        tval += .01 * mVelocity;

        if (tval > 1) tval = 0;

        //Given a Point, a Destination Tangent, and a Direction to move in,
        //We can build two constant coordinate surfaces
        //Here we will build constant V and W surfaces as we move in U direction
        TFrame_ tf(mFrameB);
        Point p = mFrameA.pos();

        //Normalize(p <= t);
        //THese are Constant V and W surfaces as we move in U direction from p to tf 
        DualSphere svu = Tops::Surface(p, tf.v());
        DualSphere swu = Tops::Surface(p, tf.w());

        // Now we can extact the Vector directions there
        Vec v = Tops::Unit(svu ^ p);
        Vec w = Tops::Unit(swu ^ p);
        Vec u = (v ^ w).duale();

        // And turn them back into normalized Tangents
        Pair tu = Tops::Element(u, p);
        Pair tv = Tops::Element(v, p);
        Pair tw = Tops::Element(w, p);

        if (bDrawTransformation && !bDrawPointV){
            // calculate the unique transformation that takes one tangent to the other
            // curvature values control acceleration of the transformation
            Pair gen = Tops::CalcGen(tu, tf.u(), mCurvatureA, mCurvatureB);
            Boost bst = Gen::boost(gen * tval);
            Pair t = Tops::Xf(tu, bst ); 
            Point tp = Tops::Xf(mFrameA.pos(), bst ); 
            Draw(t, 1,1,1);
            Draw(Round::sphere(tp, 0.1), 1,1,1);
        }

        draw(Round::sphere(p, 0.2));
        draw(tf.u(),1,0,0);
        draw(tf.v(),0,1,0);
        draw(tf.w(),0,0,1);

        if (bDrawConstructedFrame){
            draw(tu,1,0,0);
            draw(tv,0,1,0);
            draw(tw,0,0,1);
        }



        //now use built in functions
        //TFrame_ tf
        if (bDrawPointV){
            Point pv = mFrameC.pos();
            draw(Round::sphere(pv,.1));
            TFrame_ tfv (pv, tf, TDIR::v);
            draw(tfv.u(),1,0,0);
            draw(tfv.v(),0,1,0);
            draw(tfv.w(),0,0,1);


            if (bDrawTransformation){

                Pair genU = Tops::CalcGen(tu, tf.u(), Tops::Surface(tu, mCurvatureA), tfv.suv());
                Pair genV = Tops::CalcGen(tfv.v(), tf.v(), Tops::Surface(tfv.v(), mCurvatureB), svu);
                Boost bstU  = Gen::boost(genU * tval);
                Boost bstV = Gen::boost(genV * tval);

                Pair ptu = Tops::Xf(tu, bstU ); 
                Point tpu = Tops::Xf(p, bstU ); 
                Draw(ptu, 1,1,1); 
                Draw(Round::sphere(tpu, 0.1), 1,1,1);

                Pair ptv = Tops::Xf(tfv.v(), bstV ); 
                Point tpv = Tops::Xf(pv, bstV ); 
                Draw(ptv, 1,1,1); 
                Draw(Round::sphere(tpv, 0.1), 1,1,1);

            }

            if (bDrawCoordinateSurfaces){
                DrawRound( tfv.suv(), 1,0,0, .2);
                DrawRound( tfv.swv(), 0,0,1, .2);
            }

        }

         if (bDrawCoordinateSurfaces){
            DrawRound(svu, 0,1,0,.2);
            DrawRound(swu, 0,0,1,.2);
        }

        if (bDrawCoordinateSurfaceU && !bDrawPointV){
            DrawRound(Tops::Surface(tu, mCurvatureA), 1,0,0, .2);
            DrawRound(Tops::Surface(tf.u(), mCurvatureB), 0,1,0, .2);
        }
 
    }

};

int main(){
    MyApp().start();
}