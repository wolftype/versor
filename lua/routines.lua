--Table insertion
AddTab = function(x,y)
	for i, iv in ipairs(y) do
		table.insert(x,iv)	
	end
end

--Table Membership
Member = function(x,list)
	for _, v in ipairs(list) do
		if x.id == v.id then return true end
	end
	return nil
end

--generic table member
isMember = function(x,list)
	for _, v in ipairs(list) do
		if x == v then return true end
	end
	return nil
end

--string manipulation
up = function(x)
	return string.upper(x)
end

lower = function(x)
	return string.lower(x)
end

--Print Table
function printTable(x,n)	
	local tab = ""
	n = n or 0	
	for i=0, n, 1 do tab = tab .. ">>>" end
	if type(x) == "table" then		
		print(tab .. " TABLE num of Terms:",#x,"\t")
		for i, iv in ipairs(x) do
			print (tab.." Term: ", i)
			printTable(iv,n+1)		
		end
	else
		print(tab.." "..x)		
	end
end