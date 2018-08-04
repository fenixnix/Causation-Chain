include("./emcc.pri")

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/nemcc.h \
    $$PWD/ncryptomsg.h

SOURCES += \
    $$PWD/nemcc.cpp \
    $$PWD/ncryptomsg.cpp
