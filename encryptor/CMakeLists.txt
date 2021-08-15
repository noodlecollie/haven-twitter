add_executable(encryptor
	main.cpp
	mainwindow.ui
	mainwindow.h
	mainwindow.cpp
)

target_link_libraries(encryptor PUBLIC Qt5::Widgets)
target_link_libraries(encryptor PUBLIC qt-aes)
