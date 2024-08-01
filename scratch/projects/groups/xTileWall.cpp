
#include <vsr/vsr_app.h>
#include <vsr/util/vsr_stat.h>

using namespace vsr;
using namespace vsr::cga;
using namespace gfx;

#define GROUT 0.0625
#define OFFSET (1.75+GROUT/2.0)
#define ROWWIDTH (OFFSET+GROUT)
#define TILEWIDTH (3.5*2+GROUT*3)
#define BORDER .1
#define WINDOWWIDTH 30.5
#define WINDOWWIDTH2 30
#define WINDOWGAP 4.625
#define WINDOWHEIGHT 31.25
#define WIDTH 112.0
#define HEIGHT 60.0
#define WIDTH2WIN 37.25
#define HEIGHT2WIN (10.75-ROWWIDTH)

enum class Sector {
  Left=0, Top, Right, Bottom
};

enum class Tone {
  White=0, Cloudy, Stormy, Dark
};

Vec3f TileColor (Tone tone){
  switch (tone){
    case Tone::White:
      return Vec3f (1.0,1.0,1.0);
    case Tone::Cloudy:
      return Vec3f (.9,.9,.9);
    case Tone::Stormy:
      return Vec3f (.7,.7,.7);
    case Tone::Dark:
      return Vec3f (.6,.6,.6);
    default:
      return Vec3f(1,0,0);
  }
};

void drawWindows (){
  Vec bl = Vec(-WINDOWWIDTH/2.0, -WINDOWHEIGHT/2.0, 0.0);
  Vec tl = Vec(-WINDOWWIDTH/2.0, WINDOWHEIGHT/2.0, 0.0);
  Vec tr = Vec(WINDOWWIDTH/2.0, WINDOWHEIGHT/2.0, 0.0);
  Vec br = Vec(WINDOWWIDTH/2.0, -WINDOWHEIGHT/2.0, 0.0);

  Vec posA (-WIDTH/2.0 + WIDTH2WIN + WINDOWWIDTH/2.0 , -HEIGHT/2.0 + HEIGHT2WIN + WINDOWHEIGHT/2.0, .01);
  Vec posB (-WIDTH/2.0 + WIDTH2WIN + WINDOWWIDTH + WINDOWGAP + WINDOWWIDTH2/2.0 , -HEIGHT/2.0 + HEIGHT2WIN + WINDOWHEIGHT/2.0, .01);

  GL::push();
  GL::translate(posA.begin());
  glBegin (GL_TRIANGLE_STRIP);
  glColor3f (0,0,0);
  GL::Quad (bl,tl,br,tr);
  glEnd();
  GL::pop();

  GL::push();
  GL::translate(posB.begin());
  glBegin (GL_TRIANGLE_STRIP);
  glColor3f (0,0,0);
  GL::Quad (bl, tl,br,tr);
  glEnd();
  GL::pop();
};


void drawSquare (const Vec& pos, const Vec3f& col, float scale = 1){
  Vec bl = Vec(-OFFSET, -OFFSET, 0.0);
  Vec tl = Vec(-OFFSET, OFFSET, 0.0);
  Vec tr = Vec(OFFSET, OFFSET, 0.0);
  Vec br = Vec(OFFSET, -OFFSET, 0.0);

  GL::push();
  GL::translate(pos.begin());
  GL::scale (scale);
  glBegin (GL_TRIANGLE_STRIP);
  glColor3f (col.x, col.y, col.z);
  GL::Quad (bl, tl, br, tr);
  glEnd();
  GL::pop();
};

void drawEmSquare (const Vec& pos, const Vec3f& col){
  drawSquare (pos, Vec3f(0,0,1));
  drawSquare (pos, col, .9);
};


void drawTriangle (const Vec& pos, const Vec3f& col, Sector sector, float scale =1){
  Vec a,b,c;

  switch (sector) {
    case Sector::Left:
     {
      a = Vec(-OFFSET, -OFFSET, 0.0);
      b = Vec(-OFFSET, OFFSET, 0.0);
      c = Vec(0,0,0);//pos;
      break;
     }
    case Sector::Top:
     {
      a = Vec(-OFFSET, OFFSET, 0.0);
      b = Vec(OFFSET, OFFSET, 0.0);
      c = Vec(0,0,0);//pos;
      break;
     }
    case Sector::Right:
     {
      a = Vec(OFFSET, OFFSET, 0.0);
      b = Vec(OFFSET, -OFFSET, 0.0);
      c = Vec(0,0,0);//pos;
      break;
     }
    case Sector::Bottom:
     {
      a = Vec(OFFSET, -OFFSET, 0.0);
      b = Vec(-OFFSET, -OFFSET, 0.0);
      c = Vec(0,0,0);//pos;
      break;
     }
    default:
      break;
  }

  GL::push();
  GL::translate (pos.begin());
  GL::scale (scale);
  glBegin(GL_TRIANGLES);
  glColor3f (col.x, col.y, col.z);
  GL::Tri(a,b,c);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glColor3f (0,0,0);
  GL::vertex(a);
  GL::vertex(b);
  GL::vertex(c);
  glEnd();
  GL::pop();
};

