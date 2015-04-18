--[[
--
--    VERSOR FOR CUDA CODE GENERATOR
--
--
--    currently generates THRUST code
--]]


require "cosmo" 
local V = require "C3"

local M = {}
M.__index = M

setmetatable(M, {

	  __call = function(_, t)
	
  	  t = t or {}
		
      local m = setmetatable({}, M)

      return m
  
    end
  }
)

--GENERATE TYPE
function M:type(iv)

  local template = [[ $make_type[=[
     struct $name {
      typedef thrust::tuple<$vars> $name;
     
      template<int N>
      thrust::device_vector<float> result(N);
     }
    ]=]
  ]]
	
	local code = cosmo.f(template){
		
	  make_type = function()
		
    		local out = ""
			
				for k, kv in ipairs(iv.bases) do
					local id = V:basisString(kv)
					out = out .. "float" 
					if k < #iv.bases then out = out .."," end
				end
				
				cosmo.yield{
					name = iv.name,
					vars = out,
				}
		
    end		
	}
	
	return code
end        

--GENERATE FUNCTION
function M:product(a,b,op)
  local template = [[ $make_product[=[
    struct $func : public thrust::binary_function<$ta, $tb, $rt>
    {
      __host__ __device__
        $rt operator()( const $ta& a, const $tb& b) const
        {
          return $do_combo;
        }
  ]=]
  ]]
end




local type = V:getProductType("Pnt", "Pnt", "gp")   
local bst = M:type(type)
print(bst)

return M

