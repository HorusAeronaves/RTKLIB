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
    src/window.cpp

HEADERS += \
    src/window.h

FORMS += \
    src/window.ui

DebugBuild {
    DESTDIR  = $${OUT_PWD}/debug
} else {
    DESTDIR  = $${OUT_PWD}/release
}

copydata.commands = $$QMAKE_COPY "$$shell_path($$PWD/configs.conf)" "$$shell_path($$DESTDIR)"
copydata2.commands = $$QMAKE_COPY "$$shell_path($$PWD/configs2.conf)" "$$shell_path($$DESTDIR)"
first.depends = $(first) copydata copydata2
export(first.depends)
export(copydata.commands)
export(copydata2.commands)
QMAKE_EXTRA_TARGETS += first copydata copydata2
