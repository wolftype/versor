/** * Command Line Utility
 *
 * Inputs binary operation, outputs type
 *
 * Also, allows construction of binary functions,
 * prototype for a larger operational geometry program
 */

#include <vsr/space/vsr_cga3D_op.h>

#include <iostream>
#include <string>
#include <map>

using namespace vsr;
using namespace vsr::cga;

using namespace std;

#define WRAP(T)                                                                \
  template <>                                                                  \
  CGAType Wrapper<T>::TypeID ()                                                \
  {                                                                            \
    return CGAType::T;                                                         \
  }

//Enum types ... add new ones here as compiler errors grow
//and add Wrapper<T>::TypeID () overrides as necessary
enum class CGAType
{
  Sca = 1,
  Vec,
  RotVec,
  Biv,
  Tri,
  Rot,
  Inf,
  Mnk,
  Pnt,
  Par,
  Cir,
  Sph,
  Lin,
  Dll,
  Pln,
  Dlp,
  Tnv,
  Tnb,
  Tnt,
  Drv,
  Drb,
  Drt,
  Trs,
  Mot,
  Bst,
  Pss,
  IP,
  OP,
  gp
};

//Map string to type, e.g. from a command line argument
map<string, CGAType> SMap;

//purely virtual base for runtime type deduction
struct WrapperBase
{
  virtual CGAType TypeID () = 0;

  virtual void set (void *) = 0;
  virtual void print () { cout << "BASIC" << endl; }
  //virtual void draw () { cout << "I don't know how" << endl; }

  virtual std::shared_ptr<WrapperBase> gp (WrapperBase *) = 0;
  //virtual std::shared_ptr<WrapperBase> OP (WrapperBase *) = 0;
  //virtual std::shared_ptr<WrapperBase> IP (WrapperBase *) = 0;

  virtual void gp_into (WrapperBase *, WrapperBase *) = 0;
  virtual void set_from_gp (WrapperBase *, WrapperBase *) = 0;
};

template <typename T>
struct Wrapper : public WrapperBase
{
  T val;

  Wrapper () {}
  Wrapper (const T &v) : val (v) {}

  CGAType TypeID () override;
  void set (void *v) override { val = *((T *) v); }
  virtual void print () override { val.print (); }

  std::shared_ptr<WrapperBase> gp (WrapperBase *) override;
  void gp_into (WrapperBase *rh, WrapperBase *res) override;
  void set_from_gp (WrapperBase *, WrapperBase *) override;
};

WRAP (Sca)
WRAP (Vec)
WRAP (Biv)
WRAP (Tri)
WRAP (Rot)
WRAP (RotVec)
WRAP (Inf)
WRAP (Mnk)
WRAP (Pnt)
WRAP (Par)
WRAP (Cir)
WRAP (Sph)
WRAP (Lin)
WRAP (Dll)
WRAP (Pln)
WRAP (Dlp)
WRAP (Tnv)
WRAP (Tnb)
WRAP (Tnt)
WRAP (Drv)
WRAP (Drb)
WRAP (Drt)
WRAP (Trs)
WRAP (Mot)
WRAP (Bst)
WRAP (Pss)

//intermediate type that pops up on occasion:
template <>
CGAType Wrapper<decltype (Vec () * Rot ())>::TypeID ()
{
  return CGAType::RotVec;
};

template <typename T>
std::shared_ptr<WrapperBase> MakeNewFromVal (const T &val)
{
  return std::shared_ptr<WrapperBase> (new Wrapper<T> (val));
}

#define MAKESHAREDCASE(T)                                                      \
  case CGAType::T:                                                             \
    return std::shared_ptr<WrapperBase> (new Wrapper<T> ());                   \
    break;

std::shared_ptr<WrapperBase> MakeNewFromString (const string &s)
{
  switch (SMap[s])
    {
      MAKESHAREDCASE (Sca)
      MAKESHAREDCASE (Vec)
      MAKESHAREDCASE (RotVec)
      MAKESHAREDCASE (Biv)
      MAKESHAREDCASE (Tri)
      MAKESHAREDCASE (Rot)
      MAKESHAREDCASE (Inf)
      MAKESHAREDCASE (Mnk)
      MAKESHAREDCASE (Pnt)
      MAKESHAREDCASE (Par)
      MAKESHAREDCASE (Cir)
      MAKESHAREDCASE (Sph)
      MAKESHAREDCASE (Lin)
      MAKESHAREDCASE (Dll)
      MAKESHAREDCASE (Pln)
      MAKESHAREDCASE (Dlp)
      MAKESHAREDCASE (Tnv)
      MAKESHAREDCASE (Tnb)
      MAKESHAREDCASE (Tnt)
      MAKESHAREDCASE (Drv)
      MAKESHAREDCASE (Drb)
      MAKESHAREDCASE (Drt)
      MAKESHAREDCASE (Trs)
      MAKESHAREDCASE (Mot)
      MAKESHAREDCASE (Bst)
      MAKESHAREDCASE (Pss)
    }

  return std::shared_ptr<WrapperBase> (new Wrapper<Sca> ());
}
#undef MAKESHAREDCASE

