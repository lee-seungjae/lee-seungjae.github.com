-- 유티에프-팔
package.path = 'compiler/?.lua'
require 'strict'
require 'string_ext'
local io_ext = require 'io_ext'

ngx = nil
jit = nil

-------------------------------------------------------------------------------
local function load_sources() --> Dictionary<string, string>
	local sources = {}
	io_ext.set_ls_path('compiler\\ls.exe')
	for i, v in io_ext.dir('*.txt') do
		local content = io_ext.read_text_file(v)
		local id = v:sub(1, #v - 4)
		sources[id] = content
	end
	return sources
end

-------------------------------------------------------------------------------
local function preprocess(source) --> { meta = Dictionary<string, string>, lines = string[] }
	local meta = {}

	-- !KEY=VALUE 형태의 메타데이타를 찾아낸다
	local arr = source:gsub('\r', ''):split('\n', false)
	while arr[1]:starts_with('!') do
		local first = arr[1]

		local delim = first:find('=')
		assert(delim)
		local k = first:sub(2, delim - 1):trim()
		local v = first:sub(delim + 1):trim()
		meta[k] = v

		table.remove(arr, 1)
	end

	-- 처음 빈줄들을 쳐낸다
	while arr[1]:trim() == '' do
		table.remove(arr, 1)
	end

	-- 마지막 빈줄들도 쳐낸다.
	while arr[#arr]:trim() == '' do
		arr[#arr] = nil
	end

	return { meta = meta, lines = arr }
end

-------------------------------------------------------------------------------
local function compile_and_write(template, preprocessed_sources, doc_id, pp)
	print('컴파일 중: ' .. doc_id)

	local function check_meta(key)
		if not pp.meta[key] then
			error('필수 메타데이터가 없습니다: ' .. key .. ' (' .. doc_id .. ')')
		end
	end
	check_meta('제목')
	check_meta('작성')
	check_meta('최종수정')

	local compiled_lines = {}

	local current_line = 1
	local function peek_line()
		return pp.lines[current_line]
	end

	local function next_line()
		local rv = peek_line()
		current_line = current_line + 1
		return rv
	end

	local function escape(ln)
		local function link_external_with_caption(s, pos)
			s = s:sub(2, #s - 1)
			local delim = s:find(':')
			local caption = s:sub(1, delim - 1)
			local href = s:sub(delim + 1)
			return string.format('<a href="%s" class="external_link">%s</a>', href, caption)
		end
		local function link_external_without_caption(s)
			local prefix = ''
			if s:starts_with(' ') then
				prefix = ' '
				s = s:sub(2)
			end
			return prefix .. string.format('<a href="%s" class="external_link">%s</a>', s, s)
		end
		local function link_internal_with_caption(s)
			s = s:sub(2, #s - 1)
			local delim = s:find(':')
			local caption = s:sub(1, delim - 1)
			local href = s:sub(delim + 1)
			local x = preprocessed_sources[href]
			if not x then
				error('문서를 찾을 수 없습니다: ' .. s .. ' in ' .. doc_id)
			end
			return string.format('<a href="%s.html" class="internal_link">%s</a>', href, caption)
		end
		local function link_internal_without_caption(s)
			s = s:sub(2, #s - 1)
			local x = preprocessed_sources[s]
			if not x then
				error('문서를 찾을 수 없습니다: ' .. s .. ' in ' .. doc_id)
			end
			return string.format('<a href="%s.html" class="internal_link">%s</a>', s, x.meta['제목'])
		end

		local rv = ln
			:gsub('%<', '&lt;')
			:gsub('%>', '&gt;')
 			:gsub('%[[^%:]*%:https?%:%/%/[^%]]*%]', link_external_with_caption)
			:gsub('%[[^%:]*%:[^%]]*%]', link_internal_with_caption)
			:gsub('%[[^%]]*%]', link_internal_without_caption)
			:gsub('^https?%:%/%/[^%) ]*', link_external_without_caption)
			:gsub(' https?%:%/%/[^%) ]*', link_external_without_caption)
		return rv
	end

	local function escape_tag_only(ln)
		local rv = ln
			:gsub('%<', '&lt;')
			:gsub('%>', '&gt;')
		return rv
	end

	local function next_paragraph()
		local ln = next_line()

		-- 종료.
		if ln == nil then
			return
		end

		-- 한 라인짜리.
		if ln:match("^%-%-%-%-%-+$") then
			return { '<hr class="compiled">' }
		end
		if ln:starts_with('#') then
			for i = 3, 1, -1 do
				if ln:starts_with(string.rep('#', i)) then
					return { ln:sub(i + 1) }, '<h' .. i .. ' class="compiled">', '</h' .. i .. '>'
				end
			end
		end
		if ln == '' then
			return {}, '<div class="compiled_emptyline">', '</div>'
		end

		-- 블럭 처리.
		if ln == '{{{' then
			local pbody = {}
			while true do
				local ln = next_line()
				if (ln == nil) or (ln == '}}}') then
					break;
				end
				table.insert(pbody, escape_tag_only(ln) .. '<br>')
			end
			return pbody, '<div class="compiled_quote">', '</div>'
		end
		if ln == '<TAG>' then
			local pbody = {}
			while true do
				local ln = next_line()
				if (ln == nil) or (ln == '</TAG>') then
					break;
				end
				table.insert(pbody, ln .. '<br>')
			end
			return pbody, '<div>', '</div>'
		end
		if ln:lower():starts_with('<code:') then
			local lang = ln:sub(7, #ln - 1)
			local pbody = { '<pre name="code" class="' .. lang .. '">' }
			while true do
				local ln = next_line()
				if (ln == nil) or (ln:lower() == '</code>') then
					break;
				end
				table.insert(pbody, ln)
			end
			table.insert(pbody, '</pre>')
			return pbody
		end
		if ln:starts_with(' * ') then
			local pbody = { escape(ln:sub(4)) }
			while true do
				local ln = peek_line()
				if (ln == nil) or (ln == '') then
					break
				end
				if ln:starts_with(' * ') then
					break
				end
				next_line()
				table.insert(pbody, '<br>' .. escape(ln))
			end
			return pbody, '<li class="compiled">', '</li>'
		end

		local pbody = {}
		table.insert(pbody, escape(ln))
		return pbody, '<p class="compiled">', '</p>'
	end

	while true do
		local pbody, ph, pf = next_paragraph()
		if pbody == nil then break end
		
		if ph then table.insert(compiled_lines, ph) end
		for _, ln in ipairs(pbody) do
			table.insert(compiled_lines, ln)
		end
		if pf then table.insert(compiled_lines, pf) end
	end
	
	local c = {
		TITLE = pp.meta['제목'],
		CONTENT = table.concat(compiled_lines, '\n'),
		FIRST_DATE = pp.meta['작성'],
		PAGES = {},
	}
	io_ext.write_text_file('..\\' .. doc_id .. '.html', template(c))
end

-------------------------------------------------------------------------------
local function write_index(template, preprocessed_sources)
	print('인덱스 쓰는 중')

	local c = {
		TITLE = '',
		CONTENT = '',
		PAGES = {}
	}

	for k, v in pairs(preprocessed_sources) do
		table.insert(c.PAGES, {
			LINK = k .. '.html',
			TITLE = v.meta['제목'],
			DATE = v.meta['작성'],
			TYPE = v.meta['타입']
		});
	end

	table.sort(
		c.PAGES,
		function(a, b)
			return a.DATE > b.DATE
		end)

	io_ext.write_text_file('..\\index.html', template(c));
end

-------------------------------------------------------------------------------
local function load_template(templateFile)
	local t = require 'lua-resty-template.lib.resty.template'
	local v = t.new(templateFile)

	return function(data)
		local result = {}
		t.print = function(s) result[#result + 1] = s end
		v.render(data)
		return table.concat(result, "\n")
	end
end

-------------------------------------------------------------------------------
local template = load_template('compiler\\template.html')
local sources = load_sources()
local preprocessed_sources = {}
for k, v in pairs(sources) do
	print('전처리 중: ' .. k)
	preprocessed_sources[k] = preprocess(v)
end
for k, v in pairs(preprocessed_sources) do
	compile_and_write(template, preprocessed_sources, k, v)
end
write_index(template, preprocessed_sources)

print('E.N.D\n\n\n')
