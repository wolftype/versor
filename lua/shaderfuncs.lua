--GENERATE GLSL SHADER CODE FROM AN ARBITRARY GEOMETRIC ALGEBRA

require "cosmo"
local V = require "versor"   
 
local M = {}
M.__index = M

setmetatable(M, {
	__call = function(_, t)   
        local m = setmetatable({}, M)    
		m.algebra = t.algebra 
		return m    
	end
	 }
 )
    
--GENERATE a MULTIVECTOR TYPE (struct with floats on a basis set)  
-- e.g:
--
-- struct Vec{
--		float e1, e2, e3;	
--}  
--
function M:TypeGen(type) 
	
	local iv = self.algebra.types[type]

	local template = [[ $make_type[=[
	struct $name {
		float $vars;
	};
	]=]
	]]
	
	local code = cosmo.f(template){
		
		make_type = function()

			local out = ""
	
			for k, kv in ipairs(iv.bases) do
				local id = self.algebra:basisString(kv)
				out = out .. id--string.gsub( id, "_","")
				if k < #iv.bases then out = out .."," end
			end
		
			local cargs = ""
			local cinit = ""
			for k, kv in ipairs(iv.bases) do  
				local id = self.algebra:basisString(kv) 
				cargs = cargs .. "float "..id
				if k < #iv.bases then cargs = cargs .. "," end
				cinit = cinit .. id .."("..id..")"
				if k < #iv.bases then cinit = cinit .. "," end
			end
			cosmo.yield{
				name = iv.name,
				vars = out,

			}
		
		end		
	}
	
	return code

end  



--unrolled type from argument
function M:AType(tx, id, td)
	return ""..tx.name .. " "..td.." = "..id..";"
end


--WRITE BASIS BLADE from INPUTS a AND b
function M:PToken(sx, sy, tx, n1, n2)
 	local out = ""      
	--local iter = 0
	for k, kv in pairs(tx) do   
	   if kv.r.w == -1 then out = out .. " - " 
	   	else if k ~= 1 then out = out .. " + " end
	   end  
	   sa = self.algebra:basisString( sx.bases[kv.a] )
	   sb = self.algebra:basisString( sy.bases[kv.b] )
	   out = out ..n1.."."..sa.." * "..n2.."."..sb     
	end
	return out
end

--unrolled type from productList
function M:PType(tx, sx, sy, n1, n2 )

	 local res = self.algebra:getProductListType(tx)   
	 --print(res)
	 local out = "(\n"
	 local sep = ","	
	
	--for each result blade  
	local iter = 0
	for i, iv in pairs(tx.inst) do    
		iter = iter +1		
		out = out .. self:PToken(sx, sy, iv, n1, n2)
		if ( iter < #tx.blades ) then out = out .. sep end
	    out = out .."\n"					    
	end   
	
	return out .. ")"
	
end
        
function M:ProdGen(tx, ty, op)                 
	local lh = self.algebra.types[tx]
	local rh = self.algebra.types[ty]
    local list = self.algebra:productList(lh.bases,rh.bases,op)
 	return self:PType(list, lh, rh, "a","b")
end


-- M:VToken = function (tx)
-- 	out = ""
-- 	for k, kv in ipairs (tx) do
-- 		if kv.w == -1 then out = out .. "-" end
-- 		out = out .. "a[" .. k-1 .. "]"
-- 	end
-- 	return out
-- end 


--unrolled type from versionList
function M:VType(ttx, opname, lhid) 
	local lhid = lhid or "a"
	local tx = self.algebra.types[ttx]
    local res = self.algebra:versionList(ttx,opname)
	out = "("
	local sep = ","	
	
	for i=1, #tx.bases  do
		if res.inst[i] then out = out .. "-" end
		out = out .. lhid.."." .. self.algebra:basisString(tx.bases[i])
		if ( i < #tx.bases ) then out = out .. sep end
	end
	out = out .. ")"
	return out  	
end 
-- 
-- --casting
-- M:CType = function (tx, rhid)
-- 
-- 	local out = "("
-- 	local sep = ","	
-- 	for i, iv in ipairs(tx) do
--  	    --out = out .. rhid .. "."..tx.bases[iv[2]].id:gsub("_","")
-- 		if ( i < #tx ) then out = out .. sep end
-- 	end
-- 	out = out .. ")"	
-- 	return out	
-- end
-- 
-- M:SType = function(tx, lhid, rhid)
-- 	local out = "("
-- 	local sep = ","	
-- 	for i, iv in ipairs(tx) do
-- 		out = out .. lhid .. "["..iv[1].."] ".. iv[3] .. rhid .. "["..iv[2].."]"
-- 		if ( i < #tx ) then out = out .. sep end
-- 	end
-- 	out = out .. ")"	
-- return out
-- end            

return M