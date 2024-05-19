local iwadIdx = Settings.DoomIWad

local iwadArray = {
	'doom1.wad',
	'doom.wad',
	'doom2.wad',
	'plutonia.wad',
	'tnt.wad',
	'Freedm.wad',
	'Freedomm1.wad',
	'Freedoom2.wad'
}

local iwad = "wads\\" .. iwadArray[iwadIdx]

Output("-iwad\n")
Output(iwad .. "\n")

if Settings.DoomMergePWad ~= "" then
	Output("-merge\n")
	for i in string.gmatch(Settings.DoomMergePWad, "%S+") do
		Output("wads\\" .. i .. "\n")
	end
end

if Settings.DoomPWad ~= "" then
	Output("-file\n")
	for i in string.gmatch(Settings.DoomPWad, "%S+") do
		Output("wads\\" .. i .. "\n")
	end
end
