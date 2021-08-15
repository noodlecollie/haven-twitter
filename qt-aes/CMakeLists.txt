add_library(qt-aes STATIC
	aesni/aesni-enc-cbc.h
	aesni/aesni-enc-ecb.h
	aesni/aesni-key-exp.h
	qaesencryption.h
	qaesencryption.cpp
)

target_link_libraries(qt-aes Qt5::Core)
target_include_directories(qt-aes PUBLIC .)
