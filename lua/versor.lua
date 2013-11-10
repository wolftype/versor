--Lua Geometric Algebra generator
--Author: Pablo Colapinto
--Email:  wolftype@gmail.com

--[[

Builds Euclidean and Conformal Basis 
Operations In Arbitrary Metric

Space time Algebra, for instance,
is possible by setting metric to
{-1,-1,-1,1} and calling buildEuclidean. 


3D Conformal is build by 

]]

package.path = "util/?.lua;"..package.path
require "routines" 

local bit = require("bit")
local tobit = bit.tobit
local bnot = bit.bnot
local band = bit.band
local bxor = bit.bxor
local bor = bit.bor
local lshift = bit.lshift
local rshift = bit.rshift

module("versor", package.seeall)
 
--BASIS UTILITY FUNCTIONS 
local symidx = {gp=1, op=2, ip=3}  


blade = function(b, wt)
	return {id = b, w = wt}
end 

bitString = function(x,n)  
	local out = ""
	local tmp = x
	for i,iv in ipairs(n or {1,1,1,1,1}) do
		if band(tmp,1) == 1 then out =  "1" ..out
		else out =  "0" .. out end
		tmp = rshift(tmp,1)
	end
	 
	return out
end


basisString = function(x)
	local tmp = x
	local n = 0
	local out = ""
	while tmp ~= 0 do
		n = n+1
		if band(tmp,1) == 1 then out = out .. n end
		tmp = rshift(tmp,1)
	end
	if n > 0 then return "e"..out 
	else return "s" end
end

--CONVERT: string name to bit rep int 
basisBit = function(name)
	if(name == "s") then
		return 0
	end

	local x = 0
	local lastn = tonumber(name:sub(name:len()))
	for i=lastn, 1, -1 do
		x = lshift(x, 1)
		if(name:find(tostring(i), nil, false)) then
			x = x+1
		end
	end
	return x
end

--CONVERT: list of strings "e1" . . . to list of ints
toBasisList = function (b)
	t = {}
   for i, iv in ipairs( b ) do
	   table.insert(t, basisBit(iv))
	end
	return t 
end  

--PRINT FORMAT: pass in a blade and get bit rep, weight, and string format
bladeString = function(x)
	local out = bitString (x.id)
	out = out .. " " .. x.w .. " " .. basisString(x.id)
	return out
end

keyString = function(x)
	local out = ""
	for k, kv in ipairs( keys[ x ] ) do
		if (kv ~= 0 ) then out = out .. k .. "\t" .. kv end
	end
	return out
end



compareBasis = function(a,b)
	local l = grade(a) - 1.0 / a
	local r = grade(b) - 1.0 / b
	return l < r
end


sign = function(a,b)
	local n = rshift(a,1)
	local sum = 0
	while n ~= 0 do
		sum = sum + grade( band(n,b) )
		n = rshift(n,1)
	end
	if band(sum,1) == 0 then return 1
	else return -1 end
end

grade = function (x)
	local n = 0
	while( x ~= 0 ) do
		if ( band(x,1) == 1 ) then n = n + 1 end
		x = rshift(x,1)
	end
	return n
end   

--BASIC PRODUCTS

inner = function(a,b)
	local tmp = product(a,b)
	local g = grade(b) - grade(a)
	if grade(a) > grade(b) or grade(tmp.id) ~= g then 
		return blade(0,0)
	else return tmp end
end

outer = function(a, b)
	if ( band(a,b) ~= 0 ) then return blade(0,0) end
	return product(a,b)
end

product = function(a, b)
	local res = bxor(a,b)
	local s = sign(a,b)
	return blade(res, s)
end 

--HERMITIAN OPERATORS 

involute = function(x)
	local g = grade(x)
	local n = math.pow(-1,g)
	return blade(x,n)
end

reverse = function(x)
	local g = grade(x)
	local n = math.pow( -1, ( g*(g-1) / 2.0 ) )
	return blade(x,n)
end

