QT       += widgets core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageInputWindow.cpp \
    InputWindow.cpp \
    Manager_window.cpp \
    Reader.cpp \
    TitreInputWindow.cpp \
    VideoInputWindow.cpp \
    Writer.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    ImageInputWindow.h \
    InputWindow.h \
    Manager_window.h \
    MainWindow.h \
    Reader.h \
    TitreInputWindow.h \
    VideoInputWindow.h \
    Writer.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
