####gloox库########
HEADERS +=  $$PWD/../libs/gloox/config.h \
            $$PWD/../libs/gloox/connectionbase.h \
            $$PWD/../libs/gloox/connectiondatahandler.h \
            $$PWD/../libs/gloox/connectiontcpbase.h \
            $$PWD/../libs/gloox/connectiontcpclient.h \
            $$PWD/../libs/gloox/dns.h \
            $$PWD/../libs/gloox/gloox.h \
            $$PWD/../libs/gloox/loghandler.h \
            $$PWD/../libs/gloox/logsink.h \
            $$PWD/../libs/gloox/macros.h \
            $$PWD/../libs/gloox/mutex.h \
            $$PWD/../libs/gloox/mutexguard.h \
            $$PWD/../libs/gloox/prep.h \
            $$PWD/../libs/gloox/util.h

SOURCES +=  $$PWD/../libs/gloox/connectiontcpbase.cpp \
            $$PWD/../libs/gloox/connectiontcpclient.cpp \
            $$PWD/../libs/gloox/dns.cpp \
            $$PWD/../libs/gloox/gloox.cpp \
            $$PWD/../libs/gloox/logsink.cpp \
            $$PWD/../libs/gloox/mutex.cpp \
            $$PWD/../libs/gloox/prep.cpp \
            $$PWD/../libs/gloox/util.cpp

INCLUDEPATH += $$PWD/../libs/gloox

unix: {
    HEADERS += $$PWD/../libs/gloox/config.h.unix
    LIBS +=-lresolv
}
