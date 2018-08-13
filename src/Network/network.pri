QT += network websockets

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/ipclassify.h \
    $$PWD/qipendpoint.h \
    $$PWD/udpnetwork.h \
    $$PWD/messageprotocol.h \
    $$PWD/nwsnetwork.h \
    $$PWD/ntcpnetwork.h \
    $$PWD/nhttprequest.h

SOURCES += \
    $$PWD/ipclassify.cpp \
    $$PWD/qipendpoint.cpp \
    $$PWD/udpnetwork.cpp \
    $$PWD/messageprotocol.cpp \
    $$PWD/nwsnetwork.cpp \
    $$PWD/ntcpnetwork.cpp \
    $$PWD/nhttprequest.cpp