#define MAKEGPCASE(T)                                                          \
  case CGAType::T:                                                             \
    return MakeNewFromVal (val * dynamic_cast<Wrapper<T> *> (a)->val);

#define MAKEOPCASE(T)                                                          \
  case CGAType::T:                                                             \
    return MakeNewFromVal (val ^ dynamic_cast<Wrapper<T> *> (a)->val);

#define MAKEIPCASE(T)                                                          \
  case CGAType::T:                                                             \
    return MakeNewFromVal (val <= dynamic_cast<Wrapper<T> *> (a)->val);

template <typename T>
std::shared_ptr<WrapperBase> Wrapper<T>::gp (WrapperBase *a)
{
  switch (a->TypeID ())
    {
      MAKEGPCASE (Sca)
      MAKEGPCASE (Vec)
      MAKEGPCASE (RotVec)
      MAKEGPCASE (Biv)
      MAKEGPCASE (Tri)
      MAKEGPCASE (Rot)
      MAKEGPCASE (Inf)
      MAKEGPCASE (Mnk)
      MAKEGPCASE (Pnt)
      MAKEGPCASE (Par)
      MAKEGPCASE (Cir)
      MAKEGPCASE (Sph)
      MAKEGPCASE (Lin)
      MAKEGPCASE (Dll)
      MAKEGPCASE (Pln)
      MAKEGPCASE (Dlp)
      MAKEGPCASE (Tnv)
      MAKEGPCASE (Tnb)
      MAKEGPCASE (Tnt)
      MAKEGPCASE (Drv)
      MAKEGPCASE (Drb)
      MAKEGPCASE (Drt)
      MAKEGPCASE (Trs)
      MAKEGPCASE (Mot)
      MAKEGPCASE (Bst)
      MAKEGPCASE (Pss)
    }
  //default:
  return MakeNewFromVal (Sca ());
};


#define MAKEGPINTOCASE(T)                                                      \
  case CGAType::T:                                                             \
    auto tmp = val * dynamic_cast<Wrapper<T> *> (a)->val;                      \
    result->set (&tmp);                                                        \
    break;

#define MAKEOPINTOCASE(T)                                                      \
  case CGAType::T:                                                             \
    auto tmp = val ^ dynamic_cast<Wrapper<T> *> (a)->val;                      \
    result->set (&tmp);                                                        \
    break;

#define MAKEIPINTOCASE(T)                                                      \
  case CGAType::T:                                                             \
    auto tmp = val <= dynamic_cast<Wrapper<T> *> (a)->val;                     \
    result->set (&tmp);                                                        \
    break;

template <typename T>
void Wrapper<T>::gp_into (WrapperBase *a, WrapperBase *result)
{
  switch (a->TypeID ())
    {
      MAKEGPINTOCASE (Sca)
      MAKEGPINTOCASE (Vec)
      MAKEGPINTOCASE (RotVec)
      MAKEGPINTOCASE (Biv)
      MAKEGPINTOCASE (Tri)
      MAKEGPINTOCASE (Rot)
      MAKEGPINTOCASE (Inf)
      MAKEGPINTOCASE (Mnk)
      MAKEGPINTOCASE (Pnt)
      MAKEGPINTOCASE (Par)
      MAKEGPINTOCASE (Cir)
      MAKEGPINTOCASE (Sph)
      MAKEGPINTOCASE (Lin)
      MAKEGPINTOCASE (Dll)
      MAKEGPINTOCASE (Pln)
      MAKEGPINTOCASE (Dlp)
      MAKEGPINTOCASE (Tnv)
      MAKEGPINTOCASE (Tnb)
      MAKEGPINTOCASE (Tnt)
      MAKEGPINTOCASE (Drv)
      MAKEGPINTOCASE (Drb)
      MAKEGPINTOCASE (Drt)
      MAKEGPINTOCASE (Trs)
      MAKEGPINTOCASE (Mot)
      MAKEGPINTOCASE (Bst)
      MAKEGPINTOCASE (Pss)
    }
};

template <typename T>
void Wrapper<T>::set_from_gp (WrapperBase *a, WrapperBase *b)
{
  a->gp_into (b, this);
}

struct BinOp
{
  std::shared_ptr<WrapperBase> argA;
  std::shared_ptr<WrapperBase> argB;
  std::shared_ptr<WrapperBase> result;

  CGAType lh_type;
  CGAType rh_type;

  bool bResultNeedsInit = true;

  void SetArgA (std::shared_ptr<WrapperBase> a)
  {
    if (lh_type != a->TypeID ())
      bResultNeedsInit = true;

    argA = a;
    lh_type = a->TypeID ();
  }

  void SetArgB (std::shared_ptr<WrapperBase> b)
  {
    if (rh_type != b->TypeID ())
      bResultNeedsInit = true;

    argB = b;
    rh_type = b->TypeID ();
  }

  template <typename T, typename S>
  BinOp (const T &_a, const S &_b)
      : argA (MakeNewFromVal (_a)), argB (MakeNewFromVal (_b))
  {
  }

