TARGET = RTKUSER

CONFIG += \
    c++14 \
    static

QT += \
    core \
    gui \
    widgets

SOURCES += \
    src/main.cpp \
    src/window.cpp

HEADERS += \
    src/window.h

FORMS += \
    src/window.ui
