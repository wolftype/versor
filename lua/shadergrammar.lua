
require "versor"
require "shaderfuncs"


local lpeg = require "lpeg"
local C = lpeg.C 		--Capture
local Ct = lpeg.Ct		--Capture to Table

local R = lpeg.R		--Range
local S = lpeg.S		--Set
local P = lpeg.P		--Pattern
local V = lpeg.V		--Rule

--space, tab, new line, return (zero or more)
local space 	= 	S" \t\n\r"
local zspace 	= 	S" \t\n\r"^0	

--scope
local open 		= "(" * zspace
local close 	= ")" * zspace
local scope 	=  S"()"		

--GA operations, inner outer geo
local product 	= 	S"<^*"
--reversion, inversion, etc
local version   = S"!~"

--product capture
local gop = C( product ) * zspace	

--versions
local vop = C( version ) * zspace

--not a letter
local nonletter = space + scope + version + product 

--one or more consecutive letters (i.e a type)
local word = C( (1 - nonletter )^1 ) * zspace

--0 or more negative signs followed by 1 or more characters in range 0-9
local number = C( P"-"^-1 * R("09")^1 ) * zspace

--word followed by parenthesised expression
--local cop = C() * zspace

-- local Exp, Term, Factor = V"Exp", V"Term", V"Factor"
-- 
-- local grammar = P{
-- 	Exp,
-- 	Exp = Ct(Factor * (gop * Factor)^0);  -- A Factor Followed by One or More Factors
-- 	Factor = Term + Ct(vop^-1 * Term);    -- Versions (reversions, inversions, etc)
-- 	Term = word + (open * Exp * close);   -- Word or Parenthesized Expression
-- } 

local Exp, Term, Factor, Cast, Parenth = V"Exp", V"Term", V"Factor", V"Cast", V"Parenth"

local grammar = P{
	Exp,
	Exp = Ct( Factor * (gop^-1 * Factor)^0 );  			-- One or More Factors
	Factor = Ct(Cast) + Term + Ct( vop^-1 * Term );    	-- Versions (reversions, inversions, etc)
	Term = Cast + word + Parenth;   					-- Cast, Word or Parenthesized Expression
	Cast = word^-1 * Parenth;	  						-- Cast
	Parenth = open * Exp * close;
}

grammar = zspace * grammar * -1



-----------------------------------------------

local parse = function (x)
	local res = grammar:match(x)
	return res
end


local printcast = function(k, cast, op)
	
	local out = " (\n"
	local sep = ","
	
	local c = castList( cast[2], cast[3] )

	if op[1] == "gp" then 

		local tx = productList( op[2], op[3], op[1] )
		
		--get token at idx
		for i, iv in ipairs (c) do
			local token = tx [ iv[2] + 1 ]
			for j, jv in pairs(token) do
				out = out .. makeShaderPToken(op[2], op[3], jv, "t"..op[4], "t"..op[5] ) 
				if i < #c  then out = out .. sep end
				out = out .. "\n"
			end
		end
		
	end

	out = out .. ");"
	
	return out
	
end

--pass in list
local assemble = function(tmp)

	local out = ""
	local finished = false
	
	for k, kv in ipairs(tmp) do

		local iter = k
		local head = ""
		local typ = ""
		local var = ""
		
		local constructor = false
		
		local nextop = {}
		
		if k < #tmp then
			nextop = tmp[k+1]
		end
		
		--IF we are casting to a type, get type
		if  nextop[1] == "cast" then
		
			typ = nextop[2].id 
			head = " t" .. (k+1) --nextop[2].id
			var =  printcast(k, nextop, kv)
			
			iter = iter + 1
		
		else

			if kv[1] == "arg" then 
				var = makeShaderAType( kv[2], kv[2].id:lower() , "t"..k ) 
			end
			
			if kv[1] == "reverse" then
				 typ = kv[2].id
				 head = " t"..k.." = " 
				 var = kv[1].."( t".. (k-1) ..");"--makeVType( versionList( kv[2], kv[1] ), "t"..(k-1) )  
			end
			
			--if product
			if (kv[1] == "gp") or (kv[1] == "op") or (kv[1] == "ip") then 
				
				constructor = true
				
				local pl =  productList( kv[2], kv[3], kv[1])
				local ty = getProductListType( pl )
				
				typ = ty.id
				if typ == "Muv" then typ = "Vec"..#ty.bases end
				head = " t"..k.." "
				var = makeShaderPType( pl, kv[2], kv[3], "t"..( kv[4] ), "t" .. ( kv[5] ) ) .. ";" 
			end
			
			--if casting operation, increment (skip)
			if kv[1] == "cast" then
				--k = k + 1
				--head = " t"..(k-1)
				--iter = iter + 1
			end
		
		end
		
		if finished == false then
			if iter == #tmp then 
				finished = true
				head = "return " .. typ
			else
				head = typ .. head
				if constructor then
					head = head .. " = " .. typ
				end
			end
		end
		
		out = out .. "\n" .. head  .. var
	end	
	
	return out
