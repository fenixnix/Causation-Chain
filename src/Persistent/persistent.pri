INCLUDEPATH += \
    $$PWD/

HEADERS += \
    $$PWD/ndatastore.h \
    $$PWD/nsimplestore.h

SOURCES += \
    $$PWD/ndatastore.cpp \
    $$PWD/nsimplestore.cpp

include("./database/database.pri")
