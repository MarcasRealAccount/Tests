newoption({
	trigger = "verbose",
	description = "Print debug info"
})

local function SetDefaultGlobals()
	if not GLOBALS then GLOBALS = {} end
	if not GLOBALS_PUSHED then GLOBALS_PUSHED = {} end
	if not GLOBALS_APPS then GLOBALS_APPS = {} end
	if not GLOBALS.currentPath then GLOBALS.currentPath = "" end
	if not GLOBALS.fileName then GLOBALS.fileName = "premake5.lua" end
	if not GLOBALS.filePath then GLOBALS.filePath = "premake5.lua" end
	if not GLOBALS.verbose then GLOBALS.verbose = _OPTIONS["verbose"] end
end

-- Set the default global variables
SetDefaultGlobals()

local function deepcopy(orig)
	local orig_type = type(orig)
	local copy
	if orig_type == 'table' then
		copy = {}
		for orig_key, orig_value in next, orig, nil do
			copy[deepcopy(orig_key)] = deepcopy(orig_value)
		end
		setmetatable(copy, deepcopy(getmetatable(orig)))
	else -- number, string, boolean, etc
		copy = orig
	end
	return copy
end

-- Global APP structure
local APP = {
	workspaceName = "Premake Workspace",
	platforms = {},
	configurations = {},
	thirdPartyFolder = "Third_Party/",
	defaultIncludeDir = "/inc/",
	defaultSourceDir = "/src/",
	defaultDebugFolder = "/run/",
	startApp = nil
}

function APP.PushGlobals()
	if not GLOBALS then SetDefaultGlobals() end
	table.insert(GLOBALS_PUSHED, deepcopy(GLOBALS))
end

function APP.PopGlobals()
	GLOBALS = table.remove(GLOBALS_PUSHED)
end

-- Print a message if '--verbose' flag is set
function APP.DebugLog(message)
	if GLOBALS and GLOBALS.verbose then
		print(message)
	end
end

-- Prints the global variables if '--verbose' flag is set
function APP.DebugLogGlobals()
	APP.DebugLog("--------------------------")
	APP.DebugLog("|  Premake App Globals    ")
	APP.DebugLog("|> File Name: " .. GLOBALS.fileName .. "'")
	APP.DebugLog("|> File Path: " .. GLOBALS.filePath .. "'")
	APP.DebugLog("|> Current Path: '" .. GLOBALS.currentPath .. "'")
	APP.DebugLog("|> Verbose: '" .. tostring(GLOBALS.verbose) .. "'")
	APP.DebugLog("--------------------------")
end

-- Set's the name of the workspace
function APP.SetWorkspaceName(workspaceName)
	APP.workspaceName = workspaceName
	APP.DebugLog("Set workspace name to '" .. workspaceName .. "'")
end

-- Add's a platform to the workspace
function APP.AddPlatform(platform)
	table.insert(APP.platforms, platform)
	APP.DebugLog("Added platform '" .. platform .. "' to the workspace")
end

-- Clear's all platforms from the workspace
function APP.ClearPlatforms()
	APP.platforms = {}
	APP.DebugLog("Cleared all platforms from the workspace")
end

-- Add's a configuration to the workspace
function APP.AddConfiguration(configuration)
	table.insert(APP.configurations, configuration)
	APP.DebugLog("Added configuration '" .. configuration .. "' to the workspace")
end

-- Clear's all configurations from the workspace
function APP.ClearConfigurations()
	APP.configurations = {}
	APP.DebugLog("Cleared all configurations from the workspace")
end

-- Set's the third party folder for this workspace
function APP.SetThirdPartyFolder(thirdPartyfolder)
	APP.thirdPartyFolder = thirdPartyFolder
	APP.DebugLog("Set third party folder to '" .. thirdPartyFolder .. "'")
end

-- Set's the include folder for new projects this workspace
function APP.SetDefaultIncludeFolder(includeFolder)
	APP.defaultIncludeDir = includeFolder
	APP.DebugLog("Set include folder to '" .. includeFolder .. "'")