conjugate = function(x)
	local g = grade(x)
	local n = math.pow( -1, (g * (g+1) / 2.0) )
	return blade(x,n)
end   

--feed in a table of blades -> compress (combine like terms) and clean (remove zero terms)
compress = function(x)
	local tally = {}
	local res = {}
	
	--collect like terms
	for i,iv in ipairs(x) do
		--print (bladeString(iv))
		local exists = 0
		
		for k,kv in ipairs(tally) do
			if iv.id == kv.id then 
			--	print (iv.id, kv.id)
				exists = 1
			--	print (kv.w, iv.w)
				kv.w = kv.w + iv.w -- sum in weight
				--print (kv.w)
			end
		end
		--if doesn't exist, add it
		if (exists == 0) then
			table.insert(tally,iv)
		end		
	end
	
	--remove zeros
	for i,iv in ipairs(tally) do
		if (iv.w ~= 0) then 
		--	print (bladeString(iv))
			table.insert(res, iv ) 
		end
	end
	
	return res
end

order = function(c)
	local res = {}
	local tblades = {}
	for i,iv in pairs(c) do
		table.insert( tblades, i )
	end
	
	table.sort(tblades, compareBasis)

	return { blades = tblades, inst = c }
end


--check similarity of two flags
keyCheck = function(fa,fb)

	local res
	
	for i, iv in ipairs(fa) do
		if ( fa[i] == fb[i] ) then res = true
		else res = false return res end 
	end
	
	return res
end

basisKeys = function(basis)
	local ids = {}
	for i=1, #basis do
		local name = basis[i]
		local id = basisBit(name)
		ids[i] = id
	end
	return ids
end

basisKey = function(basis)
	local ids = basisKeys(basis)
	local keys = keyCalc(ids)
	local v = 0
	for i=1, #keys do
		v = v+keys[i]
	end
	return v
end

flag = function(n) 	
	local numKeys = math.ceil( n / 32 )
	local tmp = {}
	for i=1,numKeys do tmp[i] = 0 end
	return tmp
end 
   
--list of values returned
calcProd = function(ma, mb, inst)

	local out = {}

	local t = 0	
	for i, iv in ipairs(inst) do 
		t = t + 1
		local res = 0
		for j, jv in ipairs(iv) do
			if (jv.r > 0) then res = res + ma[jv.a] * mb[jv.b] 
			else res = res - ma[jv.a] * mb[jv.b] end
		end
		out[t] = res
	end

	return out
end

