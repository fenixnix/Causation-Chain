#-------------------------------------------------
#
# Project created by QtCreator 2018-04-01T06:45:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = EmccLib
TEMPLATE = lib

DEFINES += EMCCLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        emcclib.cpp \
    emcc/uECC.c

HEADERS += \
        emcclib.h \
    emcc/asm_arm.inc \
    emcc/asm_arm_mult_square.inc \
    emcc/asm_arm_mult_square_umaal.inc \
    emcc/asm_avr.inc \
    emcc/asm_avr_mult_square.inc \
    emcc/curve-specific.inc \
    emcc/platform-specific.inc \
    emcc/types.h \
    emcc/uECC.h \
    emcc/uECC_vli.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
