if(NOT CURL_FOUND)
	set(BUILD_CURL_EXE OFF)
	set(BUILD_SHARED_LIBS OFF)

	if(WIN32)
		set(CURL_STATIC_CRT OFF)
		set(ENABLE_UNICODE OFF)
	endif()

	add_subdirectory(curl)
endif()
