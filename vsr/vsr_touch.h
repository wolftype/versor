#ifndef VSR_TOUCH_H_INCLUDED
#define VSR_TOUCH_H_INCLUDED
 

//CONTROLLER?

       /// Executes onTouch
        template <class A> void touch(A& s, const Pnt& x, double t);
        template <class A> void touch(A&, double t = 1.0);
        
        void touch ( Vec& s, double t){ touch(s, Ro::dls(s,.5), t); }
        void touch( Tnv& s, double t){ touch(s, Ro::dls(s,.5), t); }
        void touch( Drv& s, double t){ touch(s, Ro::dls(s,.5), t); }
        
        void touch ( Pnt& s, double t) { touch(s, Ro::dls_pnt( Ro::loc(s), .2), t); }
        void touch ( Sph& s, double t) { touch(s, s, t); }

        void touch( Cir& s, double t){ touch(s, Ro::sur(s), t); }
        void touch( Par& s, double t){ touch(s, Ro::sur(s), t); }

        void touch( Lin& s, double t){ touch(s, Ro::dls_pnt( Fl::loc(s, Ori(1), 0) ), t); }
        void touch( Pln& s, double t){ touch(s, Ro::dls_pnt( Fl::loc(s, Ori(1), 0) ), t); }

        void touch( Dll& s, double t){ touch(s, Ro::dls_pnt( Fl::loc(s, Ori(1), 1) ), t); }
        void touch( Dlp& s, double t){ touch(s, Ro::dls_pnt( Fl::loc(s, Ori(1), 1) ), t); }


        /// Transform a State 's' Centered at position 'pos' by velocity 't'
        template <class A> void xf ( A * s, const Pnt& pos, double t );


        void touch( Frame& f, double t = 1.0);
        ///Maniupulate Frame e by clicking on Frame f
        void touch( Frame& f, Frame& e, double t = 1.0);
 
        //void windowTransform();
        void stateTransform();
        
        //NAVIGATION
        void keyboardCamSpin(float acc, bool trigger);
        void keyboardCamTranslate(float acc, bool trigger);
        void keyboardModelTransform(float acc, bool trigger);
        void mouseModelTransform(float acc, bool trigger);
        void mouseCamTranslate(float acc, bool trigger);
        void mouseCamSpin(float acc, bool trigger);
        
        void onMouseMove();        
        void onMouseDown();
        void onMouseDrag();
        void onMouseUp();        
        void onKeyDown();
        void onKeyUp();
        


        /// Transform a Frame 'f' by velocity 't'
        void xfFrame( Frame * f, double t);
        /// Transform a Frame 'e' Centered at Position 'f' by velocity 't'
        void xfFrame( Frame * f, Frame * e, double t);        




        void print(){}
        
        double& db() { return mRot; }
        double& dv() { return mVel; }      



template <class A> 
void Interface :: touch( A& s, const Pnt& x, double t){
        
        if ( !keyboard.alt && !keyboard.shift ){
        
            //physics
            static double dt = 1;
            static double acc = .9;
            dt *= acc;
            
            if ( mouse.isDown ){
                dt = t; // Reset acc
                if ( pntClicked( x ) ) {
                    select( &s );
                }
            }
            
            if ( isSelected( &s ) ){
                xf(&s,x,dt);
            }
        
        }
}

    //Element, Center Point, Amt
    template <class A> 
    void Interface :: xf ( A * s, const Pnt& pos, double t ) {

        //Address of State
        A& ts = *s;
        
        //Center of Defining Sphere
        Vec tv ( Ro::loc(pos) );   
                
        //2D coordinates of Defining Sphere
        Vec sc = screenCoord(tv, scene().xf );
        
        //Point in 3D space on Projection Ray closest to sphere.
        Pnt cp  = Fl::loc( vd().ray, Ro::loc(pos), 1);        
        
        switch(keyboard.code){
        
            case 's': //SCALE
            {
                Vec tm1 = mouse.pos + mouse.drag - sc;
                Vec tm2 = mouse.pos - sc; 
                
                //Drag towards or away from element . . . 
                int neg = (tm1.norm() > tm2.norm()) ? 1 : -1; 
                auto tsd = Gen::dil( Ro::loc(pos), mouse.drag.norm() * t * neg );
                ts = Op::sp( ts,  tsd);
                
                break;
            }
            case 'g': //TRANSLATE
            {
                ts = Op::sp (ts, Gen::trs( mouse.dragCat * t ) );
                break;
            }
            case 'r': //ROTATE local line
            {
                Dll td = pos <= Drb( mouse.dragBivCat * t );
                ts = Op::sp ( ts, Gen::mot( td ) );
                break;
            }
            
            case 'a': //(DEPRECATED!) scale using RO:: instead of Ro::loc 
            {
                //printf("scale\n");s
                Vec tm1 = mouse.pos + mouse.drag - sc;
                Vec tm2 = mouse.pos - sc; 
                
                //Drag towards or away from element
                int neg = (tm1.norm() > tm2.norm()) ? 1 : -1; 
                ts = Op::sp( ts, Gen::dil( Ro::cen( pos ), mouse.drag.norm() * t * neg ) );
                break;
            }

            case 't': // twist about global line (experimental)
            {
                Dll td = Op::dl( mouse.origin ^ mouse.dragCat ^ Inf(1) );
                ts = Op::sp ( ts, Gen::mot(td) );
                break;
            }

            default://case 'q': //DESELECT
            {
                toggleSelect(s);
                break;
            }
        }
    }

#endif