--[[

very very close: 
1 - casting
2 - lin from cir, etc,
3 - not all spinor operations return a "sure" return type -- can we predetermine?
maybe a set of equivalences (dll -> par)

some returns cast in a way such that the intermediate types are not all necessary . . .
...unroll future uses and only calculate the necessary ones?  

]]

--SET SEARCH PATHS FOR LUA REQUIRES
--package.path = "../?.lua;" .. package.path

local C3 = require "C3"
require "cosmo"
require "shadergrammar"

--SET FILE WRITE PATH
local env = os.getenv("HOME")
local path = env.."/code/versor/branches/subspace/lua/staticCGA/gen/shaders"
local cpath = env.."/code/versor/branches/subspace/lua/staticCGA/gen/vsr/shaders"
local prefix = ""

--Table insertion
local AddTab = function(x,y)
	for i, iv in ipairs(y) do
		table.insert(x,iv)
	
	end
end
--DEFINE OPERATORS
local operators = {
	op = "op",--"operator ^", 
	ip = "ip",--"operator <=", 
	gp = "gp",--"operator *",
}

local EUStates = {Sca, Vec, Biv, Tri}
local EUBivectors = {Biv}
local EUVersors = {Rot}
local EUInter = {Vec_Rot}

local CGAStates = States
local CGAVersors = Versors
local CGAGens = {Biv, Drv, Tnv, Dll, Par}  

local CGATypes = AllStates 


local AllEUTypes = {}
AddTab(AllEUTypes,EUStates)
AddTab(AllEUTypes,EUVersors)
AddTab(AllEUTypes,EUInter)

--SHADER TEST
local myStates =  {Mtt, Vec_Biv, Vec, Rtc, Tri, Mot, Biv, Pnt_Pnt,Dlp, Drb, Sca, Inf, Pss,  Sph, Drv, Pnt, Dll, Lin, Cir, Par }--{Sca, Inf, Tnv, Tvt, Rtc, Pnt_Par, Mtt, Par, Tnv_Flp, Pnt, Pnt_Pnt, Sph, Cir, Dll, Lin}--{Inf, Pss, Drv, Tnv, Pnt, Par, Cir, Sph, Dll, Lin, Dlp}
local myVersors = {Rot, Trs, Mot, Dil, Trv}


local myTypes = {}
AddTab(myTypes, myStates)
AddTab(myTypes, myVersors)

--CONFORMAL
--local myStates = CGAStates
-- local myVersors = CGAVersors
-- local myTypes = CGATypes

local Dualize = { Drb, Pnt, Par, Cir, Sph, Dll, Lin, Dlp, Pln }
local Reverse = { Biv, Pss, Pnt_Pnt, Rtc }
local Inverse = { Pss, Pnt, Biv }
local Involute = { Pnt, Par, Biv }
local XForm  = { {Cir, Mot, "sp"} }--{ {Tnv, Trs, "sp1", Par}, {Pnt, Pnt_Pnt,"sp"} }-- Par, Cir, Sph, Dll, Lin, Dlp, Pln}
local GPTypes = { {Mtt, Pln_Pln}, {Vec, Biv}, {Biv, Vec}, {Biv, Inf}, {Pnt, Pnt},  {Tri, Biv} }--{ {Pnt, Sca}, {Trs, Sca}, {Pnt_Pnt, Sca}, {Pnt, Pnt}, {Pnt, Pss}, {Par, Pss}, {Cir, Pss}, {Sph, Pss}, {Lin, Pss}, {Dll, Pss} }
local OPTypes ={ {Vec, Biv}, {Dlp, Pnt}, {Par, Pnt}, {Dll, Par}, {Pnt, Drv}, {Pnt, Pnt}, {Par, Inf} }--{ {Pnt, Pnt}, {Pnt, Drv}, {Pnt, Par}, {Par, Inf} }
local IPTypes ={ {Vec, Biv}, {Pnt, Drv}, {Pnt, Pnt}, {Inf, Pnt}, {Par, Par},{Inf, Sph}}--{ {Pnt, Par}, {Pnt, Cir}, {Pnt, Sph} }
local SumTypes = { Dll }
local CastTypes = {}


--USEFUL FUNCS
local up = function(x)
	return string.upper(x)
end

