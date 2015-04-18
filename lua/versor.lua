--Lua Geometric Algebra generator [arbitrary metrics]
--Authors: Pablo Colapinto and Wesley Smith
--Email:  wolftype@gmail.com  

--[[
Builds Euclidean and Conformal Basis 
Operations In Arbitrary Metric  

**SEE R#.lua for sample usage**
**SEE C2.lua for sample usage**

Space Time Algebra, for instance,
is possible by setting metric to
{-1,-1,-1,1} 

EXAMPLE:

local V = require"versor"
local EGA = V{
  metric={1,1,1},
  types = {
    {name="vec", bases={"e1","e2","e3"}},
    {name="biv", bases={"e12","e13","e23"}},
  }
}


local V = require"versor"
local STA = V{
  metric={-1,-1,-1,1},
  types = {
    { name = "typename", bases={"e1","e2" ... }},
    {},
  }
}
--]]

local bit = require("bit")
local tobit = bit.tobit
local bnot = bit.bnot
local band = bit.band
local bxor = bit.bxor
local bor = bit.bor
local lshift = bit.lshift
local rshift = bit.rshift

--------------------------------------------------------
--------------------------------------------------------
--------------------------------------------------------

local symidx = {gp=1, op=2, ip=3}  

--MAKE BLADE FROM BIT REPRESENTATION (BIT REP) AND WEIGHT
local Blade = function(_id, _w)
	return { id=_id, w=_w }
end

--MAKE TYPE
local Type = function(key, bases, name)
	return { key=key, bases=bases, name=name, generated=false, dual=false }
end

--------------------------------------------------------
--------------------------------------------------------
--------------------------------------------------------
--BASIS BLADE UTILITY FUNCTIONS  
--------------------------------------------------------
--------------------------------------------------------
--------------------------------------------------------

--GET GRADE OF BASIS BLADE
local grade = function (x)
	local n = 0
	while(x ~= 0) do
		if(band(x, 1) == 1) then n = n + 1 end
		x = rshift(x, 1)
	end
	return n
end

--DETERMINE SIGN FLIP OF TWO BASIS BLADES
local sign = function(a,b)
	local n = rshift(a, 1)
	local sum = 0
	while(n ~= 0) do
		sum = sum + grade( band(n, b) )
		n = rshift(n, 1)
	end
	if(band(sum, 1) == 0) 
		then return 1
		else return -1
	end
end

--DETERMINE GEOMETRIC PRODUCT OF TWO BASIS BLADES
local product = function(a, b)
	local res = bxor(a, b)
	local s = sign(a, b)
	return Blade(res, s)
end

--DETERMINE OUTER PRODUCT OF TWO BASIS BLADES
local outer = function(a, b)
	if(band(a, b) ~= 0)
		then return Blade(0, 0)
		else return product(a, b)
	end
end

--INVOLUTION OF BLADE
local involute = function(x)
	local g = grade(x)
	local n = math.pow(-1, g)
	return Blade(x, n)
end

--REVERSE OF BLADE
local reverse = function(x)
	local g = grade(x)
	local n = math.pow(-1, (g*(g-1)/2.0))
	return Blade(x, n)
end

--CONJUGATION OF BLADE
local conjugate = function(x)
	local g = grade(x)
	local n = math.pow(-1, (g*(g+1)/2.0) )
	return Blade(x, n)
end


--------------------------------------------
--------------------------------------------
--ORDERING
--------------------------------------------
--------------------------------------------

--COMPARE TWO BASIS BLADES 
local compareBasis = function(a, b)
	local l = grade(a) - 1.0 / a
	local r = grade(b) - 1.0 / b
	return l < r
end

