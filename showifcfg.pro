
MYNAME = showifcfg

TEMPLATE = app
QT += core gui declarative

RESOURCES = $${MYNAME}.qrc

MOC_DIR = temp
OBJECTS_DIR = temp
RCC_DIR = temp

INCLUDEPATH += include/

SOURCES = src/showshell.cpp src/main.cpp
HEADERS = include/showshell.h
