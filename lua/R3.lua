--[[

EXAMPLE USE R3   

]]


local V = require"versor"
local C2 = V{
	metric={1, 1, 1, -1},
	types = {
		{ name="Vec2", bases={"e1", "e2"} },
		{ name="Biv2", bases={"e12"} },
		{ name="Pss", bases={"e1234"} },
		{ name="Rot", bases={"s", "e12"} },
		{ name="Pnt", bases={"e1", "e2", "e3", "e4"}, dual=true },
		{ name="Par", bases={"e12", "e13", "e14", "e23", "e24", "e34"} },
		{ name="Dll", bases={"e1", "e2", "e4"}, dual=true },
		{ name="Lin", bases={"e134", "e234", "e124"} },
		{ name="Flp", bases={"e14", "e24", "e34"} },
		{ name="Drv", bases={"e14", "e24"} },
		{ name="Tnv", bases={"e13", "e23"} },
		{ name="Dil", bases={"s", "e34"} },
		{ name="Trs", bases={"s", "e14", "e24"} },
		{ name="Mot", bases={"s", "e12", "e14", "e24"} },
		{ name="Bst", bases={"s", "e12", "e13", "e14", "e23", "e24", "e34"} },
	},
	conformal=true
}

function point(x, y)
	return C2.Pnt(x, y, 1, (x*x+y*y)*0.5)
end

local p1 = point(0, 0)
local p2 = point(1, 0)
print(p1, p2, p1^p2)

--[[
print(table.concat(E3.basis, " "))
for k, v in pairs(E3.types) do
	print(k)
	print("\tbases:", table.concat(v.bases, " "))
	print("\tdual:", v.dual)
	print("\tgenerated:", v.generated)
	print("\tkey:", table.concat(v.key, " "))
	print("\tname:", v.name)
	print""
end
--]]

  
  --[[
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


                                  


--]]