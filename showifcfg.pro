
MYNAME = showifcfg

TEMPLATE = app
VERSION = 0.3.0

QT += core declarative

CONFIG += mobility
MOBILITY += systeminfo
MOBILITY += sensors
MOBILITY += network

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ./include

RESOURCES = $${MYNAME}.qrc

TARGET = showifcfg
showifcfg.path = /usr/local/bin

MOC_DIR = temp
OBJECTS_DIR = temp
RCC_DIR = temp

INCLUDEPATH += include/

SOURCES = \
    src/ip-interface.cpp \
    src/ip-list.cpp \
    src/main.cpp \
    src/qml-text-browser.cpp \

HEADERS = \
    include/gpl2.h \
    include/ip-interface.h \
    include/ip-list.h \
    include/property.h \
    include/qml-text-browser.h

INSTALLS = showifcfg

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    notes.txt

unix:!symbian:!maemo5 {
    target.path = /opt/showifcfg/bin
    INSTALLS += target
}

BLD_ADDONS = ""
unix:!symbian:!maemo5 {
    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
    BLD_ADDONS += "desktop"
}

unix:!symbian:!maemo5 {
    icon.files = showifcfg.png
    icon.path = /usr/share/icons/hicolor/64x64/apps
    INSTALLS += icon
    BLD_ADDONS += "icon"
}

BLD_TGT = ""
maemo5 {
  BLD_TGT += "maemo5"
}
unix {
  BLD_TGT += "unix"
}
maemo6 {
  BLD_TGT += "maemo6"
}
unix:maemo5 {
  BLD_TGT += "unix:maemo5"
}
unit:maemo6 {
  BLD_TGT += "unix:maemo6"
}


message ("build target system $${BLD_TGT}")
message ("build addons $${BLD_ADDONS}")
message ("CONFIG $${CONFIG}")
