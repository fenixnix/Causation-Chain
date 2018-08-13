QT += network

INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/np2pnode.h \
    $$PWD/nsubnet.h \
    $$PWD/np2pringnet.h \ 
    $$PWD/np2pserverinterface.h \
    $$PWD/npeerdata.h \
    $$PWD/p2pnetwork.h

SOURCES += \
    $$PWD/np2pnode.cpp \
    $$PWD/nsubnet.cpp \
    $$PWD/np2pringnet.cpp \ 
    $$PWD/np2pserverinterface.cpp \
    $$PWD/npeerdata.cpp \
    $$PWD/p2pnetwork.cpp
