project(webeditor)
cmake_minimum_required(VERSION 2.8)

# mkdir build
# cmake -G "Visual Studio 14 2015 Win64" --config Release -DFLTK_DIR:PATH=C:/projects/fltk-1.3.4-recompile ..
# cmake --build . --config Release

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
    set(CMAKE_CXX_FLAGS " -std=c++11")
endif()

if (UNIX)
	add_definitions(-Dunix)
endif()

find_package(FLTK REQUIRED)

include_directories(${FLTK_INCLUDE_DIR})
include_directories("sundown/src")
include_directories("sundown/html")
include_directories("FtpClient")

#add_executable(webeditor WIN32
add_executable(webeditor 
	webeditormain.cpp
	webeditor.cpp
	sundown/src/markdown.c
	sundown/src/stack.c
	sundown/src/buffer.c
	sundown/src/autolink.c
	sundown/html/html.c
	sundown/html/html_smartypants.c
	sundown/html/houdini_html_e.c
	sundown/html/houdini_href_e.c
	FtpClient/BlockingSocket.cpp
	FtpClient/FTPClient.cpp
	FtpClient/FTPDataTypes.cpp
	FtpClient/FTPFileStatus.cpp
	FtpClient/FTPListParse.cpp
	)
if (UNIX)
	target_link_libraries(webeditor ${FLTK_LIBRARIES} dl)
else()
	target_link_libraries(webeditor ${FLTK_LIBRARIES})
endif()



