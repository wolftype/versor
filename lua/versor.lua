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
local STA = V{
  metric={-1,-1,-1,1},
  types = {
    { name = "typename", bases={"e1","e2" ... }},
    {},
  }
}

]]

local bit = require("bit")
local tobit = bit.tobit
local bnot = bit.bnot
local band = bit.band
local bxor = bit.bxor
local bor = bit.bor
local lshift = bit.lshift
local rshift = bit.rshift


local symidx = {gp=1, op=2, ip=3}  

--BASIS UTILITY FUNCTIONS  
local blade = function(b, wt)
	return { id=b, w=wt }
end

local Type = function(key, bases, name)
	return { key=key, bases=bases, name=name, generated=false, dual=false }
end

local classname = function(name)
	--return "_"..name;
	return name
end

local grade = function (x)
	local n = 0
	while(x ~= 0) do
		if(band(x, 1) == 1) then n = n + 1 end
		x = rshift(x, 1)
	end
	return n
end

local compareBasis = function(a, b)
	local l = grade(a) - 1.0 / a
	local r = grade(b) - 1.0 / b
	return l < r
end

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

local product = function(a, b)
	local res = bxor(a, b)
	local s = sign(a, b)
	return blade(res, s)
end

local outer = function(a, b)
	if(band(a, b) ~= 0)
		then return blade(0, 0)
		else return product(a, b)
	end
end

local involute = function(x)
	local g = grade(x)
	local n = math.pow(-1, g)
	return blade(x, n)
end

local reverse = function(x)
	local g = grade(x)
	local n = math.pow(-1, (g*(g-1)/2.0))
	return blade(x, n)
end

local conjugate = function(x)
	local g = grade(x)
	local n = math.pow(-1, (g*(g+1)/2.0) )
	return blade(x, n)
end

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

local basisBits = function(bases)
	local ids = {}
	for i=1, #bases do
		ids[i] = basisBit(bases[i])
	end
	return ids
end

local basisNames = function(ty)
	table.sort(ty)
	
	local coords = {}
	for i=1, #ty do
		coords[i] = basisString(ty[i])
	end
	return coords
end

local keyCheck = function(k1, k2)
	if(#k1 ~= #k2) then return false end
	for i=1, #k1 do
		if(k1[i] ~= k2[i]) then return false end
	end
	return true
end

local order = function(c)
	local tblades = {}
	for i in pairs(c) do
		tblades[#tblades+1] = i
	end
	table.sort(tblades, compareBasis)

	return {
		blades=tblades, 
		inst=c
	}
end

--feed in a table of blades -> compress (combine like terms) and clean (remove zero terms)
local compress = function(x)
	-- collect like terms
	local tally = {}
	for i=1, #x do
		local iv = x[i]
		if(tally[iv.id]) 
			then tally[iv.id].w = tally[iv.id].w + iv.w;
			else tally[iv.id] = blade(iv.id, iv.w);
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
		m.basis = m:buildBasis()
		m.types = m:buildTypes()
		
		if(t.conformal) then
			m.values = m:buildConformalValues()
			m.products = m:buildConformal()
		else
			m.products = m:buildEuclidean()
		end
		
		m.subspaces = m:buildSubspaces()
		m:registerSubspaces()
		m:createTypes(t.types)
		
		m.api = m:generate(t)
		for name in pairs(m.api.constructors) do
			m[name] = m.api.constructors[name]
		end
		m.initialized = true
		return m
	end
})