end

-- Set's the source folder for new projects in this workspace
function APP.SetDefaultSourceFolder(sourceFolder)
	APP.defaultSourceDir = sourceFolder
	APP.DebugLog("Set source folder to '" .. sourceFolder .. "'")
end

-- Set's the debug folder for new projects in this workspace
function APP.SetDefaultDebugFolder(debugFolder)
	APP.defaultDebugFolder = debugFolder
	APP.DebugLog("Set debug folder to '" .. debugFolder .. "'")
end

-- Set's the startup app for this workspace
function APP.SetStartApp(app)
	APP.startApp = app
	APP.DebugLog("Set startup app to '" .. app.name .. "'")
end

-- Get's or loads a third party app at 'APP.thirdPartyFolder + app + /premakeApp.lua'
function APP.GetThirdPartyApp(app)
	APP.DebugLog("Getting Third Party App '" .. app .. "'")
	local modulePath = APP.thirdPartyFolder .. app .. "/premakeApp.lua"
	APP.PushGlobals()
	GLOBALS.fileName = "premakeApp.lua"
	GLOBALS.filePath = GLOBALS.currentPath .. modulePath
	GLOBALS.currentPath = GLOBALS.currentPath .. APP.thirdPartyFolder .. app .. "/"
	APP.DebugLogGlobals()
	local ret = dofile(modulePath)
	if not ret.name then
		for _, app in pairs(ret) do
			app.group = "Third_Party_Apps"
		end
	else
		ret.group = "Third_Party_Apps"
	end
	APP.PopGlobals()
	return ret
end

-- Get's or loads a third party library at 'APP.thirdPartyFolder + app + .lua'
function APP.GetThirdPartyLibrary(library)
	APP.DebugLog("Getting Third Party Library '" .. library .. "'")
	local modulePath = APP.thirdPartyFolder .. library .. ".lua"
	APP.PushGlobals()
	GLOBALS.fileName = library .. ".lua"
	GLOBALS.filePath = GLOBALS.currentPath .. modulePath
	GLOBALS.currentPath = GLOBALS.currentPath .. APP.thirdPartyFolder .. library .. "/"
	APP.DebugLogGlobals()
	local ret = dofile(modulePath)
	if not ret.name then
		for _, app in pairs(ret) do
			app.group = "Third_Party_Libs"
		end
	else
		ret.group = "Third_Party_Libs"
	end
	APP.PopGlobals()
	return ret
end

-- Get's or loads a app at 'app + /premakeApp.lua'
function APP.GetApp(app)
	while string.sub(app, -1, -1) == "/" do
		app = string.sub(app, 1, -2)
	end
	APP.DebugLog("Getting App '" .. app .. "'")
	local path = string.sub(app, 1, string.find(app, "/[^/]*$"))
	local name = string.sub(app, string.find(app, "/[^/]*$") + 1)
	local modulePath = app .. "/premakeApp.lua"
	APP.PushGlobals()
	GLOBALS.fileName = "premakeApp.lua"
	GLOBALS.filePath = GLOBALS.currentPath .. modulePath
	GLOBALS.currentPath = GLOBALS.currentPath .. path .. name .. "/"
	APP.DebugLogGlobals()
	local ret = dofile(modulePath)
	APP.PopGlobals()
	return ret
end

-- Get's or loads a library at 'library + .lua'
function APP.GetLibrary(library)
	while string.sub(library, -1, -1) == "/" do
		library = string.sub(library, 1, -2)
	end
	APP.DebugLog("Getting Library '" .. library .. "'")
	local path = string.sub(library, 1, string.find(library, "/[^/]*$"))
	local name = string.sub(library, string.find(library, "/[^/]*$") + 1)
	local modulePath = path .. name .. ".lua"
	APP.PushGlobals()
	GLOBALS.fileName = name .. ".lua"
	GLOBALS.filePath = GLOBALS.currentPath .. modulePath
	GLOBALS.currentPath = GLOBALS.currentPath .. path .. name .. "/"
	APP.DebugLogGlobals()
	local ret = dofile(modulePath)
	if not ret.name then
		for _, app in pairs(ret) do
			app.group = "Libs"
		end
	else
		ret.group = "Libs"
	end
	APP.PopGlobals()
	return ret
