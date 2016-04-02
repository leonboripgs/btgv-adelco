QT     += core qml quick gui axcontainer sql winextras
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = blister-multitagv
TEMPLATE = app

VERSION=0.0.0.1 # major.minor.patch.build
VERSION_PE_HEADER=1.0
QMAKE_TARGET_DESCRIPTION=BLISTER MULTI TAG VISUAL VERIFICATION
QMAKE_TARGET_COPYRIGHT=nikos.lykouropoulos@gmail.com

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        uibackend.cpp \
        uicontroller.cpp

HEADERS  += \
        mainwindow.h \
        logger.h \
        uibackend.h \
        uicontroller.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

OTHER_FILES += \
    qml/MainWindow.qml \
    qml/MainPanelView.qml \
    qml/TopMenu.qml \
    gml.qrc
