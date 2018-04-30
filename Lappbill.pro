#-------------------------------------------------
#
# Project created by QtCreator 2017-07-19T18:13:17
#
#-------------------------------------------------

QT += core gui # KConfigCore KConfigGui KI18n webengine webenginewidgets printsupport

INCLUDEPATH += $$PWD/libharu/include
DEPENDPATH += $$PWD/libharu/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#QMAKE_CXXFLAGS += -std=c++11

TARGET = Lappbill
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    settingsdialog.cpp

HEADERS  += \
        database.hpp \
    mainwindow.hpp \
    invoicetemplate.hpp \
    settingsdialog.hpp

FORMS    += mainwindow.ui \
    settingsdialog.ui

DISTFILES += \
    invoice.html \
    invoice.css \
    new.html \
    README.md
