add_rules("mode.debug", "mode.release")

target("mdCompiler")
	set_kind("binary")
	add_files("src/*.cpp")
	-- add_files("main.cpp", "parse_char.cpp")