function M:generate(t)
	
  local binopCode = self:generateBinops(t.binops)
	local typeCode = self:generateRegisteredTypes()
	local typeCodeAliases = {}
	
  for name in pairs(typeCode) do
		local ty = self.types[name]
		if(ty.alias and name == ty.alias) then
			typeCodeAliases[name] = typeCode[name]
		end
	end

	local functionBody = {"api = { classes={}, constructors={} }"}
	
  for name in pairs(typeCode) do
		if(not typeCodeAliases[name]) then
			local code = typeCode[name]
			
			functionBody[#functionBody+1] = table.concat({
					code,
					"api.constructors."..name.." = "..classname(name),
					"api.classes."..name.." = "..classname(name)
				}, "\n")
			if(self.types[name].alias) then
				local aliasName = self.types[name].alias
				local aliasCode = typeCodeAliases[aliasName]
				functionBody[#functionBody+1] = table.concat({
						aliasCode,
						"api.constructors."..aliasName.." = "..classname(aliasName),
						"api.classes."..aliasName.." = "..classname(aliasName)
					}, "\n")
			end
		end
	end
	
	for i=1, #binopCode do
		functionBody[#functionBody+1] = binopCode[i]
	end
	--print(table.concat(functionBody, "\n\n"))
	local f, err = loadstring(table.concat(functionBody, "\n\n"))
	--print(f, err)
	local env = {
		space = self,
		setmetatable = setmetatable,
		type = type,
		table = table,
	}
	setfenv(f, env)
	f()
	return env.api
  
end

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

function M:metricInner(a, b)
	local tmp = self:metricProduct(a, b)
	local g = grade(b) - grade(a)
	if(grade(a) > grade(b) or grade(tmp.id) ~= g)
		then return blade(0,0)
		else return tmp
	end
end

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

function M:basesKey(bases)
	local key = self:key()
	for i=1, #bases do
		local b = bases[i]
		local ty = self.types[basisString(b)]
		for k=1, #ty.key do
			key[k] = bor(key[k], ty.key[k])
		end
	end
	return key
end

function M:buildBasis()
	local basis = {0}
	local basisMap = { [0]=true }

	-- make e1, e2, e3, e4 . . .
	local nb = 1
	for i=1, #self.metric do
		basis[#basis+1] = nb
		basisMap[nb] = true
		nb = lshift(nb, 1)
	end

	-- make e12, e23 . . .
	for i=1, #basis do
		for j=1, #basis do
			if(i ~= j) then
				local r = outer(basis[i], basis[j])
				if(r.id ~= 0 and not basisMap[r.id]) then
					basis[#basis+1] = r.id
					basisMap[r.id] = true
				end
			end
		end
	end
	table.sort(basis, compareBasis)
	
	return basis
end

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

function M:bladeTable()
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

function M:checkMink(x)
	local v = band(x, self.values.eplane)
	if((v == 0) or (v == self.values.eplane))
		then return false
		else return true
	end
end

function M:buildEuclidean()
	local S = self:bladeTable()
	for i=1, #self.basis do
		local iv = self.basis[i]
		for j=1, #self.basis do
			local jv = self.basis[j]
			
			local gp = self:metricProduct(iv, jv)
			local op = outer(iv, jv)
			local ip = self:metricInner(iv, jv)
			
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

function M:pushMink(x)
	if(band(x, self.values.no) == self.values.no) then
		local t = bxor(x, self.values.no)
		return {
			blade(bxor(t, self.values.ep), 0.5),
			blade(bxor(t, self.values.em), 0.5)
		}
	elseif(band(x, self.values.ni) == self.values.ni) then
		local t = bxor(x, self.values.ni)
		return {
			blade(bxor(t, self.values.ep), -1),
			blade(bxor(t, self.values.em), 1)
		}
	end
end

function M:popMink(x)
	if(band(x, self.values.ep) == self.values.ep) then
		local t = bxor(x, self.values.ep)
		return {
			blade(bxor(t, self.values.no), 1),
			blade(bxor(t, self.values.ni), -0.5)
		}
	elseif(band(x, self.values.em) == self.values.em) then
		local t = bxor(x, self.values.em)
		return {
			blade(bxor(t, self.values.no), 1),
			blade(bxor(t, self.values.ni), 0.5)
		}
	end
end

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

function M:buildConformalBinop(S, iv, jv)
	-- get list of blades in Minkowskian (diagonal) metric
	local tmpA
	if(self:checkMink(iv))
		then tmpA = self:pushMink(iv)
		else tmpA = { blade(iv, 1) }
	end
	local tmpB
	if(self:checkMink(jv))
		then tmpB = self:pushMink(jv)
		else tmpB = { blade(jv, 1) }
	end
	
	local gpTally = {}
	local opTally = {}
	local ipTally = {}
	for a=1, #tmpA do
		local av = tmpA[a]
		for b=1, #tmpB do
			local bv = tmpB[b]
			-- calculate products in mink metric
			local gp = self:metricProduct(av.id, bv.id)
			local op = outer(av.id, bv.id)
			local ip = self:metricInner(av.id, bv.id)
			
			-- push onto tally stack
			gpTally[#gpTally+1] = blade(gp.id, gp.w*av.w*bv.w)
			opTally[#opTally+1] = blade(op.id, op.w*av.w*bv.w)
			ipTally[#ipTally+1] = blade(ip.id, ip.w*av.w*bv.w)
		end
	end
		
	local gpPop = self:accumMink(compress(gpTally))
	local opPop = self:accumMink(compress(opTally))
	local ipPop = self:accumMink(compress(ipTally))
	
	S[iv].gp[jv] = compress(gpPop)
	S[iv].op[jv] = compress(opPop)
	S[iv].ip[jv] = compress(ipPop)
end

function M:buildConformalValues()
	local no = lshift(1, #self.metric-2)
	local ni = lshift(1, #self.metric-1)
	return {
		no=no, ni=ni,
		ep=no, em=ni,
		eplane=bor(no, ni)
	}
end

function M:buildConformal()
	local S = self:bladeTable()
	for i=1, #self.basis do
		local ib = self.basis[i]
		S[ib].involute = involute(ib)
		S[ib].reverse = reverse(ib)
		S[ib].conjugate = conjugate(ib)
		
		for j=1, #self.basis do
			local jb = self.basis[j]
			self:buildConformalBinop(S, ib, jb)
		end
	end
	return S
end

local _subspaceNames = {"Vec", "Biv", "Tri", "Quad", "Penta", "Hexa", "Hepta", "Octo"}
function M:buildSubspaces()
	local subspaces = {}
	for i=1, #self.metric do
		subspaces[i] = {
			name=_subspaceNames[i],
			bases={}
		}
	end

	for i=1, #self.basis do
		local b = self.basis[i]
		local g = grade(b)
		if(g > 0) then
			local bases = subspaces[g].bases
			bases[#bases+1] = b
		end
	end
	return subspaces
end

function M:registerSubspaces()
	for i=1, #self.subspaces do
		local iv = self.subspaces[i]
		self.types[iv.name] = Type(self:basesKey(iv.bases), iv.bases, iv.name)
	end
end

function M:aliasType(oty, nty)
	self.types[nty] = self.types[oty];
	self.types[oty].alias = nty
end

function M:createType(bases, name, aliasExisting)
	local key = self:basesKey(bases)
	for tyname, ty in pairs(self.types) do
		if(keyCheck(key, ty.key)) then
			if(aliasExisting) then
				self:aliasType(tyname, name)
				return name
			else
				return tyname
			end
		end
	end

	self.types[name] = Type(key, bases, name)
	return name
end

function M:productList(bases1, bases2, opname)
	local tally = {}
	
	-- fetch table pairs of values in types
	local idx = 1 --bug fix (was 0)
	for i=1, #bases1 do
		local iv = bases1[i]
		for j=1, #bases2 do
			local jv = bases2[j]

			local prod = self.products[iv][opname][jv]
			for k=1, #prod do
				local instruction = {
					a=i, b=j, 
					ida=basisString(iv),
					idb=basisString(jv),
					r=prod[k]
				}
				tally[idx] = instruction
				idx = idx+1
			end
		end
	end

	local combined = {}
	-- check for similar ids in the tally, or if weight is 0	
	for i=1, #tally do
		local instruction = tally[i]
		if(instruction.r.w ~= 0) then
			local b = instruction.r.id
			if(combined[b]) then
				local instructions = combined[b]
				instructions[#instructions+1] = instruction
			else
				combined[b] = {instruction}
			end
		end
	end
	return order(combined)
end

function M:generateType(name)
	local ty = self.types[name]
	local coords = basisNames(ty.bases)
	
	local cname = classname(name)
	local setfields = {}
	local getfields = {}
	for i=1, #coords do
		setfields[i] = "mv["..i.."] = "..coords[i]
	end
	
	local code = {[[
	local ]]..cname..[[ = {}
	]]..cname..[[.__index = ]]..cname..[[
	setmetatable(]]..cname..[[, {
		__call = function(_, ]]..table.concat(coords, ", ")..[[)
			if(type(]]..coords[1]..[[) == "object") then
				return ]]..cname..[[.cast(]]..coords[1]..[[)
			else
				local mv = {}
				]]..table.concat(setfields, "\n")..[[
				setmetatable(mv, ]]..cname..[[)
				return mv
			end
		end
	})
	
	function ]]..cname..[[:__add(b)
		return self:add(b)
	end
	
	function ]]..cname..[[:__sub(b)
		return self:sub(b)
	end
	
	function ]]..cname..[[:__mul(b)
		return self:gp(b)
	end
	
	function ]]..cname..[[:__div(b)
		return self:div(b)
	end
	
	function ]]..cname..[[:__pow(b)
		return self:op(b)
	end
	
	function ]]..cname..[[:__tostring()
		return self.type.."("..table.concat(self, ", ")..")"
	end
	
	]]..cname..[[.type = "]]..name..[["
	]]..cname..[[._cast = {}
	]]..cname..[[._ip = {}
	]]..cname..[[._op = {}
	]]..cname..[[._gp = {}
	]]..cname..[[._add = {}
	]]..cname..[[._sub = {}
	
	function ]]..cname..[[:inverse()
		local rev = self:reverse()
		local sca = self:gp(rev)[1]
		return rev:gp(1/sca)
	end
	
	function ]]..cname..[[:ip(b)
		if(not ]]..cname..[[._ip[b.type]) then
			space:createBinop("ip", self.type, b.type)
		end
		return self._ip[b.type](self, b)
	end
	
	function ]]..cname..[[:op(b)
		if(not ]]..cname..[[._op[b.type]) then
			space:createBinop("op", self.type, b.type)
		end
		return self._op[b.type](self, b)
	end
	
	function ]]..cname..[[:gp(b)
		if(type(b) == "number") then
			b = space.s(b)
		end
	
		if(not ]]..cname..[[._gp[b.type]) then
			space:createBinop("gp", self.type, b.type)
		end
		return self._gp[b.type](self, b)
	end
	
	function ]]..cname..[[:sp(b)
		local v = b:inverse():gp(self):gp(b)
		return ]]..cname..[[(v)
	end
	
	function ]]..cname..[[:div(b)
		return self:gp(b:inverse())
	end
	
	function ]]..cname..[[:add(b)
		if(type(b) == "number") then
			b = space.s(b)
		end
	
		if(not ]]..cname..[[._add[b.type]) then
			space:createAffineOp("add", self.type, b.type)
		end
		return self._add[b.type](self, b)
	end
	
	function ]]..cname..[[:sub(b)
		if(type(b) == "number") then
			b = space.s(b)
		end
	
		if(not ]]..cname..[[._sub[b.type]) then
			space:createAffineOp("sub", self.type, b.type)
		end
		return self._sub[b.type](self, b)
	end
	]],}
	
	code[#code+1] = self:generateUnop("reverse", name)
	code[#code+1] = self:generateUnop("involute", name)
	code[#code+1] = self:generateUnop("conjugate", name)
	
	ty.generated = true
	return table.concat(code, "\n\n")
end

function M:createCast(toName, fromName)
end

function M:generateUnop(opname, tyname)
	local ty = self.types[tyname]	
	local coords = basisNames(ty.bases)
	
	local ops = {}
	for i=1, #ty.bases do
		local v = ty.bases[i]
		local blade = self.products[v][opname]
		ops[i] = "self["..i.."]"
		if(blade.w < 0) then
			ops[i] = "-"..ops[i]
		end
	end
	
	local cname = classname(tyname)
	return "function "..cname..":"..opname..[[()
	return ]]..cname..[[(]]..table.concat(ops, ", ")..[[)
	end
	]]
end

function M:binopResultType(opname, tyname1, tyname2)
	local ty1 = self.types[tyname1]
	local ty2 = self.types[tyname2]
	
	local op = self:productList(ty1.bases, ty2.bases, opname)
	local tynameRes
	if(#op.blades == 0) 
		then tynameRes = "s"
		else tynameRes = self:createType(op.blades, tyname1..tyname2.."_"..opname, false)
	end
	return tynameRes
end

function M:generateResType(tynameRes)
	-- TODO: consolidate this with the generate() function
	local code = self:generateType(tynameRes)
	local functionBody = {"api = { classes:{}, constructors:{} };"}
	functionBody[#functionBody+1] = table.concat({
			code,
			"api.constructors."..tynameRes.." = "..classname(tynameRes)
			"api.classes."..tynameRes.." = "..classname(tynameRes)
		}, "\n")
	
	local f = loadstring(table.concat(functionBody, "\n\n"))
	local env = { space=self }
	f()
	local api = assert(env.api)
	for name in pairs(api.classes) do
		self.api.classes[name] = api.classes[name]
	end
	for name in pairs(api.constructors) do
		self.api.constructors[name] = api.constructors[name]
	end
end

function M:generateBinop(opname, tyname1, tyname2)
	local ty1 = self.types[tyname1]	
	local ty2 = self.types[tyname2]	
	
	local op = self:productList(ty1.bases, ty2.bases, opname)
	local tynameRes = self:binopResultType(opname, tyname1, tyname2)
		
	local tyRes = self.types[tynameRes]
	if(not tyRes) then
		print("ERROR: gentype " .. tyname1..tyname2.."_"+opname, table.concat(op.blades, " "));
	elseif(self.initialized and not tyRes.generated) then
		self:generateResType(tynameRes)
	end

	local ops = {}
	if(#op.blades == 0) then
		ops[0] = "0"
	else
		for i=1, #op.blades do
			local blade = op.blades[i]
			local inst = op.inst[blade]
			local instbops = {}
			for j=1, #inst do
				local instop = inst[j]
				local bop = "self["..instop.a.."]*b["..instop.b.."]";
				if(instop.r.w < 0) then
					bop = "-"..bop;
				end
				instbops[#instbops+1] = bop
			end
			ops[#ops+1] = table.concat(instbops, " + ")
		end
	end

	return table.concat({
		classname(tyname1).."._"..opname.."."..tyname2..[[ = function(self, b)
		return ]]..tynameRes.."("..table.concat(ops, ",\n").."\n)",
		"end"
	}, "\n")
end

function M:createBinop(opname, tyname1, tyname2)
	local resultType = self:binopResultType(opname, tyname1, tyname2)
	local code = self:generateBinop(opname, tyname1, tyname2)
	local env = { 
		space=self, 
		[classname(tyname1)]=self.api.classes[tyname1],
		[resultType] = self.api.constructors[resultType]
	}
	local f = loadstring(code)
	setfenv(f, env)
	f()
end

function M:createAffineOp(opname, tyname1, tyname2)
	local opsym = opname == "add" and "+" or "-"

	local ty1 = self.types[tyname1]
	local ty2 = self.types[tyname2]
	local bases1Map = {}
	local bases2Map = {}
	local basesMap = {}
	for i=1, #ty1.bases do
		bases1Map[ ty1.bases[i] ] = i
		basesMap[ ty1.bases[i] ] = ty1.bases[i]
	end
	for i=1, #ty2.bases do
		bases2Map[ ty2.bases[i] ] = i
		basesMap[ ty2.bases[i] ] = ty2.bases[i]
	end
	local bases = {}
	for name, b in pairs(basesMap) do
		bases[#bases+1] = b
	end
	table.sort(bases)


	local ops = {}
	for i=1, #bases do
		local operands = {}
		local second = false
		if(bases1Map[ bases[i] ]) then
			operands[#operands+1] = "self["..bases1Map[ bases[i] ].."]"
		end
		if(bases2Map[ bases[i] ]) then
			second = true
			operands[#operands+1] = "b["..bases2Map[ bases[i] ].."]"
		end
		local op
		if(#operands == 2) then
			op = table.concat(operands, opsym)
		else
			op = operands[0]
			if(second and opname == "sub") then
				op = opsym..op;
			end
		end
		ops[i] = op
	end
	
	local tynameRes = self:createType(bases, tyname1..tyname2.."_"..opname, false);
	local tyRes = self.types[tynameRes]
	if(self.initialized and not tyRes.generated) then
		self:generateResType(tynameRes)
	end

	local code = table.concat({
		classname(tyname1).."._"..opname.."."..tyname2..[[ = function(self, b)
		return ]]..tynameRes.."("..table.concat(ops, ", ")..[[)
		end]]
	}, "\n")
	
	local env = { 
		space=self, 
		[classname(tyname1)]=self.api.classes[tyname1],
		[tynameRes] = self.api.constructors[tynameRes]
	}
	local f = loadstring(code)
	setfenv(f, env)
	f()
end

function M:generateRegisteredTypes()
	local code = {}
	for name, ty in pairs(self.types) do
		if(not ty.generated)
			then code[name] = self:generateType(name)
			else code[name] = "local "..classname(name).." = "..classname(ty.name)
		end
	end
	return code
end

function M:generateBinops(binops)
	local code = {}
	for i=1, #binops do
		local v = binops[i]
		code[i] = self:generateBinop(v.op, v.types[1], v.types[2]);
	end
	return code
end

function M:ip(a, b)
	return a:ip(b)
end

function M:op(a, b)
	return a:op(b)
end

function M:gp(a, b)
	if(type(a) == "number") then
		a = self.s(a)
	end
	return a:gp(b)
end

function M:sp(a, b)
	return a:sp(b)
end

function M:createTypes(types)
	for i=1, #types do
		local ty = types[i]
		local name = self:createType(basisBits(ty.bases), ty.name, true)
		if(type(ty.dual) == "nil") then
			self.types[name].dual = ty.dual
		end
	end
end

return M