--GA computes a list of operations
GA = {

  		--Store Basis Combinatorics (a Cayley Graph) 
		new = function(self,...)  
			
		   gax = 	{

			S = {},    			--CAYLEY PRODUCT GRAPH
            numTypes = 0,
			metric = arg,		--METRIC (use {1,1,1} for 3D Euclidean, {1,1,1,1,-1} for 3D Conformal or {-1,-1,-1,1} for Spacetime, etc )
			basis = {},			--BASIS BLADES
			subspace = {}, 		--BASIC SUBSPACE CREATION (optional)
			keys = {}, 			--TYPE KEY STORAGE  (for checking for function existance)
			types = {},			--TYPES 
			instructions = {},  --COMBINATORIC INSTRUCTIONS 

			ORIGIN = 0,          --These are set in the conformal case
			INFINITY = 0,
			EP = 0,
			EM = 0,
			EPLANE = 0, 
			
			--METATABLE OVERLOAD
			MV = { 
				-- desc = "base",
				-- idx = 0,
				__mul = function(a,b) return self:mvProduct(a, b, "gp") end, -- *  (geometric product)
				__pow = function(a,b) return self:mvProduct(a, b, "op") end, -- ^  (outer product)
				__concat  = function(a,b) return self:mvProduct(a, b, "ip") end,  -- .. (inner product)
				__call = function(table,...) return table:new(...) end, 
				
			     print = function(self) for i, iv in ipairs(self) do print(iv) end end
			},

		  } 
		
	       setmetatable(gax, self)
		
		   gax.MV.__index = gax.MV --looks into inherited type for members   
 
			function gax.MV:new(...)
			
				x = {}
				if( type(arg[1]) == "table" ) then --x = arg[1]
					for k,v in ipairs(arg[1]) do x[k] = v end
				else 
					for k,v in ipairs(arg) do x[k] = v end
				end   
			
				setmetatable(x, self)
				self.__index = self
			
				--METAMETHODS must be explicity INHERITED ..
			    local m=getmetatable(self)
			
				if m then
			        for k,v in pairs(m) do
			            if not rawget(self,k) and k:match("^__") then
			                self[k] = m[k]
			            end
			        end
			    end
			
				return x
			end 
		
		

		   return gax 
		end, 
		



	printB = function(self)
		for i,iv in ipairs(self.basis) do
			print( bitString(iv), basisString(iv), keyString(basisString(iv) ) )
		end
	end,


	buildBasis = function(self) 
	    
		print("building basis\n");
		
		m = self.metric

		--make e1, e2, e3, e4 . . .
		table.insert(self.basis,0)
		local nb = 1
		for i,iv in ipairs(m) do
			table.insert(self.basis,nb)
			nb = lshift(nb,1)
		end
	
		--make e12, e23 . . .
		local tally = {}
		for i, iv in ipairs(self.basis) do
			for j, jv in ipairs(self.basis) do
				if (i~=j) then 
					local r = outer(iv,jv)
					if ( r.id ~= 0 ) and ( isMember(r.id, self.basis) == nil )  then 
						table.insert(self.basis, r.id )
					end
				end
			end
		end
	
		table.sort(self.basis, compareBasis)
	
	
		for i, iv in ipairs(self.basis) do

			local f = flag( #self.basis)

			local k = math.ceil( i / 32 );
			--print(k)
			local shft = i - (32 * (k-1) )
			--print(shft)
			f[k] = lshift(1, shft-1) 
			local id = basisString(iv) 
			--print (id, bitString( basisBit(id) ) )
			self.keys[ id  ] = f
	        self.types[ id ] ={ desc = id, idx = i, bases = {iv} } -- {iv} --
		end 
		
		self.numTypes = #self.basis  
		
		-- for  i, iv in ipairs(self.basis) do print(iv) end
		
	   --for i, iv in pairs( self.keys ) do print( i, basisString( self.types[i][1] ) ) end
	
	end,		


	metricProduct = function(self,a,b)   
    	local tmp = product(a,b)
		local bs = band(a,b)
		local i = 1
		while bs ~= 0 do
			if band(bs,1) == 1 then tmp.w = tmp.w * self.metric[i] end
			bs = rshift(bs,1)
			i = i+1
		end
		return tmp
	end,


	metricInner = function(self,a,b)
		local tmp = self:metricProduct(a,b)
		local g = grade(b) - grade(a)
		if grade(a) > grade(b) or grade(tmp.id) ~= g then 
			return blade(0,0)
		else return tmp end
	end,



	--Check For presence of Minkowskian Basis
	checkMink = function(self, x)
		if ( band(x, self.EPLANE) == 0 ) or ( band(x, self.EPLANE) == self.EPLANE ) then 
			return false else return true end
	end,

	--Push into e+.e- Minkowskian diagonal metric from a null basis (for calculating metric products)
	pushMink = function(self, x)	
		if (band(x, self.ORIGIN)==self.ORIGIN) then
			local t = bxor(x, self.ORIGIN)
			-- print( bitString(x),"push origin", bitString(t))
			return { blade( bxor(t,self.EP), .5),  blade( bxor(t,self.EM), .5) }
		elseif (band(x,self.INFINITY)==self.INFINITY) then
			local t = bxor(x, self.INFINITY)
			-- print(bitString(x),"push infinity", bitString(t))
			return { blade( bxor(t,self.EP), -1),  blade( bxor(t,self.EM), 1) }	
		end
	end,

	--Pop back into degenerate null basis from nondegenerate Minkowskian (after xor-ing)
	popMink = function(self, x)
		if (band(x, self.EP)==self.EP) then
			local t = bxor(x, self.EP)
			-- print(bitString(x), "pop ep", bitString(t))
			return { blade( bxor(t,self.ORIGIN), 1),  blade( bxor(t,self.INFINITY), -.5) }
		elseif (band(x,self.EM)==self.EM) then
			local t = bxor(x, self.EM)
			-- print(bitString(x), "pop em", bitString(t))
			return { blade( bxor(t,self.ORIGIN), 1),  blade( bxor(t,self.INFINITY), .5) }	
		end	
	end,


	buildEuclidean = function(self)

		self:prepTable()
	
		for i,iv in ipairs(self.basis) do
			--local idA = basisString(iv)
			for j,jv in ipairs(self.basis) do
			--	local idB = basisString(jv)
				local gp_res = self:metricProduct(iv,jv)
				local op_res = outer( iv,jv) 
				local ip_res = self:metricInner( iv,jv)
			
				self.S[iv].gp[jv] = {gp_res}
				self.S[iv].op[jv] = {op_res}
				self.S[iv].ip[jv] = {ip_res}
				self.S[iv].involute = involute(iv)
				self.S[iv].reverse = reverse(iv)
				self.S[iv].conjugate = conjugate(iv)
			end
		end
		                            
		
		print("Euclidean Basis:")			
		for i,iv in ipairs (self.basis) do
			print (self.S[iv].id )
		end
	end,



	conformal = function(self, iv, jv)
		
		--get list of blades in minkowskian (diagonal) metric
		local tmpA
		if self:checkMink(iv)==true then tmpA = self:pushMink(iv)
		else tmpA = { blade(iv,1) } end
	
		local tmpB
		if self:checkMink(jv)==true then tmpB = self:pushMink(jv)
		else tmpB = { blade(jv,1) } end

		local gpTally = {}
		local opTally = {}
		local ipTally = {}

		local gpPop = {}
		local opPop = {}
		local ipPop = {}
	
		for k,kv in ipairs(tmpA) do
			--print (bladeString(kv))
			for l, lv in ipairs(tmpB) do
			--	print (bladeString(lv))
				--calculate products in mink metric
				local gp_res = self:metricProduct(kv.id,lv.id)
				--print ( bladeString(blade(gp_res.id, gp_res.w * kv.w * lv.w)) )
				local op_res = outer( kv.id,lv.id) 
				local ip_res = self:metricInner( kv.id,lv.id)
				--push onto tally stack
				table.insert(  gpTally, blade(gp_res.id, gp_res.w * kv.w * lv.w) )
				table.insert(  opTally, blade(op_res.id, op_res.w * kv.w * lv.w) )
				table.insert(  ipTally, blade(ip_res.id, ip_res.w * kv.w * lv.w) )
			end
		end
		
		local gpCompress = compress(gpTally)
		local opCompress = compress(opTally)
		local ipCompress = compress(ipTally)

		self.S[iv].ip[jv] = {}
		self.S[iv].op[jv] = {}
		self.S[iv].gp[jv] = {}
		self.S[iv].involute = involute(iv)
		self.S[iv].reverse = reverse(iv)
		self.S[iv].conjugate = conjugate(iv)
	
		for k, kv in ipairs(gpCompress) do		
			local tmp
			if self:checkMink(kv.id)==true then
				tmp = self:popMink(kv.id)
				for m, mv in ipairs(tmp) do
					mv.w = mv.w * kv.w
				end
			else tmp = {kv} end
		
			AddTab(gpPop, tmp)
		end

		for k, kv in ipairs(opCompress) do		
			local tmp
			if self:checkMink(kv.id)==true then
				tmp = self:popMink(kv.id)
				for m, mv in ipairs(tmp) do
					mv.w = mv.w * kv.w
				end
			else tmp = {kv} end
		
			AddTab(opPop, tmp)
		end

		for k, kv in ipairs(ipCompress) do		
			local tmp
			if self:checkMink(kv.id)==true then
				tmp = self:popMink(kv.id)
				for m, mv in ipairs(tmp) do
					mv.w = mv.w * kv.w
				end
			else tmp = {kv} end
		
			AddTab(ipPop, tmp)
		end

	
		AddTab( self.S[iv].gp[jv], compress(gpPop) )
		AddTab( self.S[iv].op[jv], compress(opPop) )
		AddTab( self.S[iv].ip[jv], compress(ipPop) )
		
	end,



	buildConformal = function(self)
	
		self:prepTable()
	
		self.ORIGIN = lshift(1,#self.metric-2)
		self.INFINITY = lshift(1,#self.metric-1)
		self.EP = self.ORIGIN
		self.EM = self.INFINITY
		self.EPLANE = bor(self.ORIGIN,self.INFINITY)

		for i,iv in ipairs(self.basis) do

			--local idA = basisString(iv)
			self.S[iv].involute = involute(iv)
			self.S[iv].reverse = reverse(iv)
			self.S[iv].conjugate = conjugate(iv)
			
			for j, jv in ipairs(self.basis) do
				self:conformal(iv,jv)
			end

		end  
		
		print("Conformal Basis:")			
		for i,iv in ipairs (self.basis) do
			print (self.S[iv].id )
		end
   
	end,



	buildSubspaces = function(self)
	
		local d = { "Vec", "Biv", "Tri", "Quad", "Penta", "Hexa", "Hepta", "Octo"}
	

		for i=1,#self.metric,1 do
			self.subspace[i] = { desc = d[i], bases = {} }
		end

		for i,iv in ipairs(self.basis) do
			local g = grade(iv)
			if g>0 then 
				table.insert(self.subspace[g].bases, iv)
			end
		end
	
	
		--PRINT
		for i,iv in ipairs(self.subspace) do
	
			print("SUBSPACE: "..i)
	
			for j, jv in ipairs(iv.bases) do
				print( bitString(jv), basisString(jv) )
			end
		
		end
	end,


	--INITIALIZE CAYLEY PRODUCT TABLE
	prepTable = function(self)
		for i,iv in ipairs(self.basis) do
			local name = basisString(iv)
			self.S[iv] = { id = name, basis = iv, gp = {}, op = {}, ip = {} }
		end
	end,   
   

	registerSubspaces= function(self)
		print"register Subspaces"
		for i, iv in ipairs( self.subspace ) do
			self.keys[iv.desc] = keyCalc(iv.bases) 
			self.types[iv.desc] = { bases = iv.bases }
			print (iv.desc, keyString(iv.desc) )
		end
		print ("\n")
	end,
	
	--pass in a list of bases
	keyCalc = function(self, x) 
		--print (x[1],  basisString(x[1]))
		local f = flag( #self.basis )
		for i, iv in ipairs(x) do 
		   -- print( basisString(iv), self.keys[ basisString(iv) ] )
			for k, kv in ipairs( self.keys[ basisString(iv) ] ) do
				f[k] = bor( f[k], kv )
			end
		end
		return f
	end,
	
	--pass in a product list's blades to make a new MV type
	makeType = function(self, p, nd, force)  
	   -- print( nd, p[1] )
		local f = self:keyCalc( p )
		local sum = 0	
		for i, iv in ipairs(f) do sum = sum + iv end
		if sum == 0 then return end

		local t = ""

		for i,iv in pairs( self.keys ) do
			if keyCheck(f, iv) then 

				if(force) then
					self:replaceType(i, nd)  
					--self:makeMV( nd )  
					return nd
				else
					return i
				end
			end
		end

			--if function hasn't returned yet, then type is NEW  
			--increment number of types 
			self.numTypes = self.numTypes+1
			
			--add to keys  
		    self.keys[ nd ] = f
			--add to built types
			self.types[ nd ] = {desc = nd, idx = self.numTypes, bases=p}  
			--set type to mv metatable for operators
			self:makeMV( nd )

			return nd
	end,
	
	replaceType = function(self, oty, nty)
		local v = self.keys[oty]
		self.keys[oty] = nil
		self.keys[nty] = v

		local t = types[oty]
		self.types[oty] = nil
		self.types[nty] = t

		for i=1, #self.subspace do
			local subs = self.subspace[i]
			if(subs.desc == oty) then
				subs.desc = nty
				break
			end
		end 
		

	end,
	
	--list of instructions for how to combine tx and ty multivectors
	productList = function(self, tx, ty, operation)

		local tally = {} 		--running tally
		local combined = {}		--compressed tally

		--fetch table pairs of values in types
		idx = 0
		for i,iv in ipairs( tx ) do
            --print (iv)
		    for j,jv in ipairs( ty ) do 
				   		
                --print (iv, jv)
				
				prod = self.S[iv][operation][jv]
				
				for k, kv in ipairs(prod) do
					instruction = {a = i, b = j, ida = basisString(iv), idb = basisString(jv), r = kv}  --indices, ids, and return basis list
					idx = idx + 1
					tally[idx] = instruction 
				end    
				
		   end
		end

		-- --check for similar ids in the tally, or if weight is 0	
		for i,iv in ipairs(tally) do

			-- print ( basisString( iv.r.id) , iv.a, iv.b, iv.r.w )
			local exists = 0

			--if basis already in a table, insert additional instruction
			for j, jv in pairs(combined) do
				if iv.r.id == j then
					exists = 1
					if iv.r.w ~= 0 then
						table.insert(jv, iv )
					end
				end
			end

			--otherwise, add the new instruction to table
			if exists == 0 then	
				if iv.r.w ~= 0 then
					local newBasis = iv.r.id
					combined[newBasis] = {iv}
				end
			end
		end

		return order(combined)   --> returns { blades, inst }
	end,
	
	--List of -1 or 1s
	versionList = function (self, tx, operation)
		local res = {}
		for i, iv in ipairs(tx.bases) do
			res[i] = self.S[iv][operation].w
		end
		return res
	end,

	--Cast from TY to TX
	castList = function (tx, ty)
		local res = {}
		for i, iv in ipairs(tx.bases) do
			for k, kv in ipairs(ty.bases) do
				if iv == kv then				
					table.insert( res, {i, k} )
				end 
			end
		end	
		return res
	end,

	--add TY to TX
	sumList = function(tx, ty, op)
		local cl = castList(tx,ty)
		for i, iv in ipairs(cl) do
			iv[3] = op
		end
	end,       
	

	-- -- 
	--  
	mvProduct = function (self, ma, mb, op)
		--index of function  
		print("mv product")  

		local idx = ma.idx * 1000 * 3 + mb.idx * 3 + symidx[op]
	   -- return idx    
	     --print(self.idx)
	   -- memoized
		if (self.instructions[idx]==nil) then 
			self.instructions[idx] = self:productList( ma.bases, mb.bases, op ) 
			
			local tmp = self:makeType( self.instructions[idx].blades, ma.desc.."_"..op.."_"..mb.desc, false ) --don't force replace name    	
		    self.instructions[idx].type = tmp  			
			print(ma.desc.. symbol[op] .. mb.desc .. ": first time calculating, assembling instructions . . .")		
		end 
			
		return self:instance( ma, mb, self.instructions[idx]  )   
		 
	end, 
	

	
	instance = function(self, ma, mb, instruct ) 
		      return self.types[instruct.type]( calcProd(ma.bases,mb.bases,instruct.inst) )
	end,
    
    makeMV = function(self, iv )
	   	setmetatable( self.types[ iv ], self.MV)
		self.types[ iv ].__index = self.MV
		local m=getmetatable(self.MV)

		if m then
	        for k,v in pairs(m) do
	            if not rawget(self.MV,k) and k:match("^__") then
	                self.MV[k] = m[k]
	            end
	        end
		end
   end

}  

GA.__index = GA -- metamethod looks into GA for members . . .   


-- GA.__call = function(...)       

   	  




