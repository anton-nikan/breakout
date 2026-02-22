add_rules("mode.debug", "mode.release")
if is_mode("debug") or is_mode("check") or is_mode("profile") then
	add_defines("_DEBUG")
else
	add_defines("NDEBUG")
end

set_languages("c++23")
add_requires("sfml")

target("sdk")

set_kind("static")
add_includedirs("include")
add_files(
	"src/sdk/*.cpp"
)
add_packages("sfml")
add_ldflags("-ObjC")
