require "cosmo" 
local V = require "C3"

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

			--local out = ""
			--for i, iv in ipairs(myTypes) do
				local out = ""
			
				for k, kv in ipairs(iv.bases) do
					local id = V:basisString(kv)
					out = out .. string.gsub( id, "_","")
					-- 	print(out)
					if k < #iv.bases then out = out .."," end
				end
				
				local cargs = ""
				local cinit = ""
				for k, kv in ipairs(iv.bases) do  
					local id = V:basisString(kv) 
					cargs = cargs .. "float "..id
					if k < #iv.bases then cargs = cargs .. "," end
					cinit = cinit .. id:gsub("_","").."("..id..")"
					if k < #iv.bases then cinit = cinit .. "," end
				end
				cosmo.yield{
					name = iv.name,
					vars = out,
					--for c++ testing only:
					--args =  cargs ,
					--init = cinit
				}
			
			--end
		
		end		
	}
	
	return code

end        

local type = V:getProductType("Pnt", "Pnt", "gp")   
local bst = typeGen(type)
print(bst)