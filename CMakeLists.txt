cmake_minimum_required (VERSION 3.10)

project (StrRoster)

add_library(
	StrRoster STATIC
	FileUtil.cc
	HashFunctions.cc
	StrRoster.cc
)

add_executable(
	Test Test.cc
)


target_link_libraries(
	Test
	PRIVATE
		StrRoster
)
