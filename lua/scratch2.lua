--local C3 = require "C3" 
local V = require "versor"

local R4 = V{ 
  metric={1,1,1,1},
  types = {
  { name= "Vec3", bases={"e1","e2","e3"} },
  { name= "VecR4", bases={"e1","e2","e3","e4"} },
  { name= "Biv", bases={"e14","e24","e34"} },
  { name= "Rot", bases={"s","e14","e24","e34"} },
  }
} 

local pd = R4:getProductList("VecR4","Rot","gp");
R4:createType(pd.blades,"Vec_Rot",true);

local t = R4.types["vec_rot"]

local S = require "shaderfuncs"  

local T = S{ algebra=R4 }

local vec4 = T:TypeGen( "VecR4" );
local biv = T:TypeGen( "Biv" )  
local rot = T:TypeGen( "Rot" )  
local vecrot = T:TypeGen( "Vec_Rot" );

print(vec4) 
print(biv)
print(rot)
print(vecrot) 
 
local prodA = T:ProdGen( "VecR4", "Rot", "gp")
print(prodA)

print("//REVERSE OF ROT:")
print("Rot reverse(Rot r){")
local reverse = T:VType( "Rot", "reverse")
print(reverse)

print("//GP OF VEC_ROT * ROT:")
print("VecR4 gp(Vec_Rot v, Rot r){")
local prodB = T:ProdGen( "Vec_Rot", "Rot", "gp")          
print(prodB)


--local
--print("REVERSE OF ROT:")
--local reverse = T:VType( "Bst", "reverse")

-- print(type)
-- print(prod)   