  BinOp (std::shared_ptr<WrapperBase> _a, std::shared_ptr<WrapperBase> _b)
      : argA (_a), argB (_b)
  {
  }

  virtual void calc () = 0;
};

struct WProduct : BinOp
{
  template <typename T, typename S>
  WProduct (const T &_a, const S &_b) : BinOp (_a, _b)
  {
  }

  void calc () override
  {
    if (bResultNeedsInit)
      {
        result = argA->gp (argB.get ());
        bResultNeedsInit = false;
      }
    else
      result.get ()->set_from_gp (argA.get (), argB.get ());
  }
};
//
//struct OuterProduct : BinOp
//{
//  void Calc () override
//  {
//    if (bResultNeedsInit)
//      {
//        result = argA->op (argB.get ());
//        bResultNeedsInit = false;
//      }
//    else
//      argA->op_into (argB.get (), result.get ());
//  }
//};
//
//struct InnerProduct : BinOp
//{
//  void Calc () override
//  {
//    if (bResultNeedsInit)
//      {
//        result = argA->ip (argB.get ());
//        bResultNeedsInit = false;
//      }
//    else
//      argA->ip_into (argB.get (), result.get ());
//  }
//};

std::vector<string> parse (const string &s)
{
  std::vector<string> res;
  size_t begin_pos = 0;
  size_t end_pos = 0;
  do
    {
      end_pos = s.find_first_of (" ", begin_pos);
      res.push_back (s.substr (begin_pos, end_pos - begin_pos));
      begin_pos = end_pos + 1;
    }
  while (end_pos != string::npos);

  return res;
}


void printInfo (const string &s)
{
  switch (SMap[s])
    {
      case CGAType::OP:
      case CGAType::IP:
      case CGAType::gp:
        cout << "operator\n" << endl;
        break;
      case CGAType::Vec:
        Vec::basis::print ();
        break;
      case CGAType::Biv:
        Biv::basis::print ();
        break;
      case CGAType::Rot:
        Rot::basis::print ();
        break;
      case CGAType::Inf:
        Inf::basis::print ();
        break;
      case CGAType::Mnk:
        Mnk::basis::print ();
        break;
      case CGAType::Pnt:
        Pnt::basis::print ();
        break;
      case CGAType::Par:
        Par::basis::print ();
        break;
      case CGAType::Cir:
        Cir::basis::print ();
        break;
      case CGAType::Sph:
        Sph::basis::print ();
        break;
      default:
        cout << "unknown" << endl;
    }
}

int main (int argc, char *argv[])
{

  SMap["^"] = CGAType::OP;
  SMap["<="] = CGAType::IP;
  SMap["*"] = CGAType::gp;
  SMap["Rot"] = CGAType::Rot;
  SMap["Vec"] = CGAType::Vec;
  SMap["Biv"] = CGAType::Biv;
  SMap["Inf"] = CGAType::Inf;
  SMap["Mnk"] = CGAType::Mnk;
  SMap["Pnt"] = CGAType::Pnt;
  SMap["Cir"] = CGAType::Cir;
  SMap["Par"] = CGAType::Par;
  SMap["Sph"] = CGAType::Sph;

  if (argc > 1)
    {
      auto com = parse (argv[1]);
      for (auto &i : com)
        cout << i << endl;

      if (com.size () == 3)
        if (com[1] == "*")
          {
            //auto res = Multiply (MakeNewFromString (com[0]).get (),
            //                                MakeNewFromString (com[2]).get ());
            //           res->print ();
          }
    }

  auto va = MakeNewFromString ("Vec");
  auto vb = MakeNewFromString ("Vec");
  auto biv = MakeNewFromString ("Biv");
  auto ra = MakeNewFromString ("Rot");
  auto rb = MakeNewFromString ("Rot");

  WProduct product_vv (va, biv);
  WProduct product_rr ((Rot (1, .5, 0, 0)), (Vec (1, 1, 1)));

  product_vv.calc ();
  product_rr.calc ();

  product_vv.result.get ()->print ();
  product_rr.result.get ()->print ();

  //  auto res = va->mult (vb.get ());

  //  res.get()->get();

  return 0;
}



/// OOOOOLLLLLLDDD
//Wraps a WrapperBase
//struct HyperNumber
//{
//  std::shared_ptr<WrapperBase> thing;
//
//  HyperNumber ();
//
//  template <typename T>
//  HyperNumber (const T &val)
//  {
//    fromValue (val);
//  }
//
//  template <typename T>
//  void fromValue (const T &val)
//  {
//    thing = new Wrapper<T> (val);
//  }
//
//  void fromString (const string &s)
//  {
//    switch (SMap[s])
//      {
//        case CGAType::VEC:
//          thing = std::shared_ptr<WrapperBase> (new Wrapper<Vec> ());
//          break;
//        case CGAType::ROT:
//          thing = std::shared_ptr<WrapperBase> (new Wrapper<Rot> ());
//          break;
//      }
//  }
//
//  void fromMult (WrapperBase *a, WrapperBase *b) { thing = a->mult (b); }
//};
