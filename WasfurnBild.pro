QT       += core gui widgets network


TARGET      =   WasfurnBild

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bilder.qrc

VERSION     =   0.2.0

DEFINES     +=  APP_VERSION=\\\"$$VERSION\\\"
DEFINES     += APP_NAME=\\\"$$TARGET\\\"

win32:QMAKE_TARGET_COMPANY      = Alsweider
win32:QMAKE_TARGET_PRODUCT      = WasfurnBild
win32:QMAKE_TARGET_DESCRIPTION  = WasfurnBild: Desktop Background Image Reader



