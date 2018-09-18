#-------------------------------------------------
#
# Project created by QtCreator 2018-03-22T13:18:26
#
#-------------------------------------------------

QT       += core gui websockets concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OriginWallet
TEMPLATE = app

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
        main.cpp \
        MainWindow.cpp \
    HttpRequest.cpp \
    emcc/uECC.c \
    aes/aes.c \
    Reciver.cpp

HEADERS += \
        MainWindow.h \
    HttpRequest.h \
    Define.h \
    emcc/uECC.h \
    Password.h \
    aes/aes.h \
    Reciver.h \
    emcc/asm_arm.inc \
    emcc/asm_arm_mult_square.inc \
    emcc/asm_arm_mult_square_umaal.inc \
    emcc/asm_avr.inc \
    emcc/asm_avr_mult_square.inc \
    emcc/curve-specific.inc \
    emcc/platform-specific.inc \
    emcc/uECC_vli.h \
    HttpGet.h

FORMS += \
        MainWindow.ui

TRANSLATIONS += \
        wallet.ts

RESOURCES += \
    wallet.qrc
