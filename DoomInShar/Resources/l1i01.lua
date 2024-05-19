local Path = GetPath()
local GamePath = "/GameData/" .. Path

local P3DFile = P3D.P3DFile(GamePath)

for chunk in P3DFile:GetChunks(P3D.Identifiers.Texture) do
	if chunk.Name == 'int_kwik_vidgame.bmp' then
		local img = chunk:GetChunk(P3D.Identifiers.Image)
		assert(img ~= nil)
		local imgdata = img:GetChunk(P3D.Identifiers.ImageData)
		assert (imgdata ~= nil)
		imgdata.ImageData = ReadFile("/ModData/Resources/int_kwik_vidgame.bmp.png")
	elseif chunk.Name == 'int_kwik_vidname.bmp' then
		local img = chunk:GetChunk(P3D.Identifiers.Image)
		assert(img ~= nil)
		local imgdata = img:GetChunk(P3D.Identifiers.ImageData)
		assert (imgdata ~= nil)
		imgdata.ImageData = ReadFile("/ModData/Resources/int_kwik_vidname.bmp.png")
	end
end

P3DFile:Output()