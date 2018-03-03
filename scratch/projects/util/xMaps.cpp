/*
 * =====================================================================================
 *
 *       Filename:  xMaps.cpp
 *
 *    Description:  just what is going on with maps
 *
 *        Version:  1.0
 *        Created:  02/11/2014 19:01:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <map>
#include <stdio.h>

#include "vsr_set.h"

using vsr::Set;

using namespace std;

struct something{
  something(){}
  const char * name = "hello";
};

map< int, something*> mymap;

std::vector< something * > set;

void fun(){

  for (int i = 0; i < 100; ++i){
    set.push_back( new something() );
    mymap.insert( pair< int, something*>(i, set[set.size()-1] )  );
  }
}

int main(){
  
  fun();

  for (auto& i : mymap){
    auto& d = i.second;
    printf("%s\n",d -> name);
  }
}
