//
//  Interface.h
//  A Control Object: Manipulate vsr objects using screen interaction (to be fed mouse input data)
//  
//  NOTE: ASSUMES CONFORMAL METRIC R4,1
//
//  Created by Pablo Colapinto on 3/24/12.
//  Copyright (c) 2012 Wolftype. All rights reserved.
//

#ifndef VSR_INTERFACE_INCLUDED
#define VSR_INTERFACE_INCLUDED


#include "gfx/gfx_gl.h"
#include "gfx/gfx_glu.h"
#include "gfx/gfx_xfmatrix.h"  
#include "gfx/gfx_scene.h"

#include "operations.h"
#include <map>  


using namespace gfx;

namespace vsr  {
    

	struct MouseData {
    
        enum  {
            Up = 1, Down, Left, Right
        };
        
		float x, y, dx, dy, ddx, ddy, xrel, yrel; //< 2D Mouse Position, first and second derivatives of motion, position relative to TL corner
		
        Vec click, pos, move, accel, cat, drag, dragAccum, dragCat, projectFar, projectNear, projectMid;
        Pnt origin;
		Biv biv, bivCat, dragBiv, dragBivCat;
        
        int gesture;  // stores major direction of mouse movement
        
		bool isDown, isMoving, newClick;		
		void toggle() { newClick = !newClick; }
	};
	
	struct ViewData  {
		float w, h; 
        Vec z; 
        Dll ray, clickray; 
	};
    
	struct KeyboardData {
		bool shift, alt, caps, ctrl, meta, f, down;
		int code;
		KeyboardData() : shift(0), alt(0), caps(0), ctrl(0), code(0){}
	};    
    
    /* Mapped Key values ripped from GLV */
    namespace Key{
        enum {
            
            // Standard ASCII non-printable characters
            Enter		=3,		/**< */
            Backspace	=8,		/**< */
            Tab			=9,		/**< */
            Return		=13,	/**< */
            Escape		=27,	/**< */
            Delete		=127,	/**< */
			
            // Non-standard, but common keys.
            F1=256, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, 
            Insert, Left, Up, Right, Down, PageDown, PageUp, End, Home
        };
    }
    
    /*! Abstract Interface Class */
    class Interface {
        
        int mMode;  ///< Edit Mode State (not implemented)
        
    public:
    
        
        typedef std::map<string,bool> SBMap;      

        /// Map of object addresses and whether they are selected
        SBMap selectMap;                            
        
        
        /*! Abstract View Implementation Data to store info from Windowing System */
        struct Impl {                      

            Interface * interface;  
            
            Impl(Interface * i ) : interface(i) {}          // Register with Interface upon construction

            //ViewData holds Window Information
           // ViewData data;            
            
            virtual ~Impl(){}

            // All implementations should define fullScreenToggle method and getData methods
            virtual void fullScreenToggle() {};            
            virtual void getViewData( void * udata){};  
            virtual void getKeyboardData( void * udata){};
            virtual void getMouseData( void * udata){};
			virtual void getHIDData( void * udata ){}; //other input devic
        };
        
        
        Interface(){};     


        virtual void init() = 0;
        
        void inputCalc();        ///< Calculate Mouse Movements based on x and dx
        void viewCalc();         ///< Calculate Window Matrices, Screen Coordinates of Mouse
        
        Impl * impl;       		 ///< Implementation of Window Information (Width, Height) and Inputs (Keyboard, Mouse)  
        
		ViewData		view;
        MouseData       mouse;
        KeyboardData    keyboard;
        
		ViewData& vd() { return view; }//vimpl -> data; }
        //ViewData vd() const { return vimpl -> data; }
  
        KeyboardData& kd() { return keyboard; }
        MouseData& md() { return mouse; }

        
        // template <class A> static Vec screenCoord(const A& p, const XformMat& );
        // template <class A> bool pntClicked(const A&, double rad = .05);  

        Vec click(){ return mouse.click;   }
        Vec pos(){ return mouse.pos;     } 
  
        /*! Set Mode */
        int& mode() { return mMode; }
        /*! Get Mode */
        int mode() const { return mMode; }
        /// Check Interface Mode
        bool mode(int q) { return mMode & q; }  
       /// Enable Mode    
        void enable(int bitflag) { mMode |= bitflag; }
        void disable(int bitflag) { mMode &= ~bitflag; }
        void toggle(int bitflag)  { mMode & bitflag ? disable(bitflag) : enable(bitflag); }  

        template <class A > bool isSelected( A * );
        template <class A > void select( A * );
        template <class A > void deselect( A * );
        template <class A > void toggleSelect( A * );
    };

    // template< class A > bool Interface :: isSelected ( A * a ){
    //     stringstream s; s << a;
    //     bool tmp = selectMap[ s.str() ];
    //     return tmp;
    // }
    // 
    // template < class A > void Interface :: select( A * a) {
    //     stringstream s; s << a;
    //     selectMap[ s.str() ] = true;
    // }
    // 
    // template < class A > void Interface :: deselect( A * a) {
    //     stringstream s; s << a;
    //     selectMap[ s.str() ] = false;
    // }
    // 
    // template < class A > void Interface :: toggleSelect( A * a) {
    //     stringstream s; s << a;
    //     selectMap[ s.str() ] = !selectMap[ s.str() ];
    // }    
        
    // /// Screen Coordinates of Target point
    // template <class A> Vec Interface :: screenCoord(const A& p, const XformMat& xf){
    //     Vec sc = gfx::GL::project(p[0], p[1], p[2], xf );
    //     sc[0] /= xf.viewport[2]; sc[1] /= xf.viewport[3]; sc[2] = 0;
    //     
    //     return sc;
    // }
    // 
    // template <class A> bool Interface :: pntClicked( const A& x, double rad ) {
    //     Vec v = mouse.click;  // [0,0] is bottom left corner [1.0,1.0] is top right
    //     Vec p = Ro::loc(x);
    //     
    //     Vec sc = screenCoord(p, scene().xf);
    // 
    //     Vec dist = (v - sc);
    //     return (dist.norm() < rad) ? true : false;
    // } 
  
}



#endif