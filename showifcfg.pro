
MYNAME = showifcfg

TEMPLATE = app
VERSION = 0.2.0

QT += core gui declarative

RESOURCES = $${MYNAME}.qrc

TARGET = showifcfg
showifcfg.path = /usr/bin
showifcfg.files = showifcfg

MOC_DIR = temp
OBJECTS_DIR = temp
RCC_DIR = temp

INCLUDEPATH += include/

SOURCES = src/main.cpp
HEADERS = 

INSTALLS = showifcfg
