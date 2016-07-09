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

	return { meta = meta, lines = arr }
end

-------------------------------------------------------------------------------
local function compile_and_write(template, preprocessed_sources, doc_id, pp)
	print(doc_id, type(pp.lines))

	local function check_meta(key)
		if not pp.meta[key] then
			error('필수 메타데이터가 없습니다: ' .. key .. ' (' .. doc_id .. ')')
		end
	end
	check_meta('제목')
	check_meta('작성')
	check_meta('최종수정')

	local compiled_text = '컴파일된 텍스트'

	local c = {
		TITLE = pp.meta['제목'],
		CONTENT = compiled_text,
		FIRST_DATE = '퍼스트_데이트',
		PAGES = {},
	}

	io_ext.write_text_file('..\\' .. doc_id .. '.html', template(c))
end

-------------------------------------------------------------------------------
local function write_index(template, preprocessed_sources)
	local c = {
		TITLE = '',
		CONTENT = '',
		PAGES = {
			{ LINK='link', TITLE='title', DATE='date', TYPE='SLIDE' },
			{ LINK='link', TITLE='title', DATE='date', TYPE='SLIDE' },
			{ LINK='link', TITLE='title', DATE='date', TYPE='' },
			{ LINK='link', TITLE='title', DATE='date', TYPE='' },
			{ LINK='link', TITLE='title', DATE='date', TYPE='' },
		}
	}	
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

do return end




-------------------------------------------------------------------------------
-- 세션 리스트 읽어들임
local sessionsText = readTextFile('session_data/session_list.json')
local decodeResult, _, err = json.decode(sessionsText)
if decodeResult == nil then
	print('session_list.json 해석 중 에러')
	return
end
print('session_list.json 해석 완료.')

local sessions = json.decode(sessionsText).session_list

for _, v in pairs(sessions) do
	v.tracks = { v.track1, v.track2, v.track3, v.track4 }
	v.track1 = nil
	v.track2 = nil
	v.track3 = nil
	v.track4 = nil

	v.slidePublic = (v.slidePublic == "공개")
	v.videoPublic = (v.videoPublic == "공개")
	v.scriptPublic = (v.scriptPublic == "공개")
end

-------------------------------------------------------------------------------
-- 자바스크립트가 가져갈 수 있도록 포맷 맞춰서 다시 저장
do
	local jsPrefix =
		'if (typeof g_sessionSrcList == "undefined") g_sessionSrcList = [];\n' ..
		'g_sessionSrcList.push(\n'

	local jsPostfix = '' ..
		');\n'

	local encodeOption = { indent = true }
	writeTextFile(
		'output_server\\session_list.js',
		jsPrefix .. json.encode({sessions = sessions}, encodeOption) .. jsPostfix)
end
print('output_server\\session_list.js 작성 완료.')



-------------------------------------------------------------------------------
print('세션 html 출력 중...')
local template = loadTemplate('program/web_template/index.html')

do
	for _, v in ipairs(sessions) do
		local script
		if v.scriptPublic then
			script = readTextFile('session_data/scripts/' .. v.conf .. '/' .. v.id .. '.txt')
			if not script then
				print(string.format('[경고] %s: 스크립트가 공개로 설정되어 있는데 스크립트 파일이 없습니다.', v.id))
			end
		else
			script = '[비공개]'
		end

		if v.videoPublic and not exists('output_cdn/' .. v.conf .. '/videos/' .. v.id .. '.mp4') then
			print(string.format('[경고] %s: 비디오가 공개로 설정되어 있는데 비디오 파일이 없습니다.', v.id))
		end

		if v.slidePublic and not exists('output_cdn/' .. v.conf .. '/slides/' .. v.id .. '/index.html') then
			print(string.format('[경고] %s: 슬라이드가 공개로 설정되어 있는데 슬라이드 파일이 없습니다.', v.id))
		end

		local id = v.Id
		local c =
		{
			['NRP_CONFID_HERE'] = v.conf,
			['NRP_SESSIONID_HERE'] = v.id,
			['NRP_TITLE_HERE'] = v.title,
			['NRP_SUBTITLE_HERE'] = v.subtitle or '',
			['NRP_DESCRIPTION_HERE'] = v.description,
			['NRP_SPEAKER_HERE'] = v.speaker,
			['NRP_SPEAKER_INTRO_HERE'] = v.speakerIntro,
			['NRP_FROM_HERE'] = v.from,
			['NRP_SCRIPT_HERE'] = script,
			['NRP_BASE_HREF_HERE'] = ' --> <BASE HREF="../.."/> <!-- ',
			['NRP_OG_TITLE_HERE'] = ' --> <meta property="og:title" content="NDC Replay - ' .. v.title .. '" /> <!-- ',
			['NRP_OG_DESCRIPTION_HERE'] = ' --> <meta property="og:description" content="' .. v.description .. '" /> <!-- '
		}

		local outputDir1 = 'output_server\\' .. v.conf
		local outputDir2 = 'output_server\\' .. v.conf .. '\\sessions'
		local outputFile = 'output_server\\' .. v.conf .. '\\sessions\\' .. v.id .. '.html'
		os.execute(string.format('if not exist %s mkdir %s', outputDir1, outputDir1))
		os.execute(string.format('if not exist %s mkdir %s', outputDir2, outputDir2))

		--print(outputFile .. ' 작성 중...')

		writeTextFile(outputFile, template(c))
	end
end

print('E.N.D\n\n\n')
