INCLUDEPATH += $$PWD/ \

#LIBS += -L$$PWD/Lib \
#    -lEmccQLib

HEADERS += \
    $$PWD/emcc/types.h \
    $$PWD/emcc/uECC.h \
    $$PWD/emcc/uECC_vli.h \
    $$PWD/emcclib.h

SOURCES += \
    $$PWD/emcc/uECC.c \
    $$PWD/emcclib.cpp
