# -------------------------------------------------
# Project created by QtCreator 2010-01-19T18:45:30
# -------------------------------------------------
QT += network \
    xml \
    xmlpatterns \
    dbus

QT -= gui

INCLUDEPATH += ./header \
    /usr/include/

LIBS += -L/usr/lib/ -lgnokii

TARGET = SyncThunderGnokii

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    xgnokiiapi.cpp

HEADERS += xgnokiiapi.h \
    Global.h
