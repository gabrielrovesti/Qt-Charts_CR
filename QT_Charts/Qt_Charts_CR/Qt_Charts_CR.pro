QT       += core gui
QT       += xml
QT       += core gui charts
QT       += core
QT       += gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CandleStick.cpp \
    Mapper.cpp \
    bar.cpp \
    chart.cpp \
    exceptions.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    nomiclassi.cpp \
    pie.cpp \
    tablewidget.cpp

HEADERS += \
    CandleStick.h \
    bar.h \
    chart.h \
    exceptions.h \
    line.h \
    mainwindow.h \
    mapper.h \
    nomiclassi.h \
    pie.h \
    tablewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui

RESOURCES += \
    risorse.qrc
