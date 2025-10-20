QT += widgets
CONFIG += c++17
CONFIG(debug, debug|release) {
    CONFIG += console
}
SOURCES += main.cpp \
           lockwindow.cpp
HEADERS += lockwindow.h