local util = function()

	local code = [[
	
	//Point from Vector
	Pnt null(vec3 s){
	    return Pnt ( s[0], s[1], s[2], 1.0, dot(s,s) / 2.0 );
	}
	
	//Line from Eye and Direction
	Lin ray(vec3 eye, vec3 dir){
		return op( op( null(eye), Drv( dir.x, dir.y, dir.z ) ), Inf(1) );
	}
	
	
	]]
	return code;
end

--GENERATE typedef
local typeGen = function()
--$name ($args) : $init {}

	local template = [[ $make_type[=[
	struct $name {
		float $vars;
	};
	]=]
	]]
	
	local code = cosmo.f(template){
		
		make_type = function()

			--local out = ""
			for i, iv in ipairs(myTypes) do
				local out = ""
			
				for k, kv in ipairs(iv.bases) do
				out = out .. string.gsub(kv.id, "_","")
				-- 	print(out)
				if k < #iv.bases then out = out .."," end
				end
				
				local cargs = ""
				local cinit = ""
				for k, kv in ipairs(iv.bases) do
					cargs = cargs .. "float "..kv.id
					if k < #iv.bases then cargs = cargs .. "," end
					cinit = cinit .. kv.id:gsub("_","").."("..kv.id..")"
					if k < #iv.bases then cinit = cinit .. "," end
				end
				cosmo.yield{
					name = iv.id,
					vars = out,
					--for c++ testing only:
					--args =  cargs ,
					--init = cinit
				}
			
			end
		
		end		
	}
	
	return code

end

local sumGen = function(tx)
	local template = [[ $make_function[=[
	$rtype sum($rtype a, $rtype b){
		return $rtype( $output );
	}
	]=]
	]]
	local code = cosmo.f(template){
		make_function = function()
			sum = ""
			for i, iv in ipairs(tx.bases) do
				tid = iv.id:gsub("_","")
				sum = sum .. "a."..tid.." + b."..tid
				if i < #tx.bases then sum = sum .."," end 
			end
			cosmo.yield{
				rtype = tx.id,
				output = sum
			}
		end
	}
	
	return code
end
--GEOMETRIC OPERATIONS
local productGen = function(tx, ty, top)

	local list = C3:productList(tx, ty, top)
	local typ =  C3:getProductListType(list)
	
	if ( typ.name ~= "Muv" and typ.name ~= "_N" ) then
				
		local template = [[ $make_function[=[
 $rtype $fname ($lhand a, $rhand b) { 
	return $rtype $rcombo;
}			]=]
		]]

		local code = cosmo.f ( template ) {	
			make_function = function()
			
				cosmo.yield{
					
					rtype = typ.name,
					fname = operators[top],-- .."<"..tx.id..","..ty.id..","..typ.id..">",
					lhand = tx.name,
					rhand = ty.name,
					rcombo = makeShaderPType( list, tx,ty, "a", "b" )
				}
			
			end
		}
	
		return code
	end

	return ""
end

--INVERSIONS, REVERSIONS, INVOLUTIONS
local versionGen = function(tx, op)

	local list = C3:versionList(tx, op)
	local typ =  C3:getProductListType(list)
	
	if ( type(typ) == "table" ) then
				
		local template = [[ 
		$make_function[=[
		
$lhand $fname ($lhand a) { 
	return $lhand $rcombo;
}
			]=]
		]]

		local code = cosmo.f ( template ) {	
			make_function = function()
			
				cosmo.yield{
					fname = op,
					lhand = tx.name,
					rcombo = makeShaderVType(list, "a")
				}
			
			end
		}
	
		return code
	end

	return ""
end


--VERSOR APPLICATION op is "re" or "sp"; ct is the optional force-cast-to type
local versorGen = function(tx, ty, op, ct2)
	
	local exp = ""
	if op == "sp" then exp = tx.id .. "(" ..ty.id .. " * ".. tx.id .. " * " .. " !" .. ty.id..")" end
	if op == "sp0" then exp = ty.id .. " * ".. tx.id .. " * " .. " !" .. ty.id end
	if op == "sp1" then exp = ct2.id .. "(" ..ty.id .. " * ".. tx.id .. " * " .. " !" .. ty.id..")" end
	if op == "re" then exp = ty.id .. " * ".. tx.id .. " * " .. " !" .. ty.id end
	
	local ct = getExp(exp)
		
	local template = [[ $make_function[=[ 	
$rtype $fname ($obj $arga, $gen $argb) {					
					$assemble
}
		]=]
	
	]]
	
	local code = cosmo.f(template){
		make_function = function()
			cosmo.yield{
				rtype = ct.id,
				fname = op,
				obj = tx.id,
				gen = ty.id,
				arga = tx.id:lower(),
				argb = ty.id:lower(),
				assemble = makeExp(exp)
			}
		end
	}
	
	return code
