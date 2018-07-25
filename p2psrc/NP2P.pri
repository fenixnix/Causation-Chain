QT += network

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/nodeinfo.h \
    $$PWD/np2pnode.h \
    $$PWD/p2pfullnodenetwork.h \
    $$PWD/nsubnet.h \
    $$PWD/ncryptop2p.h \
    $$PWD/np2pringnet.h \ 
    $$PWD/np2pserverinterface.h

SOURCES += \
    $$PWD/nodeinfo.cpp \
    $$PWD/np2pnode.cpp \
    $$PWD/p2pfullnodenetwork.cpp \
    $$PWD/nsubnet.cpp \
    $$PWD/ncryptop2p.cpp \
    $$PWD/np2pringnet.cpp \ 
    $$PWD/np2pserverinterface.cpp
