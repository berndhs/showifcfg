
MYNAME = showifcfg

TEMPLATE = app
VERSION = 0.1.0

QT += core gui declarative

RESOURCES = $${MYNAME}.qrc

TARGET = bin/showifcfg

MOC_DIR = temp
OBJECTS_DIR = temp
RCC_DIR = temp

INCLUDEPATH += include/

SOURCES = src/showshell.cpp src/main.cpp
HEADERS = include/showshell.h