end

local makeCGAShader = function()
	
	print( typeGen() )
	
	for i, iv in ipairs(OPTypes) do

		-- local tmp = getProductType(iv,Pss,"gp")		
		-- if checkMembership(tmp, myTypes) then
		-- 	print (productGen(iv, Pss, "gp"))
		-- end 
		print(versionGen(iv,"inverse"))
		
		for k, kv in ipairs(OPTypes) do
			local tmp = getProductType(iv,kv,"op")
			if checkMembership(tmp, myTypes) then
				print (productGen(iv, kv, "op") )
			end
			local tmp = getProductType(iv,kv,"ip")
			if checkMembership(tmp, myTypes) then
				print (productGen(iv, kv, "ip") )
			end
			local tmp = getProductType(iv,kv,"gp")
			if checkMembership(tmp, myTypes) then
				print (productGen(iv, kv, "gp") )
			end
		end
		
		for k, kv in ipairs(myVersors) do
			print (versorGen(iv, kv, "sp"))
		end
	end
	
	print( util() )
end

local makeNullPointXForms = function()
	print (typeGen())
	--Geometric Products
	for i, iv in ipairs(GPTypes) do
		local tmp = getProductType(iv[1],iv[2],"gp")
		if checkMembership(tmp, myTypes)  then
			print (productGen(iv[1], iv[2], "gp") )
		else
			print ("// NOT FOUND: "..tmp.id)
		end
	end
	--DUALIZATION
	for i, iv in ipairs(Dualize) do
		print(productGen(iv, Pss, "gp"))
	end
	--REVERSION
	print("//REVERSION")
	for i, iv in ipairs(XForm) do
		print(versionGen(iv[2], "reverse"))
	end
	for i, iv in ipairs(Reverse) do
		print (versionGen(iv, "reverse"))
	end
	--INVERSE
	--INVOLUTION
	for i, iv in ipairs(Involute) do
		print (versionGen(iv, "involute"))
	end
	--OUTERPRODUCTS
	for i, iv in ipairs(OPTypes) do
		local tmp = getProductType(iv[1],iv[2],"op")
		if checkMembership(tmp, myTypes)  then
			print (productGen(iv[1], iv[2], "op") )
		else
			print ("// NOT FOUND: "..tmp.id)
		end
	end
	
	--INNERPRODUCTS
	for i, iv in ipairs(IPTypes) do
		local tmp = getProductType(iv[1],iv[2],"ip")
		if checkMembership(tmp, myTypes) then
			print (productGen(iv[1], iv[2], "ip") )
		else
			print ("// NOT FOUND: "..tmp.id)
		end
	end
		
	for i, iv in ipairs(XForm) do
			local tmp = getProductType(iv[1],iv[2],"gp")
			if checkMembership(tmp, myTypes) ~= true then
				print ("// NEED TO GENERATE " .. tmp.id )
			end
			if iv[3] == "sp1" then 
				print (versorGen(iv[1],iv[2],iv[3],iv[4]) )	
			else
				print (versorGen(iv[1],iv[2],iv[3]) )	
			end
			local tmp = getProductType(iv[2],iv[2],"gp")
			if checkMembership(tmp, myTypes) ~= true then
				print ("// NEED TO GENERATE " .. tmp.id )
			end
			print( productGen(iv[2],iv[2],"gp") )
	end
	
	for i, iv in ipairs (SumTypes) do
		print ( sumGen(iv) )
	end
	--CASTING
	-- for i, iv in ipairs(CastTypes) do
	-- 	print (castGen(iv[1], iv[2]))
	-- end

	print(util())
end

makeNullPointXForms()

--makeCGAShader()