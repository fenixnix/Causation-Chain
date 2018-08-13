INCLUDEPATH += $$PWD/

DISTFILES += \
    $$PWD/README.md \
    $$PWD/LICENSE.txt

HEADERS += \
    $$PWD/types.h \
    $$PWD/uECC.h \
    $$PWD/uECC_vli.h \
    $$PWD/necc.h \
    $$PWD/platform-specific.inc

SOURCES += \
    $$PWD/uECC.c \
    $$PWD/necc.cpp
