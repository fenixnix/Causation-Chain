QT += websockets

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/nclientinterface.h \
    $$PWD/ntimesync.h \
    $$PWD/ntimesynctester.h \
    $$PWD/ncmdpacker.h \
    $$PWD/wtccmddefine.h \
    $$PWD/nwebsocket.h \
    $$PWD/httprequest.h \
    $$PWD/onnconnector.h

SOURCES += \
    $$PWD/nclientinterface.cpp \
    $$PWD/ntimesync.cpp \
    $$PWD/ntimesynctester.cpp \
    $$PWD/ncmdpacker.cpp \
    $$PWD/nwebsocket.cpp \
    $$PWD/onnconnector.cpp
