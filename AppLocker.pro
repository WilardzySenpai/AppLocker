QT += widgets network core

CONFIG += c++17

CONFIG(debug, debug|release) {
    CONFIG += console
}

SOURCES += main.cpp \
           lockwindow.cpp

HEADERS += lockwindow.h \
           version.h

FORMS += \
    lockwindow.ui

RESOURCES += resources.qrc

RC_FILE = AppLocker.rc