local templatedSceneTest = APP.GetOrCreateApp("WinGDI Window")
templatedSceneTest.kind = "WindowedApp"
templatedSceneTest.location = ""
templatedSceneTest.includeDir = ""
templatedSceneTest.sourceDir = ""
templatedSceneTest.resourceDir = "assets/"
templatedSceneTest.debugDir = ""

templatedSceneTest.AddState("system:linux", function()
	linkoptions("-pthread -Wl, rpath=\\$$ORIGIN")
	links("dl")
end)
templatedSceneTest.AddState("system:ios", function()
	files({
		templatedSceneTest.GetLocalFilePath(templatedSceneTest.resourceDir .. "Info.plist"),
		templatedSceneTest.GetLocalFilePath(templatedSceneTest.resourceDir)
	})
end)
templatedSceneTest.AddState({ "system:macosx or ios", "files:**cpp" }, function()
	compileas("Objective-C++")
end)

return templatedSceneTest