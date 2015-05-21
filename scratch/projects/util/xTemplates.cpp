/*
 * =====================================================================================
 *
 *       Filename:  xTemplates.cpp
 *
 *    Description:  template experiments
 *
 *
 *        Version:  1.0
 *        Created:  03/10/2014 13:19:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#define CA constexpr auto
#define SCA static constexpr auto  
#define SC static constexpr  
#define RETURNS(X) ->decltype(X){return X;}




template<int IDX>
struct Get{  
  template<typename T>
  SCA Call( const T& a ) RETURNS ( Get<IDX-1>::Call(a.TAIL) )
};

template<>
struct Get<0>{
  template<typename T>
  SCA Call( const T& a) RETURNS ( a )
};



template<typename X, typename ... XS >
struct MVInstance{
  
  static const int NUM = sizeof...(XS)+1;

  constexpr MVInstance(){}

  X HEAD;
  MVInstance<XS...> TAIL;
  

  template<int IDX>
  auto get() RETURNS(
    Get<IDX>( *this )
  )


};


int main(){


}

