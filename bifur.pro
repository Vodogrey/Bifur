QT += core gui widgets printsupport

CONFIG += c++11

TARGET = bifur
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    iterationmaping.cpp \
    mathcalc.cpp \
    iterationcalc.cpp \
    paramdiagrammaping.cpp \
    paramdiagram.cpp

HEADERS += \
    qcustomplot.h \
    mainwindow.h \
    iterationmaping.h \
    mathcalc.h \
    iterationcalc.h \
    paramdiagrammaping.h \
    paramdiagram.h
