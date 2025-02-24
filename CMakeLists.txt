cmake_minimum_required (VERSION 3.16)

SET (CMAKE_C_COMPILER             "clang")
SET (CMAKE_CXX_COMPILER           "clang++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -v")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

project(glfw1 VERSION 1.0)
ENABLE_LANGUAGE(CXX)

set(CMAKE_CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



# includes
set (COMMON_INCLUDES
	)
# sources
set(COMMON_SOURCES
	sub.cpp
	object.hpp
	shape.hpp
	window.hpp
)

set(TARGET_SOURCES
	main.cpp
)
set(TEST_SOURCES
	test_main.cpp
)

# generate compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# default target 'spike.out'
add_executable(${PROJECT_NAME}.out ${COMMON_SOURCES} ${TARGET_SOURCES})
add_executable(test_${PROJECT_NAME}.out ${COMMON_SOURCES} ${TEST_SOURCES})

# target options
target_compile_options(${PROJECT_NAME}.out PRIVATE -O0 -g -Wall)
# if you want to override per project
# target_compile_features(${PROJECT_NAME}.out PRIVATE cxx_std_23)

target_compile_options(test_${PROJECT_NAME}.out PRIVATE -O0 -g -Wall)
# if you want to override per project
# target_compile_features(test_${PROJECT_NAME}.out PRIVATE cxx_std_23)

# use precompiled header
target_precompile_headers(${PROJECT_NAME}.out PRIVATE headers.hpp)
target_precompile_headers(test_${PROJECT_NAME}.out PRIVATE headers.hpp)

# include
target_include_directories(${PROJECT_NAME}.out PUBLIC
	"/opt/homebrew/Cellar/glfw/3.4/include"
	"/opt/homebrew/Cellar/glew/2.2.0_1/include"
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}"
	"${COMMON_INCLUDES}"
	)
target_include_directories(test_${PROJECT_NAME}.out PUBLIC
	"/opt/homebrew/Cellar/glfw/3.4/include"
	"/opt/homebrew/Cellar/glew/2.2.0_1/include"
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/external"
	"${COMMON_INCLUDES}"
	)
# link
target_link_directories(${PROJECT_NAME}.out PRIVATE
	"/opt/homebrew/Cellar/glfw/3.4/lib"
	"/opt/homebrew/Cellar/glew/2.2.0_1/lib"
	)
target_link_libraries(${PROJECT_NAME}.out PRIVATE
	"glfw3"
	"GLEW"
	"-framework Cocoa"
	"-framework IOKit"
	"-framework CoreVideo"
	"-framework OpenGL"
	)
target_link_directories(test_${PROJECT_NAME}.out PRIVATE
	)
target_link_libraries(test_${PROJECT_NAME}.out PRIVATE
	)


# custom target: run
add_custom_target(run
	DEPENDS ${PROJECT_NAME}.out
	DEPENDS run_command
	)
add_custom_command(OUTPUT run_command
	USES_TERMINAL
	COMMAND ${PROJECT_NAME}.out
	)
# copy compile_commands.json
add_custom_command(TARGET run POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
)

# custom target: copy
if (APPLE)
	set(CLIP pbcopy)
else()
	set(CLIP clip.exe)
endif()

add_custom_target(copy COMMAND echo > cmake_tmp
	COMMAND tail +2 ${PROJECT_SOURCE_DIR}/main.cpp >> cmake_tmp
	COMMAND cat ${PROJECT_SOURCE_DIR}/headers.hpp cmake_tmp | ${CLIP}
	COMMAND rm cmake_tmp
	DEPENDS ${PROJECT_NAME}.out
	)

# custom target: runtest
add_custom_target(runtest COMMAND test_${PROJECT_NAME}.out)

# custom target: lldb
# uses custom command. otherwise, stdin for lldb is closed by ninja
add_custom_target(lldb
	DEPENDS ${PROJECT_NAME}.out
	DEPENDS lldb_command
	)

add_custom_command(OUTPUT lldb_command
	USES_TERMINAL
	COMMAND lldb ${PROJECT_NAME}.out
	)

