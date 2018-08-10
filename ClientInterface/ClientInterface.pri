QT += websockets

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/nclientinterface.h \
    $$PWD/ntimesync.h \
    $$PWD/ntimesynctester.h \
    $$PWD/ncmdpacker.h \
    $$PWD/wtccmddefine.h \
    $$PWD/httprequest.h \
    $$PWD/onnconnector.h \
    $$PWD/onnrequest.h

SOURCES += \
    $$PWD/nclientinterface.cpp \
    $$PWD/ntimesync.cpp \
    $$PWD/ntimesynctester.cpp \
    $$PWD/ncmdpacker.cpp \
    $$PWD/onnconnector.cpp \
    $$PWD/onnrequest.cpp
