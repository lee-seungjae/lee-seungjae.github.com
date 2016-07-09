-------------------------------------------------------------------------------
function string.split(str, div, ignoreEmpty) -- credit: http://richard.warburton.it
	if (div == '' or str == nil or str == '') then return nil end
	local pos, arr = 0, {}
	-- for each divider found
	for st, sp in function ()return string.find(str, div, pos, true) end do
		local x = string.sub(str, pos, st - 1)
		if x ~= '' or not ignoreEmpty then
			table.insert(arr, x) -- Attach chars left of current divider
		end
		pos = sp + 1 -- Jump past current divider
	end
	table.insert(arr, string.sub(str, pos)) -- Attach chars right of last divider
	return arr
end

-------------------------------------------------------------------------------
function string:trim()
	return self:match('^()%s*$') and '' or self:match('^%s*(.*%S)')
end

-------------------------------------------------------------------------------
function string:starts_with(b, ignore_case)
	assert(type(b) == 'string')
	assert(ignore_case == nil or type(ignore_case) == 'boolean')

	local a = self
	if ignore_case then
		a = a:lower()
		b = b:lower()
	end
	return a:sub(1, b:len()) == b
end

-------------------------------------------------------------------------------
function string:ends_with(b, ignore_case)
	assert(type(b) == 'string')
	assert(ignore_case == nil or type(ignore_case) == 'boolean')

	local a = self
	if ignore_case then
		a = a:lower()
		b = b:lower()
	end
	return a:sub(-b:len(), -1) == b
end

-------------------------------------------------------------------------------
function string:contains(b, ignore_case)
	assert(type(b) == 'string')
	assert(ignore_case == nil or type(ignore_case) == 'boolean')

	local a = self
	if ignore_case then
		a = a:lower()
		b = b:lower()
	end
	return a:find(b)
end
