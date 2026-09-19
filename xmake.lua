add_rules("mode.debug", "mode.release")
if is_mode("debug") or is_mode("check") or is_mode("profile") then
	add_defines("_DEBUG")
else
	add_defines("NDEBUG")
end

set_languages("c++23")
add_requires("cppfront")

includes("sdk")

target("breakout-cppfront")

set_kind("binary")
add_includedirs("src", "sdk/include")
add_rules("cppfront")
add_files(
	"src/*.cpp2",
	"src/*.h2"
)

on_load(function (target)
	target:add("includedirs", path.join(target:autogendir(), "src"))
end)

add_deps("sdk")
add_packages("cppfront")
