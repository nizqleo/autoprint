#-------------------------------------------------
#
# Project created by QtCreator 2020-06-03T17:53:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autoprint
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        auxiliary.cpp \
        main.cpp \
        mainwindow.cpp \
        models/datasetmodel.cpp \
        models/order.cpp \
        models/ordermodel.cpp \
        models/pattern.cpp \
        models/printer.cpp \
        models/task.cpp \
        pages/datamaintainance.cpp \
        pages/dialog.cpp \
        pages/pickuplist.cpp \
        pages/printersetting.cpp \
        pages/printing.cpp \
        pages/printtaskmanagement.cpp \
        pages/startworking.cpp

HEADERS += \
        auxiliary.h \
        mainwindow.h \
        models/datasetmodel.h \
        models/order.h \
        models/ordermodel.h \
        models/pattern.h \
        models/printer.h \
        models/task.h \
        pages/datamaintainance.h \
        pages/dialog.h \
        pages/pickuplist.h \
        pages/printersetting.h \
        pages/printing.h \
        pages/printtaskmanagement.h \
        pages/startworking.h

FORMS += \
        mainwindow.ui \
        pages/datamaintainance.ui \
        pages/dialog.ui \
        pages/pickuplist.ui \
        pages/printersetting.ui \
        pages/printing.ui \
        pages/printtaskmanagement.ui \
        pages/startworking.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    record
