INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/netsync.h \
    $$PWD/mainnetserver.h

SOURCES += \
    $$PWD/netsync.cpp \
    $$PWD/mainnetserver.cpp

include("../src/p2p/np2p.pri")
include("../src/Network/network.pri")
