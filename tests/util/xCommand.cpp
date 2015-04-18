
#include "vsr_cga3D_op.h"  
#include <iostream>
#include <string>
#include <map>

using namespace vsr;
using namespace vsr::cga3D;
     
using namespace std;


template<int S>
struct Me{
  static void print() { printf("me%d\n", S); }
};

template<>
struct Me<2>{
  static void print() { printf("meno\n"); }
};

map<string, int> SMap;


enum {
  VEC = 1,
  BIV,
  ROT,
  INF,
  MNK,
  PNT,
  PAR,
  CIR,
  SPH,
  LIN,
  DLL,
  PLN,
  DLP,
  TNV,
  TNB,
  TNT,
  DRV,
  DRB,
  DRT,
  TRS,
  MOT,
  BST,
  IP, OP, GP
};


template<class A, class B, int OP>
void prod(const A& a, const B& b){
  auto r = a*b;
  r.bprint();
}


void printInfo(const string& s){
    cout << s << " type id: " <<  SMap[ s ] << endl;

    switch ( SMap[s] ) {
      case OP:
      case IP:
      case GP:
        cout << "operator\n" << endl;
        break;
      case VEC:
        Vec::bprint();
        break;
      case BIV:
        Biv::bprint();
        break;
      case ROT:
        Rot::bprint();
        break;
      case INF:
        Inf::bprint();
        break;
      case MNK:
        Mnk::bprint();
        break;
      case PNT:
        Pnt::bprint();
        break;
      case PAR:
        Par::bprint();
        break;
      case CIR:
        Cir::bprint();
        break;
      case SPH:
        Sph::bprint();
        break;
      default:
        cout << "unknown" << endl; 
    }

}

int main(int argc, char *argv[] ){

  SMap["^"]   = OP;
  SMap["<="]  = IP;
  SMap["*"]   = GP;
  SMap["Rot"] = ROT;
  SMap["Vec"] = VEC;
  SMap["Biv"] = BIV;
  SMap["Inf"] = INF;
  SMap["Mnk"] = MNK;
  SMap["Pnt"] = PNT;
  SMap["Cir"] = CIR;
  SMap["Par"] = PAR;
  SMap["Sph"] = SPH;
                     
  for (int i = 0; i < argc - 1; ++i){
    printInfo(argv[i+1]);
  }

  (Pair() * Con()).print();
  (Tnv() * Con()).print();
  //Vec a;
  //a.print();
  //Prod<GP>( a, a );

	return 0;

}
