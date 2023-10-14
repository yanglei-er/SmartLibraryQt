QT += core gui bluetooth widgets network sql

CONFIG += c++17
CONFIG -= debug_and_release

TARGET = "SmartLibrary"
VERSION = 0.1.5.0
QMAKE_TARGET_PRODUCT = "SmartLibrary"
QMAKE_TARGET_DESCRIPTION = "Smart library"
QMAKE_TARGET_COMPANY = "Yanglei"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2023 Yanglei.All rights reserved."
RC_ICONS = smartlibrary.ico
RC_LANG = 0x0004

SOURCES += \
    addbook.cpp \
    bluetooth.cpp \
    bookcell.cpp \
    bookinfo.cpp \
    bookmanage.cpp \
    borrowbook.cpp \
    btdevcell.cpp \
    database.cpp \
    editbook.cpp \
    findbook.cpp \
    globalprocess.cpp \
    main.cpp \
    mainwindow.cpp \
    network.cpp \
    returnbook.cpp \
    tools.cpp \
    viewbook.cpp

HEADERS += \
    addbook.h \
    bluetooth.h \
    bookcell.h \
    bookinfo.h \
    bookmanage.h \
    borrowbook.h \
    btdevcell.h \
    database.h \
    editbook.h \
    findbook.h \
    globalprocess.h \
    mainwindow.h \
    network.h \
    returnbook.h \
    tools.h \
    viewbook.h

FORMS += \
    addbook.ui \
    bluetooth.ui \
    bookcell.ui \
    bookmanage.ui \
    borrowbook.ui \
    btdevcell.ui \
    editbook.ui \
    findbook.ui \
    mainwindow.ui \
    returnbook.ui \
    viewbook.ui

RESOURCES += \
    res.qrc