end

-- Loads the local app at 'premakeApp.lua'
function APP.GetLocalApp()
	APP.DebugLog("Getting Local App")
	APP.PushGlobals()
	GLOBALS.filePath = "premakeApp.lua"
	GLOBALS.fileName = "premakeApp.lua"
	APP.DebugLogGlobals()
	local ret = dofile("premakeApp.lua")
	APP.PopGlobals()
	return ret
end

-- Get's or Creates a new app
function APP.GetOrCreateApp(name)
	if GLOBALS_APPS[name] then
		return GLOBALS_APPS[name]
	end
	
	local app = {}
	app.name = name
	-- TODO: Implement a way for this to work:
	--app.group = string.sub(GLOBALS.currentPath, 1, -2)
	--app.group = string.sub(app.group, 1, string.find(app.group, "/[^/]*$"))
	app.group = "Apps"
	app.currentPath = GLOBALS.currentPath
	app.location = name .. "/"
	app.objectDir = "Output/" .. name .. "/Obj/"
	app.outputDir = "Output/" .. name .. "/Bin/"
	app.libraryDir = "Output/" .. name .. "/Lib/"
	app.includeDir = name .. APP.defaultIncludeDir
	app.sourceDir = name .. APP.defaultSourceDir
	app.resourceDir = name .. APP.defaultDebugFolder .. "assets/"
	app.debugDir = name .. APP.defaultDebugFolder
	app.addLink = true
	app.cppDialect = "C++17"
	app.rtti = "Off"
	app.exceptionHandling = "On"
	app.warnings = "Default"
	app.dependencies = {}
	app.states = {}
	app.flags = { "MultiProcessorCompile" }
	
	app.ToString = function()
		local str = "name = '" .. app.name .. "'\n"
		str = str .. "group = '" .. app.group .. "'\n"
		str = str .. "currentPath = '" .. app.currentPath .. "'\n"
		str = str .. "location = '" .. app.location .. "'\n"
		str = str .. "objectDir = '" .. app.objectDir .. "'\n"
		str = str .. "outputDir = '" .. app.outputDir .. "'\n"
		str = str .. "libraryDir = '" .. app.libraryDir .. "'\n"
		str = str .. "includeDir = '" .. app.includeDir .. "'\n"
		str = str .. "sourceDir = '" .. app.sourceDir .. "'\n"
		str = str .. "resourceDir = '" .. app.resourceDir .. "'\n"
		str = str .. "debugDir = '" .. app.debugDir .. "'\n"
		str = str .. "cppDialect = '" .. app.cppDialect .. "'\n"
		str = str .. "rtti = '" .. app.rtti .. "'\n"
		str = str .. "exceptionHandling = '" .. app.exceptionHandling .. "'\n"
		str = str .. "warnings = '" .. app.warnings .. "'\n"
		str = str .. "dependencies = '" .. #app.dependencies .. "'\n"
		str = str .. "states = '" .. #app.states .. "'\n"
		str = str .. "flags = '" .. #app.flags .. "'"
		return str
	end
	
	app.AddFlag = function(flag)
		table.insert(app.flags, flag)
		APP.DebugLog("Added Flag '" .. flag .. "' to '" .. app.name .. "'")
	end
	app.GetLocalFilePath = function(file)
		return app.currentPath .. file
	end
	app.AddFile = function(file)
		if not app.files then app.files = {} end
		
		table.insert(app.files, file)
		APP.DebugLog("Added file '" .. file .. "' to '" .. app.name "'")
	end
	app.AddDependency = function(dependency)
		app.dependencies[dependency.name] = dependency
		APP.DebugLog("Added dependency '" .. dependency.name .. "' to '" .. app.name .. "'")
	end
	app.AddState = function(filter, func)
		table.insert(app.states, { filter = filter, func = func })
		if type(filter) == "table" then
			local str = "Added state { "
			for i, flt in pairs(filter) do
				str = str .. "'" .. flt .. "'"
				if i < #filter then
					str = str .. ", "
				end
			end
			APP.DebugLog(str .. " } to '" .. app.name .. "'")
		else
			APP.DebugLog("Added state '" .. filter .. "' to '" .. app.name .. "'")
		end
	end
	app.GetAllIncludedDirectories = function(includeDirs)
		table.insert(includeDirs, app.currentPath .. app.includeDir)
		for name, dep in pairs(app.dependencies) do
			dep.GetAllIncludedDirectories(includeDirs)
		end
	end
	
	GLOBALS_APPS[name] = app
	APP.DebugLog("Creating App '" .. name .. "'")
	return app
