QT       += core gui network \
            quick quickwidgets \
            quick
RC_FILE = Test.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acceptfriend.cpp \
    addfriend.cpp \
    chatwindow.cpp \
    communicator.cpp \
    emojimodel.cpp \
    login.cpp \
    main.cpp \
    client.cpp \
    message.cpp \
    mylog.cpp \
    signup.cpp \
    userprofile.cpp \
    qnchatmessage.cpp \
    sendfile.cpp

HEADERS += \
    acceptfriend.h \
    addfriend.h \
    chatwindow.h \
    client.h \
    communicator.h \
    emojimodel.h \
    login.h \
    message.h \
    mylog.h \
    signup.h \
    userprofile.h \
    qnchatmessage.h \
    sendfile.h

FORMS += \
    acceptfriend.ui \
    addfriend.ui \
    chatwindow.ui \
    client.ui \
    login.ui \
    signup.ui \
    userprofile.ui \
    sendfile.ui

RESOURCES += \
    chatwindowsrc.qrc \
    src.qrc \
    qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    EmojiPicker.pro.user \
#    EmojiPicker.qml
