add_library(twitcurl STATIC
	base64.h
	base64.cpp
	HMAC_SHA1.h
	HMAC_SHA1.cpp
	oauthlib.h
	oauthlib.cpp
	SHA1.h
	SHA1.cpp
	twitcurl.h
	twitcurl.cpp
	twitcurlurls.h
	urlencode.h
	urlencode.cpp
)

if(CURL_FOUND)
	target_link_libraries(twitcurl PRIVATE ${CURL_LIBRARIES})
else()
	# Use project from submodule
	target_link_libraries(twitcurl PRIVATE libcurl)
endif()

if(MSVC)
	target_compile_definitions(twitcurl PRIVATE _CRT_SECURE_NO_WARNINGS)
	target_compile_options(twitcurl PRIVATE
		/wd4267
		/wd4477
	)
endif()

target_include_directories(twitcurl PUBLIC .)
