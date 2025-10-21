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

    bool bDrawAll;
    bool bDrawCoordinateSurfaces;
    bool bDrawCoordinateSurfaceU;
    bool bDrawConstructedFrame;
    bool bDrawTransformation;
    bool bDrawPointV;
    bool bDrawFullFrame;
    bool bDrawCircle;
    bool bDrawTangent;
    bool bDrawNetCircle;

    void onSetup(){
        mFrameA.set(-2,0,0);
        mFrameB.set(2,0,0);
        mFrameC.set(2,-2,0);
        objectController.attach(&mFrameA);
        objectController.attach(&mFrameB);
        objectController.attach(&mFrameC);
        mVelocity = 1;
        mColor = Vec(1,1,1);
    }

    void onDrawGui() {
        bool prevDrawAll = bDrawAll;
        gui(bDrawAll, "Draw All");
        if (bDrawAll != prevDrawAll) {
            bDrawCircle = bDrawAll;
            bDrawTangent = bDrawAll;
            bDrawCoordinateSurfaceU = bDrawAll;
            bDrawTransformation = bDrawAll;
            bDrawFullFrame = bDrawAll;
            bDrawCoordinateSurfaces = bDrawAll;
            bDrawConstructedFrame = bDrawAll;
            bDrawPointV = bDrawAll;
            bDrawNetCircle = bDrawAll;
        }

        gui(bDrawCircle, "Draw Circle");
        gui(bDrawTangent, "Draw Tangent");
        gui(bDrawCoordinateSurfaceU, "Draw Coordinate Surface U");
        gui(bDrawTransformation, "Draw Transformation");

        gui(bDrawFullFrame, "Draw Full Frame");
        gui(bDrawCoordinateSurfaces, "Draw Coordinate Surfaces");
        gui(bDrawConstructedFrame, "Draw Constructed Frame");

        gui(bDrawPointV, "Draw Point");
        gui(bDrawNetCircle, "Draw Net Circle");

        gui(mCurvatureA, "CurvatureA", -10, 10);
        gui(mCurvatureB, "CurvatureB", -10, 10);

        gui(mVelocity, "Velocity", 0, 10);
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

        // Let's draw the circle
        if (bDrawCircle){
            DrawRound(p ^ tf.u(),0  ,0,0);
        }
        //Extract the Tangent at p from the Circle it shares with the destination Tangent
        Pair tu = Tops::NormalizePair(p <= (p ^ tf.u()).inv());

        //A Surface of constant U:
        DualSphere suv = Tops::Surface(tu, mCurvatureA);

        //And now
        if (bDrawTransformation && !bDrawPointV){
            // calculate the unique transformation that takes one tangent to the other
            // curvature values control acceleration of the transformation
            Pair gen = Tops::CalcGen(tu, tf.u(), mCurvatureA, mCurvatureB);
            Boost bst = Gen::boost(gen * tval);
            Pair t = Tops::Xf(tu, bst );
            Point tp = Tops::Xf(mFrameA.pos(), bst );
            Draw(t, 1,.2,.2);
            Draw(Round::sphere(tp, 0.1), 1,.2,.2);
        }

        //For the others, we Normalize(p <= t) for t = tv and t = tw;
        //THese are Constant V and W surfaces as we move in U direction from p to tf
        DualSphere svu = Tops::Surface(p, tf.v());
        DualSphere swu = Tops::Surface(p, tf.w());

        // Now we can extract the other two Tangents at p]
        Pair tv = Tops::NormalizePair(p ^ (svu).inv());
       // Pair tv = Tops::Element(svu, p);
        Pair tw = Tops::Element(swu, p);


        draw(Round::sphere(p, 0.1),1,0,0);
        draw(Round::sphere(mFrameB.pos(),0.1),1,0,0);
        draw(tf.u(),1,0,0);

        if (bDrawTangent){
            draw(tu,1,0,0);
        }

        if (bDrawFullFrame){
            draw(tf.v(),0,1,0);
            draw(tf.w(),0,0,1);
        }

        if (bDrawConstructedFrame){
            draw(tu,1,0,0);
            draw(tv,0,1,0);
            draw(tw,0,0,1);
        }

        //now use built in functions
        if (bDrawPointV){
            // SURFACE PATCH LAYOUT (World Coordinates):
            //
            //   Y
            //   ^
            //   |    p (0,0)                     tf (1,0)
            //   |    [mFrameA: (-2,0,0)]         [mFrameB: (2,0,0)]
            //   |      ●-------------------------●
            //   |      |                         |
            //   |      |      Surface Patch      |
            //   |      |                         |
            //   |      ●-------------------------●  ---> X
            //   |    po (0,1)                    pu (1,1)
            //   |    [calculated ~(-2,-2,0)]     [mFrameC: (2,-2,0)]
            //   |
            //   +------------------------------------------
            //
            // Point naming:
            //   p  = mFrameA.pos()  - Origin point (top-left, y=0)
            //   tf = mFrameB frame  - Defines U direction (top-right, y=0)
            //   pu = mFrameC.pos()  - User-positioned point (bottom-right, y=-2)
            //   po = calculated     - Completes the patch (bottom-left, y=-2)

            Point pu = mFrameC.pos();

            Circle cir = mFrameA.pos() ^ mFrameB.pos() ^ mFrameC.pos();
            Point po = Round::split( (suv ^ cir.dual()).dual() )[0];

            if (bDrawNetCircle){
                draw(cir,0,0,0);
            }

            draw(Round::sphere(pu,.1));
            draw(Round::sphere(po,.1));

            TFrame_ tfu (pu, tf, TDIR::v);
            TFrame_ tfo (po, tfu, TDIR::u);

            draw(tfo.u(),1,0,0);
            draw(tfo.v(),0,1,0);
            draw(tfo.w(),0,0,1);

            draw(tfu.u(),1,0,0);
            draw(tfu.v(),0,1,0);
            draw(tfu.w(),0,0,1);




            if (bDrawTransformation){

                DualSphere svu0 = Tops::Surface(suv, tfu.v());

                Pair genU = Tops::CalcGen(tu, tf.u(), suv, tfu.suv());
                Pair genV = Tops::CalcGen(tfu.v(), tf.v(), svu0, svu);
                Boost bstU  = Gen::boost(genU * tval);
                Boost bstV = Gen::boost(genV * tval);

                Draw(Tops::Xf(tu, bstU ), 1,0,0);
                Draw(Round::sphere(Tops::Xf(p, bstU ), 0.1), 1,.2,.2);

                Draw(Tops::Xf(tfu.v(), bstV ), 0,1,0);
                Draw(Round::sphere(Tops::Xf(pu, bstV ), 0.1), .2,1,.2);

                Draw(Tops::Xf(tfo.u(), bstU), 1,0,0);
                Draw(Round::sphere(Tops::Xf(po, bstU), 0.1), 1,0,0);

                Draw(Tops::Xf(tfo.v(), bstV), 0,1,0);
                Draw(Round::sphere(Tops::Xf(po, bstV), 0.1), 0,1,0);



                DrawRound(diagonal.spin(Bst(genU) * Bst(genV)), 0,1,1);

                if (bDrawCoordinateSurfaces){
                    //DrawRound (suv0,1,0,0, .2);
                    DrawRound (svu0,0,1,0, .2);
                }

            }

            if (bDrawCoordinateSurfaces){
                DrawRound( tfu.suv(), 1,0,0, .2);
                DrawRound( tfu.swv(), 0,0,1, .2);
            }

        }

         if (bDrawCoordinateSurfaces){
            DrawRound(svu, 0,1,0,.2);
            DrawRound(swu, 0,0,1,.2);
        }

        if (bDrawCoordinateSurfaceU){
            DrawRound(Tops::Surface(tu, mCurvatureA), 1,0,0, .2);
            if (!bDrawPointV) DrawRound(Tops::Surface(tf.u(), mCurvatureB), 1,0,0, .2);
        }

    }

};

int main(){
    MyApp().start();
}