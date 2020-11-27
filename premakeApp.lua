local ecsWithoutStdAny = APP.GetApp("Template Tests/ECSWithoutStdAny")
local templatedSceneTest = APP.GetApp("Template Tests/Templated Scene Test")
local sfinaeVariadicTemplateBasedOnEnums = APP.GetApp("Template Tests/Sfinae Variadic Template Based on Enums")
local betterPointers = APP.GetApp("Template Tests/Better Pointers")
ecsWithoutStdAny.group = "Template Tests"
templatedSceneTest.group = "Template Tests"
sfinaeVariadicTemplateBasedOnEnums.group = "Template Tests"
betterPointers.group = "Template Tests"

local WinGDIWindow = APP.GetApp("WinGDI/Window")
WinGDIWindow.group = "WinGDI"

local CrossPlatformWindowSystem = APP.GetApp("CrossPlatform/WindowSystem")
CrossPlatformWindowSystem.group = "Cross Platform"

return { WinGDIWindow, betterPointers, sfinaeVariadicTemplateBasedOnEnums, templatedSceneTest, ecsWithoutStdAny }