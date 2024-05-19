local Path = GetPath()
local GamePath = GetGamePath(Path)

local File = ReadFile(GamePath)
local MFK = MFKLexer.Lexer:Parse(File)

MFK:SetAll("GagPlayFMV", 1, "doom.dcl", "loot.rmv")

MFK:Output(true)
