#-------------------------------------------------
#
# Project created by QtCreator 2020-06-03T17:53:03
#
#-------------------------------------------------

QT       += core gui xml

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
        API/api.cpp \
        API/localapi.cpp \
        auxiliary.cpp \
        main.cpp \
        mainwindow.cpp \
        models/ar4filemodel.cpp \
        models/arx4.cpp \
        models/color.cpp \
        models/colortask.cpp \
        models/colortaskmodel.cpp \
        models/datasetmodel.cpp \
        models/order.cpp \
        models/ordermodel.cpp \
        models/pattern.cpp \
        models/printer.cpp \
        models/task.cpp \
        models/taskmodel.cpp \
        pages/addprinttask.cpp \
        pages/dashboard.cpp \
        pages/datamaintainance.cpp \
        pages/dialog.cpp \
        pages/pickuplist.cpp \
        pages/printersetting.cpp \
        pages/printtaskmanagement.cpp \
        pages/startworking.cpp

HEADERS += \
        API/api.h \
        API/localapi.h \
        auxiliary.h \
        mainwindow.h \
        models/ar4filemodel.h \
        models/arx4.h \
        models/color.h \
        models/colortask.h \
        models/colortaskmodel.h \
        models/datasetmodel.h \
        models/order.h \
        models/ordermodel.h \
        models/pattern.h \
        models/printer.h \
        models/task.h \
        models/taskmodel.h \
        pages/addprinttask.h \
        pages/dashboard.h \
        pages/datamaintainance.h \
        pages/dialog.h \
        pages/pickuplist.h \
        pages/printersetting.h \
        pages/printtaskmanagement.h \
        pages/startworking.h

FORMS += \
        mainwindow.ui \
        pages/addprinttask.ui \
        pages/dashboard.ui \
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
