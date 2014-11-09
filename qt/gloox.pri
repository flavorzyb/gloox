####gloox库########
HEADERS +=  $$PWD/../libs/gloox-1.0.11/src/config.h \
            $$PWD/../libs/gloox-1.0.11/src/connectionbase.h \
            $$PWD/../libs/gloox-1.0.11/src/connectiondatahandler.h \
            $$PWD/../libs/gloox-1.0.11/src/connectiontcpbase.h \
            $$PWD/../libs/gloox-1.0.11/src/connectiontcpclient.h \
            $$PWD/../libs/gloox-1.0.11/src/dns.h \
            $$PWD/../libs/gloox-1.0.11/src/gloox.h \
            $$PWD/../libs/gloox-1.0.11/src/loghandler.h \
            $$PWD/../libs/gloox-1.0.11/src/logsink.h \
            $$PWD/../libs/gloox-1.0.11/src/macros.h \
            $$PWD/../libs/gloox-1.0.11/src/mutex.h \
            $$PWD/../libs/gloox-1.0.11/src/mutexguard.h \
            $$PWD/../libs/gloox-1.0.11/src/prep.h \
            $$PWD/../libs/gloox-1.0.11/src/util.h

SOURCES +=  $$PWD/../libs/gloox-1.0.11/src/connectiontcpbase.cpp \
            $$PWD/../libs/gloox-1.0.11/src/connectiontcpclient.cpp \
            $$PWD/../libs/gloox-1.0.11/src/dns.cpp \
            $$PWD/../libs/gloox-1.0.11/src/gloox.cpp \
            $$PWD/../libs/gloox-1.0.11/src/logsink.cpp \
            $$PWD/../libs/gloox-1.0.11/src/mutex.cpp \
            $$PWD/../libs/gloox-1.0.11/src/prep.cpp \
            $$PWD/../libs/gloox-1.0.11/src/util.cpp

INCLUDEPATH += $$PWD/../libs/gloox-1.0.11/src

unix: HEADERS += $$PWD/../libs/gloox-1.0.11/src/config.h.unix

unix: LIBS +=-lresolv
