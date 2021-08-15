cmake_minimum_required(VERSION 3.1.0)

project(haven-twitter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(CURL)

add_subdirectory(submodules)
add_subdirectory(twitcurl)

add_executable(haven-twitter
	qt-aes/aesni/aesni-enc-cbc.h
	qt-aes/aesni/aesni-enc-ecb.h
	qt-aes/aesni/aesni-key-exp.h
	qt-aes/qaesencryption.h
	qt-aes/qaesencryption.cpp
	authconfigmanager.h
	authconfigmanager.cpp
	authconfigworkerthread.h
	authconfigworkerthread.cpp
	debugoauthcredentialinput.h
	debugoauthcredentialinput.cpp
	main.cpp
	mainwindow.ui
	mainwindow.h
	mainwindow.cpp
	splashdialogue.ui
	splashdialogue.h
	splashdialogue.cpp
	twitterapplication.h
	twitterapplication.cpp
)

target_link_libraries(haven-twitter PUBLIC Qt5::Widgets)
target_link_libraries(haven-twitter PUBLIC twitcurl)
