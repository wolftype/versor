/*
 * =====================================================================================
 *
 *       Filename:  vsr_print.h
 *
 *    Description:  routines for printing to tikz (postscript)
 *
 *        Version:  1.0
 *        Created:  10/17/2014 19:26:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sstream>

using namespace std;

namespace vsr {

 namespace tikz{

  string begin(){
    return "\\begin{tikzpicture}";
  }
  string end(){
    return "\\end{tikzpicture}";
  }

  template<class T>
  string coord(const T& t){
    stringstream os;
    os << "("<<ZERO(t[0])<<","<<ZERO(t[1])<<","<<ZERO(t[2])<<")";
    return os.str();
  }

  template<class T>
  string coord(string name, const T& t){
    stringstream os ; os<< "\\coordinate"+name+" at "+coord(t)<<";"<< endl;
    return os.str();
  }

  string name(string n, int x){
    stringstream os; os <<"("<<n<<x<<")";
    return os.str();
  }

  /* string drawPS(string style = "[->]"){ */
  /*   return "\\draw"+style; */
  /* } */
  template<class T>
  string draw(const T& from, const T& to, string style = "->"){
    stringstream os;
    os << "\\draw["<<style<<"]"<<coord(from) << "--" << coord(to) << ";"<<endl;
    return os.str();
  }


}


}//vsr::
