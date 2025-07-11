// SURFACE PATCH

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

    vsr::util::Grid grid;
    Frame mFrame;
   // TFrameBox mTFrameBox;

    float mU, mV, mW;

    float mWidth, mHeight, mDepth;

    void onSetup(){

        grid.init(20, 20);

        objectController.attach(&mFrame);

        mWidth = mHeight = mDepth = 2;

        mU = mV = mW = 0.5;
    }

    void onDrawGui() {
        gui(mWidth, "Width",1, 5);
        gui(mHeight, "Height",1, 5);
        gui(mDepth, "Depth",1, 5);

        gui(mU, "U",0, 1);
        gui(mV, "V",0, 1);
        gui(mW, "W",0, 1);

    }

    void onDraw(){

       // draw(mFrame);

        Point p000 = PAO;
        Point p100 = Round::null(mWidth,0,0);
        Point p010 = Round::null(0,mHeight,0);
        Point p001 = Round::null(0,0,mDepth);
        Point p110 = Round::null(mWidth,mHeight,0);
        Point p011 = Round::null(0,mHeight,mDepth);
        Point p101 = Round::null(mWidth,0,mDepth);
        Point p111 = Round::null(mWidth,mHeight,mDepth);

        int div = 20;
//        CubicLattice<Point> lattice(div, div, div, mWidth/div, mHeight/div, mDepth/div);

        Frame f111(p111, mFrame.rotor());

        TFrameBox mTFrameBox(f111, {p000, p100, p010, p110, p001, p101, p011, p111});

        // mTFrameBox.setPoints({p000, p100, p010, p110, p001, p101, p011, p111});
        // mTFrameBox.buildBackwards(f111);

        for(int i=0; i<div; ++i){
            for(int j=0; j<div; ++j){
                for(int k=0; k<div; ++k){
                    auto p = mTFrameBox.calcMap(i/float(div), j/float(div), k/float(div));
                    draw(p);
                }
            }
        }

        auto logs = mTFrameBox.calcTensor(mW);

         Point tp = Tops::Xf(p000, Gen::bst(logs[2]));
        // Point tp2 = Tops::Xf(tp, Gen::bst(logs[0]));
        // Point tp3 = Tops::Xf(tp, Gen::bst(logs[1]));

        Pair otu = mTFrameBox.o().t[0];
        Pair otv = mTFrameBox.o().t[1];
        Pair vtu = mTFrameBox.v().t[0];
        Pair vtv = mTFrameBox.v().t[1];

        // u0 = tu moved back to w = z
        Pair logW = mTFrameBox.dwvu0() * mW;
        Pair logU = mTFrameBox.duwv0() * mU;
        Pair logV = mTFrameBox.dvwu1() * mV;

        Con bw = Gen::bst(logV) * Gen::bst(logU) * Gen::bst( logW );

        Pair u00 = Tops::NormalizePair( otu.spin( bw ) );
        Pair v00 = Tops::NormalizePair( otv.spin( bw ) );

        draw(u00);
        draw(v00);

        // Frame temp(tp2);
        // draw(temp);
        // temp.pos() = tp2;
        // draw(temp);
        // temp.pos() = tp3;
        // draw(temp);

       grid.plot(tp, logs[0], logs[1]);
       // grid.plot(5,5);
        grid.draw(true, 1.0);

        Frame f000(p000, mTFrameBox.o().rotor());
        Frame f001(p001, mTFrameBox.w().rotor());
        Frame f010(p010, mTFrameBox.v().rotor());
        Frame f011(p011, mTFrameBox.vw().rotor());
        Frame f100(p100, mTFrameBox.u().rotor());
        Frame f101(p101, mTFrameBox.uw().rotor());
        Frame f110(p110, mTFrameBox.uv().rotor());

        draw(f000);
        draw(f001);
        draw(f010);
        draw(f011);
        draw(f100);
        draw(f101);
        draw(f110);
        draw(f111);
    }

};

int main(){
    MyApp().start();
}