// SURFACE PATCH

#include <vsr/vsr_app.h>
#include <vsr/form/vsr_tangent.h>
#include <vsr/draw/vsr_cga3D_helpers.h>
#include <vsr/form/vsr_rigid.h>
#include <vsr/util/vsr_draw_util.h>

using namespace gfx;
using namespace vsr;
using namespace vsr::cga;


struct MyApp : App {

    vsr::util::Grid grid;
    Frame mFrame;
    Frame mFrameB;

    float mU, mV, mW;

    float mWidth, mHeight, mDepth;

    float kBoxAlpha;
    int kDiv;
    float kBoostAmountX, kBoostAmountY, kBoostAmountZ;
    float kRadius;

    void onSetup(){

        kDiv = 20;

        grid.init(kDiv, kDiv);

        objectController.attach(&mFrame);
        objectController.attach(&mFrameB);

        mWidth = mHeight = mDepth = 2;

        mU = mV = mW = 0.5;

        kBoxAlpha = 0.5;

        kBoostAmountX = kBoostAmountY = kBoostAmountZ = 0.0;

        kRadius = 1;

        mFrame.set(-3,0,0);
    }

    void onDrawGui() {
        gui(mWidth, "Width",1, 5);
        gui(mHeight, "Height",1, 5);
        gui(mDepth, "Depth",1, 5);
        gui(kRadius, "Radius",0, 5);

        gui(mU, "U",0, 1);
        gui(mV, "V",0, 1);
        gui(mW, "W",0, 1);

        gui(kBoxAlpha, "Box Alpha",0, 1);
        gui(kBoostAmountX, "Boost X",-1, 1);
        gui(kBoostAmountY, "Boost Y",-1, 1);
        gui(kBoostAmountZ, "Boost Z",-1, 1);

        if (ImGui::Button("Reset All Values")) {
            mWidth = mHeight = mDepth = 2;
            mU = mV = mW = 0.5;
            kBoxAlpha = 0.5;
            kBoostAmountX = kBoostAmountY = kBoostAmountZ = 0.0;
            kRadius = 1;
            mFrame.rotor() = Rot(1);
        }
    }

    void onDraw(){
       // GL::lightsOff();

        Point p000 = Round::null(-mWidth/2.0,-mHeight/2.0,-mDepth/2.0);
        Point p100 = Round::null(mWidth/2.0,-mHeight/2.0,-mDepth/2.0);
        Point p010 = Round::null(-mWidth/2.0,mHeight/2.0,-mDepth/2.0);
        Point p001 = Round::null(-mWidth/2.0,-mHeight/2.0,mDepth/2.0);
        Point p110 = Round::null(mWidth/2.0,mHeight/2.0,-mDepth/2.0);
        Point p011 = Round::null(-mWidth/2.0,mHeight/2.0,mDepth/2.0);
        Point p101 = Round::null(mWidth/2.0,-mHeight/2.0,mDepth/2.0);
        Point p111 = Round::null(mWidth/2.0,mHeight/2.0,mDepth/2.0);

        Point pnts[8] = {p000, p100, p010, p110, p001, p101, p011, p111};

        TPointBox tbox(pnts);

        mFrameB.scale(kRadius);
        mFrameB.pos() = Round::null(-mWidth/2.0,0,0);

        //Pair genX = mFrameB.ipx() * kBoostAmountX;
        // First, boost through the left
        Pair genX = tbox.cl().dual() * kBoostAmountX;
        Bst bstX = Gen::bst(genX);
        for(int i=0; i<8; ++i){
            pnts[i] = Tops::Xf(pnts[i], bstX);
        }
        tbox.setPoints(pnts);
        // Then boost through the bottom
        Pair genY = tbox.cb().dual() * kBoostAmountY;
        Bst bstY = Gen::bst(genY);
        for(int i=0; i<8; ++i){
            pnts[i] = Tops::Xf(pnts[i], bstY);
        }
        tbox.setPoints(pnts);
        // Then boost through the front
        Pair genZ = tbox.cf().dual() * kBoostAmountZ;
        Bst bstZ = Gen::bst(genZ);
        for(int i=0; i<8; ++i){
            pnts[i] = Tops::Xf(pnts[i], bstZ);
        }
        tbox.setPoints(pnts);

       // Con con =  Gen::bst(genX) * Gen::bst(genZ) * Gen::bst(genY);

        for(int i=0; i<8; ++i){
            Draw(Round::sphere(pnts[i], 0.01), 1, 1, 1);
        }


        Frame f111(pnts[7], mFrame.rotor());

        TFrameBox mTFrameBox(f111, pnts);

        // Draw the Six Sides
        for (int side = 0; side < 6; ++side){
            for(int i=0; i<kDiv; ++i){
                float ti = i/float(kDiv-1);
                for(int j=0; j<kDiv; ++j){
                    float tj = j/float(kDiv-1);
                    int idx = i*kDiv + j;
                    switch(side){
                        case 0:
                            grid.pnts[idx] = mTFrameBox.calcMap(ti, tj,0);
                            break;
                        case 1:
                            grid.pnts[idx] = mTFrameBox.calcMap(ti, 0, tj);
                            break;
                        case 2:
                            grid.pnts[idx] = mTFrameBox.calcMap(0, ti, tj);
                            break;
                        case 3:
                            grid.pnts[idx] = mTFrameBox.calcMap(ti, 1, tj);
                            break;
                        case 4:
                            grid.pnts[idx] = mTFrameBox.calcMap(ti, tj, 1);
                            break;
                        case 5:
                            grid.pnts[idx] = mTFrameBox.calcMap(1, ti, tj);
                            break;
                    }
                }
            }
            grid.draw(false, kBoxAlpha);
        }

        draw(mFrame);

        // Frame f000(pnts[0], mTFrameBox.o().rotor());
        // Frame f001(pnts[4], mTFrameBox.w().rotor());
        // Frame f010(pnts[2], mTFrameBox.v().rotor());
        // Frame f011(pnts[5], mTFrameBox.vw().rotor());
        // Frame f100(pnts[1], mTFrameBox.u().rotor());
        // Frame f101(pnts[6], mTFrameBox.uw().rotor());
        // Frame f110(pnts[3], mTFrameBox.uv().rotor());

        // draw(f000);
        // draw(f001);
        // draw(f010);
        // draw(f011);
        // draw(f100);
        // draw(f101);
        // draw(f110);
        // draw(f111);

        Mesh mesh = gfx::Mesh::Cone(1, 1, 20, 20);
        mesh.drawElements();

        draw(mTFrameBox.pbox.cl());
        draw(mTFrameBox.pbox.cr());
        draw(mTFrameBox.pbox.cf());
        draw(mTFrameBox.pbox.cb());
        draw(mTFrameBox.pbox.ct());
        draw(mTFrameBox.pbox.cb());
        draw(mTFrameBox.pbox.ck());



    }

};

int main(){
    MyApp().start();
}