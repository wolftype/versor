--[[

EXAMPLE USE R3   

]]


local V = require "versor"
  
local t = V.GA:new(1,1,1)     	--note: alt V.GA:new(1,1,1,1,-1)  for CGA3D
t:buildEuclidean()   			--note: alt use buildConformal()  for CGA3D


local vec = { id = "Vec", desc = "Vector", bases = {"e1","e2","e3"} }    
local rot = { id = "Rot", desc = "Rotor", bases = {"s", "e12","e13","e23"} }  

--more types etc...

--Convert strings to ints and register type (could make this a method of GA)
vec.blades = V.toBasisList( vec.bases )   --Convert
t:makeType( vec.blades, vec.id, false )   --Register

rot.blades = V.toBasisList( rot.bases )   --Convert
t:makeType( rot.blades, rot.id, false )   --Register
   
--ACTUAL INSTANCES  
local v = t.types["Vec"](1,0,0) 
local r = t.types["Rot"](1,1,1,1)     


--Operate!  the Metatable should call into the GA mvProduct, but having trouble accessing members correctly    
local c = r * v * r 			--note, r.inverse() etc still needs to be defined


                                  

