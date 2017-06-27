TARGET = RTKUSER

CONFIG += \
    c++14 \
    static

RC_ICONS = icon/icon.ico

QT += \
    core \
    gui \
    widgets

SOURCES += \
    src/main.cpp \
    src/window.cpp \
    src/convbin.cpp \
    src/rnxtortkp.cpp \
    src/postokml.cpp

HEADERS += \
    src/window.h \
    src/convbin.h \
    src/rnxtortkp.h \
    src/postokml.h

FORMS += \
    src/window.ui

DebugBuild {
    DESTDIR  = $${OUT_PWD}/debug
} else {
    DESTDIR  = $${OUT_PWD}/release
}

GIT_VERSION = $$system(git --git-dir $$_PRO_FILE_PWD_/../../.git --work-tree $$PWD describe --always --tags)
DEFINES += 'GIT_VERSION=\\"$$GIT_VERSION\\"'

copydata.commands = $$QMAKE_COPY "$$shell_path($$PWD/configs.conf)" "$$shell_path($$DESTDIR)"
copydata2.commands = $$QMAKE_COPY "$$shell_path($$PWD/configs2.conf)" "$$shell_path($$DESTDIR)"
first.depends = $(first) copydata copydata2
export(first.depends)
export(copydata.commands)
export(copydata2.commands)
QMAKE_EXTRA_TARGETS += first copydata copydata2