end

-- Calls premake functions on the app
function APP.PremakeApp(app)
	if app.premaked then return end
	
	local deps = {}
	local sysIncludeDirectories = {}
	for name, dep in pairs(app.dependencies) do
		if dep.addLink then
			table.insert(deps, name)
		end
		APP.PremakeApp(dep)
		dep.GetAllIncludedDirectories(sysIncludeDirectories)
	end
	
	APP.DebugLog("Premake function called on app '" .. app.name .. "'")
	group(app.group)
	cppdialect(app.cppDialect)
	exceptionhandling(app.exceptionHandling)
	flags(app.flags)
	rtti(app.rtti)
	project(app.name)
	debugdir(app.debugDir)
	links(deps)
	location(app.currentPath .. app.location)
	xcodebuildresources(app.currentPath .. app.resourceDir)
	warnings(app.warnings)
	objdir(app.objectDir)
	includedirs(app.currentPath .. app.includeDir)
	sysincludedirs(sysIncludeDirectories)
	if app.files then
		local Files = {}
		for i, file in pairs(app.files) do
			table.insert(Files, app.currentPath .. file)
		end
		files(Files)
	else
		files({
			app.currentPath .. app.includeDir .. "**.h",
			app.currentPath .. app.includeDir .. "**.hpp",
			app.currentPath .. app.sourceDir .. "**.h",
			app.currentPath .. app.sourceDir .. "**.hpp",
			app.currentPath .. app.sourceDir .. "**.c",
			app.currentPath .. app.sourceDir .. "**.cpp"
		})
	end
	if app.kind then
		kind(app.kind)
	end
	
	for i, state in pairs(app.states) do
		filter(state.filter)
		state.func()
	end
	
	if project().kind == "StaticLib" or project().kind == "SharedLib" then
		targetdir(app.libraryDir)
	else
		targetdir(app.outputDir)
	end
	
	filter("configurations:Debug")
		optimize("Off")
		symbols("On")
		defines({ "_DEBUG", "_CRT_SECURE_NO_WARNINGS" })
	
	filter("configurations:Release")
		optimize("Full")
		symbols("Off")
		defines({ "_RELEASE", "NDEBUG", "_CRT_SECURE_NO_WARNINGS" })
	
	filter("system:windows")
		toolset("msc")
		defines({ "NOMINMAX" })
	
	filter("system:not windows")
		toolset("gcc")
	
	filter("system:linux")
		debugenvs({ "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../%{OUTDIR}" })
	
	filter({})
	
	app.premaked = true
end

-- Calls all premake functions for this workspace, that includes the created apps too
function APP.PremakeWorkspace()
	APP.DebugLog("Premake workspace called with '" .. APP.startApp.name .. "' as startup project")
	workspace(APP.workspaceName)
	platforms(APP.platforms)
	configurations(APP.configurations)
	for name, app in pairs(GLOBALS_APPS) do
		APP.PremakeApp(app)
	end
	workspace(APP.workspaceName)
	startproject(APP.startApp.name)
end

return APP