QT += network

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/np2pnode.h \
    $$PWD/p2pfullnodenetwork.h \
    $$PWD/nsubnet.h \
    $$PWD/np2pringnet.h \ 
    $$PWD/np2pserverinterface.h \
    $$PWD/ncryptomsg.h \
    $$PWD/npeerdata.h

SOURCES += \
    $$PWD/np2pnode.cpp \
    $$PWD/p2pfullnodenetwork.cpp \
    $$PWD/nsubnet.cpp \
    $$PWD/np2pringnet.cpp \ 
    $$PWD/np2pserverinterface.cpp \
    $$PWD/ncryptomsg.cpp \
    $$PWD/npeerdata.cpp
