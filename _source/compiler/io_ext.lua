-- 유티에프-팔

local io_ext = {}
local ls_path = 'ls.exe'

-------------------------------------------------------------------------------
function io_ext.set_ls_path(new_path)
	ls_path = new_path
end

-------------------------------------------------------------------------------
-- for _, p in dir('.') do
-- 	  print(p .. ';')
-- end
function io_ext.dir(path)
	local fp = io.popen(ls_path .. ' "' .. path .. '"', 'r')
	
	local t = {}

	local ln = fp:read("*l")
	while ln do
		t[#t + 1] = ln
		ln = fp:read("*l")
	end
	fp:close()
	
	return ipairs(t)
end

-------------------------------------------------------------------------------
function io_ext.read_text_file(path) --> string?
	local fp = io.open(path, 'rt')
	if not fp then
		return nil
	end

	local content = fp:read('*a')
	fp:close()
	if content:sub(1, 3) == string.char(0xEF, 0xBB, 0xBF) then
		content = content:sub(4)
	end
	return content
end

-------------------------------------------------------------------------------
function io_ext.write_text_file(path, content) --> ()
	local fp = io.open(path, 'wt')
	fp:write(content)
	fp:close()
end

-------------------------------------------------------------------------------
function io_ext.exists(path) --> bool
	local fp = io.open(path, 'r')
	if not fp then
		return false
	end
	fp:close()
	return true
end

-------------------------------------------------------------------------------
return io_ext