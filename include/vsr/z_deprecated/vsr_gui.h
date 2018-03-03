/*
 * =====================================================================================
 *
 *       Filename:  vsr_gui.h
 *
 *    Description:  gui table
 *
 *        Version:  1.0
 *        Created:  01/20/2014 16:38:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), wolftype (gmail)
 *   Organization:  Media Arts and Technology Program, UCSB
 *
 * =====================================================================================
 */

#ifndef  vsr_gui_INC
#define  vsr_gui_INC

#include "GLV/glv.h"
#include "vsr_std_types.h"
#include <iostream>
#include <sstream>


using namespace std;

//using namespace gfx;


namespace glv {


enum {
    SLIDER,
    SLIDERS,
    SLIDER2D,
    BUTTON,
    BUTTONS,
    DIALER,
    COLORSLIDER
  };



  //typedef Widget Widget;
  typedef map<string, Widget*> WidgetMap;
  typedef map<string, Widget*>::iterator WidgetPtr; 

    class Gui : public Table {

        /* Map of Widgets (index by name) */
        WidgetMap mWidget;

        /* Placement Defaults */
        Placer * mPlacer;

        bool bDesc;        //show descriptor?
        bool bVisible;    //draw to screen?

              void _init(){

                  enable ( Controllable |  DrawBorder | DrawBack | FocusHighlight );

                  int spacing    = 10;
                  mPlacer = new Placer(*this, Direction::S, Place::TL, spacing, spacing);

                  Label :: Spec spec(Place::CL, width() + 20, 0);
                  Label * tmp = new Label ( name(), spec);

                  *mPlacer << *tmp;

                  addHandler(Event::MouseDrag, Behavior::mouseMove);
              }

        public:

                  Gui() : Table("<"), bVisible(true) {
                      _init();
                  }

                  Gui(int _w , int _h ) : Table("<"), bVisible(true) {
                      this -> w = _w;
                      this -> h = _h;
                      _init();
                  }


                  Gui(string _name): Table("<"), bVisible(true) {
                      name(_name);
                      _init();
                  }

          void add ( Widget* v) {
            mWidget[ v->name() ] = v;

            Label :: Spec spec(Place::CL, v -> width() + 20, 0);
            Label * tmp = new Label (v->name(), spec);

            //cout << "adding and placing pointer " << (v->name()) << "to gui." << endl;
            *mPlacer << ( ( *mWidget[ v->name() ] ) << *tmp );

          }

                  /*!  add widget and automatically determine type of widget */
                  template<class T>
                  Gui& operator () (T& val, string nm = "", float min = 0, float max = 1){

                      using namespace vsr;
                      static int it = 0;
                      stringstream name;

                      if (nm == "" ) 
                          name << typeid(T).name() << "_" << it; 
                      else name << nm;


                      switch ( vsr::Types[ typeid(T).name() ] ){
                          case vsr::BOOLEAN:
                              add(BUTTON, name.str(), val);
                              break;
                          case FLOAT:
                          case DOUBLE:
                          case INT:
                              add(DIALER, name.str(), val, min, max);
                              break;
                          default:
                              break;
                      }

                      it++;
                      return *this;
                  }
                  /*!  add widget and automatically determine type of widget */
                  template<class T>
                  Gui& operator () (T& val, float max){
                      return (*this)(val, "", 0, max);
                  }
          /*! add widget: enum type, name, min, max, attach, num */
          template <class V>
          void add (int, string, V& val, float min = 0., float max = 1. );

                  void add ( Widget* v, const std::string& _name){

                      mWidget[ _name ] = v;  
                      Label * tmp = new Label (_name );

                      Box * box = new Box();
                      *this << ( *box << *mWidget[ _name]  << *tmp );  

                      arrange();    
                  }

          Widget& widget(string name) { return *mWidget[name]; }      ///< Get Widget
          WidgetMap& widget() { return mWidget; }              ///< Get Widget Map

          void updateValues();

          /* Render on Screen */
          virtual void onDraw(GLV& g){
            glTranslated(5,h-10,0);
            glColor3f(1,1,1);
            draw::text( name().c_str() );
          }


    };


    template <class V>
    void Gui :: add (int _type, string _name, V& val, float min , float max ){

      Widget * s;
      switch (_type){
        case SLIDER:
        {
          Slider * ns = new Slider(glv::Rect(100,20));
          ns -> attachVariable(val, 0 );
          s = (Widget*)ns;
          break;
        }
        case SLIDER2D:
        {
          Slider2D * ns = new Slider2D();
          s = (Widget*)ns;
          break;
        }
        case BUTTON:
        {
                  cout << " adding Button to gui " << endl; 
          Button *ns = new Button(glv::Rect(20,20));
          ns -> attachVariable(val, 0);
          s = (Widget*)ns;
          break;
        }
        case DIALER:
        {
          NumberDialer * ns = new NumberDialer(4,3,max,min);
          ns -> attachVariable(val, 0);
          s = (Widget*)ns;
          break;
        }
      }

      s->colors().text.set(0,1,0); 
      s->colors().fore.set(0,1,0);  
      //s->colors().set(0,1,0);
      s->name( _name );
      add((Widget*)s, _name);
    }

} //glv gui
                          
#endif   /* ----- #ifndef vsr_gui_INC  ----- */
