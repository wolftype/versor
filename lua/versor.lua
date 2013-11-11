--Lua Geometric Algebra generator [arbitrary metrics]
--Author: Pablo Colapinto
--Email:  wolftype@gmail.com  
--free to use and distribute

--[[

Builds Euclidean and Conformal Basis 
Operations In Arbitrary Metric  

**SEE R3.lua for sample usage**

Space time Algebra, for instance,
is possible by setting metric to
{-1,-1,-1,1} and calling buildEuclidean (probably not the right name . . .)

local sta = GA:new(-1,-1,-1,1)
sta.buildEuclidean()


3D Conformal is built by 

local cga = GA:new(1,1,1,1,-1)  
cga.buildConformal()

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
	print(f, err)
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
	local idx = 0
	for i=1, #bases1 do
		local iv = bases1[i]
		for j=1, #bases1 do
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

--[[
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
--]]



--CONVERT: string name to bit rep int 
--[==[

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






--BASIC PRODUCTS

inner = function(a,b)
	local tmp = product(a,b)
	local g = grade(b) - grade(a)
	if grade(a) > grade(b) or grade(tmp.id) ~= g then 
		return blade(0,0)
	else return tmp end
end



--HERMITIAN OPERATORS 





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
		new = function(super,...)  
			
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
				__mul = function(a,b) return super.mvProduct(super,a, b, "gp") end, -- *  (geometric product)
				__pow = function(a,b) return super.mvProduct(super,a, b, "op") end, -- ^  (outer product)
				__concat  = function(a,b) return super.mvProduct(super,a, b, "ip") end,  -- .. (inner product)
				__call = function(table,...) return table:new(...) end, 
				
			     print = function(self) for i, iv in ipairs(self) do print(iv) end end
			},

		  } 
		
	       setmetatable(gax, super)
		
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
		
		
           gax:buildBasis()
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



	0formal = function(self)
	
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


--[[
THIS IS HOW TO USE IT:


local B = require "versor"
   
local t = B.GA:new(1,1,1)
t:buildEuclidean() 


local vec = { id = "Vec", desc = "Vector", bases = {"e1","e2","e3"} }    
local rot = { id = "Rot", desc = "Rotor", bases = {"s", "e12","e13","e23"} }  

vec.blades = B.toBasisList( vec.bases )   
t:makeType( vec.blades, vec.id, false )   

rot.blades = B.toBasisList( rot.bases ) 
t:makeType( rot.blades, rot.id, false )  

for i,iv in ipairs( t.types["Vec"](0,0,1) ) do
   print(iv)
end      

local v = t.types["Vec"](1,0,0) 
local r = t.types["Rot"](1,1,1,1)

local test = v * v


]]  

   	  


--]==]
return M

