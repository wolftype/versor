/*
 *
 *  SEE ALSO vsr_rigid.h for alternative (uses pointers)
 *  see also vsr_fold_molecules for early implementations
 *
 *
 */


#ifndef VSR_H_FOLD_INCLUDED
#define VSR_H_FOLD_INCLUDED

#include "space/vsr_cga3D_op.h"
#include "form/vsr_set.h"
#include "form/vsr_rigid.h"

namespace vsr{ namespace cga{



/*-----------------------------------------------------------------------------
 *  FOLD LINE CONSTRUCTION
 *-----------------------------------------------------------------------------*/
struct Fold {

    //Perimeter lines of a triangle
    static vector<Line> Lines(const Point& a, const Point& b, const Point& c){
      vector<Line> vl;
      vl.push_back( (a ^ b ^ Inf(1)).runit() );
      vl.push_back( (b ^ c ^ Inf(1)).runit() );
      vl.push_back( (c ^ a ^ Inf(1)).runit() );
      return vl;
    }

    //Perimeter lines of a quadralateral
    static vector<Line> Lines(const Point& a, const Point& b, const Point& c, const Point& d){
      vector<Line> vl;
      vl.push_back( (a ^ b ^ Inf(1)).runit() );
      vl.push_back( (b ^ c ^ Inf(1)).runit() );
      vl.push_back( (c ^ d ^ Inf(1)).runit() );
      vl.push_back( (d ^ a ^ Inf(1)).runit() );
      return vl;
    }

    //Perimeter lines of an arbitrary polygon
    static vector<Line> Lines( const vector<Point>& src ){
        vector<Line> vl;
        for (int i = 0; i < src.size()-1; ++i){
          vl.push_back( ( src[i] ^ src[i+1] ^ Inf(1) ).unit() );
        }
        vl.push_back( (src.back() ^ src[0] ^ Inf(1) ).unit() );
        return vl;
    }
    
    //Perimeter lines of an arbitrary polygon
    static vector<Line> Lines( Point * src, int num ){
        vector<Line> vl;
        for (int i = 0; i < num -1; ++i){
          vl.push_back( ( src[i] ^ src[i+1] ^ Inf(1) ).unit() );
        }
        vl.push_back( (src[num-1] ^ src[0] ^ Inf(1) ).unit() );
        return vl;
    }

    //Radial lines out from a center
    static vector<Line> Lines( const Point& center, const vector<Point>& nodes){
        
        vector<Line> vl;
        for (auto i : nodes){
          vl.push_back( ( center ^ i ^ Inf(1) ).unit() );
        }

        return vl;
    }


    static vector<Line> Bisect( const vector<Line>& lines ){
        vector<Line> vl;
        vl.push_back( (lines.back() - lines[0]).unit() );
        for (int i = 0; i < lines.size()-1; ++i){
          vl.push_back( ( lines[i] - lines[i+1] ).unit() );
        }
        return vl;
    }

    static Point Meet ( const vector<Line>& lines) {
      return Construct::meet( lines[0], lines[1] ) ;
    }

    static Point Perpendicular( const Point& c, const Line& line){
      return Flat::loc(line, c, false).null();
    }

    static vector<Point> Perpendiculars( const Point& c, const vector<Line>& lines){
      vector<Point> vp;

      for (auto i : lines){
        vp.push_back( Flat::loc(i, c, false).null() ); //location of point closest to line
      }

      return vp;
    }


    static vector<Circle> Circles( const vector<Point>& src, const vector<Line>& crease ){
       vector<Circle> vc;
       for (int i = 0; i < src.size(); ++i) {
           vc.push_back( src[i] ^ crease[i].dual()  );
       }

       return vc;
    }


};


} } //vsr::cga

#endif

