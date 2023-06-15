TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread
SOURCES += \
        ../example/main.cpp \
    ../src/uics_cmds.cpp\
    ../src/uics_state.cpp \
    ../src/fhtable.cpp \
    ../src/uics_sockets.cpp \
    ../src/state_handler.cpp \
    ../src/msghandler.cpp \
    ../src/inst_log.cpp \
    ../example/threaded_f1.cpp \
    ../example/test.cpp \
    ../src/uics_sndmsg.cpp \
    ../src/inst_time.cpp \
    ../src/uics_join.cpp

HEADERS += \
    ../src/uics_state.h \
    ../src/states.h \
    ../src/uics_cmds.h\
    ../src/insthandle.h \
    ../src/fhtable.h \
    ../src/uics.h \
    ../src/uics_sockets.h \
    ../src/state_handler.h \
    ../src/msghandler.h \
    ../src/uics_ports.h \
    ../src/uics_config.h \
    ../src/inst_log.h \
    ../example/test.h \
    ../example/threaded_f1.h \
    ../src/uics_sndmsg.h \
    ../src/inst_time.h \
    ../src/uics_join.h \
    ../src/uics_err_code.h

INCLUDEPATH += ../src \
                ../example \