--ORDER A LIST OF INSTRUCTIONS
local order = function(c)
	local tb = {}
	for i in pairs(c) do
		tb[#tb+1] = i
	end
	table.sort(tb, compareBasis)

	return {
		blades=tb, 
		inst=c
	}
end


---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
-- STRING AND BIT CONVERSIONS
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------


--STRING REP FROM BLADE BIT REP
local basisString = function(x)
	local n = 0
	local res = ""
	while(x ~= 0) do
		n = n+1
		if(band(x, 1) == 1) then res = res..n end
		x = rshift(x, 1)
	end
	if(n > 0)
		then return "e"..res
		else return "s"
	end
end

--BIT REP FROM STRING REP
local basisBit = function(name)
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

--BIT REPS FOR A LIST OF STRING REPS
local basisBits = function(bases)
	local ids = {}
	for i=1, #bases do
		ids[i] = basisBit(bases[i])
	end
	return ids
end

--STRING REPS FOR A LIST OF BIT REPS
local basisNames = function(ty)
	table.sort(ty)
	
	local coords = {}
	for i=1, #ty do
		coords[i] = basisString(ty[i])
	end
	return coords
end

---------------------------------------------------
---------------------------------------------------
--KEY CHECK
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------

local keyCheck = function(k1, k2)
	if(#k1 ~= #k2) then return false end
	for i=1, #k1 do
		if(k1[i] ~= k2[i]) then return false end
	end
	return true
end


---------------------------------------------------
---------------------------------------------------
-- OPERATIONS ON LISTS
---------------------------------------------------
---------------------------------------------------

--COMPRESS AND CLEAN A LIST OF INSTRUCTIONS
local compress = function(x)
	-- collect like terms
	local tally = {}
	for i=1, #x do
		local iv = x[i]
		if(tally[iv.id]) 
			then tally[iv.id].w = tally[iv.id].w + iv.w;
			else tally[iv.id] = Blade(iv.id, iv.w);
		end	
	end
	
	-- remove zeros
	local res = {}
	for id in pairs(tally) do
		local iv = tally[id]
		if(iv.w ~= 0) then 
			res[#res+1] = iv
		end
	end
	return res
end


--------------------------------------------------
-- SIMPLE CODE GEN
--------------------------------------------------

--FEED IN A PRODUCT LIST, SPIT OUT SOME CODE
local productListCode = function(x)
  
  local inst = {}     
  for i, iv in ipairs( x.blades ) do

    local out = ""

    for k, kv in ipairs( x.inst[ iv ] ) do
       if (kv.r.w < 0) then out = out .. "-"
       elseif ( k > 1  ) then out = out .. "+" end
       out = out .. " a["..(kv.a-1).."]*b[" .. (kv.b-1).."] "
    end
    if (i < #x.blades)then out = out.."," end
    out = out .. " // " .. basisString( iv )
        
    inst[#inst+1] = out
  
  end

  return inst
end


---------------------------------------------------
---------------------------------------------------
---------------------------------------------------
-- BUILDING AN ALGEBRAIC SPACE
---------------------------------------------------
---------------------------------------------------
---------------------------------------------------

local M = {}
M.__index = M

setmetatable(M, {

	  __call = function(_, t)
	
  	t = t or {}
		t.metric = t.metric or {1, 1, 1}
		t.types = t.types or {}
		t.binops = t.binops or {}
		
		local m = setmetatable({}, M)
		m.metric = t.metric
		m.basis = m:buildBasis()                  -- Complete Basis
		m.types = m:buildTypes()                  -- Save Basis as Types
		
		if(t.conformal) then
			m.values = m:buildConformalValues()     -- no, ni, eplane
			m.products = m:buildConformal()         -- unary and binary push and pop
		else
			m.products = m:buildMetric()  --< Build on diagonal Metric
		end
		
		--m.subspaces = m:buildSubspaces()
		--m:registerSubspaces()
		m:createTypes(t.types)
		--m.api = m:generate(t)
		--for name in pairs(m.api.constructors) do
		--	m[name] = m.api.constructors[name]
		--end
		--m.initialized = true
		return m
	end
})


--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
-- GENERIC TYPE CREATION
--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------

-- SWAP TYPE NAME
function M:aliasType(oty, nty)
	self.types[nty] = self.types[oty];
	self.types[oty].alias = nty
end

-- MAKE BASIS BLADES 
function M:buildBasis()
	local basis = {0}
	local basisMap = { [0]=true } --tmp

  --make e1, e2, etc.
	local nb = 1
	for i=1, #self.metric do
		basis[#basis+1] = nb
		basisMap[nb] = true
		nb = lshift(nb, 1)
	end

	-- make e12, e23 . . . e123
	for i=1, #basis do
		for j=1, #basis do
			if(i ~= j) then
				local b = outer(basis[i], basis[j])
				if(b.id ~= 0 and not basisMap[b.id]) then
					basis[#basis+1] = b.id
					basisMap[b.id] = true
				end
			end
		end
	end

	table.sort(basis, compareBasis)
	return basis
end


--MAKE TYPES FROM BASIS BLADES
function M:buildTypes()
	local types = {};
	for i=1, #self.basis do
		local b = self.basis[i]
		local key = self:key(b)
		local name = basisString(b)
		types[name] = Type(key, {b}, name)
	end
	return types
end

--CONFORMAL VALUES
function M:buildConformalValues()
	local no = lshift(1, #self.metric-2)
	local ni = lshift(1, #self.metric-1)
	return {
		no=no, ni=ni,
		ep=no, em=ni,
		eplane=bor(no, ni)
	}
end

--MAKE ONE TYPE FROM LIST OF BLADES, NAME, AND BOOLEAN (WHETHER TO ALIAS THE NAME)
function M:createType(bases, name, aliasExisting)

	local key = self:basesKey(bases)            --unique key

	for tyname, ty in pairs(self.types) do      --check if type exists

		if(keyCheck(key, ty.key)) then            --IF type exists		
    	if(aliasExisting) then                  --alias name
				self:aliasType(tyname, name)
				return name
			else                                    --or ...
				return tyname                         --keep original name
			end
    end
  
  end

	self.types[name] = Type(key, bases, name)  --OTHERWISE, make new type
	return name
end

--MAKE MANY TYPES FROM LIST OF TYPES
function M:createTypes(types)
	for i=1, #types do
		local ty = types[i]
		local name = self:createType(basisBits(ty.bases), ty.name, true)
		if(type(ty.dual) == "nil") then
			self.types[name].dual = ty.dual
		end
	end
end

-------------------------------------------------------------
-------------------------------------------------------------
-------------------------------------------------------------
--KEYS FOR TYPE TRACKING
-------------------------------------------------------------
-------------------------------------------------------------
-------------------------------------------------------------

--KEEP TRACK OF BASES BY GENERATING KEYS FOR EACH . . .
function M:key(b)
	local nkeys = math.ceil(#self.basis/32)
	local key = {}
	for i=1, nkeys do
		key[i] = 0
	end
	
	if(b) then
		local k = math.ceil((b+1)/32);
		local shft = (b+1) - 32*(k-1);
		key[k] = lshift(1, shft-1)
	end
	
	return key
end

--GENERATE A KEY FOR EACH TYPE BY LIST OF BASES
function M:basesKey(bases)
	local key = self:key()
	for i=1, #bases do
		local b = bases[i]
		local ty = self.types[ basisString(b) ]
		for k=1, #ty.key do
			key[k] = bor(key[k], ty.key[k])
		end
	end
	return key
end


--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
-- METRIC PRODUCT TABLES
--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------

-- METRIC GEOMETRIC PRODUCTS
function M:metricProduct(a, b)
	local tmp = product(a,b)
	local bs = band(a,b)
	local i = 1
	while(bs ~= 0) do
		if(band(bs, 1) == 1) then tmp.w = tmp.w*self.metric[i] end
		bs = rshift(bs, 1)
		i = i+1
	end
	return tmp
end

--METRIC INNER
function M:metricInner(a, b)
	local tmp = self:metricProduct(a, b)
	local g = grade(b) - grade(a)
	if(grade(a) > grade(b) or grade(tmp.id) ~= g)
		then return Blade(0,0)
		else return tmp
	end
end

--EMPTY CAYLEY TABLE OF BLADE-ON-BLADE OPERATIONS 
function M:emptyBladeTable()
	local S = {}
	for i=1, #self.basis do
		local b = self.basis[i]
		local name = basisString(b)
		S[b] = {
			id=name,
			basis=b,
			gp={}, op={}, ip={}
		}
	end
	return S
end

--BUILD BASIS PRODUCT TABLE ON DIAGONAL METRIC
function M:buildMetric()
	local S = self:emptyBladeTable()
	for i=1, #self.basis do
		local iv = self.basis[i]
		for j=1, #self.basis do
			local jv = self.basis[j]
			
			local gp = self:metricProduct(iv, jv)
			local op = outer(iv, jv)
			local ip = self:metricInner(iv, jv)
			
      --straight up blades
			S[iv].gp[jv] = {gp}
			S[iv].op[jv] = {op}
			S[iv].ip[jv] = {ip}
			S[iv].involute = involute(iv)
			S[iv].reverse = reverse(iv)
			S[iv].conjugate = conjugate(iv)
		end
	end
	return S
end       


------------------------------
--UNARY OPERATRIONS
function M:versionList(tyname, opname)
	local ty = self.types[tyname]	
	--local coords = basisNames(ty.bases)
	
	local ops = {}
	for i=1, #ty.bases do
		local v = ty.bases[i]
		local blade = self.products[v][opname]
		if(blade.w < 0) then
			ops[i] = true
		else
			ops[i] = false
		end
	end   
	
	return {blades=ty.bases,inst=ops} 
end

--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
-- CONFORMAL PRODUCT TABLES
--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------

--CHECK FOR PRESENCE OF NULL BASIS BLADE
function M:checkMink(x)
	local v = band(x, self.values.eplane)
	if((v == 0) or (v == self.values.eplane))
		then return false
		else return true
	end
end

--PUSH INTO DIAGONALIZED METRIC
function M:pushMink(x)
	if(band(x, self.values.no) == self.values.no) then
		local t = bxor(x, self.values.no)
		return {
			Blade(bxor(t, self.values.ep), 0.5),
			Blade(bxor(t, self.values.em), 0.5)
		}
	elseif(band(x, self.values.ni) == self.values.ni) then
		local t = bxor(x, self.values.ni)
		return {
			Blade(bxor(t, self.values.ep), -1),
			Blade(bxor(t, self.values.em), 1)
		}
	end
end

--POP OUT OF DIAGONALIZED METRIC
function M:popMink(x)
	if(band(x, self.values.ep) == self.values.ep) then
		local t = bxor(x, self.values.ep)
		return {
			Blade(bxor(t, self.values.no), 1),
			Blade(bxor(t, self.values.ni), -0.5)
		}
	elseif(band(x, self.values.em) == self.values.em) then
		local t = bxor(x, self.values.em)
		return {
			Blade(bxor(t, self.values.no), 1),
			Blade(bxor(t, self.values.ni), 0.5)
		}
	end
end

--ACCUMULATE (and POP in the process)
function M:accumMink(blades)
	local res = {}
	for i=1, #blades do
		local iv = blades[i]
		if(self:checkMink(iv.id)) then
			local minkBlades = self:popMink(iv.id)
			for j=1, #minkBlades do
				local jv = minkBlades[j]
				jv.w = jv.w*iv.w;
			end
			for j=1, #minkBlades do
				res[#res+1] = minkBlades[j]
			end
		else
			res[#res+1] = iv
		end
	end
	return res
end


--BUILD BASIS PRODUCT TABLE ON CONFORMAL METRIC
function M:buildConformal()
	local S = self:emptyBladeTable()
	for i=1, #self.basis do
		local ib = self.basis[i]
    --Unary Operations
		S[ib].involute = involute(ib)
		S[ib].reverse = reverse(ib)
		S[ib].conjugate = conjugate(ib)
		--Binary Operations
		for j=1, #self.basis do
			local jb = self.basis[j]
			self:buildConformalBinop(S, ib, jb)
		end
	end
	return S
end

--BUILD OPERATION FOR TWO SPECIFIC TYPES
function M:buildConformalBinop(S, iv, jv)
	-- get list of blades in Minkowskian (diagonal) metric
	local tmpA
	if(self:checkMink(iv))
		then tmpA = self:pushMink(iv)
		else tmpA = { Blade(iv, 1) }
	end
	local tmpB
	if(self:checkMink(jv))
		then tmpB = self:pushMink(jv)
		else tmpB = { Blade(jv, 1) }
	end
	
	local gpTally = {}
	local opTally = {}
	local ipTally = {}
	for a=1, #tmpA do
		local av = tmpA[a]
		for b=1, #tmpB do
			local bv = tmpB[b]
			-- calculate products in diagonal mink metric
			local gp = self:metricProduct(av.id, bv.id)
			local op = outer(av.id, bv.id)
			local ip = self:metricInner(av.id, bv.id)
			
			-- push onto tally stack
			gpTally[#gpTally+1] = Blade(gp.id, gp.w*av.w*bv.w)
			opTally[#opTally+1] = Blade(op.id, op.w*av.w*bv.w)
			ipTally[#ipTally+1] = Blade(ip.id, ip.w*av.w*bv.w)
		end
	end
		
	local gpPop = self:accumMink(compress(gpTally))
	local opPop = self:accumMink(compress(opTally))
	local ipPop = self:accumMink(compress(ipTally))
	
	S[iv].gp[jv] = compress(gpPop)
	S[iv].op[jv] = compress(opPop)
	S[iv].ip[jv] = compress(ipPop)
end



--LIST OF OPERATIVE INSTRUCTIONS FROM TWO LISTS OF BASES BLADES
--RETURNS {blades, inst}
function M:productList(bases1, bases2, opname)
	
	-- fetch table pairs of values in types
	local tally = {}

	for i=1, #bases1 do
		local iv = bases1[i]
		for j=1, #bases2 do
			local jv = bases2[j]

      --Cayley Table (list of return ids and wts )
			local prod = self.products[iv][opname][jv]  
                  
      --Generate Instruction for each 
			for k=1, #prod do
			
      	local instruction = {
					a=i, b=j, 
					r=prod[k]
				}
			
      	tally[#tally+1] = instruction
			end
		end
	end

	-- check for similar ids in the tally, or if weight is 0	
	local combined = {}

	for i=1, #tally do
		local instruction = tally[i]
		if(instruction.r.w ~= 0) then                     --if not 0
			local b = instruction.r.id
			if(combined[b]) then                            --if r.id already added in
				local instructions = combined[b]
				instructions[#instructions+1] = instruction   --add current instruction to list
			else
				combined[b] = {instruction}                   --otherwise make new
			end
		end
	end
	return order(combined)                              --sort blades list     
end
           
------------------           
--TYPE MANAGEMENT 
------------------
                    
function M:getProductList(type1, type2, opname)
	return self:productList( self.types[type1].bases, self.types[type2].bases, opname ) 
end       

function M:getProductListType(list)
	local key = self:basesKey( list.blades )             
	local name = "unknown"
	for i, iv in pairs( self.types ) do
	   if keyCheck(iv.key, key) then name=iv.name end 
	end
	return name
end
 
function M:getProductType(type1, type2, opname)  
	local list = self:getProductList(type1, type2, opname)
	local key = self:basesKey( list.blades )             
	local type = nil
	for i, iv in pairs( self.types ) do
	   if keyCheck(iv.key, key) then type=iv end 
	end
	return type
end

--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
--CODEGEN STRINGS
--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------

function M:codegenProduct(bases1, bases2, opname)
  local t = self:productList(bases1,bases2,opname)
  return productListCode(t)
end

--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------
--PRETTY PRINTING
--------------------------------------------------------------
--------------------------------------------------------------
--------------------------------------------------------------

function M:printBases()
  for i=1, #self.basis do
     print( basisString( self.basis[i] ) )
  end
end

function M:printTable()

  for k, kv in pairs(self.products) do
    for j, jv in pairs(kv.gp) do
      print( basisString(k) .. " * " .. basisString(j) .." = ")
      for m=1, #jv do print("\t"..basisString(jv[m].id)  ) end
    end
    for j, jv in pairs(kv.op) do
      print( basisString(k) .. " ^ " .. basisString(j) .." = ")
      for m=1, #jv do print("\t"..basisString(jv[m].id)  ) end
    end
    for j, jv in pairs(kv.ip) do
      print( basisString(k) .. " <= " .. basisString(j) .." = ")
      for m=1, #jv do print("\t"..basisString(jv[m].id)  ) end
    end
  end

end

function M:printProduct(type1, type2, opname)
  local topname = opname or "gp"
  local p = self:codegenProduct( self.types[type1].bases, self.types[type2].bases, topname )
  for i=1,#p do print(p[i]) end
end

function M:printType(t)
  print(t)
  for i=1,#self.types[t].bases do
    print( basisString(self.types[t].bases[i]) )
  end
end 

function M:basisString(t) return basisString(t) end

return M
