cmake_minimum_required(VERSION 2.8)

project(NN-aim)

file(GLOB HEADERS NN-aim/*.h)
file(GLOB SOURCES NN-aim/*.cpp)

include_directories(NN-aim/include/SDL2)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


if(WIN32)
	if (MSVC14)
		target_link_libraries(${PROJECT_NAME} legacy_stdio_definitions.lib)
	endif()

	target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/NN-aim/lib/win32/SDL2.lib)
	target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/NN-aim/lib/win32/SDL2main.lib)
else()
	target_link_libraries(${PROJECT_NAME} SDL2)
	add_definitions(-std=c++11)
endif()

