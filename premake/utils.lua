local UTILS = {}

function UTILS.AddPlatforms()
	if _ACTION == "android-studio" then
		APP.AddPlatform("armeabi-v7a")
		APP.AddPlatform("arm64-v8a")
		APP.AddPlatform("x86")
		APP.AddPlatform("x86_64")
		return
	end
	
	if os.ishost("windows") then
		APP.AddPlatform("x64")
		return
	end
	
	local arch = os.outputof("uname -m")
	APP.AddPlatform(arch)
end

function UTILS.AddConfigurations()
	APP.AddConfiguration("Debug")
	APP.AddConfiguration("Release")
end

return UTILS