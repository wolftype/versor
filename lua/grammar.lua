--[[
A FUNCTION is parsed in form

funcname (arguments) return operation

which is captured like so:

{ { funcname, {arguments} }, { return, {operation} } }

]]


--require "types"
--require "funcs"
local lpeg = require "lpeg"
local type = type
local print = print 
local ipairs = ipairs

module("grammar")

local C = lpeg.C 		--Capture
local Ct = lpeg.Ct		--Capture to Table

local R = lpeg.R		--Range
local S = lpeg.S		--Set
local P = lpeg.P		--Pattern
local V = lpeg.V		--Rule

--space, tab, new line, return 
local space 	= 	S" \t\n\r"
local zspace 	= 	S" \t\n\r"^0 --(zero or more)	

--scope
local open 		= P"(" * zspace
local close 	= P")" * zspace
local scope 	=  S"()"		

--GA operations, inner outer geo
local product 	= 	S"<^*"
--reversion, inversion, involution, etc
local version   = S"!~#"

local seps = S"," * zspace

local func = C(P"="*zspace*"function")^-1
	
local ret = C(P("return")) * zspace 

local reserved = ret
	
--capital letters	
local cap = R('AZ')
local low = R('az')

--product capture
gop = C( product ) * zspace	

--versions
vop = C( version ) * zspace

--not a letter
local nonletter = space + scope + version + product + seps + reserved

word = P(1 - nonletter)^1

--A Capital followed by three or more consecutive letters (i.e a type)
local types = C( cap * word ) * zspace 

--variables are lowercase and may be comma separated
local var = C( low * word - reserved) * (seps + zspace)

--0 or more negative signs followed by 1 or more characters in range 0-9
local number = C( P"-"^-1 * R("09")^1 ) * zspace

local instance = types^1 * var^1


Fun, Exp, Sig, Factor, Term, Version, Cast, Instance, Parenth, Arg, Return = V"Fun",V"Exp", V"Sig", V"Factor", V"Term", V"Version", V"Cast", V"Instance",V"Parenth",V"Arg", V"Return"

Expr = P{
	Exp,
	Exp  = Ct(Factor * (gop^-1 * Factor)^0);
	Factor 	= Term + Ct(Version) + Ct(Cast); 
	Term 	= var + Parenth + instance; 					-- An Instance, Variable (predefined), Or Parenthetical 
	Version = vop^-1 * Term;                    -- A Unary Op
	Cast 	= types^-1 * Parenth;                 -- A Cast  
	Parenth = open * Exp * close;
}

Expr = zspace * Expr * -1

Func = P{
	Fun,
	Fun		= Ct(Sig * Expr^0 * Return); 					
	Sig 	= Ct( C( word ) * zspace * Ct(Arg) );	  -- Function Signature
	Arg 	= open * Ct(instance)^1 * close;				-- Function Arguments
	Return 	= Ct(ret * Expr);								      -- Return Statement
}

Func = zspace * Func * -1

parse = function (x)
	local res = Func:match(x)
	return res
end

function ctPrint(x,n)	
	local tab = ""
	n = n or 0	
	for i=0, n, 1 do tab = tab .. ">>>" end
	if type(x) == "table" then		
		print(tab .. " TABLE num of Terms:",#x,"\t")
		for i, iv in ipairs(x) do
			print (tab.." Term #", i)
			ctPrint(iv,n+1)		
		end
	else
		print(tab.." "..x)		
	end
end

local c = "ab * dll * vec"
local q = Expr:match(c)
ctPrint(q)

local c = [[ testfunc ( Vec ab, Vec bd, Vec cd ) return ab ^ bd ^ cd ]]
ctPrint (parse(c),0)



