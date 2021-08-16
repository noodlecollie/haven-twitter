add_executable(haven-twitter
	resources/resources.qrc
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
target_link_libraries(haven-twitter PUBLIC qt-aes)