end


local tassemble = function(tmp)
	for k, kv in ipairs(tmp) do
		
	end
end
--prodco = coroutine.create( function(a, b) print("co") end )
--GA evaluator (use with coroutines?)


GenEx = {
	
	init = function(self) 

		self.list = {}

		self.varname = coroutine.create(

		function()
			local it = 1
			while true do
				local a = coroutine.yield(it-1) -- yield varnum back 
				self.list[it] = a
				it = it + 1
			end
		end
		)

		coroutine.resume(self.varname) 
	
	end,

	disp = function(self)  print("disp") for i, iv in ipairs(self.list) do print(i) end end, 
	
	list = {},

	varname,

	evalGA = function(self,x)
	
		local tvar
		
		if type(x) == "string" then	
			local func = {"arg", _G[x] }
					local bool, iter = coroutine.resume(self.varname, func)
			return _G[x], iter
		
		else --is a table

		
				--Two element operation (cast or inversion, involution, reversion)
				if ( #x == 2 ) then --type(x[1]) == "string" ) then
				
					if ( vop:match(x[1]) ) then
						

						local lh, ovar = self:evalGA(x[2])
						if (x[1]=="~") then 
							--local tmp = versionList(lh, "reverse")
					
							local func = {"reverse",lh}

							local bool, iter = coroutine.resume(self.varname, func)
							return lh, iter 
						end
				
						if (x[1]=="!") then 
							--local tmp = versionList(lh, "reverse")

							local func = {"reverse",lh}

							local bool, iter = coroutine.resume(self.varname, func)
							return lh, iter 
						end
			
					else 
						if ( word:match( x[1] ) ) then
					
							local rh, rhid = self:evalGA( x[2] )
							local func = {"cast", _G[ x[1] ], rh, rhid}
							local bool, iter = coroutine.resume(self.varname, func)
							return _G[ x[1] ], iter

						end
					end
			
				else
			
					local lh, lvar = self:evalGA(x[1])	
					
					if type(x[2]) ~= "table" then
											
						local bool, tlvar

						for i = 2, #x, 2 do
									
							tlvar = lvar
								
					 		local op = x[i]
							local rh, rvar = self:evalGA(x [i+1])
							--print (rh.id)
				
							local top = ""
				
							if (op == "^") 		then  top = "op"
							elseif (op == "*") 	then  top = "gp" 
							elseif (op == "<")  then  top = "ip" 
							elseif (op == "-") then  top = "sub"
							elseif (op == "+") then  top = "add"
							end			
				
							local func = {top, lh, rh, tlvar, rvar}
							bool, lvar = coroutine.resume(self.varname, func)	
								
							local tmp = productList(lh, rh, top)
							lh = getProductListType(tmp)
						end				
					end
					return lh, lvar
				end
		end
	end
}

local function ctPrint(x)
	if type(x) == "table" then
		print("\tTABLE num of Terms:",#x,"\t")
		print(">>>")
		for i, iv in ipairs(x) do
			print ("Term: ", i)
			ctPrint(iv)		
		end
		print("\t")
	else
		
		print(x)
		
	end
end



function makeExp(tmp)

	local c = parse(tmp) 
	
	GenEx:init()
	GenEx:evalGA( c )
	
	return assemble ( GenEx.list )
	
end


getExp = function(tmp)

	local c = parse(tmp) 

	GenEx:init()
	GenEx:evalGA( c )

	local r = GenEx.list[#GenEx.list]
	if r[1] == "cast" then return r[2] end
	if r[1] == "arg" then return r[2] end
	if r[1] == "reverse" then return r[2] end
	if (r[1] == "gp" or r[1] == "op" or r[1] == "ip" ) then
		local pl =  productList( r[2], r[3], r[1])
		return getProductListType( pl )		
	end
end


-- local out =  "Vec ( Vec * Biv ) * Vec (Biv * Vec) "
local exp =  "Rot * Vec * !Rot"

