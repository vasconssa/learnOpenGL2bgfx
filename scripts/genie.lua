--
-- Copyright (c) 2012-2018 Daniele Bartolini and individual contributors.
-- License: https://github.com/dbartolini/crown/blob/master/LICENSE
--

CROWN_DIR = (path.getabsolute("..") .. "/")
local CROWN_THIRD_DIR  = (CROWN_DIR .. "3rdparty/")
local CROWN_BUILD_DIR  = (CROWN_DIR .. "build/")
BGFX_DIR = (CROWN_DIR .. "3rdparty/bgfx/")
BIMG_DIR = (CROWN_DIR .. "3rdparty/bimg/")
BX_DIR = (CROWN_DIR .. "3rdparty/bx/")

function copyLib()
end

newoption {
	trigger = "with-tools",
	description = "Build with tools."
}

newoption {
	trigger = "no-level-editor",
	description = "Do not build Level Editor."
}

solution "crown"
	configurations {
		"debug",
		"development",
		"release",
	}

	platforms {
		"x32",
		"x64",
		"native"
	}

	language "C++"

	configuration {}

dofile ("toolchain.lua")
toolchain(CROWN_BUILD_DIR, CROWN_THIRD_DIR)


dofile ("crown.lua")
group "engine"
crown_project("", "WindowedApp", {})

group "libs"
dofile (BGFX_DIR .. "scripts/bgfx.lua")
bgfxProject("", "StaticLib")

dofile (BX_DIR .. "scripts/bx.lua")
dofile (BIMG_DIR .. "scripts/bimg.lua")
dofile (BIMG_DIR .. "scripts/bimg_encode.lua")
dofile (BIMG_DIR .. "scripts/bimg_decode.lua")



