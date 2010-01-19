# -------------------------------------------------
# Project created by QtCreator 2010-01-19T18:45:30
# -------------------------------------------------
QT += network \
    xml \
    xmlpatterns \
    dbus

QT -= gui

INCLUDEPATH += ./header \
    ~/versionsverwaltung/git/gnokii/include

TARGET = SyncThunderGnokii

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    xgnokiiapi.cpp

HEADERS += xgnokiiapi.h \
    Global.h
