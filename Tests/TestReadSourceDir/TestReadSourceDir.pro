include(../../Common.pri)
include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
        tst_readdir.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../FCBin/ -lCopyManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../FCBin/ -lCopyManager
else:unix: LIBS += -L$$PWD/../../../FCBin/ -lCopyManager

SOURCES += \
        main.cpp
