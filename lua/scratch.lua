
local C3 = require "C3" 
local V = require "versor"  
require "cosmo"
 
local type = C3:getProductType("Pnt", "Pnt", "gp")  
        
print(type.name)     
                        

--WRITE BASIS BLADE from INPUTS a AND b
local makeShaderPToken = function(sx, sy, tx, n1, n2)
 	local out = ""      
	--local iter = 0
	for k, kv in pairs(tx) do   
	  --print(kv.a)
	   --iter = iter+1
	   if kv.r.w == -1 then out = out .. " - " 
	   	else if k ~= 1 then out = out .. " + " end
	   end  
	   sa = C3:basisString( sx.bases[kv.a] )
	   sb = C3:basisString( sy.bases[kv.b] )
	   out = out ..n1.."."..sa.." * "..n2.."."..sb     
	end
	return out
end

--unrolled type from productList
local makeShaderPType = function(tx, sx, sy, n1, n2 )

	 local res = C3:getProductListType(tx)   
	 --print(res)
	 local out = "(\n"
	 local sep = ","	
	
	--for each result blade  
	local iter = 0
	for i, iv in pairs(tx.inst) do    
		iter = iter +1
		--for j, jv in pairs(iv) do   		
			out = out .. makeShaderPToken(sx, sy, iv, n1, n2)
			if ( iter < #tx.blades ) then out = out .. sep end
	    	out = out .."\n"					
		--end	  
	end   
	
	return out .. ")"
	
end

--GENERATE typedef
local typeGen = function(iv)

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
				local id = C3:basisString(kv)
				out = out .. string.gsub( id, "_","")
				-- 	print(out)
				if k < #iv.bases then out = out .."," end
			end
		
			local cargs = ""
			local cinit = ""
			for k, kv in ipairs(iv.bases) do  
				local id = C3:basisString(kv) 
				cargs = cargs .. "float "..id
				if k < #iv.bases then cargs = cargs .. "," end
				cinit = cinit .. id:gsub("_","").."("..id..")"
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


--GEOMETRIC OPERATIONS
local productGen = function(ttx, tty, top)  
	
	local tx = C3.types[ttx]
	local ty = C3.types[tty]

	local list = C3:getProductList(ttx, tty, top)
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
					fname = top,-- .."<"..tx.id..","..ty.id..","..typ.id..">",
					lhand = ttx,
					rhand = tty,
					rcombo = makeShaderPType( list, tx,ty, "a", "b" )
				}
			
			end
		}
	
		return code
	end

	return ""
end
     
local bst = productGen("Pnt", "Pnt","gp")
print(bst)


