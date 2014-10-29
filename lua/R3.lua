local M = require"versor"


local R3 = M{ 
  metric={1,1,1},
  types = {
  { name= "Vec", bases={"e1","e2","e3"} },
  { name= "Biv", bases={"e12","e13","e23"} },
  { name= "Pss", bases={"e123"} },
  { name= "Rot", bases={"s","e12","e13","e23"} },
  }
} 

--BASIS
R3:printBases()

for i, iv in pairs( R3.types ) do
  print(i)
end
--PRODUCT TABLE
--R3:printTable()

R3:printProduct("Vec","Vec","gp")



local C2 = M{ 
  metric={1,1,1,-1},
  types = {
  { name= "Vec", bases={"e1","e2"} },
  { name= "Biv", bases={"e12"} },
  { name= "Pss", bases={"e1234"} },
  { name= "Rot", bases={"s","e12"} },
  },
  conformal = true
} 

