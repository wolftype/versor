--[[

EXAMPLE USE R3   

]]


local V = require "versor"
  
local t = V.GA:new(1,1,1)     	--note: alt V.GA:new(1,1,1,1,-1)  for CGA3D
t:buildEuclidean()   			--note: alt use buildConformal()  for CGA3D


local vec = { id = "Vec", desc = "Vector", bases = {"e1","e2","e3"} }    
local rot = { id = "Rot", desc = "Rotor", bases = {"s", "e12","e13","e23"} }  

--more types etc...

--Convert strings to ints and register type
vec.blades = B.toBasisList( vec.bases )   
t:makeType( vec.blades, vec.id, false )   

rot.blades = B.toBasisList( rot.bases ) 
t:makeType( rot.blades, rot.id, false )  
   
--ACTUAL INSTANCES  
local v = t.types["Vec"](1,0,0) 
local r = t.types["Rot"](1,1,1,1)     


--Operate!  the Metatable should call into the GA mvProduct, but having trouble accessing it correctly
local c = r * v * r 			--note, r.inverse() etc still needs to be defined


                                  