struct ColorManager
{
  int iter = 0;

  int SeedNumber = 101;

  Vec3f Gen(){
    Vec3f col = TileColor (static_cast<Tone>(iter));
    iter++;
    if (iter == 4)
      iter = 0;
    return col;
  }

  static Vec3f Random(){
    return TileColor (static_cast<Tone>(Rand::Int(3)));
  }

  static Vec3f Random(const Vec& pos, bool bRandom=false){
    if (bRandom){
      return TileColor (static_cast<Tone>(Rand::Int(3)));
    }
//    float tx = (pos[0] + WIDTH/2.0) / WIDTH;
    float ty = (pos[1] + HEIGHT/2.0) / HEIGHT;
    float ty2 = fabs(1 - 2 * (pos[1] + HEIGHT/2.0) / HEIGHT);
 //   bool bx = Stat::Prob (tx);
    bool by = Stat::Prob (ty);
    bool by2 = Stat::Prob (ty2);
    bool bran = Rand::Boolean();


    Tone toneA = by ? Tone::White : Tone::Dark;
    Tone toneB = by ? Tone::Cloudy : Tone::Stormy;
    Tone toneC = by2 ? Tone::White : Tone::Dark;
    Tone toneD = by2 ? Tone::Cloudy : Tone::Stormy;

    return TileColor ( bran ? toneA : toneB );
  }


  static Tone Switch (bool a, bool b){
    return (a ? (b ? Tone::White : Tone::Cloudy ) : (b ? Tone::Stormy : Tone::Dark ) );
  }
};

void drawTile (const Vec& pos){

    drawSquare (pos, ColorManager::Random(pos));

    drawTriangle (pos + Vec(-OFFSET, OFFSET,0), ColorManager::Random(pos), Sector::Top);
    drawTriangle (pos + Vec(0, 2*OFFSET,0), ColorManager::Random(pos), Sector::Bottom);
    drawTriangle (pos + Vec(OFFSET, OFFSET, 0), ColorManager::Random(pos), Sector::Top);

    drawTriangle (pos + Vec(OFFSET, OFFSET, 0), ColorManager::Random(pos), Sector::Right);
    drawTriangle (pos + Vec(2*OFFSET, 0, 0), ColorManager::Random(pos), Sector::Left);
    drawTriangle (pos + Vec(OFFSET,-OFFSET, 0), ColorManager::Random(pos), Sector::Right);

    drawTriangle (pos + Vec(OFFSET,-OFFSET, 0), ColorManager::Random(pos), Sector::Bottom);
    drawTriangle (pos + Vec(0,-2*OFFSET, 0), ColorManager::Random(pos), Sector::Top);
    drawTriangle (pos + Vec(-OFFSET, -OFFSET,0), ColorManager::Random(pos), Sector::Bottom);

    drawTriangle (pos + Vec(-OFFSET, -OFFSET,0), ColorManager::Random(pos), Sector::Left);
    drawTriangle (pos + Vec(-2*OFFSET, 0,0), ColorManager::Random(pos), Sector::Right);
    drawTriangle (pos + Vec(-OFFSET, OFFSET,0), ColorManager::Random(pos), Sector::Left);


}

struct MyApp : App {

  ColorManager cm;

  void setup(){

    scene.camera.pos (0,0,70);
    scene.camera.lens.far() = 1000.0;

    glutReshapeWindow (1125,605);

  }

  void onDraw(){
    GL::lightsOff();
    glDisable(GL_DEPTH_TEST);

    Rand::Seed(cm.SeedNumber);

    int numX = 16;
    int numY = 9;

    for (int i = 0; i<numX; ++i)
    {
      float ti = (float)i/numX;
      float ii = -WIDTH/2.0 + i * TILEWIDTH;
      for (int j = 0; j<numY; ++j)
      {
        float tj = (float)j/numY;
        float jj = -HEIGHT/2.0 + j * TILEWIDTH;
        Vec v (ii, jj, 0.0);
        drawTile (v);
      }
    }

    drawWindows();


    //cout << "*************" << endl;
  }

  void reseed(){
    cm.SeedNumber++;
  }

  void onKeyDown(const gfx::Keyboard &k){
   if (k.code=='s') reseed();

   App::onKeyDown(k);
  }

};


int main(){


  MyApp app;
  app.start();

  return 0;
}
