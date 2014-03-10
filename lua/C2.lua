--[[

EXAMPLE USE C2   

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
--print(p1, p2, p1^p2)

local t = C2:productList( C2.types["Vec2"].bases, C2.types["Vec2"].bases, "ip" )

print(#t.blades)

--for i, iv in pairs(  C2.types["Vec2"].bases ) do print(i,iv) end

for i=1, #t.blades do 
  for k, kv in pairs(t.inst[ t.blades[i] ]) do
   print( "["..kv.a.."] * [" .. kv.b.."]" )  
  end 
end

