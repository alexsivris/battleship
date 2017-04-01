#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T13:54:21
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = battleship
TEMPLATE = app


SOURCES += main.cpp\
        battleship.cpp \
    net.cpp \
    guiserver.cpp \
    guiclient.cpp \
    fieldtile.cpp \
    field.cpp \
    ship.cpp \
    client.cpp \
    server.cpp \
    battlefield.cpp \
    netconnect.cpp \
    master.cpp


HEADERS  += battleship.h \
    net.h \
    guiserver.h \
    guiclient.h \
    ship.h  \
    fieldtile.h \
    field.h \
    battlefield.h \
    netconnect.h \
    master.h

FORMS    += battleship.ui \
    guiserver.ui \
    guiclient.ui \
    battlefield.ui
