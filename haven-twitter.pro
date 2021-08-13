QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authconfigmanager.cpp \
    authconfigworkerthread.cpp \
    debugoauthcredentialinput.cpp \
    main.cpp \
    mainwindow.cpp \
    qt-aes/qaesencryption.cpp \
    splashdialogue.cpp \
    twitcurl/HMAC_SHA1.cpp \
    twitcurl/SHA1.cpp \
    twitcurl/base64.cpp \
    twitcurl/oauthlib.cpp \
    twitcurl/twitcurl.cpp \
    twitcurl/urlencode.cpp \
    twitterapplication.cpp

HEADERS += \
    authconfigmanager.h \
    authconfigworkerthread.h \
    debugoauthcredentialinput.h \
    mainwindow.h \
    qt-aes/aesni/aesni-enc-cbc.h \
    qt-aes/aesni/aesni-enc-ecb.h \
    qt-aes/aesni/aesni-key-exp.h \
    qt-aes/qaesencryption.h \
    splashdialogue.h \
    twitcurl/HMAC_SHA1.h \
    twitcurl/SHA1.h \
    twitcurl/base64.h \
    twitcurl/oauthlib.h \
    twitcurl/twitcurl.h \
    twitcurl/twitcurlurls.h \
    twitcurl/urlencode.h \
    twitterapplication.h

FORMS += \
    debugoauthcredentialinput.ui \
    mainwindow.ui \
    splashdialogue.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lcurl
